#ifndef PC_PC_IMAGE_HIST_BIN_U32_H
#define PC_PC_IMAGE_HIST_BIN_U32_H
#include <stdbool.h>
#include <stdint.h>
/* Uniformly map a uint8 sample over [0,256) to one of bin_count bins.
 * False for null output or bin_count==0; failure preserves *bin. */
bool pc_image_hist_bin_u32(uint8_t value, uint32_t bin_count, uint32_t *bin);
#endif
