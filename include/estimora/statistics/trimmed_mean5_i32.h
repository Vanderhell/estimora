#ifndef ESTIMORA_STATISTICS_TRIMMED_MEAN5_I32_H
#define ESTIMORA_STATISTICS_TRIMMED_MEAN5_I32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Drop one minimum and maximum, then round the mean of the middle three. */
int32_t estimora_trimmed_mean5_i32(int32_t a, int32_t b, int32_t c,
                             int32_t d, int32_t e);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_TRIMMED_MEAN5_I32_H */
