# Testing and verification

## Direct API suite

`tests/test_audit.c` calls all 19 public functions. It uses a fixed xorshift seed (`0xA341316C`), explicit `CHECK` failure handling (never `assert`), and 100,000 deterministic cases for each applicable numerical operation. Reference calculations use widened integer sums, separately written cumulative/redistribution/quantile formulas, insertion sort, or the C math library; expected values are not obtained from production functions. Boundary checks include `INT32_MIN`, `INT32_MAX`, `UINT32_MAX`, empty and invalid inputs, overflow, negative rounding and documented alias behavior.

The entropy comparison uses the exact two-bin Shannon formula `-(p log2(p) + q log2(q))/8` in the host math library and checks the documented bound `2^-18 + 0.5 * 2^-fractional_bits`. Across the 100,000 seeded distributions and their varying precisions, the maximum absolute quantized error observed was **0.00195244758256**. This includes the allowed final Q-format rounding term; it is not the maximum over all possible histograms.

## Host toolchain results

Every enabled CTest target passed (14/14, including the direct audit suite) in each configuration below. All builds used C99 mode and strict warnings (`-Wall -Wextra -Wpedantic -Werror` or `/W4 /WX`).

| Compiler / version | Configuration | Build and tests |
|---|---|---|
| GCC 16.1.0 (MinGW-w64 UCRT POSIX) | Debug | PASS, 14/14 |
| GCC 16.1.0 (MinGW-w64 UCRT POSIX) | Release | PASS, 14/14 |
| Clang 22.1.8 (GNU Windows target) | Debug | PASS, 14/14 |
| Clang 22.1.8 (GNU Windows target) | Release | PASS, 14/14 |
| MSVC 19.42.34444 | Debug | PASS, 14/14 |
| MSVC 19.42.34444 | Release | PASS, 14/14 |

The MSVC Release suite confirms that legacy `assert` checks remain enabled: test sources undefine `NDEBUG` before including `<assert.h>`. The new audit suite uses its own non-assert check macro.

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

ESP32-S3 hardware validation uses `tests/esp32/estimora_esp32/estimora_esp32.ino`. To repeat with Arduino CLI on an Ozobot DRVKit, stage the sketch and production C files in a temporary Arduino sketch directory (the directory and `.ino` basename must match):

```powershell
$sketch = Join-Path $env:TEMP 'estimora_esp32'
$build = Join-Path $env:TEMP 'estimora_esp32_build'
New-Item -ItemType Directory -Force $sketch | Out-Null
Copy-Item tests\esp32\estimora_esp32\estimora_esp32.ino $sketch -Force
Copy-Item src\*.c $sketch -Force
Copy-Item include\estimora\statistics\*.h $sketch -Force
arduino-cli compile --fqbn esp32:esp32:ozobot_drvkit --build-path $build $sketch
arduino-cli upload --fqbn esp32:esp32:ozobot_drvkit -p COM36 --input-dir $build
arduino-cli monitor -p COM36 --config baudrate=115200
```

The firmware prints one `PASS`/`FAIL` line per API and a final `ESTIMORA_ALL19_PASS` marker only when all expected outputs match.

## Sanitizers and CI

GCC AddressSanitizer and UndefinedBehaviorSanitizer jobs are configured in `.github/workflows/ci.yml` on Ubuntu 24.04. Local sanitizer execution on the Windows host was **NOT RUN**: its installed MinGW GCC has no `libasan` or `libubsan`, the installed Clang has no Windows sanitizer runtimes, and WSL is unavailable in this environment. The final GitHub Actions run and each job result are recorded below after push.

| Remote workflow check | Result |
|---|---|
| GCC Debug / Release | Pending remote run |
| Clang Debug / Release | Pending remote run |
| MSVC Debug / Release | Pending remote run |
| GCC ASan | Pending remote run |
| GCC UBSan | Pending remote run |
