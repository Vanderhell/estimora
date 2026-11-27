# Performance and ESP32-S3 footprint

The values below are historical measurements from the pre-namespace source revision. Namespace migration preserved the normalized implementation bodies, but this revision has not been reflashed or remeasured. Treat the timing and object-footprint data as historical evidence for the same algorithms and toolchain conditions, not as a measurement of the current packaged artifact.

## Current validation-sketch compile (not hardware execution)

The namespaced ESP32-S3 validation sketch compiled with Arduino CLI 1.4.1, Arduino ESP32 core 3.3.10, and board target `esp32:esp32:ozobot_drvkit`. It was not flashed or executed because no authorized ESP32-S3 appeared in the read-only device inventory. The build report was 291,877 bytes program storage (of 3,145,728) and 26,140 bytes global memory (of 327,680) for the complete sketch plus framework. These figures do not isolate Estimora. Current cycles, library-only flash/RAM, and stack remain NOT MEASURED.

## Historical measurement method

Measurements were made on an Ozobot DRVKit identified by esptool as ESP32-S3 revision v0.2, 240 MHz, with 8 MB PSRAM. Arduino ESP32 core 3.3.10 and its default optimized build were used. `tests/esp32/estimora_esp32/estimora_esp32.ino` calls each API with representative fixed inputs. The firmware repeats each operation 1,000 times and reports average `ESP.getCycleCount()` delta per iteration. Values include the loop, assignment and cycle-read overhead; they are observed representative timings, not worst-case bounds. Do not compare them across different board/core/compiler settings as if they were invariant.

## Historical observed cycles per call

| API | Average cycles |
|---|---:|
| `estimora_histogram_total_u32` | 2,608 |
| `estimora_hist_cdf_u32` | 5,470 |
| `estimora_hist_clip_redistribute_u32` | 4,932 |
| `estimora_hist_dynamic_range_u32` | 1,359 |
| `estimora_hist_entropy_norm_q` | 6,537 |
| `estimora_hist_equalize_u8` | 48,180 |
| `estimora_hist_low_percentile_u32` | 1,749 |
| `estimora_hist_high_percentile_u32` | 4,675 |
| `estimora_hist_valley_count_u32` | 2,633 |
| `estimora_image_hist_bin_u32` | 33 |
| `estimora_mean3_i32` | 210 |
| `estimora_mean4_i32` | 81 |
| `estimora_mean5_i32` | 222 |
| `estimora_median_network5_i32` | 32 |
| `estimora_median_network7_i32` | 59 |
| `estimora_running_median3_i32` | 63 |
| `estimora_sensor_triple_median_i32` | 23 |
| `estimora_trimmed_mean5_i32` | 319 |
| `estimora_weighted_sensor_fuse_i32` | 248 |

## Historical footprint

| Measurement | Result | Scope |
|---|---:|---|
| Program storage reported by Arduino | 291,877 bytes (9% of configured 3,145,728 bytes) | Entire validation firmware, Arduino core and support code. Not an isolated library size. |
| Dynamic memory reported by Arduino | 26,140 bytes (7% of configured 327,680 bytes) | Entire firmware globals, including validation histograms, serial/framework state and library. Not attributable to ESTIMORA alone. |
| ESTIMORA source-object flash sections | 2,475 bytes: 2,391 `.text` + 84 `.literal`; no `.rodata` | Sum of 19 compiled ESTIMORA C object sections at the same board/toolchain/flags. This is direct object contribution and excludes shared compiler/runtime helpers and link-time alignment. |
| ESTIMORA object `.data` + `.bss` | 0 bytes | The 19 production translation units declare no static initialized or zero-initialized storage in this build. |
| Stack | NOT MEASURED | No stack bound is inferred from a single watermark. |

The complete linker map and sketch build are generated in the directory passed to Arduino CLI with `--build-path`. `HW VERIFIED` applies to the tested board/revision and the test cases above; it does not certify worst-case cycle limits, other variants or an application-specific footprint.


Current namespaced revision: HW NOT VERIFIED; no available board was identified as an authorized ESP32-S3 test target. No current-revision cycles, flash, static RAM, or stack figures are claimed.
