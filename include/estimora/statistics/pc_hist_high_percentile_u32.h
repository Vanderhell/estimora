#ifndef PC_HIST_HIGH_PERCENTILE_U32_H
#define PC_HIST_HIGH_PERCENTILE_U32_H

#include <stdbool.h>
#include <stdint.h>

/* Upper-tail nearest-rank percentile of a 256-bin histogram. Percent is
 * 0..100; zero selects the maximum occupied bin. Failure preserves output. */
bool pc_hist_high_percentile_u32(const uint32_t histogram[256],
                                 uint32_t percent,
                                 uint32_t *bin_out);

#endif
