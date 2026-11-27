#ifndef ESTIMORA_STATISTICS_HISTOGRAM_TOTAL_U32_H
#define ESTIMORA_STATISTICS_HISTOGRAM_TOTAL_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Sum count bins without wrapping. A zero-length histogram has total zero and
 * may use a null bins pointer. Returns false for null output, a null nonempty
 * input, or U32 overflow; failure preserves a nonnull output value. */
bool estimora_histogram_total_u32(const uint32_t *bins, size_t bin_count,
                            uint32_t *total_out);


#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_HISTOGRAM_TOTAL_U32_H */
