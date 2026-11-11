#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pc_hist_cdf_u32.h"
#include "pc_hist_clip_redistribute_u32.h"
#include "pc_hist_dynamic_range_u32.h"
#include "pc_hist_entropy_norm_q.h"
#include "pc_hist_equalize_u8.h"
#include "pc_hist_high_percentile_u32.h"
#include "pc_hist_low_percentile_u32.h"
#include "pc_hist_valley_count_u32.h"
#include "pc_histogram_total_u32.h"
#include "pc_image_hist_bin_u32.h"
#include "pc_mean3_i32.h"
#include "pc_mean4_i32.h"
#include "pc_mean5_i32.h"
#include "pc_median_network5_i32.h"
#include "pc_median_network7_i32.h"
#include "pc_running_median3_i32.h"
#include "pc_sensor_triple_median_i32.h"
#include "pc_trimmed_mean5_i32.h"
#include "pc_weighted_sensor_fuse_i32.h"

#define CHECK(expr) do { if (!(expr)) { \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr); \
    return __LINE__; \
} } while (0)
#define CASES UINT32_C(100000)

static uint32_t rng = UINT32_C(0xA341316C);
static double max_entropy_error;

static uint32_t random_u32(void)
{
    uint32_t x = rng;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    rng = x;
    return x;
}

static int32_t signed_bits(uint32_t u)
{
    if (u <= (uint32_t)INT32_MAX) return (int32_t)u;
    return (int32_t)((int64_t)u - INT64_C(4294967296));
}

static void fill_histogram(uint32_t h[256], uint32_t ceiling)
{
    size_t i;
    for (i = 0u; i < 256u; ++i) h[i] = random_u32() % ceiling;
    h[random_u32() & 255u] += 1u;
}

static void sort_i32(int32_t *values, size_t count)
{
    size_t i;
    for (i = 1u; i < count; ++i) {
        const int32_t value = values[i];
        size_t j = i;
        while (j > 0u && values[j - 1u] > value) {
            values[j] = values[j - 1u];
            --j;
        }
        values[j] = value;
    }
}

static int32_t mean4_reference(int32_t a, int32_t b, int32_t c, int32_t d)
{
    const int64_t sum = (int64_t)a + b + c + d;
    const long double exact = (long double)sum / 4.0L;
    const long double lower = floorl(exact);
    const long double part = exact - lower;
    int64_t answer = (int64_t)lower;
    if (part > 0.5L || (part == 0.5L && (answer % 2) != 0)) ++answer;
    return (int32_t)answer;
}

static int32_t median_reference(const int32_t *input, size_t count)
{
    int32_t values[7];
    size_t i;
    for (i = 0u; i < count; ++i) values[i] = input[i];
    sort_i32(values, count);
    return values[count / 2u];
}

static int test_histogram_total(void)
{
    uint32_t bins[256], out = UINT32_C(0xA5A5A5A5);
    uint32_t trial;
    CHECK(pc_histogram_total_u32(NULL, 0u, &out) && out == 0u);
    CHECK(pc_histogram_total_u32(NULL, 0u, NULL) == false);
    CHECK(!pc_histogram_total_u32(NULL, 1u, &out) && out == 0u);
    bins[0] = UINT32_MAX; bins[1] = 0u;
    CHECK(pc_histogram_total_u32(bins, 2u, &out) && out == UINT32_MAX);
    bins[1] = 1u; out = 77u;
    CHECK(!pc_histogram_total_u32(bins, 2u, &out) && out == 77u);
    bins[0] = 11u; bins[1] = 23u;
    CHECK(pc_histogram_total_u32(bins, 2u, &bins[0]) && bins[0] == 34u);
    for (trial = 0u; trial < CASES; ++trial) {
        const size_t count = (size_t)(random_u32() % 257u);
        uint64_t expected = 0u;
        size_t i;
        for (i = 0u; i < count; ++i) {
            bins[i] = random_u32() % UINT32_C(1000000);
            expected += bins[i];
        }
        out = UINT32_C(0xDEADBEEF);
        CHECK(pc_histogram_total_u32(count == 0u ? NULL : bins, count, &out));
        CHECK(out == (uint32_t)expected);
    }
    return 0;
}

