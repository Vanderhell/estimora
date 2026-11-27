#ifndef ESTIMORA_STATISTICS_MEAN5_I32_H
#define ESTIMORA_STATISTICS_MEAN5_I32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exact arithmetic mean of five int32 values, rounded to nearest integer. */
int32_t estimora_mean5_i32(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_MEAN5_I32_H */
