#include "pc_image_hist_bin_u32.h"
#include <stddef.h>

bool pc_image_hist_bin_u32(uint8_t value, uint32_t bin_count, uint32_t *bin)
{
    uint32_t result;
    if (bin == NULL || bin_count == 0u) return false;
    result = (uint32_t)(((uint64_t)value * (uint64_t)bin_count) / UINT64_C(256));
    *bin = result;
    return true;
}