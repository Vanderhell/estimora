#include <primitivecore/statistics/descriptive/moments/pc_weighted_sensor_fuse_i32.h>
#include <limits.h>

bool rm_weighted_sensor_fuse_i32(int32_t value0, uint32_t weight0,
                                 int32_t value1, uint32_t weight1,
                                 int32_t *out)
{
    int64_t numerator, denominator, quotient, remainder;
    if (out == 0 || weight0 > (uint32_t)INT32_MAX || weight1 > (uint32_t)INT32_MAX ||
        (weight0 == 0 && weight1 == 0)) return false;
    numerator = (int64_t)value0 * (int64_t)weight0 +
                (int64_t)value1 * (int64_t)weight1;
    denominator = (int64_t)weight0 + (int64_t)weight1;
    quotient = numerator / denominator;
    remainder = numerator % denominator;
    if (remainder >= 0) {
        if (remainder * 2 >= denominator) ++quotient;
    } else if (-remainder * 2 >= denominator) {
        --quotient;
    }
    *out = (int32_t)quotient;
    return true;
}
