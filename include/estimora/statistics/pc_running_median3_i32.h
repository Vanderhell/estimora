#ifndef RM_RUNNING_MEDIAN3_I32_H
#define RM_RUNNING_MEDIAN3_I32_H
#include <stdbool.h>
#include <stdint.h>
typedef struct { int32_t samples[3]; int32_t median; uint8_t next; uint8_t count; } rm_running_median3_i32_state;
/* Zero-initialize state. First two outputs pass through; thereafter median is the last-three median. */
bool rm_running_median3_i32(int32_t sample, rm_running_median3_i32_state *state);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_running_median3_i32
#define pc_running_median3_i32 rm_running_median3_i32
#endif
#ifndef pc_running_median3_i32
#define pc_running_median3_i32 rm_running_median3_i32
#endif
#ifndef pc_running_median3_i32_state
#define pc_running_median3_i32_state rm_running_median3_i32_state
#endif
#ifndef PC_RUNNING_MEDIAN3_I32_H
#define PC_RUNNING_MEDIAN3_I32_H RM_RUNNING_MEDIAN3_I32_H
#endif

