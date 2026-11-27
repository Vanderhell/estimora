#ifndef ESTIMORA_STATISTICS_HIST_ENTROPY_NORM_Q_H
#define ESTIMORA_STATISTICS_HIST_ENTROPY_NORM_Q_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/* Approximate normalized Shannon entropy of all 256 byte-intensity bins.
 * Q output uses caller fractional_bits 0..31; error from exact entropy / 8
 * is <= 2^-18 + 0.5*2^-fractional_bits. Failure preserves output. */
bool estimora_hist_entropy_norm_q(const uint32_t histogram[256],
                            unsigned fractional_bits,
                            uint32_t *entropy_q);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_HIST_ENTROPY_NORM_Q_H */
