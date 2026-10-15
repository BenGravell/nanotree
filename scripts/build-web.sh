#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

if [[ -f build/emsdk/emsdk_env.sh ]]; then
  source build/emsdk/emsdk_env.sh
fi

emcmake cmake -S build/web-raylib -B build/web-raylib-build \
  -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF
cmake --build build/web-raylib-build --parallel 2

mkdir -p dist
em++ -o dist/index.html src/main.cpp -std=c++20 -O3 -Wall \
  -I src -I build/web-raylib/src \
  -s USE_GLFW=3 -s ASYNCIFY \
  --preload-file assets --shell-file shell.html -DPLATFORM_WEB \
  build/web-raylib-build/raylib/libraylib.a

for file in index.html index.js index.wasm index.data; do
  test -s "dist/$file"
done
