#include "../include/pc_hist_low_percentile_u32.h"
#include <assert.h>
#include <stdint.h>

static uint32_t h[256];
static uint32_t state = UINT32_C(0x4447D15C);

static uint32_t next_u32(void)
{
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}

static uint32_t ref_percentile(uint32_t percent)
{
    uint64_t total = 0u, cumulative = 0u;
    for (size_t i = 0u; i < 256u; ++i) total += h[i];
    uint64_t target = percent == 0u ? 1u : (total * percent + 99u) / 100u;
    for (size_t i = 0u; i < 256u; ++i) {
        cumulative += h[i];
        if (cumulative >= target) return (uint32_t)i;
    }
    assert(0); return 0u;
}

int main(void)
{
    uint32_t bin = UINT32_C(0xDEADBEEF);
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    assert(!pc_hist_low_percentile_u32(h, 0u, &bin));
    assert(bin == UINT32_C(0xDEADBEEF));
    assert(!pc_hist_low_percentile_u32(h, 101u, &bin));
    assert(bin == UINT32_C(0xDEADBEEF));
    assert(!pc_hist_low_percentile_u32(NULL, 50u, &bin));
    assert(!pc_hist_low_percentile_u32(h, 50u, NULL));

    h[7] = 1u; h[200] = 99u;
    assert(pc_hist_low_percentile_u32(h, 0u, &bin) && bin == 7u);
    assert(pc_hist_low_percentile_u32(h, 1u, &bin) && bin == 7u);
    assert(pc_hist_low_percentile_u32(h, 2u, &bin) && bin == 200u);
    assert(pc_hist_low_percentile_u32(h, 100u, &bin) && bin == 200u);
    uint32_t previous = 0u;
    for (uint32_t p = 0u; p <= 100u; ++p) {
        assert(pc_hist_low_percentile_u32(h, p, &bin));
        assert(bin >= previous);
        previous = bin;
    }

    for (size_t i = 0u; i < 256u; ++i) h[i] = UINT32_MAX;
    assert(pc_hist_low_percentile_u32(h, 100u, &bin) && bin == 255u);
    for (uint32_t p = 0u; p <= 100u; ++p) {
        assert(pc_hist_low_percentile_u32(h, p, &bin));
        assert(bin == ref_percentile(p));
    }

    for (size_t k = 0u; k < 20000u; ++k) {
        uint32_t p = next_u32() % 101u;
        for (size_t i = 0u; i < 256u; ++i) h[i] = next_u32();
        assert(pc_hist_low_percentile_u32(h, p, &bin));
        assert(bin == ref_percentile(p));
    }
    return 0;
}
