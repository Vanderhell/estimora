#include "../include/pc_hist_contrast_stretch_u8.h"
#include <assert.h>
#include <stdint.h>

static uint32_t hist[256];
static uint8_t lut[256];

int main(void)
{
    size_t i;
    for (i = 0u; i < 256u; ++i) lut[i] = 0xA5u;
    assert(!pc_hist_contrast_stretch_u8(hist, lut));
    for (i = 0u; i < 256u; ++i) assert(lut[i] == 0xA5u);
    assert(!pc_hist_contrast_stretch_u8(NULL, lut));
    assert(!pc_hist_contrast_stretch_u8(hist, NULL));

    for (i = 0u; i < 256u; ++i) hist[i] = 0u;
    hist[91] = UINT32_MAX;
    assert(pc_hist_contrast_stretch_u8(hist, lut));
    for (i = 0u; i < 256u; ++i) assert(lut[i] == i);

    for (i = 0u; i < 256u; ++i) hist[i] = 0u;
    hist[10] = UINT32_MAX;
    hist[110] = UINT32_MAX;
    assert(pc_hist_contrast_stretch_u8(hist, lut));
    assert(lut[9] == 0u && lut[10] == 0u && lut[110] == 255u && lut[111] == 255u);
    assert(lut[11] == 3u); /* 2.55 rounds to 3 */
    assert(lut[12] == 5u); /* 5.1 rounds to 5 */
    for (i = 1u; i < 256u; ++i) assert(lut[i] >= lut[i-1u]);

    /* All distinct endpoint spans expose integer rounding and edge errors. */
    for (size_t low = 0u; low < 256u; ++low) {
        for (size_t high = low + 1u; high < 256u; ++high) {
            for (i = 0u; i < 256u; ++i) hist[i] = 0u;
            hist[low] = 1u;
            hist[high] = UINT32_MAX;
            assert(pc_hist_contrast_stretch_u8(hist, lut));
            for (i = 0u; i < 256u; ++i) {
                uint32_t q, r, d = (uint32_t)(high - low);
                if (i <= low) q = 0u;
                else if (i >= high) q = 255u;
                else {
                    uint32_t n = (uint32_t)(i - low) * 255u;
                    q = n / d; r = n % d;
                    if (r > d-r || (r == d-r && (q & 1u))) ++q;
                }
                assert(lut[i] == q);
            }
        }
    }
    for (i = 0u; i < 256u; ++i) hist[i] = 0u;
    hist[20] = 1u;
    hist[26] = 1u;
    assert(pc_hist_contrast_stretch_u8(hist, lut));
    assert(lut[21] == 42u); /* 42.5 ties to even */
    assert(lut[23] == 128u); /* 127.5 ties to even */
    return 0;
}
