#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

if [ ! -d "$ROOT/docs" ]; then
  printf 'canonical docs directory missing: %s/docs\n' "$ROOT" >&2
  exit 1
fi

if [ -e "$ROOT/Documentation" ]; then
  printf 'legacy Documentation tree found inside yai: %s/Documentation\n' "$ROOT" >&2
  exit 1
fi

if [ -e "$ROOT/docs/internal/architecture/core-spine/new-core/docs" ]; then
  printf 'old active docs copy still exists inside yai docs/internal\n' >&2
  exit 1
fi

printf 'check-doc-canonical-location: ok\n'
