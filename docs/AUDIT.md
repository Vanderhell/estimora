# ESTIMORA technical audit

Audit target: `Vanderhell/estimora`, branch `main`, portable C99 sources under `include/estimora/statistics/` and `src/`.

## Result

All 19 public callable functions were inspected and have direct deterministic coverage in `tests/test_audit.c`. The audit found **no confirmed production-algorithm defect**. No production algorithm or public declaration was changed. The new host audit suite runs 100,000 deterministic cases per applicable numeric API; boundary, invalid-input, alias and failure-preservation cases are described in [Testing](TESTING.md).

| Classification | Finding | Evidence / resolution |
|---|---|---|
| BUG (test coverage) | Three callable APIs (`pc_histogram_total_u32`, `pc_mean3_i32`, `pc_mean4_i32`) had no direct enabled test target. | Added direct calls, boundary checks and 100,000-case references in `tests/test_audit.c`; registered it with CTest. |
| BUG (Release validation) | Existing enabled tests use `assert`, which is disabled when Release defines `NDEBUG`. | The affected test sources now undefine `NDEBUG` before including assertions. Release runs execute their checks. |
| BUG (strict Clang portability) | Production C sources lacked final newline characters; hosted Clang with `-Wpedantic -Werror` rejected them as `-Wnewline-eof`. | Added standard final newlines to the affected production translation units. The remote Clang build is rerun after this correction. |
| BUG (documentation) | The API reference reported missing coverage and contained malformed generated call snippets. | Replaced with a reviewed per-API contract and links to the direct audit test. |
| OK | Histogram accumulation, CDF, clipping/redistribution, range, percentiles, entropy and equalization use bounded widened intermediates or check overflow before writing. | Source review plus independent integer/floating reference comparisons and boundary cases. |
| OK | Signed means and weighted fusion avoid signed overflow by widening to `int64_t`; rounding behavior matches their public contracts for positive and negative inputs. | Full-width seeded differential tests and explicit `INT32_MIN`/`INT32_MAX` and tie cases. |
| OK | Median networks and rolling median use comparisons only; the stateful API validates canonical state fields before mutation. | 100,000 deterministic reference comparisons, malformed-state preservation and reset checks. |
| OK | Public declarations remain C99-compatible; source has no mandatory heap allocation or operating-system calls. | Strict C99 GCC, Clang and MSVC builds; source and API inspection. |

## API register

| Public function | Status | Principal checks |
|---|---|---|
| `pc_histogram_total_u32` | OK | Empty/null, max count, sum overflow, output preservation, output/input alias, 100,000 reference sums. |
| `pc_hist_cdf_u32` | OK | Empty, exact in-place, monotonic/exact prefix totals, overflow and output preservation, 100,000 histograms. |
| `pc_hist_clip_redistribute_u32` | OK | Positive/zero limit, exact alias, count conservation, maximum counts and residual allocation, 100,000 histograms. |
| `pc_hist_dynamic_range_u32` | OK | Empty/null, singleton and endpoint cases, 100,000 references. |
| `pc_hist_entropy_norm_q` | OK | Empty, precision 0..31 validation, one-bin, uniform and maximum counts; 100,000 two-bin comparisons and error bound. |
| `pc_hist_equalize_u8` | OK | Empty, one-level identity, count overflow, failure preservation, tie-to-even integer LUT reference, 100,000 histograms. |
| `pc_hist_low_percentile_u32` | OK | Empty, percent 0/100/out of range, nearest-rank reference, 100,000 histograms. |
| `pc_hist_high_percentile_u32` | OK | Empty, percent 0/100/out of range, nearest-rank reference, 100,000 histograms. |
| `pc_hist_valley_count_u32` | OK | Null, flat/plateau boundaries and 100,000 independent plateau scans. |
| `pc_image_hist_bin_u32` | OK | Zero bins, maximum bin count, null output and 100,000 widened formula comparisons. |
| `pc_mean3_i32` | OK | Full-width values, negative inputs, permutations and 100,000 widened references. |
| `pc_mean4_i32` | OK | Full-width values, positive/negative nearest-even ties, permutations and 100,000 references. |
| `pc_mean5_i32` | OK | Full-width values, negative inputs, permutations and 100,000 widened references. |
| `pc_median_network5_i32` | OK | Full-width inputs, repeated values and 100,000 independent sort references. |
| `pc_median_network7_i32` | OK | Full-width inputs, repeated values and 100,000 independent sort references. |
| `pc_running_median3_i32` | OK | Initialization, first/second sample, wraparound, reset, invalid state and 100,000 rolling comparisons. |
| `pc_sensor_triple_median_i32` | OK | Full-width inputs, permutations, repeated values and 100,000 independent sort references. |
| `pc_trimmed_mean5_i32` | OK | Full-width inputs, trim edges, tie/negative rounding and 100,000 independent sort/reference comparisons. |
| `pc_weighted_sensor_fuse_i32` | OK | Zero/max weights, asymmetric weights, positive/negative ties, failure preservation and 100,000 widened references. |

## Limits

- Random testing is deterministic sampled verification, not exhaustive proof over the complete input space.
- CDF supports exact input/output alias and disjoint buffers; partial overlap is outside its documented contract. Equalization requires nonoverlapping histogram/LUT memory.
- Local Windows GCC does not provide ASan/UBSan runtime libraries. Those checks are performed by the required Ubuntu CI jobs and are recorded separately in [Testing](TESTING.md).
- ESP32-S3 hardware was available and tested. The firmware checks outputs for representative inputs and measures call-loop cycles; these are not worst-case timing guarantees.
- Stack high-water was not measured and is not claimed as a stack bound.
