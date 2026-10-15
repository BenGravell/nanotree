#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

mkdir -p build
if [[ ! -d build/emsdk ]]; then
  git clone --depth 1 --branch 6.0.12 https://github.com/emscripten-core/emsdk.git build/emsdk
fi
build/emsdk/emsdk install 6.0.12
build/emsdk/emsdk activate 6.0.12
if [[ ! -d build/web-raylib ]]; then
  git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git build/web-raylib
fi
