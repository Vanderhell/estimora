#include "pc_hist_low_percentile_u32.h"
#include <stddef.h>

bool pc_hist_low_percentile_u32(const uint32_t histogram[256],
                                uint32_t percent,
                                uint32_t *bin_out)
{
    uint64_t total = 0u, target, cumulative = 0u;
    size_t i;
    if (histogram == NULL || bin_out == NULL || percent > 100u) return false;
    for (i = 0u; i < 256u; ++i) total += histogram[i];
    if (total == 0u) return false;
    target = percent == 0u ? UINT64_C(1) :
             (total * percent + UINT64_C(99)) / UINT64_C(100);
    for (i = 0u; i < 256u; ++i) {
        cumulative += histogram[i];
        if (cumulative >= target) {
            *bin_out = (uint32_t)i;
            return true;
        }
    }
    return false;
}
