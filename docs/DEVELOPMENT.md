# Development

Run the following commands from the repository root.

## Debug build

```bash
conan install . --build=missing -of=build/conan --settings=build_type=Debug

cmake -B build/debug -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="build/conan/conan_toolchain.cmake"

cmake --build build/debug --config Debug
```

## Source layout

- `src/main.cpp`: application lifecycle and frame loop.
- `src/core/problem/`: problem data, obstacles, and editing.
- `src/core/timing/`: timing measurements and duration breakdowns.
- `src/planner/`: collision checks, neighbor selection, sampling, steering, and paths.
- `src/planner/tree.h`: tree data and all tree operations, including growth and retention.
- `src/ui/`: input history, GUI styling, and drawing.
