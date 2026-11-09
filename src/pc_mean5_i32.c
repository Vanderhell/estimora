#include "pc_mean5_i32.h"
#include <stdint.h>

int32_t pc_mean5_i32(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e)
{
    int64_t sum = (int64_t)a + (int64_t)b + (int64_t)c + (int64_t)d + (int64_t)e;
    int64_t quotient = sum / INT64_C(5);
    int64_t remainder = sum % INT64_C(5);
    if (remainder >= INT64_C(3)) ++quotient;
    else if (remainder <= -INT64_C(3)) --quotient;
    return (int32_t)quotient;
}