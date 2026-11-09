#include "pc_hist_cdf_u32.h"
#include "pc_histogram_total_u32.h"

bool pc_hist_cdf_u32(const uint32_t *bins, size_t bin_count,
                     uint32_t *cdf_out)
{
    uint32_t total, cumulative = 0u;
    size_t i;

    if (bin_count == 0u) return true;
    if (bins == NULL || cdf_out == NULL ||
        !pc_histogram_total_u32(bins, bin_count, &total)) {
        return false;
    }
    (void)total;
    for (i = 0u; i < bin_count; ++i) {
        cumulative += bins[i];
        cdf_out[i] = cumulative;
    }
    return true;
}