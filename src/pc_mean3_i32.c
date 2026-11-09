#include "primitivecore/statistics/descriptive/pc_mean3_i32.h"
#include <stdint.h>

int32_t pc_mean3_i32(int32_t a, int32_t b, int32_t c)
{
    int64_t sum = (int64_t)a + (int64_t)b + (int64_t)c;
    int64_t quotient = sum / INT64_C(3);
    int64_t remainder = sum % INT64_C(3);
    if (remainder >= INT64_C(2)) ++quotient;
    else if (remainder <= -INT64_C(2)) --quotient;
    return (int32_t)quotient;
}
