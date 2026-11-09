#ifndef PRIMITIVECORE_MEDIAN_NETWORK5_I32_H
#define PRIMITIVECORE_MEDIAN_NETWORK5_I32_H
#include <stdint.h>
/* Return the middle order statistic of five int32 values using 7 comparators. */
int32_t pc_median_network5_i32(int32_t a, int32_t b, int32_t c,
                               int32_t d, int32_t e);
#endif
