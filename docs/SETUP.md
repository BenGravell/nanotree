# Setup

Run desktop build commands from the repository root.
Web commands below assume checkouts at `~/nanotree`, `~/emsdk`, and `~/raylib`.

## Local app

### Build

```bash
conan install . --build=missing -of=build/conan --settings=build_type=Release

cmake -B build/release -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="build/conan/conan_toolchain.cmake" -DCMAKE_CXX_FLAGS="-march=native -ffast-math -flto=auto" -DCMAKE_C_FLAGS="-march=native -ffast-math -flto=auto"

cmake --build build/release --config Release
```

## Web app

### GitHub Pages

The workflow in `.github/workflows/pages.yml` builds and deploys on every push to `master`.
You can also run it manually from the Actions tab.

Set **Settings → Pages → Build and deployment → Source → GitHub Actions**
in the GitHub repository before the first deployment.

The build uses Emscripten and raylib.
Generated HTML, JavaScript, WebAssembly, and asset data are uploaded from `dist/` directly to Pages; do not commit them.
Keep `shell.html`, `src/`, and `assets/` in version control.

### Local web build

Install the EMSDK once:

```bash
git clone --depth 1 --branch 6.0.12 https://github.com/emscripten-core/emsdk.git ~/emsdk
~/emsdk/emsdk install 6.0.12
~/emsdk/emsdk activate 6.0.12
```

If `~/emsdk` already exists, use that checkout to install and activate.
Clone the web dependency once from the nanotree repository root:

```bash
git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git build/web-raylib
```

Build after changing the source or assets:

```bash
source ~/emsdk/emsdk_env.sh
bash scripts/build-web.sh
```

This requires CMake and Make, and writes the site to `dist/`.
No system-wide raylib installation is needed.
