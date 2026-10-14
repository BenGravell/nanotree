#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

case "${1:-check}" in
  check) flags=(--dry-run --Werror) ;;
  apply) flags=(-i) ;;
  *) echo "Usage: $0 [check|apply] [files...]" >&2; exit 2 ;;
esac
if (( $# > 0 )); then
  shift
fi

if (( $# > 0 )); then
  clang-format --style=file "${flags[@]}" -- "$@"
else
  find src -type f \( -name '*.cpp' -o -name '*.h' \) \
    ! -name raygui.h -print0 |
    xargs -0 -r clang-format --style=file "${flags[@]}" --
fi
