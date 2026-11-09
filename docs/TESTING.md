# Testing

| Test category | Count | Platform | Result | Evidence |
|---|---:|---|---|---|
| Self-contained CTest tests | 13 | MSVC host | PASS | CMake test gate. |
| Strict warning build | 13 | GCC 13.3 / Ubuntu 24.04 | PASS | `-Wall -Wextra -Werror`; all tests passed. |
| Preserved test sources | 40 | Repository | PRESERVED | Cross-product dependencies are excluded. |
| Hardware tests | 0 | ESP32-S3 | NOT TESTED | HW NOT VERIFIED. |

Reproduce with `cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON`,
`cmake --build build --config Release`, and
`ctest --test-dir build --output-on-failure -C Release`.

The strict GCC check is reproduced with:
`cmake -S . -B build-linux -DESTIMORA_BUILD_TESTS=ON -DCMAKE_C_FLAGS="-Wall -Wextra -Werror"`,
`cmake --build build-linux --parallel`, and
`ctest --test-dir build-linux --output-on-failure`.

Known gap: MCU footprint, stack use and numerical timing are not measured.
