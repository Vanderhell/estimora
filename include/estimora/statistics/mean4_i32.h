#ifndef ESTIMORA_STATISTICS_MEAN4_I32_H
#define ESTIMORA_STATISTICS_MEAN4_I32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exact arithmetic mean of four int32 values, rounded nearest-even. */
int32_t estimora_mean4_i32(int32_t a, int32_t b, int32_t c, int32_t d);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_MEAN4_I32_H */
