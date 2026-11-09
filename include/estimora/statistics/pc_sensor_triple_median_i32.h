#ifndef RM_SENSOR_TRIPLE_MEDIAN_I32_H
#define RM_SENSOR_TRIPLE_MEDIAN_I32_H
#include <stdint.h>
/* Stateless median of three simultaneous signed sensor values. */
int32_t rm_sensor_triple_median_i32(int32_t value0, int32_t value1, int32_t value2);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_sensor_triple_median_i32
#define pc_sensor_triple_median_i32 rm_sensor_triple_median_i32
#endif
#ifndef pc_sensor_triple_median_i32
#define pc_sensor_triple_median_i32 rm_sensor_triple_median_i32
#endif
#ifndef PC_SENSOR_TRIPLE_MEDIAN_I32_H
#define PC_SENSOR_TRIPLE_MEDIAN_I32_H RM_SENSOR_TRIPLE_MEDIAN_I32_H
#endif

