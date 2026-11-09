#ifndef PC_HIST_ENTROPY_NOPC_Q_H
#define PC_HIST_ENTROPY_NOPC_Q_H

#include <stdbool.h>
#include <stdint.h>

/* Approximate normalized Shannon entropy of all 256 byte-intensity bins.
 * Q output uses caller fractional_bits 0..31; error from exact entropy / 8
 * is <= 2^-18 + 0.5*2^-fractional_bits. Failure preserves output. */
bool pc_hist_entropy_norm_q(const uint32_t histogram[256],
                            unsigned fractional_bits,
                            uint32_t *entropy_q);

#endif
