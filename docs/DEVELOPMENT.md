# Development

Run the following commands from the repository root.

## Debug build

```bash
conan install . --build=missing -of=build/conan --settings=build_type=Debug

cmake -B build/debug -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="build/conan/conan_toolchain.cmake"

cmake --build build/debug --config Debug
```

## C++ formatting

Install `clang-format`:

```bash
sudo apt install clang-format
```

The root `.clang-format` controls the formatting rules.

Check formatting:

```bash
bash scripts/format-cpp.sh check
```

Apply formatting:

```bash
bash scripts/format-cpp.sh apply
```

Apply formatting to a single file:

```bash
bash scripts/format-cpp.sh apply path/to/file.h
```

The script defaults to `check`. Optional file paths are relative to the repository root.

## Source layout

- `src/main.cpp`: application lifecycle and frame loop.
- `src/core/problem/`: problem data, obstacles, and editing.
- `src/core/timing/`: timing measurements and duration breakdowns.
- `src/planner/`: collision checks, neighbor selection, sampling, steering, and paths.
- `src/planner/tree.h`: tree data and all tree operations, including growth and retention.
- `src/ui/`: input history, GUI styling, and drawing.