static int test_cdf(void)
{
    uint32_t bins[256], output[256], inplace[256];
    uint32_t trial;
    CHECK(pc_hist_cdf_u32(NULL, 0u, NULL));
    bins[0] = UINT32_MAX; bins[1] = 1u;
    output[0] = 91u; output[1] = 92u;
    CHECK(!pc_hist_cdf_u32(bins, 2u, output));
    CHECK(output[0] == 91u && output[1] == 92u);
    CHECK(!pc_hist_cdf_u32(NULL, 1u, output));
    CHECK(!pc_hist_cdf_u32(bins, 1u, NULL));
    bins[0] = 1u; bins[1] = 2u; bins[2] = 3u;
    CHECK(pc_hist_cdf_u32(bins, 3u, bins));
    CHECK(bins[0] == 1u && bins[1] == 3u && bins[2] == 6u);
    for (trial = 0u; trial < CASES; ++trial) {
        uint64_t sum = 0u;
        size_t i;
        fill_histogram(bins, UINT32_C(10000));
        for (i = 0u; i < 256u; ++i) sum += bins[i];
        CHECK(sum <= UINT32_MAX);
        CHECK(pc_hist_cdf_u32(bins, 256u, output));
        sum = 0u;
        for (i = 0u; i < 256u; ++i) {
            sum += bins[i];
            CHECK(output[i] == (uint32_t)sum);
            if (i > 0u) CHECK(output[i] >= output[i - 1u]);
        }
        memcpy(inplace, bins, sizeof bins);
        CHECK(pc_hist_cdf_u32(inplace, 256u, inplace));
        CHECK(memcmp(inplace, output, sizeof output) == 0);
    }
    return 0;
}

static int test_percentiles(void)
{
    uint32_t h[256], low, high, trial;
    for (trial = 0u; trial < CASES; ++trial) {
        uint64_t total = 0u;
        uint32_t p = random_u32() % 101u, target, lo = 0u, hi = 0u;
        uint64_t cumulative = 0u;
        size_t i;
        fill_histogram(h, UINT32_C(1000));
        for (i = 0u; i < 256u; ++i) total += h[i];
        target = p == 0u ? 1u : (uint32_t)((total * p + 99u) / 100u);
        for (i = 0u; i < 256u; ++i) {
            cumulative += h[i];
            if (cumulative >= target) { lo = (uint32_t)i; break; }
        }
        cumulative = 0u;
        for (i = 256u; i > 0u; --i) {
            cumulative += h[i - 1u];
            if (cumulative >= target) { hi = (uint32_t)(i - 1u); break; }
        }
        CHECK(pc_hist_low_percentile_u32(h, p, &low) && low == lo);
        CHECK(pc_hist_high_percentile_u32(h, p, &high) && high == hi);
    }
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    low = 77u; high = 88u;
    CHECK(!pc_hist_low_percentile_u32(h, 0u, &low) && low == 77u);
    CHECK(!pc_hist_high_percentile_u32(h, 100u, &high) && high == 88u);
    h[5] = 1u; h[250] = UINT32_MAX;
    CHECK(pc_hist_low_percentile_u32(h, 0u, &low) && low == 5u);
    CHECK(pc_hist_low_percentile_u32(h, 100u, &low) && low == 250u);
    CHECK(pc_hist_high_percentile_u32(h, 0u, &high) && high == 250u);
    CHECK(pc_hist_high_percentile_u32(h, 100u, &high) && high == 5u);
    low = 77u; high = 88u;
    CHECK(!pc_hist_low_percentile_u32(h, 101u, &low) && low == 77u);
    CHECK(!pc_hist_high_percentile_u32(NULL, 50u, &high) && high == 88u);
    return 0;
}

