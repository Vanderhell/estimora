#include "pc_hist_valley_count_u32.h"
#include <stddef.h>

bool pc_hist_valley_count_u32(const uint32_t histogram[256],
                              uint32_t *valley_count)
{
    uint32_t count = 0u;
    size_t i = 1u;
    if (histogram == NULL || valley_count == NULL) return false;
    while (i < 255u) {
        size_t end = i;
        uint32_t level = histogram[i];
        while (end + 1u < 256u && histogram[end + 1u] == level) ++end;
        if (histogram[i - 1u] > level && end < 255u &&
            histogram[end + 1u] > level) {
            ++count;
        }
        i = end + 1u;
    }
    *valley_count = count;
    return true;
}
