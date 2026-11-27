#ifndef ESTIMORA_STATISTICS_HIST_VALLEY_COUNT_U32_H
#define ESTIMORA_STATISTICS_HIST_VALLEY_COUNT_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/* Count interior local-minimum plateaus in a fixed 256-bin histogram.
 * A plateau counts once only when both adjacent bins are strictly higher. */
bool estimora_hist_valley_count_u32(const uint32_t histogram[256],
                              uint32_t *valley_count);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_HIST_VALLEY_COUNT_U32_H */
