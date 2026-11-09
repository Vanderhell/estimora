#ifndef PC_HIST_CLIP_REDISTRIBUTE_U32_H
#define PC_HIST_CLIP_REDISTRIBUTE_U32_H

#include <stdbool.h>
#include <stdint.h>

/* Clip 256 bins at a positive absolute limit, then redistribute excess
 * uniformly. Residual counts go to the lowest bin indices. Exact alias is
 * allowed; failure leaves output unchanged. */
bool pc_hist_clip_redistribute_u32(const uint32_t histogram[256],
                                   uint32_t limit,
                                   uint32_t output[256]);

#endif
