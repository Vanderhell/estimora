#ifndef PC_HIST_EQUALIZE_U8_H
#define PC_HIST_EQUALIZE_U8_H

#include <stdbool.h>
#include <stdint.h>

/* Build a 256-entry grayscale equalization LUT from raw uint32 histogram
 * counts. Uses CDF-min normalization and round-nearest-even. A single-level
 * histogram produces the identity LUT. Histogram and LUT storage must not
 * overlap. Failure leaves lut_out unchanged. */
bool pc_hist_equalize_u8(const uint32_t histogram[256], uint8_t lut_out[256]);

#endif
