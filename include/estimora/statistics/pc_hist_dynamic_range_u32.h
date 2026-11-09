#ifndef PC_HIST_DYNAMIC_RANGE_U32_H
#define PC_HIST_DYNAMIC_RANGE_U32_H

#include <stdbool.h>
#include <stdint.h>

/* Return highest occupied bin minus lowest occupied bin in a 256-bin
 * histogram. Empty input fails and leaves range_out unchanged. */
bool pc_hist_dynamic_range_u32(const uint32_t histogram[256],
                               uint32_t *range_out);

#endif
