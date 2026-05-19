#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
OLD_NEW_CORE="$ROOT/../yai/Documentation/internal/architecture/core-spine/new-core"

if [ ! -d "$ROOT/docs" ]; then
  printf 'canonical docs directory missing: %s/docs\n' "$ROOT" >&2
  exit 1
fi

if [ -e "$OLD_NEW_CORE/docs" ]; then
  printf 'old active docs copy still exists: %s/docs\n' "$OLD_NEW_CORE" >&2
  exit 1
fi

if [ ! -f "$OLD_NEW_CORE/README.md" ]; then
  printf 'transition README missing: %s/README.md\n' "$OLD_NEW_CORE" >&2
  exit 1
fi

if ! grep -q 'yai-core/docs' "$OLD_NEW_CORE/README.md"; then
  printf 'transition README does not point to yai-core/docs\n' >&2
  exit 1
fi

printf 'check-doc-canonical-location: ok\n'
