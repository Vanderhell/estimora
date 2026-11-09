#ifndef PC_HIST_VALLEY_COUNT_U32_H
#define PC_HIST_VALLEY_COUNT_U32_H

#include <stdbool.h>
#include <stdint.h>

/* Count interior local-minimum plateaus in a fixed 256-bin histogram.
 * A plateau counts once only when both adjacent bins are strictly higher. */
bool pc_hist_valley_count_u32(const uint32_t histogram[256],
                              uint32_t *valley_count);

#endif
