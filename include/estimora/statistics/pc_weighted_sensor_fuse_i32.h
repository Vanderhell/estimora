#ifndef PRIMITIVECORE_WEIGHTED_SENSOR_FUSE_I32_H
#define PRIMITIVECORE_WEIGHTED_SENSOR_FUSE_I32_H
#include <stdbool.h>
#include <stdint.h>
/* Fuse two signed measurements by nonnegative relative weights <= INT32_MAX.
 * Rounds the exact weighted mean to nearest, ties away from zero. Both weights
 * zero or a null output pointer returns false without changing *out. */
bool rm_weighted_sensor_fuse_i32(int32_t value0, uint32_t weight0,
                                 int32_t value1, uint32_t weight1,
                                 int32_t *out);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_weighted_sensor_fuse_i32
#define pc_weighted_sensor_fuse_i32 rm_weighted_sensor_fuse_i32
#endif
#ifndef pc_weighted_sensor_fuse_i32
#define pc_weighted_sensor_fuse_i32 rm_weighted_sensor_fuse_i32
#endif
#ifndef PC_PRIMITIVECORE_WEIGHTED_SENSOR_FUSE_I32_H
#define PC_PRIMITIVECORE_WEIGHTED_SENSOR_FUSE_I32_H PRIMITIVECORE_WEIGHTED_SENSOR_FUSE_I32_H
#endif

