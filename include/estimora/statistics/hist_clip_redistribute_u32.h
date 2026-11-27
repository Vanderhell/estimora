#ifndef ESTIMORA_STATISTICS_HIST_CLIP_REDISTRIBUTE_U32_H
#define ESTIMORA_STATISTICS_HIST_CLIP_REDISTRIBUTE_U32_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/* Clip 256 bins at a positive absolute limit, then redistribute excess
 * uniformly. Residual counts go to the lowest bin indices. Exact alias is
 * allowed; failure leaves output unchanged. */
bool estimora_hist_clip_redistribute_u32(const uint32_t histogram[256],
                                   uint32_t limit,
                                   uint32_t output[256]);

#ifdef __cplusplus
}
#endif

#endif /* ESTIMORA_STATISTICS_HIST_CLIP_REDISTRIBUTE_U32_H */