static int test_entropy(void)
{
    uint32_t h[256], result, trial;
    for (trial = 0u; trial < CASES; ++trial) {
        const uint32_t a = random_u32() % UINT32_C(1000000) + 1u;
        const uint32_t b = random_u32() % UINT32_C(1000000) + 1u;
        const unsigned bits = 8u + (random_u32() % 24u);
        const double total = (double)a + (double)b;
        const double p = (double)a / total, q = (double)b / total;
        const double exact = -(p * log2(p) + q * log2(q)) / 8.0;
        const double actual_scale = (double)(UINT32_C(1) << bits);
        const double tolerance = 1.0 / 262144.0 + 0.5 / actual_scale + 2.0e-12;
        memset(h, 0, sizeof h);
        h[trial & 255u] = a;
        h[(trial + 1u) & 255u] += b;
        CHECK(pc_hist_entropy_norm_q(h, bits, &result));
        {
            const double error = fabs((double)result / actual_scale - exact);
            if (error > max_entropy_error) max_entropy_error = error;
            CHECK(error <= tolerance);
        }
        CHECK(result <= (UINT32_C(1) << bits));
    }
    memset(h, 0, sizeof h);
    CHECK(!pc_hist_entropy_norm_q(h, 24u, &result));
    CHECK(!pc_hist_entropy_norm_q(h, 32u, &result));
    CHECK(!pc_hist_entropy_norm_q(h, 24u, NULL));
    h[0] = UINT32_MAX;
    CHECK(pc_hist_entropy_norm_q(h, 31u, &result) && result == 0u);
    for (size_t i = 0u; i < 256u; ++i) h[i] = 1u;
    CHECK(pc_hist_entropy_norm_q(h, 31u, &result) && result == (UINT32_C(1) << 31u));
    CHECK(pc_hist_entropy_norm_q(h, 0u, &result) && result == 1u);
    for (size_t i = 0u; i < 256u; ++i) h[i] = UINT32_MAX;
    CHECK(pc_hist_entropy_norm_q(h, 31u, &result) && result == (UINT32_C(1) << 31u));
    return 0;
}

static uint8_t equalize_reference(const uint32_t h[256], uint32_t total,
                                  uint32_t cdf_min, uint32_t cumulative)
{
    uint64_t numerator, denominator, quotient, remainder;
    if (total == cdf_min) return 0u;
    if (cumulative <= cdf_min) return 0u;
    numerator = (uint64_t)(cumulative - cdf_min) * 255u;
    denominator = total - cdf_min;
    quotient = numerator / denominator;
    remainder = numerator % denominator;
    if (remainder * 2u > denominator ||
        (remainder * 2u == denominator && (quotient & 1u))) ++quotient;
    (void)h;
    return (uint8_t)quotient;
}

static int test_equalize(void)
{
    uint32_t h[256], trial;
    uint8_t lut[256];
    for (trial = 0u; trial < CASES; ++trial) {
        uint64_t total64 = 0u;
        uint32_t total, cdf_min = 0u, cumulative = 0u;
        size_t i;
        fill_histogram(h, 100u);
        for (i = 0u; i < 256u; ++i) total64 += h[i];
        total = (uint32_t)total64;
        for (i = 0u; i < 256u; ++i) if (h[i] != 0u) { cdf_min = h[i]; break; }
        CHECK(pc_hist_equalize_u8(h, lut));
        if (total == cdf_min) {
            for (i = 0u; i < 256u; ++i) CHECK(lut[i] == (uint8_t)i);
        } else {
            for (i = 0u; i < 256u; ++i) {
                cumulative += h[i];
                CHECK(lut[i] == equalize_reference(h, total, cdf_min, cumulative));
                if (i > 0u) CHECK(lut[i] >= lut[i - 1u]);
            }
        }
    }
    memset(h, 0, sizeof h); h[91] = UINT32_MAX;
    CHECK(pc_hist_equalize_u8(h, lut));
    for (size_t i = 0u; i < 256u; ++i) CHECK(lut[i] == (uint8_t)i);
    h[92] = 1u; memset(lut, 0xA5, sizeof lut);
    CHECK(!pc_hist_equalize_u8(h, lut));
    for (size_t i = 0u; i < 256u; ++i) CHECK(lut[i] == 0xA5u);
    CHECK(!pc_hist_equalize_u8(NULL, lut));
    CHECK(!pc_hist_equalize_u8(h, NULL));
    return 0;
}

