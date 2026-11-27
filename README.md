# Estimora
![CI](https://github.com/Vanderhell/estimora/actions/workflows/ci.yml/badge.svg) ![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg) ![Language](https://img.shields.io/badge/language-C99-informational.svg)

![Estimora banner](docs/banner.svg)

Estimora provides fixed-size histogram analysis, quantiles, entropy approximation, integer means and medians, and weighted sensor fusion for embedded C applications.

## Design

- Portable ISO C99 public interface with fixed-width integer inputs where the operation requires them.
- Deterministic results and documented overflow, invalid-input, rounding, aliasing, and output-preservation behavior.
- No mandatory heap allocation, operating-system services, or external runtime libraries.
- Separate implementation units for selective source integration and linking.

## Quick start

```c
#include <stdint.h>
#include <estimora/statistics/histogram_total_u32.h>
#include <estimora/statistics/mean4_i32.h>

int main(void)
{
    const uint32_t bins[3] = { 2u, 5u, 3u };
    uint32_t total = 0u;
    if (!estimora_histogram_total_u32(bins, 3u, &total)) return 1;
    return total == 10u && estimora_mean4_i32(-2, 1, 2, 5) == 2 ? 0 : 1;
}
```

The public API consists of 19 functions under `include/estimora/statistics/`. See [API reference](docs/API_REFERENCE.md) for contracts and [family guide](docs/FAMILY_GUIDE.md) for operation selection.

## CMake integration

Build and run the opt-in tests:

```sh
cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Install and consume the exported target:

```cmake
find_package(Estimora CONFIG REQUIRED)
target_link_libraries(app PRIVATE Estimora::Estimora)
```

For source-tree integration, use `add_subdirectory(path/to/estimora)` and link `Estimora::Estimora`. Tests and examples are off by default for consumers.

## Direct source integration

Include the required header from `include/` and compile the corresponding `src/estimora_<operation>.c` translation unit. For example, `histogram_total_u32.h` pairs with `estimora_histogram_total_u32.c`. No global compiler flags are changed by the project CMake target.

## Validation

The enabled suite directly calls all 19 public operations, includes fixed-seed differential and boundary checks, compiles each public header independently as C99, and compiles/links the complete public header set from C++. The exact host matrix, sanitizer status, hardware evidence and known limits are recorded in [Testing](docs/TESTING.md), [Audit](docs/AUDIT.md), and [Performance](docs/PERFORMANCE.md).

## Versioning

Version 0.2.0 uses the `estimora_` public namespace and namespaced include paths. This is a source-level breaking change from 0.1.0; see [Changelog](CHANGELOG.md).

## License

Apache License 2.0, copyright Vanderhell. See [LICENSE](LICENSE) and [NOTICE](NOTICE).
