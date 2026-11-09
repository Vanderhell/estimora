# estimora

Estimora is a standalone C99 statistics library for deterministic descriptive measures, estimators, ratios and bounded streaming summaries.

## Scope

This first extraction contains 42 family headers, 19 implementation units and 40 migrated test sources. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Name and origin

Estimora combines aestimare (to estimate) with ora (a measured boundary), reflecting compact deterministic estimators.
It originates from PrimitiveCore's statistics/descriptive_and_estimators family.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
