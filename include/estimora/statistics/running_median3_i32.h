#ifndef ESTIMORA_STATISTICS_RUNNING_MEDIAN3_I32_H
#define ESTIMORA_STATISTICS_RUNNING_MEDIAN3_I32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int32_t samples[3]; int32_t median; uint8_t next; uint8_t count; } estimora_running_median3_i32_state;
/* Zero-initialize state. First two outputs pass through; thereafter median is the last-three median. */
bool estimora_running_median3_i32(int32_t sample, estimora_running_median3_i32_state *state);


#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_RUNNING_MEDIAN3_I32_H */
