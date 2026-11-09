# Estimora family guide

## What this family solves

**Statistics and estimators.** Histograms, quantiles, entropy, medians and bounded sensor estimators.

This guide describes the complete public family as an independent embedded-C product. Each primitive remains separately linkable; select only the headers and source units required by the application.

## When to use this family

Use Estimora when the application needs the operations listed below and requires deterministic behavior, explicit caller-owned memory, and no mandatory operating-system services.

## Family at a glance

```mermaid
flowchart LR
    A[Application input] --> B[Validate representation and bounds]
    B --> C[Select one public primitive]
    C --> D[Check status or output contract]
    D --> E[Use result with documented units]
```

## Primitive map

| Public header | Symbol | Purpose and contract | Validation source |
|---|---|---|---|
| `include/estimora/statistics/pc_hist_cdf_u32.h` | `pc_hist_cdf_u32` | Write raw cumulative counts: cdf[i] = sum(bins[0..i]). The output may exactly alias bins or be disjoint; partial overlap is not supported. Empty input succeeds without dereferencing either pointer. Failure leaves output unchanged. | tests/test_cdf_normalize.c |
| `include/estimora/statistics/pc_hist_clip_redistribute_u32.h` | `pc_hist_clip_redistribute_u32` | Clip 256 bins at a positive absolute limit, then redistribute excess uniformly. Residual counts go to the lowest bin indices. Exact alias is allowed; failure leaves output unchanged. | tests/test_redistribute.c |
| `include/estimora/statistics/pc_hist_dynamic_range_u32.h` | `pc_hist_dynamic_range_u32` | Return highest occupied bin minus lowest occupied bin in a 256-bin histogram. Empty input fails and leaves range_out unchanged. | tests/test_dynamic_range.c |
| `include/estimora/statistics/pc_hist_entropy_norm_q.h` | `pc_hist_entropy_norm_q` | Approximate normalized Shannon entropy of all 256 byte-intensity bins. Q output uses caller fractional_bits 0..31; error from exact entropy / 8 is <= 2^-18 + 0.5*2^-fractional_bits. Failure preserves output. | tests/test_entropy.c |
| `include/estimora/statistics/pc_hist_equalize_u8.h` | `pc_hist_equalize_u8` | Build a 256-entry grayscale equalization LUT from raw uint32 histogram counts. Uses CDF-min normalization and round-nearest-even. A single-level histogram produces the identity LUT. Histogram and LUT storage must not overlap. Failure leaves lut_out unchanged. | tests/test_equalize.c |
| `include/estimora/statistics/pc_hist_high_percentile_u32.h` | `pc_hist_high_percentile_u32` | Upper-tail nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the maximum occupied bin. Failure preserves output. | tests/test_high_percentile.c |
| `include/estimora/statistics/pc_hist_low_percentile_u32.h` | `pc_hist_low_percentile_u32` | Nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the minimum occupied bin. Failure leaves bin_out unchanged. | tests/test_percentile.c |
| `include/estimora/statistics/pc_hist_valley_count_u32.h` | `pc_hist_valley_count_u32` | Count interior local-minimum plateaus in a fixed 256-bin histogram. A plateau counts once only when both adjacent bins are strictly higher. | tests/test_valleys.c |
| `include/estimora/statistics/pc_histogram_total_u32.h` | `pc_histogram_total_u32` | Sum count bins without wrapping. A zero-length histogram has total zero and may use a null bins pointer. Returns false for null output, a null nonempty input, or U32 overflow; failure preserves a nonnull output value. | No enabled test source was found by symbol search. |
| `include/estimora/statistics/pc_image_hist_bin_u32.h` | `pc_image_hist_bin_u32` | Uniformly map a uint8 sample over [0,256) to one of bin_count bins. False for null output or bin_count==0; failure preserves *bin. | tests/test_image_hist_bin_u32.c |
| `include/estimora/statistics/pc_mean3_i32.h` | `pc_mean3_i32` | Exact arithmetic mean of three int32 values, rounded to nearest integer. | No enabled test source was found by symbol search. |
| `include/estimora/statistics/pc_mean4_i32.h` | `pc_mean4_i32` | Exact arithmetic mean of four int32 values, rounded nearest-even. | No enabled test source was found by symbol search. |
| `include/estimora/statistics/pc_mean5_i32.h` | `pc_mean5_i32` | Exact arithmetic mean of five int32 values, rounded to nearest integer. | tests/test_c.c |
| `include/estimora/statistics/pc_median_network5_i32.h` | `pc_median_network5_i32` | Return the middle order statistic of five int32 values using 7 comparators. | tests/test_median5.c |
| `include/estimora/statistics/pc_median_network7_i32.h` | `pc_median_network7_i32` | Return the middle order statistic of seven int32 values using 14 comparators. | tests/test_median7.c |
| `include/estimora/statistics/pc_running_median3_i32.h` | `pc_running_median3_i32` | Zero-initialize state. First two outputs pass through; thereafter median is the last-three median. | tests/test.c |
| `include/estimora/statistics/pc_sensor_triple_median_i32.h` | `pc_sensor_triple_median_i32` | Stateless median of three simultaneous signed sensor values. | tests/test_edge_grid.c |
| `include/estimora/statistics/pc_trimmed_mean5_i32.h` | `pc_trimmed_mean5_i32` | Drop one minimum and maximum, then round the mean of the middle three. | tests/test_trimmed_mean.c |
| `include/estimora/statistics/pc_weighted_sensor_fuse_i32.h` | `pc_weighted_sensor_fuse_i32` | Fuse two signed measurements by nonnegative relative weights <= INT32_MAX. Rounds the exact weighted mean to nearest, ties away from zero. Both weights zero or a null output pointer returns false without changing *out. | tests/test_weighted_sensor_fuse.c |

## Selection guidance

- Choose the primitive whose width, representation, rounding, saturation, wrapping, and error contract match the application.
- Do not substitute a similarly named function without checking the public header.
- Allocate state and buffers in the caller; respect documented aliasing and output-preservation rules.
- Treat entries marked without a validation source as requiring additional product-level tests before release.

## Integration checklist

1. Include only the required public header.
2. Compile the matching source translation unit or use the product CMake target.
3. Check every boolean/status return before consuming an output.
4. Add application-specific boundary tests for units, widths, invalid inputs, and overflow behavior.
5. Run the documented host tests before target integration.

## Complexity and footprint

The product is split into independent translation units. Exact flash, RAM, stack, and cycle costs depend on the selected functions, compiler, optimization, and MCU; unmeasured values remain explicitly marked PENDING in the testing documentation.

## Validation boundary

The API reference links each public entry to its implementation and available validation source. Hardware status is reported only when reproducible target evidence exists.
