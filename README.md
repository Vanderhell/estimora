# Estimora
![CI](https://github.com/Vanderhell/estimora/actions/workflows/ci.yml/badge.svg) ![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg) ![Language](https://img.shields.io/badge/language-C99-informational.svg)

![Estimora banner](docs/banner.svg)

Histograms, quantiles, entropy, medians and bounded sensor estimators.

## Why it exists

Small embedded systems need deterministic statistics over bounded integer data without heap allocation.

## What is included

- Portable C99-oriented implementation.
- Deterministic integer or fixed-point behavior documented at function level.
- No mandatory heap, threads or operating-system services.
- Separate translation units for selective linking.

## Quick start

```sh
cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

For a minimal integration, include only the required public header and compile the matching source unit. The library does not require the rest of PrimitiveCore.

## Documentation

| Need | Read |
|---|---|
| Choose a primitive | [Family guide](docs/FAMILY_GUIDE.md) |
| Check every function and contract | [API reference](docs/API_REFERENCE.md) |
| Reproduce tests and see validation limits | [Testing report](docs/TESTING.md) |
| Review design and dependencies | [Architecture](docs/ARCHITECTURE.md) |
| See measured or pending performance data | [Performance](docs/PERFORMANCE.md) |
| Review source findings and API status | [Audit register](docs/AUDIT.md) |

## At a glance

The public surface consists of 19 headers and 19 callable API entries under include/. Implementations are in src/ and tests are in tests/. State, buffers and output ownership are explicit in each header contract.

The CMake test option is ESTIMORA_BUILD_TESTS. The library has no mandatory third-party dependency. Platform-specific link requirements are documented in CMake and the affected API header.

## Validation status

All 19 public functions have direct deterministic tests. GCC, Clang and MSVC Debug/Release host builds passed 14/14 CTest targets per configuration. GCC ASan and UBSan checks are configured in GitHub Actions; their final remote results are recorded in the audit report.

ESP32-S3 validation firmware reported `ESTIMORA_ALL19_PASS`. The board revision, per-API cycle observations and clearly separated whole-firmware and library-object footprint are recorded in [Performance](docs/PERFORMANCE.md). Stack bounds remain unmeasured.

Detailed overflow, rounding, invalid-input, memory-ownership and state rules are defined by the public headers and API reference. Do not infer them from the product name alone.

## License

Apache License 2.0, copyright Vanderhell. See [LICENSE](LICENSE) and [NOTICE](NOTICE).

## Origin

A standalone embedded-C library extracted from the PrimitiveCore product catalog. It is independently buildable and linkable.
