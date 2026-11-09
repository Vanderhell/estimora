#include "pc_hist_valley_count_u32.h"
#include <stddef.h>
#include <assert.h>
#include <stdint.h>

static uint32_t h[256];
static uint32_t state = UINT32_C(0x4458A11C);

static uint32_t next_u32(void)
{
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}

static uint32_t reference(void)
{
    uint32_t count = 0u;
    for (size_t i = 1u; i < 255u;) {
        size_t end = i;
        while (end + 1u < 256u && h[end + 1u] == h[i]) ++end;
        if (end < 255u && h[i-1u] > h[i] && h[end+1u] > h[i]) ++count;
        i = end + 1u;
    }
    return count;
}

static void compare(void)
{
    uint32_t got = UINT32_MAX;
    assert(pc_hist_valley_count_u32(h, &got));
    assert(got == reference());
    assert(got <= 127u);
}

int main(void)
{
    uint32_t out = UINT32_C(0xDEADBEEF);
    assert(!pc_hist_valley_count_u32(NULL, &out));
    assert(out == UINT32_C(0xDEADBEEF));
    assert(!pc_hist_valley_count_u32(h, NULL));
    compare(); /* zero and constant histograms both have no valleys */

    /* Every interior position and plateau width; endpoints are not counted. */
    for (size_t pos = 1u; pos < 255u; ++pos) {
        for (size_t width = 1u; width <= 4u && pos + width < 255u; ++width) {
            for (size_t i = 0u; i < 256u; ++i) h[i] = 10u;
            for (size_t i = pos; i < pos + width; ++i) h[i] = 2u;
            compare();
            assert(reference() == 1u);
        }
    }
    h[0] = 1u; h[1] = 5u;
    compare();
    h[254] = 5u; h[255] = 1u;
    compare();

    /* Shift, positive scale, and reflection preserve plateau-valley count. */
    for (size_t k = 0u; k < 1000u; ++k) {
        for (size_t i = 0u; i < 256u; ++i) h[i] = next_u32() % 1000u;
        uint32_t base = reference();
        for (size_t i = 0u; i < 256u; ++i) h[i] += 1000u;
        assert(reference() == base);
        for (size_t i = 0u; i < 256u; ++i) h[i] *= 2u;
        assert(reference() == base);
        for (size_t i = 0u; i < 128u; ++i) {
            uint32_t t = h[i]; h[i] = h[255u-i]; h[255u-i] = t;
        }
        assert(reference() == base);
        compare();
    }

    for (size_t k = 0u; k < 20000u; ++k) {
        for (size_t i = 0u; i < 256u; ++i)
            h[i] = (next_u32() % 5u == 0u) ? (uint32_t)(i % 7u) : next_u32() % 1000u;
        compare();
    }
    return 0;
}
