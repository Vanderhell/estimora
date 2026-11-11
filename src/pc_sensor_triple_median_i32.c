#include "pc_sensor_triple_median_i32.h"
int32_t pc_sensor_triple_median_i32(int32_t a, int32_t b, int32_t c)
{
    int32_t t;
    if (a > b) { t = a; a = b; b = t; }
    if (b > c) { t = b; b = c; c = t; }
    if (a > b) { t = a; a = b; b = t; }
    return b;
}
