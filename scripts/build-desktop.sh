#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

case "${1:-release}" in
  release) build_type=Release ;;
  debug) build_type=Debug ;;
  *) echo "Usage: $0 [release|debug]" >&2; exit 2 ;;
esac
build_dir="build/${build_type,,}"

if ! conan profile path default >/dev/null 2>&1; then
  conan profile detect
fi
conan install . --build=missing -of="$build_dir/conan" --settings="build_type=$build_type"
cmake -S . -B "$build_dir" -G Ninja \
  -DCMAKE_BUILD_TYPE="$build_type" \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/$build_dir/conan/conan_toolchain.cmake"
cmake --build "$build_dir" --parallel 2
