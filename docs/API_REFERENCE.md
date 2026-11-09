# Estimora API reference

Complete public-header inventory generated from the current source tree. Headers remain authoritative.

## include/estimora/statistics/pc_hist_cdf_u32.h

Public API: pc_hist_cdf_u32

Contract: Write raw cumulative counts: cdf[i] = sum(bins[0..i]). The output may exactly alias bins or be disjoint; partial overlap is not supported. Empty input succeeds without dereferencing either pointer. Failure leaves output unchanged.

Header path: include/estimora/statistics/pc_hist_cdf_u32.h

## include/estimora/statistics/pc_hist_clip_redistribute_u32.h

Public API: pc_hist_clip_redistribute_u32

Contract: Clip 256 bins at a positive absolute limit, then redistribute excess uniformly. Residual counts go to the lowest bin indices. Exact alias is allowed; failure leaves output unchanged.

Header path: include/estimora/statistics/pc_hist_clip_redistribute_u32.h

## include/estimora/statistics/pc_hist_dynamic_range_u32.h

Public API: pc_hist_dynamic_range_u32

Contract: Return highest occupied bin minus lowest occupied bin in a 256-bin histogram. Empty input fails and leaves range_out unchanged.

Header path: include/estimora/statistics/pc_hist_dynamic_range_u32.h

## include/estimora/statistics/pc_hist_entropy_norm_q.h

Public API: pc_hist_entropy_norm_q

Contract: Approximate normalized Shannon entropy of all 256 byte-intensity bins. Q output uses caller fractional_bits 0..31; error from exact entropy / 8 is <= 2^-18 + 0.5*2^-fractional_bits. Failure preserves output.

Header path: include/estimora/statistics/pc_hist_entropy_norm_q.h

## include/estimora/statistics/pc_hist_equalize_u8.h

Public API: pc_hist_equalize_u8

Contract: Build a 256-entry grayscale equalization LUT from raw uint32 histogram counts. Uses CDF-min normalization and round-nearest-even. A single-level histogram produces the identity LUT. Histogram and LUT storage must not overlap. Failure leaves lut_out unchanged.

Header path: include/estimora/statistics/pc_hist_equalize_u8.h

## include/estimora/statistics/pc_hist_high_percentile_u32.h

Public API: pc_hist_high_percentile_u32

Contract: Upper-tail nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the maximum occupied bin. Failure preserves output.

Header path: include/estimora/statistics/pc_hist_high_percentile_u32.h

## include/estimora/statistics/pc_hist_low_percentile_u32.h

Public API: pc_hist_low_percentile_u32

Contract: Nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the minimum occupied bin. Failure leaves bin_out unchanged.

Header path: include/estimora/statistics/pc_hist_low_percentile_u32.h

## include/estimora/statistics/pc_hist_valley_count_u32.h

Public API: pc_hist_valley_count_u32

Contract: Count interior local-minimum plateaus in a fixed 256-bin histogram. A plateau counts once only when both adjacent bins are strictly higher.

Header path: include/estimora/statistics/pc_hist_valley_count_u32.h

## include/estimora/statistics/pc_histogram_total_u32.h

Public API: pc_histogram_total_u32

Contract: Sum count bins without wrapping. A zero-length histogram has total zero and may use a null bins pointer. Returns false for null output, a null nonempty input, or U32 overflow; failure preserves a nonnull output value.

Header path: include/estimora/statistics/pc_histogram_total_u32.h

## include/estimora/statistics/pc_image_hist_bin_u32.h

Public API: pc_image_hist_bin_u32

Contract: Uniformly map a uint8 sample over [0,256) to one of bin_count bins. False for null output or bin_count==0; failure preserves *bin.

Header path: include/estimora/statistics/pc_image_hist_bin_u32.h

## include/estimora/statistics/pc_mean3_i32.h

Public API: pc_mean3_i32

Contract: Exact arithmetic mean of three int32 values, rounded to nearest integer.

Header path: include/estimora/statistics/pc_mean3_i32.h

## include/estimora/statistics/pc_mean4_i32.h

Public API: pc_mean4_i32

Contract: Exact arithmetic mean of four int32 values, rounded nearest-even.

Header path: include/estimora/statistics/pc_mean4_i32.h

## include/estimora/statistics/pc_mean5_i32.h

Public API: pc_mean5_i32

Contract: Exact arithmetic mean of five int32 values, rounded to nearest integer.

Header path: include/estimora/statistics/pc_mean5_i32.h

## include/estimora/statistics/pc_median_network5_i32.h

Public API: pc_median_network5_i32

Contract: Return the middle order statistic of five int32 values using 7 comparators.

Header path: include/estimora/statistics/pc_median_network5_i32.h

## include/estimora/statistics/pc_median_network7_i32.h

Public API: pc_median_network7_i32

Contract: Return the middle order statistic of seven int32 values using 14 comparators.

Header path: include/estimora/statistics/pc_median_network7_i32.h

## include/estimora/statistics/pc_running_median3_i32.h

Public API: pc_running_median3_i32

Contract: Zero-initialize state. First two outputs pass through; thereafter median is the last-three median.

Header path: include/estimora/statistics/pc_running_median3_i32.h

## include/estimora/statistics/pc_sensor_triple_median_i32.h

Public API: pc_sensor_triple_median_i32

Contract: Stateless median of three simultaneous signed sensor values.

Header path: include/estimora/statistics/pc_sensor_triple_median_i32.h

## include/estimora/statistics/pc_trimmed_mean5_i32.h

Public API: pc_trimmed_mean5_i32

Contract: Drop one minimum and maximum, then round the mean of the middle three.

Header path: include/estimora/statistics/pc_trimmed_mean5_i32.h

## include/estimora/statistics/pc_weighted_sensor_fuse_i32.h

Public API: pc_weighted_sensor_fuse_i32

Contract: Fuse two signed measurements by nonnegative relative weights <= INT32_MAX. Rounds the exact weighted mean to nearest, ties away from zero. Both weights zero or a null output pointer returns false without changing *out.

Header path: include/estimora/statistics/pc_weighted_sensor_fuse_i32.h
