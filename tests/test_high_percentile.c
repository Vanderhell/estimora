#include "../include/pc_hist_high_percentile_u32.h"
#include <assert.h>
#include <stdint.h>

static uint32_t h[256];
static uint32_t state = UINT32_C(0x4448A55A);

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
    for (size_t i = 256u; i > 0u; --i) {
        cumulative += h[i-1u];
        if (cumulative >= target) return (uint32_t)(i-1u);
    }
    assert(0); return 0u;
}

int main(void)
{
    uint32_t bin = UINT32_C(0xDEADBEEF);
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    assert(!pc_hist_high_percentile_u32(h, 0u, &bin));
    assert(bin == UINT32_C(0xDEADBEEF));
    assert(!pc_hist_high_percentile_u32(h, 101u, &bin));
    assert(!pc_hist_high_percentile_u32(NULL, 50u, &bin));
    assert(!pc_hist_high_percentile_u32(h, 50u, NULL));

    for (size_t i = 0u; i < 100u; ++i) h[i] = 1u;
    assert(pc_hist_high_percentile_u32(h, 0u, &bin) && bin == 99u);
    assert(pc_hist_high_percentile_u32(h, 1u, &bin) && bin == 99u);
    assert(pc_hist_high_percentile_u32(h, 99u, &bin) && bin == 1u);
    assert(pc_hist_high_percentile_u32(h, 100u, &bin) && bin == 0u);
    uint32_t previous = 255u;
    for (uint32_t p = 0u; p <= 100u; ++p) {
        assert(pc_hist_high_percentile_u32(h, p, &bin));
        assert(bin <= previous);
        previous = bin;
    }

    for (size_t i = 0u; i < 256u; ++i) h[i] = UINT32_MAX;
    for (uint32_t p = 0u; p <= 100u; ++p) {
        assert(pc_hist_high_percentile_u32(h, p, &bin));
        assert(bin == ref_percentile(p));
    }
    for (size_t k = 0u; k < 20000u; ++k) {
        uint32_t p = next_u32() % 101u;
        for (size_t i = 0u; i < 256u; ++i) h[i] = next_u32();
        assert(pc_hist_high_percentile_u32(h, p, &bin));
        assert(bin == ref_percentile(p));
    }
    return 0;
}
