# Testing and verification

## Direct API suite

`tests/test_audit.c` calls all 19 public functions. It uses a fixed xorshift seed (`0xA341316C`), explicit `CHECK` failure handling (never `assert`), and 100,000 deterministic cases for each applicable numerical operation. Reference calculations use widened integer sums, separately written cumulative/redistribution/quantile formulas, insertion sort, or the C math library; expected values are not obtained from production functions. Boundary checks include `INT32_MIN`, `INT32_MAX`, `UINT32_MAX`, empty and invalid inputs, overflow, negative rounding and documented alias behavior.

The entropy comparison uses the exact two-bin Shannon formula `-(p log2(p) + q log2(q))/8` in the host math library and checks the documented bound `2^-18 + 0.5 * 2^-fractional_bits`. Across the 100,000 seeded distributions and their varying precisions, the maximum absolute quantized error observed was **0.00195244758256**. This includes the allowed final Q-format rounding term; it is not the maximum over all possible histograms.

## Host toolchain results

The final CTest suite contains 37 entries (17 behavior tests, 19 standalone C99 public-header translation units, and one aggregate C++ header/link smoke test). The direct audit test calls all 19 public APIs. Each registered runtime test uses checks that remain effective with `NDEBUG`; `test_audit.c` uses an explicit `CHECK` macro.

| Compiler / version | Configuration | Build and tests |
|---|---|---|
| GCC 16.1.0 (MinGW-w64 UCRT POSIX) | Debug | PASS, 37/37 |
| GCC 16.1.0 (MinGW-w64 UCRT POSIX) | Release | PASS, 37/37 |
| ClangCL 19.1.5 (Visual Studio 2022 integration) | Debug | PASS, 37/37 |
| ClangCL 19.1.5 (Visual Studio 2022 integration) | Release | PASS, 37/37 |
| MSVC 19.42.34444 | Debug | PASS, 37/37 |
| MSVC 19.42.34444 | Release | PASS, 37/37 |

All six configurations used strict warnings (`-Wall -Wextra -Wpedantic -Werror` or `/W4 /WX`). The entropy and audit test executables link the platform math library where required; the production target does not have a libm dependency. The test targets include assert-based legacy cases only after explicitly undefining `NDEBUG`; these assertions remain live in Release.

## Reproduce

GCC, using Ninja:

```sh
cmake -G Ninja -S . -B build-gcc -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release -DESTIMORA_BUILD_TESTS=ON -DESTIMORA_STRICT_WARNINGS=ON
cmake --build build-gcc --parallel
ctest --test-dir build-gcc --output-on-failure
```

Clang follows the same commands with `-DCMAKE_C_COMPILER=clang`. MSVC with the Visual Studio 2022 generator:

```powershell
cmake -S . -B build-msvc -DESTIMORA_BUILD_TESTS=ON -DESTIMORA_STRICT_WARNINGS=ON
cmake --build build-msvc --config Release --parallel
ctest --test-dir build-msvc -C Release --output-on-failure
```

Current productized revision: **HW NOT VERIFIED**. A read-only board inventory found no ESP32-S3 (only a generic serial device); no firmware was flashed. Historical pre-namespace evidence is summarized in [Performance](PERFORMANCE.md). The migrated validation sketch is `tests/esp32/estimora_esp32/estimora_esp32.ino`. To repeat with Arduino CLI on an authorized Ozobot DRVKit test device, set `$PORT` to the confirmed device port, then stage the sketch and production C files in a temporary Arduino sketch directory (the directory and `.ino` basename must match):

```powershell
$sketch = Join-Path $env:TEMP 'estimora_esp32'
$build = Join-Path $env:TEMP 'estimora_esp32_build'
New-Item -ItemType Directory -Force $sketch | Out-Null
Copy-Item tests\esp32\estimora_esp32\estimora_esp32.ino $sketch -Force
Copy-Item src\*.c $sketch -Force
$headerDir = Join-Path $sketch 'estimora\statistics'
New-Item -ItemType Directory -Force $headerDir | Out-Null
Copy-Item include\estimora\statistics\*.h $headerDir -Force
arduino-cli compile --fqbn esp32:esp32:ozobot_drvkit --build-path $build $sketch
arduino-cli upload --fqbn esp32:esp32:ozobot_drvkit -p $PORT --input-dir $build
arduino-cli monitor -p $PORT --config baudrate=115200
```

The renamed sketch compiled locally against Arduino ESP32 core 3.3.10, but was not flashed or run. Its build report showed 291,877 bytes program storage and 26,140 bytes global memory for the complete test firmware and runtime; these figures are not an isolated library measurement. The sketch prints one `PASS`/`FAIL` line per API and `ESTIMORA_ALL19_PASS` only when all expected outputs match.

## Sanitizers and CI

GCC AddressSanitizer and UndefinedBehaviorSanitizer jobs are configured in `.github/workflows/ci.yml` on Ubuntu 24.04. Local sanitizer execution on the Windows host was **NOT RUN**: its installed MinGW GCC has no `libasan` or `libubsan`, the installed Clang has no Windows sanitizer runtimes, and WSL is unavailable in this environment. The hosted run for the productization commit is recorded below once all jobs for that exact revision have completed.

| Remote workflow check | Result |
|---|---|
| GCC Debug / Release | Awaiting exact post-push run |
| Clang Debug / Release | Awaiting exact post-push run |
| MSVC Debug / Release | Awaiting exact post-push run |
| GCC ASan | Awaiting exact post-push run |
| GCC UBSan | Awaiting exact post-push run |
