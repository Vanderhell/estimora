#ifndef ESTIMORA_STATISTICS_WEIGHTED_SENSOR_FUSE_I32_H
#define ESTIMORA_STATISTICS_WEIGHTED_SENSOR_FUSE_I32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fuse two signed measurements by nonnegative relative weights <= INT32_MAX.
 * Rounds the exact weighted mean to nearest, ties away from zero. Both weights
 * zero or a null output pointer returns false without changing *out. */
bool estimora_weighted_sensor_fuse_i32(int32_t value0, uint32_t weight0,
                                 int32_t value1, uint32_t weight1,
                                 int32_t *out);


#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_WEIGHTED_SENSOR_FUSE_I32_H */
