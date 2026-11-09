#include "primitivecore/statistics/histogram/bounded/pc_hist_entropy_norm_q.h"
#include <stddef.h>

#define LOG_FRAC_BITS 24u
#define PROB_FRAC_BITS 31u
#define PROB_ONE (UINT64_C(1) << PROB_FRAC_BITS)

static uint64_t log2_u64_q24(uint64_t value)
{
    uint64_t t = value, y, z;
    uint64_t exponent = 0u;
    uint32_t fraction = 0u;
    for (; t > 1u; t >>= 1u) ++exponent;
    if (exponent <= PROB_FRAC_BITS)
        y = value << (PROB_FRAC_BITS - (uint32_t)exponent);
    else
        y = value >> ((uint32_t)exponent - PROB_FRAC_BITS);
    for (uint32_t bit = 0u; bit < LOG_FRAC_BITS; ++bit) {
        z = (y * y) >> PROB_FRAC_BITS;
        if (z >= (PROB_ONE << 1u)) {
            y = z >> 1u;
            fraction |= UINT32_C(1) << (LOG_FRAC_BITS - 1u - bit);
        } else {
            y = z;
        }
    }
    return (exponent << LOG_FRAC_BITS) | fraction;
}

static uint64_t ratio_q31_round_even(uint32_t numerator, uint64_t denominator)
{
    uint64_t scaled = (uint64_t)numerator << PROB_FRAC_BITS;
    uint64_t q = scaled / denominator, r = scaled % denominator;
    if (r > denominator - r ||
        (r == denominator - r && (q & 1u) != 0u)) ++q;
    return q;
}

bool pc_hist_entropy_norm_q(const uint32_t histogram[256],
                            unsigned fractional_bits,
                            uint32_t *entropy_q)
{
    uint64_t total = 0u, log_total, entropy_q24 = 0u;
    uint64_t denominator, numerator, quotient, remainder;
    if (histogram == NULL || entropy_q == NULL || fractional_bits > 31u)
        return false;
    for (size_t i = 0u; i < 256u; ++i) total += histogram[i];
    if (total == 0u) return false;
    log_total = log2_u64_q24(total);
    for (size_t i = 0u; i < 256u; ++i) {
        uint32_t count = histogram[i];
        if (count != 0u) {
            uint64_t probability = ratio_q31_round_even(count, total);
            uint64_t log_ratio = log_total - log2_u64_q24(count);
            entropy_q24 += (probability * log_ratio) >> PROB_FRAC_BITS;
        }
    }
    numerator = entropy_q24 << fractional_bits;
    denominator = UINT64_C(1) << (LOG_FRAC_BITS + 3u);
    quotient = numerator / denominator;
    remainder = numerator % denominator;
    if (remainder > denominator - remainder ||
        (remainder == denominator - remainder && (quotient & 1u) != 0u)) {
        ++quotient;
    }
    if (quotient > (UINT64_C(1) << fractional_bits))
        quotient = UINT64_C(1) << fractional_bits;
    *entropy_q = (uint32_t)quotient;
    return true;
}