static int test_clip(void)
{
    uint32_t h[256], output[256], trial;
    for (trial = 0u; trial < CASES; ++trial) {
        uint64_t total_in = 0u, total_out = 0u, excess = 0u;
        uint32_t limit = random_u32() % 1000u + 1u;
        uint32_t residual;
        size_t i;
        fill_histogram(h, UINT32_C(10000));
        for (i = 0u; i < 256u; ++i) {
            total_in += h[i];
            if (h[i] > limit) excess += (uint64_t)(h[i] - limit);
        }
        residual = (uint32_t)(excess % 256u);
        CHECK(pc_hist_clip_redistribute_u32(h, limit, output));
        for (i = 0u; i < 256u; ++i) {
            const uint64_t expected = (h[i] > limit ? limit : h[i]) +
                                      excess / 256u + (i < residual ? 1u : 0u);
            CHECK(output[i] == expected);
            total_out += output[i];
        }
        CHECK(total_in == total_out);
        if ((trial & 1u) == 0u) {
            uint32_t inplace[256];
            memcpy(inplace, h, sizeof h);
            CHECK(pc_hist_clip_redistribute_u32(inplace, limit, inplace));
            CHECK(memcmp(inplace, output, sizeof output) == 0);
        }
    }
    memset(h, 0, sizeof h); h[0] = UINT32_MAX; h[255] = UINT32_MAX;
    CHECK(pc_hist_clip_redistribute_u32(h, UINT32_MAX, output));
    CHECK(output[0] == UINT32_MAX && output[255] == UINT32_MAX);
    memset(h, 0, sizeof h); h[0] = UINT32_MAX;
    CHECK(pc_hist_clip_redistribute_u32(h, 1u, output));
    {
        uint64_t sum = 0u;
        for (size_t i = 0u; i < 256u; ++i) sum += output[i];
        CHECK(sum == UINT32_MAX && output[0] == UINT32_C(16777217) &&
              output[253] == UINT32_C(16777216) && output[254] == UINT32_C(16777215));
    }
    CHECK(!pc_hist_clip_redistribute_u32(h, 0u, output));
    memset(output, 0xA5, sizeof output);
    CHECK(!pc_hist_clip_redistribute_u32(h, 0u, output));
    for (size_t i = 0u; i < 256u; ++i) CHECK(output[i] == UINT32_C(0xA5A5A5A5));
    CHECK(!pc_hist_clip_redistribute_u32(NULL, 1u, output));
    CHECK(!pc_hist_clip_redistribute_u32(h, 1u, NULL));
    return 0;
}

static int test_range_valleys(void)
{
    uint32_t h[256], range, valleys, trial;
    for (trial = 0u; trial < CASES; ++trial) {
        uint32_t low = 256u, high = 0u, expected_valleys = 0u;
        size_t i = 0u;
        memset(h, 0, sizeof h);
        for (size_t j = 0u; j < 256u; ++j) h[j] = random_u32() % 8u;
        h[random_u32() & 255u] += 1u;
        for (size_t j = 0u; j < 256u; ++j) if (h[j]) { if (low == 256u) low = (uint32_t)j; high = (uint32_t)j; }
        CHECK(pc_hist_dynamic_range_u32(h, &range));
        CHECK(range == high - low);
        i = 1u;
        while (i < 255u) {
            size_t end = i;
            while (end + 1u < 256u && h[end + 1u] == h[i]) ++end;
            if (h[i] < h[i - 1u] && end < 255u && h[end + 1u] > h[i]) ++expected_valleys;
            i = end + 1u;
        }
        CHECK(pc_hist_valley_count_u32(h, &valleys));
        CHECK(valleys == expected_valleys);
    }
    memset(h, 0, sizeof h); range = 55u; valleys = 66u;
    CHECK(!pc_hist_dynamic_range_u32(h, &range) && range == 55u);
    CHECK(!pc_hist_dynamic_range_u32(NULL, &range));
    CHECK(!pc_hist_dynamic_range_u32(h, NULL));
    CHECK(!pc_hist_valley_count_u32(NULL, &valleys) && valleys == 66u);
    CHECK(!pc_hist_valley_count_u32(h, NULL));
    h[0] = 1u; h[255] = 1u;
    CHECK(pc_hist_dynamic_range_u32(h, &range) && range == 255u);
    memset(h, 0, sizeof h);
    h[10] = 5u; h[11] = 1u; h[12] = 1u; h[13] = 5u;
    CHECK(pc_hist_valley_count_u32(h, &valleys) && valleys == 1u);
    return 0;
}

static int test_image_bin(void)
{
    uint32_t trial, bin = 99u;
    for (trial = 0u; trial < CASES; ++trial) {
        const uint8_t value = (uint8_t)random_u32();
        const uint32_t count = random_u32() | 1u;
        const uint32_t expected = (uint32_t)(((uint64_t)value * count) / 256u);
        CHECK(pc_image_hist_bin_u32(value, count, &bin) && bin == expected);
    }
    CHECK(pc_image_hist_bin_u32(255u, UINT32_MAX, &bin));
    CHECK(bin == (uint32_t)(((uint64_t)255u * UINT32_MAX) / 256u));
    bin = 91u;
    CHECK(!pc_image_hist_bin_u32(0u, 0u, &bin) && bin == 91u);
    CHECK(!pc_image_hist_bin_u32(0u, 1u, NULL));
    return 0;
}

