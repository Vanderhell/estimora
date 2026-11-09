#ifndef PC_HIST_LOW_PERCENTILE_U32_H
#define PC_HIST_LOW_PERCENTILE_U32_H

#include <stdbool.h>
#include <stdint.h>

/* Nearest-rank percentile of a 256-bin histogram. Percent is 0..100;
 * zero selects the minimum occupied bin. Failure leaves bin_out unchanged. */
bool pc_hist_low_percentile_u32(const uint32_t histogram[256],
                                uint32_t percent,
                                uint32_t *bin_out);

#endif
