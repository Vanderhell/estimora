#ifndef PC_HIST_CDF_U32_H
#define PC_HIST_CDF_U32_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Write raw cumulative counts: cdf[i] = sum(bins[0..i]). The output may
 * exactly alias bins or be disjoint; partial overlap is not supported.
 * Empty input succeeds without dereferencing either pointer. Failure leaves
 * output unchanged. */
bool pc_hist_cdf_u32(const uint32_t *bins, size_t bin_count,
                     uint32_t *cdf_out);

#endif
