#include "pc_hist_dynamic_range_u32.h"
#include <stddef.h>

bool pc_hist_dynamic_range_u32(const uint32_t histogram[256],
                               uint32_t *range_out)
{
    size_t low = 0u, high = 256u;
    if (histogram == NULL || range_out == NULL) return false;
    while (low < 256u && histogram[low] == 0u) ++low;
    if (low == 256u) return false;
    while (high > 0u && histogram[high - 1u] == 0u) --high;
    *range_out = (uint32_t)((high - 1u) - low);
    return true;
}