static int test_means(void)
{
    uint32_t trial;
    for (trial = 0u; trial < CASES; ++trial) {
        const int32_t a = signed_bits(random_u32()), b = signed_bits(random_u32());
        const int32_t c = signed_bits(random_u32()), d = signed_bits(random_u32());
        const int32_t e = signed_bits(random_u32());
        const long double m3 = roundl(((long double)a + b + c) / 3.0L);
        const long double m5 = roundl(((long double)a + b + c + d + e) / 5.0L);
        CHECK(pc_mean3_i32(a, b, c) == (int32_t)m3);
        CHECK(pc_mean4_i32(a, b, c, d) == mean4_reference(a, b, c, d));
        CHECK(pc_mean5_i32(a, b, c, d, e) == (int32_t)m5);
        CHECK(pc_mean3_i32(c, b, a) == pc_mean3_i32(a, b, c));
        CHECK(pc_mean4_i32(d, c, b, a) == pc_mean4_i32(a, b, c, d));
        CHECK(pc_mean5_i32(e, d, c, b, a) == pc_mean5_i32(a, b, c, d, e));
    }
    CHECK(pc_mean3_i32(INT32_MIN, INT32_MIN, INT32_MAX) ==
          (int32_t)roundl(((long double)INT32_MIN + INT32_MIN + INT32_MAX) / 3.0L));
    CHECK(pc_mean3_i32(0, 0, 2) == 1);
    CHECK(pc_mean3_i32(0, 0, -2) == -1);
    CHECK(pc_mean4_i32(0, 0, 0, 2) == 0);
    CHECK(pc_mean4_i32(0, 0, 0, 6) == 2);
    CHECK(pc_mean4_i32(0, 0, 0, -2) == 0);
    CHECK(pc_mean4_i32(0, 0, 0, -6) == -2);
    CHECK(pc_mean5_i32(INT32_MIN, INT32_MAX, 0, 0, 0) == 0);
    CHECK(pc_mean3_i32(-7,-7,-7) == -7);
    CHECK(pc_mean4_i32(-7,-7,-7,-7) == -7);
    CHECK(pc_mean5_i32(-7,-7,-7,-7,-7) == -7);
    return 0;
}

static int test_medians(void)
{
    uint32_t trial;
    int32_t v5[5], v7[7];
    for (trial = 0u; trial < CASES; ++trial) {
        int32_t a, b, c, d, e, f, g;
        for (size_t i = 0u; i < 5u; ++i) v5[i] = signed_bits(random_u32());
        for (size_t i = 0u; i < 7u; ++i) v7[i] = signed_bits(random_u32());
        a=v5[0];b=v5[1];c=v5[2];d=v5[3];e=v5[4];
        CHECK(pc_median_network5_i32(a,b,c,d,e) == median_reference(v5,5u));
        sort_i32(v5, 5u);
        CHECK(pc_trimmed_mean5_i32(a,b,c,d,e) ==
              (int32_t)roundl(((long double)v5[1] + v5[2] + v5[3]) / 3.0L));
        a=v7[0];b=v7[1];c=v7[2];d=v7[3];e=v7[4];f=v7[5];g=v7[6];
        CHECK(pc_median_network7_i32(a,b,c,d,e,f,g) == median_reference(v7,7u));
        CHECK(pc_sensor_triple_median_i32(a,b,c) == median_reference(v7,3u));
        CHECK(pc_sensor_triple_median_i32(c,b,a) == pc_sensor_triple_median_i32(a,b,c));
    }
    {
        const int32_t edge5[5] = {INT32_MIN, INT32_MAX, 0, INT32_MIN, INT32_MAX};
        const int32_t edge7[7] = {INT32_MIN, INT32_MAX, 0, INT32_MIN, INT32_MAX, -1, 1};
        CHECK(pc_median_network5_i32(edge5[0],edge5[1],edge5[2],edge5[3],edge5[4]) == 0);
        CHECK(pc_median_network7_i32(edge7[0],edge7[1],edge7[2],edge7[3],edge7[4],edge7[5],edge7[6]) == 0);
        CHECK(pc_sensor_triple_median_i32(INT32_MIN, INT32_MAX, 0) == 0);
        CHECK(pc_trimmed_mean5_i32(INT32_MIN,INT32_MIN,0,INT32_MAX,INT32_MAX) == 0);
        CHECK(pc_median_network5_i32(9,9,9,9,9) == 9);
        CHECK(pc_median_network7_i32(-4,-4,-4,-4,-4,-4,-4) == -4);
        CHECK(pc_sensor_triple_median_i32(5,5,5) == 5);
    }
    return 0;
}

