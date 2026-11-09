# estimora

Estimora is a standalone C99 statistics library for deterministic descriptive measures, estimators, ratios and bounded streaming summaries.

## Scope

The package contains 42 public headers and 19 implementation units. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Architecture and integration

Descriptive and streaming estimators are separate C99 translation units. Use
the `estimora` CMake target with `include/`, or compile only the required
`src/*.c` files. State is explicit and caller-owned where applicable.

## API, configuration and dependencies

Public headers are under `include/estimora/statistics`. Contracts describe
sample counts, empty inputs, integer widths and rounding. `ESTIMORA_BUILD_TESTS`
controls host tests; no heap, OS, thread or third-party dependency is required.

## Resource, performance and limitations

The enabled host suite passes 13/13 tests. MCU flash, RAM, stack, cycle and
worst-case timing results are `PENDING`; HW NOT VERIFIED. Numerical results are
deterministic for identical inputs and configuration.

## Version, security, license and origin

The package is independently versioned with semantic-versioning intent before
1.0. Apache License 2.0, copyright Vanderhell. It is a self-contained product
is self-contained and is not a cryptographic library.

## Name and origin

Estimora combines aestimare (to estimate) with ora (a measured boundary), reflecting compact deterministic estimators.
Its technical scope is descriptive statistics and deterministic estimators.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
