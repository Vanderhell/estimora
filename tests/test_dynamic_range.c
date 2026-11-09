#include "pc_hist_dynamic_range_u32.h"
#include <stddef.h>
#include <assert.h>
#include <stdint.h>

static uint32_t h[256];
static uint32_t state = UINT32_C(0x4449A6E1);

static uint32_t next_u32(void)
{
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}

int main(void)
{
    uint32_t range = UINT32_C(0xDEADBEEF);
    assert(!pc_hist_dynamic_range_u32(NULL, &range));
    assert(!pc_hist_dynamic_range_u32(h, NULL));
    assert(!pc_hist_dynamic_range_u32(h, &range));
    assert(range == UINT32_C(0xDEADBEEF));

    for (size_t low = 0u; low < 256u; ++low) {
        for (size_t high = low; high < 256u; ++high) {
            for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
            h[low] = UINT32_MAX;
            h[high] = UINT32_MAX;
            assert(pc_hist_dynamic_range_u32(h, &range));
            assert(range == high - low);
        }
    }
    for (size_t k = 0u; k < 20000u; ++k) {
        for (size_t i = 0u; i < 256u; ++i) h[i] = 0u;
        size_t low = next_u32() % 256u, high = next_u32() % 256u;
        if (low > high) { size_t t = low; low = high; high = t; }
        h[low] = next_u32() | 1u;
        h[high] = next_u32() | 1u;
        assert(pc_hist_dynamic_range_u32(h, &range));
        assert(range == high - low);
    }
    return 0;
}
