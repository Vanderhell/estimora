# Performance and ESP32-S3 footprint

## Measurement method

Measurements were made on an Ozobot DRVKit identified by esptool as ESP32-S3 revision v0.2, 240 MHz, with 8 MB PSRAM. Arduino ESP32 core 3.3.10 and its default optimized build were used. `tests/esp32/estimora_esp32/estimora_esp32.ino` calls each API with representative fixed inputs. The firmware repeats each operation 1,000 times and reports average `ESP.getCycleCount()` delta per iteration. Values include the loop, assignment and cycle-read overhead; they are observed representative timings, not worst-case bounds. Do not compare them across different board/core/compiler settings as if they were invariant.

## Observed cycles per call

| API | Average cycles |
|---|---:|
| `pc_histogram_total_u32` | 2,608 |
| `pc_hist_cdf_u32` | 5,470 |
| `pc_hist_clip_redistribute_u32` | 4,932 |
| `pc_hist_dynamic_range_u32` | 1,359 |
| `pc_hist_entropy_norm_q` | 6,537 |
| `pc_hist_equalize_u8` | 48,180 |
| `pc_hist_low_percentile_u32` | 1,749 |
| `pc_hist_high_percentile_u32` | 4,675 |
| `pc_hist_valley_count_u32` | 2,633 |
| `pc_image_hist_bin_u32` | 33 |
| `pc_mean3_i32` | 210 |
| `pc_mean4_i32` | 81 |
| `pc_mean5_i32` | 222 |
| `pc_median_network5_i32` | 32 |
| `pc_median_network7_i32` | 59 |
| `pc_running_median3_i32` | 63 |
| `pc_sensor_triple_median_i32` | 23 |
| `pc_trimmed_mean5_i32` | 319 |
| `pc_weighted_sensor_fuse_i32` | 248 |

## Footprint

| Measurement | Result | Scope |
|---|---:|---|
| Program storage reported by Arduino | 291,877 bytes (9% of configured 3,145,728 bytes) | Entire validation firmware, Arduino core and support code. Not an isolated library size. |
| Dynamic memory reported by Arduino | 26,140 bytes (7% of configured 327,680 bytes) | Entire firmware globals, including validation histograms, serial/framework state and library. Not attributable to ESTIMORA alone. |
| ESTIMORA source-object flash sections | 2,475 bytes: 2,391 `.text` + 84 `.literal`; no `.rodata` | Sum of 19 compiled ESTIMORA C object sections at the same board/toolchain/flags. This is direct object contribution and excludes shared compiler/runtime helpers and link-time alignment. |
| ESTIMORA object `.data` + `.bss` | 0 bytes | The 19 production translation units declare no static initialized or zero-initialized storage in this build. |
| Stack | NOT MEASURED | No stack bound is inferred from a single watermark. |

The complete linker map and sketch build are generated in the directory passed to Arduino CLI with `--build-path`. `HW VERIFIED` applies to the tested board/revision and the test cases above; it does not certify worst-case cycle limits, other variants or an application-specific footprint.
