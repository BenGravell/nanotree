# Setup

The quick start targets Ubuntu 24.04 with Bash. Desktop use requires a graphical
session with OpenGL. Downloads require internet access.

## Install

Install Git if needed, then clone the project:

```bash
sudo apt update && sudo apt install -y git
git clone https://github.com/BenGravell/nanotree.git
cd nanotree
bash scripts/setup-ubuntu.sh
```

The setup script installs build tools, raylib system dependencies, Conan 2 and
pre-commit, then enables Git hooks for this checkout. It uses `sudo` for Ubuntu
packages; run the script as your normal user. Open a new terminal afterward so
pipx tools are on `PATH`, then return to the repository directory.

On other systems, install a C++20 compiler, CMake 3.23+, Ninja, Python, Conan 2,
pre-commit, clang-format 18, and the platform's raylib dependencies. Then run
`pre-commit install` from the checkout. No system-wide raylib installation is needed.

## Desktop

From the repository root:

```bash
bash scripts/build-desktop.sh
./build/release/nanotree
```

The build script creates a default Conan profile if missing, installs raylib 5.5,
configures CMake, and builds. Existing profiles are preserved. Rerun the same script
after source or dependency changes. Release and debug have separate dependency
output directories.

For debug builds:

```bash
bash scripts/build-desktop.sh debug
./build/debug/nanotree
```

Launch the app from the repository root so it can find `assets/`.

## Web (optional)

After installing the build tools, run:

```bash
bash scripts/setup-web.sh
bash scripts/build-web.sh
python3 -m http.server 8000 --directory dist
```

Open <http://localhost:8000> and stop the server with Ctrl+C.

Web setup installs Emscripten 6.0.12 and clones raylib 5.5 into `build/`, matching
the GitHub Pages workflow. It reuses existing checkouts on subsequent runs.
The build script loads the local SDK automatically; no shell setup is needed.
Rerun only `bash scripts/build-web.sh` after source or asset changes.
The generated site is written to `dist/`.

## Pre-commit checks

Commits check staged files for merge conflicts, valid YAML/JSON, trailing
whitespace, final newlines, and C++ formatting. Vendored `src/raygui.h` is excluded;
whitespace hooks also skip bundled font assets. The first run downloads the hook
environments.

Check everything manually:

```bash
pre-commit run --all-files
```

Whitespace hooks fix files automatically. For C++ formatting fixes, run
`bash scripts/format-cpp.sh apply`. Review and stage the edits, then commit again.
See [Development](DEVELOPMENT.md) for more formatting commands and source layout.

## GitHub Pages

Set **Settings → Pages → Build and deployment → Source → GitHub Actions** in the
GitHub repository before the first deployment.

[The workflow](../.github/workflows/pages.yml) builds and deploys on pushes to
`master`, and can also be started from the Actions tab. It uploads `dist/`
directly. Keep generated files in `build/` and `dist/` out of version control.

## Troubleshooting

- **`conan` or `pre-commit` not found:** open a new terminal after setup.
- **Missing system headers:** rerun `bash scripts/setup-ubuntu.sh` on Ubuntu.
- **Web tools or dependencies missing:** run `bash scripts/setup-web.sh` first.
- **Assets or fonts fail to load:** launch the desktop app from the repository root.

See [Usage](USAGE.md) for the app controls.
