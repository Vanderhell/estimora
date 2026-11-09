#include "../include/pc_hist_clip_redistribute_u32.h"
#include <assert.h>
#include <stdint.h>

static uint32_t h[256], out[256];
static uint32_t state = UINT32_C(0x4446C0DE);

static uint32_t next_u32(void)
{
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}

static void verify(uint32_t limit)
{
    uint64_t total = 0u, excess = 0u, got = 0u;
    uint64_t batch;
    uint32_t residual;
    assert(pc_hist_clip_redistribute_u32(h, limit, out));
    for (size_t i = 0u; i < 256u; ++i) {
        total += h[i];
        if (h[i] > limit) excess += (uint64_t)(h[i] - limit);
    }
    batch = excess / 256u;
    residual = (uint32_t)(excess % 256u);
    for (size_t i = 0u; i < 256u; ++i) {
        uint64_t expected = h[i] > limit ? limit : h[i];
        expected += batch + (i < residual ? 1u : 0u);
        assert(out[i] == expected);
        got += out[i];
    }
    assert(got == total);
}

int main(void)
{
    uint32_t sentinel[256];
    for (size_t i = 0u; i < 256u; ++i) {
        h[i] = UINT32_MAX;
        out[i] = 0xA5A5A5A5u;
        sentinel[i] = out[i];
    }
    assert(!pc_hist_clip_redistribute_u32(h, 0u, out));
    for (size_t i = 0u; i < 256u; ++i) assert(out[i] == sentinel[i]);
    assert(!pc_hist_clip_redistribute_u32(NULL, 1u, out));
    assert(!pc_hist_clip_redistribute_u32(h, 1u, NULL));

    verify(UINT32_MAX);
    verify(UINT32_MAX - 1u);
    verify(1u);
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    h[0] = 100u;
    verify(1u);
    assert(out[0] == 2u && out[98] == 1u && out[99] == 0u);
    for (size_t i = 0u; i < 256u; ++i) h[i] = UINT32_MAX;
    assert(pc_hist_clip_redistribute_u32(h, UINT32_MAX - 1u, h));
    for (size_t i = 0u; i < 256u; ++i) assert(h[i] == UINT32_MAX);

    for (size_t k = 0u; k < 20000u; ++k) {
        uint32_t limit = next_u32() | 1u;
        for (size_t i = 0u; i < 256u; ++i) h[i] = next_u32();
        verify(limit);
    }
    for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
    verify(1u);
    return 0;
}
