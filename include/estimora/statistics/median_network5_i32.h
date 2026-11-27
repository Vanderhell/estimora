#ifndef ESTIMORA_STATISTICS_MEDIAN_NETWORK5_I32_H
#define ESTIMORA_STATISTICS_MEDIAN_NETWORK5_I32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return the middle order statistic of five int32 values using 7 comparators. */
int32_t estimora_median_network5_i32(int32_t a, int32_t b, int32_t c,
                               int32_t d, int32_t e);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_MEDIAN_NETWORK5_I32_H */
