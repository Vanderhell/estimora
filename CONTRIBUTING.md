# Contributing to Estimora

Keep changes within the documented statistics and estimator scope. Preserve the public C99 contracts, fixed-width representations, rounding rules, failure behavior, and deterministic execution. Do not add mandatory heap, operating-system, threading, or third-party dependencies.

For a change, add deterministic boundary and invalid-input checks, use an independent reference for numeric behavior, and keep test checks active in Release. Run the strict CMake/CTest suite: `cmake -S . -B build -DESTIMORA_BUILD_TESTS=ON`, `cmake --build build --parallel`, and `ctest --test-dir build --output-on-failure`. Report which compilers and sanitizers were actually run.

Keep each implementation in its matching translation unit. Do not change public API semantics without an independently reproduced defect and regression test.
