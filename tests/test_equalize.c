#include "../include/pc_hist_equalize_u8.h"
#include <assert.h>
#include <stdint.h>

static uint32_t histogram[256];
static uint8_t lut[256], expected[256];
static uint32_t state = UINT32_C(0x4443E0A1);

static uint32_t next_u32(void)
{
    state ^= state << 13u; state ^= state >> 17u; state ^= state << 5u;
    return state;
}

static uint8_t reference_round_even(uint64_t numerator, uint64_t denominator)
{
    uint64_t q = numerator / denominator, r = numerator % denominator;
    if (r > denominator - r ||
        (r == denominator - r && (q & 1u) != 0u)) ++q;
    return (uint8_t)q;
}

static void check_reference(const uint32_t h[256])
{
    uint64_t total = 0u, cdf_min = 0u, cumulative = 0u;
    size_t first = 256u;
    for (size_t i = 0u; i < 256u; ++i) {
        total += h[i];
        if (first == 256u && h[i] != 0u) {
            first = i;
            cdf_min = h[i];
        }
    }
    assert(pc_hist_equalize_u8(h, lut));
    if (total == cdf_min) {
        for (size_t i = 0u; i < 256u; ++i) {
            assert(lut[i] == i);
            if (i != 0u) assert(lut[i] >= lut[i - 1u]);
        }
        return;
    }
    for (size_t i = 0u; i < 256u; ++i) {
        cumulative += h[i];
        if (cumulative <= cdf_min) expected[i] = 0u;
        else expected[i] = reference_round_even((cumulative - cdf_min) * 255u,
                                                total - cdf_min);
        assert(lut[i] == expected[i]);
        if (i != 0u) assert(lut[i] >= lut[i - 1u]);
    }
}

int main(void)
{
    for (uint32_t code = 1u; code < 256u; ++code) {
        for (size_t i = 0u; i < 256u; ++i) histogram[i] = 0u;
        uint32_t value = code;
        for (size_t i = 0u; i < 4u; ++i) {
            histogram[i] = value % 4u;
            value /= 4u;
        }
        check_reference(histogram);
    }
    for (size_t i = 0u; i < 256u; ++i) histogram[i] = 1u;
    check_reference(histogram);
    for (size_t i = 0u; i < 256u; ++i) histogram[i] = 0u;
    histogram[123] = UINT32_MAX;
    check_reference(histogram);

    for (size_t i = 0u; i < 256u; ++i) histogram[i] = 0u;
    histogram[10] = 1u; histogram[11] = 1u; histogram[12] = 1u;
    check_reference(histogram);
    assert(lut[10] == 0u && lut[11] == 128u && lut[12] == 255u);
    {
        const uint8_t pixels[3] = {10u, 11u, 12u};
        const uint8_t equalized[3] = {lut[pixels[0]], lut[pixels[1]], lut[pixels[2]]};
        assert(equalized[0] == 0u && equalized[1] == 128u && equalized[2] == 255u);
    }

    for (uint32_t trial = 0u; trial < 20000u; ++trial) {
        uint64_t total = 0u;
        for (size_t i = 0u; i < 256u; ++i) {
            histogram[i] = next_u32() % 1000u;
            total += histogram[i];
        }
        if (total == 0u) histogram[next_u32() % 256u] = 1u;
        check_reference(histogram);
    }

    for (size_t i = 0u; i < 256u; ++i) histogram[i] = 0u;
    histogram[0] = UINT32_MAX; histogram[1] = 1u;
    for (size_t i = 0u; i < 256u; ++i) lut[i] = 0xA5u;
    assert(!pc_hist_equalize_u8(histogram, lut));
    for (size_t i = 0u; i < 256u; ++i) assert(lut[i] == 0xA5u);
    for (size_t i = 0u; i < 256u; ++i) histogram[i] = 0u;
    assert(!pc_hist_equalize_u8(histogram, lut));
    for (size_t i = 0u; i < 256u; ++i) assert(lut[i] == 0xA5u);
    assert(!pc_hist_equalize_u8(NULL, lut));
    assert(!pc_hist_equalize_u8(histogram, NULL));
    return 0;
}
