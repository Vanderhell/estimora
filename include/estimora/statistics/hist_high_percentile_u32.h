#ifndef ESTIMORA_STATISTICS_HIST_HIGH_PERCENTILE_U32_H
#define ESTIMORA_STATISTICS_HIST_HIGH_PERCENTILE_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/* Upper-tail nearest-rank percentile of a 256-bin histogram. Percent is
 * 0..100; zero selects the maximum occupied bin. Failure preserves output. */
bool estimora_hist_high_percentile_u32(const uint32_t histogram[256],
                                 uint32_t percent,
                                 uint32_t *bin_out);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_HIST_HIGH_PERCENTILE_U32_H */
