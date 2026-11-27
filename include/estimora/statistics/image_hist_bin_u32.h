#ifndef ESTIMORA_STATISTICS_IMAGE_HIST_BIN_U32_H
#define ESTIMORA_STATISTICS_IMAGE_HIST_BIN_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Uniformly map a uint8 sample over [0,256) to one of bin_count bins.
 * False for null output or bin_count==0; failure preserves *bin. */
bool estimora_image_hist_bin_u32(uint8_t value, uint32_t bin_count, uint32_t *bin);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_IMAGE_HIST_BIN_U32_H */
