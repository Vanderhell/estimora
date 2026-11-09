#ifndef PRIMITIVECORE_HISTOGRAM_TOTAL_U32_H
#define PRIMITIVECORE_HISTOGRAM_TOTAL_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Sum count bins without wrapping. A zero-length histogram has total zero and
 * may use a null bins pointer. Returns false for null output, a null nonempty
 * input, or U32 overflow; failure preserves a nonnull output value. */
bool rm_histogram_total_u32(const uint32_t *bins, size_t bin_count,
                            uint32_t *total_out);
#endif

/* Canonical facade; the legacy implementation and contract are preserved. */
#ifndef pc_histogram_total_u32
#define pc_histogram_total_u32 rm_histogram_total_u32
#endif
#ifndef pc_histogram_total_u32
#define pc_histogram_total_u32 rm_histogram_total_u32
#endif
#ifndef PC_PRIMITIVECORE_HISTOGRAM_TOTAL_U32_H
#define PC_PRIMITIVECORE_HISTOGRAM_TOTAL_U32_H PRIMITIVECORE_HISTOGRAM_TOTAL_U32_H
#endif