static int test_running_median(void)
{
    pc_running_median3_i32_state state = {{0,0,0},0,0,0};
    int32_t history[3] = {INT32_MIN,INT32_MAX,0};
    uint32_t trial;
    CHECK(pc_running_median3_i32(INT32_MIN, &state) && state.median == INT32_MIN);
    CHECK(pc_running_median3_i32(INT32_MAX, &state) && state.median == INT32_MAX);
    CHECK(pc_running_median3_i32(0, &state) && state.median == 0);
    for (trial = 0u; trial < CASES; ++trial) {
        const int32_t sample = signed_bits(random_u32());
        int32_t expected;
        history[0] = history[1]; history[1] = history[2]; history[2] = sample;
        expected = median_reference(history, 3u);
        CHECK(pc_running_median3_i32(sample, &state));
        CHECK(state.median == expected);
        CHECK(state.count == 3u && state.next < 3u);
    }
    {
        pc_running_median3_i32_state bad = {{0,0,0},77,2u,1u}, before = bad;
        CHECK(!pc_running_median3_i32(1, &bad));
        CHECK(bad.samples[0] == before.samples[0] &&
              bad.samples[1] == before.samples[1] &&
              bad.samples[2] == before.samples[2] &&
              bad.median == before.median && bad.next == before.next &&
              bad.count == before.count);
    }
    CHECK(!pc_running_median3_i32(1, NULL));
    memset(&state, 0, sizeof state);
    CHECK(pc_running_median3_i32(42, &state) && state.median == 42 && state.count == 1u);
    return 0;
}

static int test_weighted_fusion(void)
{
    uint32_t trial;
    int32_t out = 777;
    CHECK(!pc_weighted_sensor_fuse_i32(1,0u,2,0u,&out) && out == 777);
    CHECK(!pc_weighted_sensor_fuse_i32(1,UINT32_MAX,2,1u,&out) && out == 777);
    CHECK(!pc_weighted_sensor_fuse_i32(1,1u,2,1u,NULL));
    CHECK(pc_weighted_sensor_fuse_i32(-1,1u,0,1u,&out) && out == -1);
    CHECK(pc_weighted_sensor_fuse_i32(0,1u,1,1u,&out) && out == 1);
    CHECK(pc_weighted_sensor_fuse_i32(0,1u,1,1u,&out) && out == 1);
    CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,INT32_MAX,INT32_MAX,INT32_MAX,&out) && out == -1);
    CHECK(pc_weighted_sensor_fuse_i32(INT32_MIN,INT32_MAX,INT32_MIN,INT32_MAX,&out) && out == INT32_MIN);
    for (trial = 0u; trial < CASES; ++trial) {
        const int32_t a = signed_bits(random_u32()), b = signed_bits(random_u32());
        uint32_t wa = random_u32() % 10001u, wb = random_u32() % 10001u;
        if (wa == 0u && wb == 0u) wb = 1u;
        const int64_t numerator = (int64_t)a * wa + (int64_t)b * wb;
        const int64_t denominator = (int64_t)wa + wb;
        const long double exact = (long double)numerator / (long double)denominator;
        const int32_t expected = (int32_t)roundl(exact);
        CHECK(pc_weighted_sensor_fuse_i32(a,wa,b,wb,&out) && out == expected);
    }
    return 0;
}

int main(void)
{
    CHECK(test_histogram_total() == 0);
    CHECK(test_cdf() == 0);
    CHECK(test_percentiles() == 0);
    CHECK(test_entropy() == 0);
    CHECK(test_equalize() == 0);
    CHECK(test_clip() == 0);
    CHECK(test_range_valleys() == 0);
    CHECK(test_image_bin() == 0);
    CHECK(test_means() == 0);
    CHECK(test_medians() == 0);
    CHECK(test_running_median() == 0);
    CHECK(test_weighted_fusion() == 0);
    printf("ESTIMORA direct API audit: all 19 APIs PASS; entropy max abs error %.12g\n",
           max_entropy_error);
    return 0;
}
