#include "primitivecore/statistics/histogram/bounded/pc_hist_equalize_u8.h"
#include <limits.h>
#include <stddef.h>

bool pc_hist_equalize_u8(const uint32_t histogram[256], uint8_t lut_out[256])
{
    uint32_t total = 0u, cdf_min = 0u;
    uint32_t cumulative = 0u;
    size_t i;

    if (histogram == NULL || lut_out == NULL) return false;
    for (i = 0u; i < 256u; ++i) {
        if (total > UINT32_MAX - histogram[i]) return false;
        total += histogram[i];
        if (cdf_min == 0u && histogram[i] != 0u) cdf_min = total;
    }
    if (total == 0u) return false;
    if (total == cdf_min) {
        for (i = 0u; i < 256u; ++i) lut_out[i] = (uint8_t)i;
        return true;
    }

    for (i = 0u; i < 256u; ++i) {
        uint64_t numerator, denominator = (uint64_t)(total - cdf_min);
        uint64_t quotient, remainder;
        cumulative += histogram[i];
        if (cumulative <= cdf_min) {
            lut_out[i] = 0u;
            continue;
        }
        numerator = (uint64_t)(cumulative - cdf_min) * UINT64_C(255);
        quotient = numerator / denominator;
        remainder = numerator % denominator;
        if (remainder > denominator - remainder ||
            (remainder == denominator - remainder && (quotient & 1u) != 0u)) {
            ++quotient;
        }
        lut_out[i] = (uint8_t)quotient;
    }
    return true;
}
