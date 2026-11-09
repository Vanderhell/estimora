# Estimora API reference

This reference explains every public header in the Estimora product. It is generated from the current headers and test sources; the header remains authoritative when a detail conflicts.

## Reading the entries
Each entry states the public symbol, implementation unit, contract, edge/failure behavior and a call extracted from the validating test source. The examples are evidence-oriented; adapt buffer ownership and numeric units to the application.

## include/estimora/statistics/pc_hist_cdf_u32.h

**Public symbol:** pc_hist_cdf_u32

**Implementation unit:** src/pc_hist_cdf_u32.c

**Purpose and contract:** Write raw cumulative counts: cdf[i] = sum(bins[0..i]). The output may exactly alias bins or be disjoint; partial overlap is not supported. Empty input succeeds without dereferencing either pointer. Failure leaves output unchanged.

**Validation source:** tests/test_cdf_normalize.c

**Test-derived call:**
```c
pc_hist_cdf_u32(raw_bins, 3u, raw_cdf\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_clip_redistribute_u32.h

**Public symbol:** pc_hist_clip_redistribute_u32

**Implementation unit:** src/pc_hist_clip_redistribute_u32.c

**Purpose and contract:** Clip 256 bins at a positive absolute limit, then redistribute excess uniformly. Residual counts go to the lowest bin indices. Exact alias is allowed; failure leaves output unchanged.

**Validation source:** tests/test_redistribute.c

**Test-derived call:**
```c
pc_hist_clip_redistribute_u32(h, limit, out\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_dynamic_range_u32.h

**Public symbol:** pc_hist_dynamic_range_u32

**Implementation unit:** src/pc_hist_dynamic_range_u32.c

**Purpose and contract:** Return highest occupied bin minus lowest occupied bin in a 256-bin histogram. Empty input fails and leaves range_out unchanged.

**Validation source:** tests/test_dynamic_range.c

**Test-derived call:**
```c
pc_hist_dynamic_range_u32(NULL, &range\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_entropy_norm_q.h

**Public symbol:** pc_hist_entropy_norm_q

**Implementation unit:** src/pc_hist_entropy_norm_q.c

**Purpose and contract:** Approximate normalized Shannon entropy of all 256 byte-intensity bins. Q output uses caller fractional_bits 0..31; error from exact entropy / 8 is <= 2^-18 + 0.5*2^-fractional_bits. Failure preserves output.

**Validation source:** tests/test_entropy.c

**Test-derived call:**
```c
pc_hist_entropy_norm_q(h, f, &q\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_equalize_u8.h

**Public symbol:** pc_hist_equalize_u8

**Implementation unit:** src/pc_hist_equalize_u8.c

**Purpose and contract:** Build a 256-entry grayscale equalization LUT from raw uint32 histogram counts. Uses CDF-min normalization and round-nearest-even. A single-level histogram produces the identity LUT. Histogram and LUT storage must not overlap. Failure leaves lut_out unchanged.

**Validation source:** tests/test_equalize.c

**Test-derived call:**
```c
pc_hist_equalize_u8(h, lut\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_high_percentile_u32.h

**Public symbol:** pc_hist_high_percentile_u32

**Implementation unit:** src/pc_hist_high_percentile_u32.c

**Purpose and contract:** Upper-tail nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the maximum occupied bin. Failure preserves output.

**Validation source:** tests/test_high_percentile.c

**Test-derived call:**
```c
pc_hist_high_percentile_u32(h, 0u, &bin\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_low_percentile_u32.h

**Public symbol:** pc_hist_low_percentile_u32

**Implementation unit:** src/pc_hist_low_percentile_u32.c

**Purpose and contract:** Nearest-rank percentile of a 256-bin histogram. Percent is 0..100; zero selects the minimum occupied bin. Failure leaves bin_out unchanged.

**Validation source:** tests/test_percentile.c

**Test-derived call:**
```c
pc_hist_low_percentile_u32(h, 0u, &bin\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_hist_valley_count_u32.h

**Public symbol:** pc_hist_valley_count_u32

**Implementation unit:** src/pc_hist_valley_count_u32.c

**Purpose and contract:** Count interior local-minimum plateaus in a fixed 256-bin histogram. A plateau counts once only when both adjacent bins are strictly higher.

**Validation source:** tests/test_valleys.c

**Test-derived call:**
```c
pc_hist_valley_count_u32(h, &got\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_histogram_total_u32.h

**Public symbol:** pc_histogram_total_u32

**Implementation unit:** src/pc_histogram_total_u32.c

**Purpose and contract:** Sum count bins without wrapping. A zero-length histogram has total zero and may use a null bins pointer. Returns false for null output, a null nonempty input, or U32 overflow; failure preserves a nonnull output value.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_image_hist_bin_u32.h

**Public symbol:** pc_image_hist_bin_u32

**Implementation unit:** src/pc_image_hist_bin_u32.c

**Purpose and contract:** Uniformly map a uint8 sample over [0,256) to one of bin_count bins. False for null output or bin_count==0; failure preserves *bin.

**Validation source:** tests/test_image_hist_bin_u32.c

**Test-derived call:**
```c
pc_image_hist_bin_u32(0u,0u,&out)&&out==99u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_mean3_i32.h

**Public symbol:** pc_mean3_i32

**Implementation unit:** src/pc_mean3_i32.c

**Purpose and contract:** Exact arithmetic mean of three int32 values, rounded to nearest integer.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_mean4_i32.h

**Public symbol:** pc_mean4_i32

**Implementation unit:** src/pc_mean4_i32.c

**Purpose and contract:** Exact arithmetic mean of four int32 values, rounded nearest-even.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_mean5_i32.h

**Public symbol:** pc_mean5_i32

**Implementation unit:** src/pc_mean5_i32.c

**Purpose and contract:** Exact arithmetic mean of five int32 values, rounded to nearest integer.

**Validation source:** tests/test_c.c

**Test-derived call:**
```c
pc_mean5_i32(INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN,INT32_MIN)==INT32_MIN);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_median_network5_i32.h

**Public symbol:** pc_median_network5_i32

**Implementation unit:** src/pc_median_network5_i32.c

**Purpose and contract:** Return the middle order statistic of five int32 values using 7 comparators.

**Validation source:** tests/test_median5.c

**Test-derived call:**
```c
pc_median_network5_i32(INT32_MIN,INT32_MAX,0,INT32_MIN,INT32_MAX)==0);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_median_network7_i32.h

**Public symbol:** pc_median_network7_i32

**Implementation unit:** src/pc_median_network7_i32.c

**Purpose and contract:** Return the middle order statistic of seven int32 values using 14 comparators.

**Validation source:** tests/test_median7.c

**Test-derived call:**
```c
pc_median_network7_i32(v[0],v[1],v[2],v[3],v[4],v[5],v[6])==oracle(v\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_running_median3_i32.h

**Public symbol:** pc_running_median3_i32

**Implementation unit:** src/pc_running_median3_i32.c

**Purpose and contract:** Zero-initialize state. First two outputs pass through; thereafter median is the last-three median.

**Validation source:** tests/test.c

**Test-derived call:**
```c
pc_running_median3_i32(INT32_MIN,&s)&&s.median==INT32_MIN&&s.count==1);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_sensor_triple_median_i32.h

**Public symbol:** pc_sensor_triple_median_i32

**Implementation unit:** src/pc_sensor_triple_median_i32.c

**Purpose and contract:** Stateless median of three simultaneous signed sensor values.

**Validation source:** tests/test_edge_grid.c

**Test-derived call:**
```c
pc_sensor_triple_median_i32(x[i],x[j],x[k]);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_trimmed_mean5_i32.h

**Public symbol:** pc_trimmed_mean5_i32

**Implementation unit:** src/pc_trimmed_mean5_i32.c

**Purpose and contract:** Drop one minimum and maximum, then round the mean of the middle three.

**Validation source:** tests/test_trimmed_mean.c

**Test-derived call:**
```c
pc_trimmed_mean5_i32(INT32_MIN,INT32_MIN,0,INT32_MAX,INT32_MAX)==0);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/estimora/statistics/pc_weighted_sensor_fuse_i32.h

**Public symbol:** pc_weighted_sensor_fuse_i32

**Implementation unit:** src/pc_weighted_sensor_fuse_i32.c

**Purpose and contract:** Fuse two signed measurements by nonnegative relative weights <= INT32_MAX. Rounds the exact weighted mean to nearest, ties away from zero. Both weights zero or a null output pointer returns false without changing *out.

**Validation source:** tests/test_weighted_sensor_fuse.c

**Test-derived call:**
```c
pc_weighted_sensor_fuse_i32(1,0,2,0,&out)&&out==777);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

