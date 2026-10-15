#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

sudo apt update
sudo apt install -y build-essential git cmake ninja-build python3 python3-venv pipx clang-format \
  libasound2-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev \
  libxi-dev libxcursor-dev libxinerama-dev libxext-dev libxfixes-dev \
  libxxf86vm-dev libwayland-dev libxkbcommon-dev

pipx install 'conan>=2,<3'
pipx install pre-commit
pipx ensurepath
export PATH="$(pipx environment --value PIPX_BIN_DIR):$PATH"
pre-commit install

echo 'Setup complete. Open a new terminal, then run: bash scripts/build-desktop.sh'
