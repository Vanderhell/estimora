#include "../include/pc_hist_cdf_normalize_q.h"
#include "roundmath/pc_hist_cdf_u32.h"
#include <assert.h>
#include <stdint.h>

static uint32_t state = UINT32_C(0x4442CDF0);
static uint32_t cdf[32], out[32], in_place[32];
static uint32_t next_u32(void)
{
    state ^= state << 13u; state ^= state >> 17u; state ^= state << 5u;
    return state;
}

static uint32_t reference(uint32_t value, uint32_t total, unsigned bits)
{
    const uint64_t scaled = (uint64_t)value * (UINT64_C(1) << bits);
    uint64_t q = scaled / total, r = scaled % total;
    if (r > (uint64_t)total - r ||
        (r == (uint64_t)total - r && (q & 1u) != 0u)) ++q;
    return (uint32_t)q;
}

int main(void)
{
    const uint32_t max_cdf[] = {1u, UINT32_MAX};
    const uint32_t invalid_cdf[] = {2u, 1u};
    const uint32_t raw_bins[] = {1u, 2u, 1u};
    uint32_t raw_cdf[3], normalized[3];
    for (size_t n = 1u; n <= 5u; ++n) {
        size_t combinations = 1u;
        for (size_t k = 0u; k < n; ++k) combinations *= 4u;
        for (size_t code = 0u; code < combinations; ++code) {
            size_t value = code;
            uint32_t total = 0u;
            for (size_t i = 0u; i < n; ++i) {
                total += (uint32_t)(value % 4u);
                value /= 4u;
                cdf[i] = total;
            }
            if (total == 0u) continue;
            for (unsigned bits = 0u; bits <= 31u; ++bits) {
                assert(pc_hist_cdf_normalize_q(cdf, n, bits, out));
                for (size_t i = 0u; i < n; ++i)
                    assert(out[i] == reference(cdf[i], total, bits));
            }
        }
    }
    assert(pc_hist_cdf_normalize_q(max_cdf, 2u, 31u, out));
    assert(out[0] == 1u && out[1] == UINT32_C(2147483648));
    out[0] = 77u; out[1] = 88u;
    assert(!pc_hist_cdf_normalize_q(invalid_cdf, 2u, 16u, out));
    assert(out[0] == 77u && out[1] == 88u);
    assert(!pc_hist_cdf_normalize_q(max_cdf, 0u, 16u, out));
    assert(!pc_hist_cdf_normalize_q(max_cdf, 2u, 32u, out));
    assert(!pc_hist_cdf_normalize_q(NULL, 1u, 16u, out));
    assert(!pc_hist_cdf_normalize_q(max_cdf, 2u, 16u, NULL));
    assert(!pc_hist_cdf_normalize_q((const uint32_t[]){0u}, 1u, 16u, out));
    in_place[0] = 1u; in_place[1] = 3u;
    assert(pc_hist_cdf_normalize_q(in_place, 2u, 8u, in_place));
    assert(in_place[0] == 85u && in_place[1] == 256u);
    assert(pc_hist_cdf_u32(raw_bins, 3u, raw_cdf));
    assert(pc_hist_cdf_normalize_q(raw_cdf, 3u, 8u, normalized));
    assert(normalized[0] == 64u && normalized[1] == 192u &&
           normalized[2] == 256u);
    for (uint32_t trial = 0u; trial < 50000u; ++trial) {
        const size_t n = (size_t)(next_u32() % 32u) + 1u;
        uint32_t total = 0u;
        const unsigned bits = next_u32() % 32u;
        for (size_t i = 0u; i < n; ++i) {
            total += next_u32() % 100000u;
            cdf[i] = total;
        }
        assert(pc_hist_cdf_normalize_q(cdf, n, bits, out));
        for (size_t i = 0u; i < n; ++i)
            assert(out[i] == reference(cdf[i], total, bits));
    }
    return 0;
}
