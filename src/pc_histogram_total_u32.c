#include "pc_histogram_total_u32.h"

bool rm_histogram_total_u32(const uint32_t *bins, size_t bin_count,
                            uint32_t *total_out)
{
    uint32_t total = 0u;
    size_t i;

    if ((total_out == NULL) || ((bins == NULL) && (bin_count != 0u))) {
        return false;
    }
    for (i = 0u; i < bin_count; ++i) {
        if (total > UINT32_MAX - bins[i]) {
            return false;
        }
        total += bins[i];
    }
    *total_out = total;
    return true;
}