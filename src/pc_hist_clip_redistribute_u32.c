#include "pc_hist_clip_redistribute_u32.h"
#include <stddef.h>

bool pc_hist_clip_redistribute_u32(const uint32_t histogram[256],
                                   uint32_t limit,
                                   uint32_t output[256])
{
    uint64_t excess = 0u, batch;
    uint32_t residual;
    size_t i;

    if (histogram == NULL || output == NULL || limit == 0u) return false;
    for (i = 0u; i < 256u; ++i) {
        if (histogram[i] > limit)
            excess += (uint64_t)(histogram[i] - limit);
    }
    batch = excess / UINT64_C(256);
    residual = (uint32_t)(excess % UINT64_C(256));
    for (i = 0u; i < 256u; ++i) {
        uint64_t value = histogram[i] > limit ? limit : histogram[i];
        value += batch;
        if (i < residual) ++value;
        /* Bound follows from excess <= 256*(UINT32_MAX-limit). */
        output[i] = (uint32_t)value;
    }
    return true;
}