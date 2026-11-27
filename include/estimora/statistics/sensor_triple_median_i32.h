#ifndef ESTIMORA_STATISTICS_SENSOR_TRIPLE_MEDIAN_I32_H
#define ESTIMORA_STATISTICS_SENSOR_TRIPLE_MEDIAN_I32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Stateless median of three simultaneous signed sensor values. */
int32_t estimora_sensor_triple_median_i32(int32_t value0, int32_t value1, int32_t value2);


#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_SENSOR_TRIPLE_MEDIAN_I32_H */
