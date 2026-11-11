#ifdef NDEBUG
#undef NDEBUG
#endif
#include "pc_hist_cdf_u32.h"
#include <stddef.h>
#include <assert.h>
#include <stdint.h>

static uint32_t rng_state = UINT32_C(0x4441CDF0);
static uint32_t next_u32(void)
{
    rng_state ^= rng_state << 13u;
    rng_state ^= rng_state >> 17u;
    rng_state ^= rng_state << 5u;
    return rng_state;
}

static uint32_t bins[16], out[16], inplace[16];

static void exhaustive_small(void)
{
    size_t n, code, combinations;
    for (n = 0u; n <= 6u; ++n) {
        combinations = 1u;
        for (size_t k = 0u; k < n; ++k) combinations *= 4u;
        for (code = 0u; code < combinations; ++code) {
            size_t value = code;
            uint32_t sum = 0u;
            for (size_t i = 0u; i < n; ++i) {
                bins[i] = (uint32_t)(value % 4u);
                value /= 4u;
            }
            assert(pc_hist_cdf_u32(n == 0u ? NULL : bins, n,
                                   n == 0u ? NULL : out));
            for (size_t i = 0u; i < n; ++i) {
                sum += bins[i];
                assert(out[i] == sum);
            }
        }
    }
}

int main(void)
{
    const uint32_t max_ok[] = {UINT32_MAX, 0u};
    const uint32_t overflow[] = {UINT32_MAX, 1u};
    uint64_t total;
    exhaustive_small();
    assert(pc_hist_cdf_u32(max_ok, 2u, out));
    assert(out[0] == UINT32_MAX && out[1] == UINT32_MAX);
    out[0] = 55u; out[1] = 66u;
    assert(!pc_hist_cdf_u32(overflow, 2u, out));
    assert(out[0] == 55u && out[1] == 66u);
    assert(!pc_hist_cdf_u32(NULL, 1u, out));
    assert(!pc_hist_cdf_u32(max_ok, 1u, NULL));
    assert(pc_hist_cdf_u32(NULL, 0u, NULL));

    for (uint32_t trial = 0u; trial < 100000u; ++trial) {
        const size_t n = (size_t)(next_u32() % 17u);
        total = 0u;
        for (size_t i = 0u; i < n; ++i) {
            bins[i] = next_u32();
            total += bins[i];
            out[i] = UINT32_C(0xA55AA55A);
        }
        if (total <= UINT32_MAX) {
            uint32_t sum = 0u;
            assert(pc_hist_cdf_u32(n == 0u ? NULL : bins, n,
                                   n == 0u ? NULL : out));
            for (size_t i = 0u; i < n; ++i) {
                sum += bins[i];
                assert(out[i] == sum);
            }
        } else {
            assert(!pc_hist_cdf_u32(bins, n, out));
            for (size_t i = 0u; i < n; ++i)
                assert(out[i] == UINT32_C(0xA55AA55A));
        }
    }

    inplace[0] = 1u; inplace[1] = 2u; inplace[2] = 3u;
    assert(pc_hist_cdf_u32(inplace, 3u, inplace));
    assert(inplace[0] == 1u && inplace[1] == 3u && inplace[2] == 6u);
    return 0;
}
