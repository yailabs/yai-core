#!/bin/sh
# YAI - source surface cleanliness guard
#
# Purpose:
#   Keep local environment roots and archived placeholders out of active source.
#
# Scope:
#   Checks environment directories, ingest placeholders and transitional banners.
#
# Non-goals:
#   Does not enforce every source file header.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

for dir in venv .venv env ENV; do
  if [ -e "$ROOT/$dir" ]; then
    printf 'local environment root found in repo: %s\n' "$dir" >&2
    exit 1
  fi
done

for dir in system/ingest include/yai/ingest; do
  if [ -e "$ROOT/$dir" ]; then
    printf 'README-only ingest placeholder root must stay archived: %s\n' "$dir" >&2
    exit 1
  fi
done

for dir in system/store system/graph system/index system/memory system/projection system/reconcile; do
  if [ ! -f "$ROOT/$dir/README.md" ]; then
    printf 'transitional shim README missing: %s/README.md\n' "$dir" >&2
    exit 1
  fi
  if ! grep -Fq 'Transitional C shim' "$ROOT/$dir/README.md"; then
    printf 'transitional shim banner missing: %s/README.md\n' "$dir" >&2
    exit 1
  fi
  if ! grep -Fq 'Future ownership: engine/yai-engine' "$ROOT/$dir/README.md"; then
    printf 'future engine ownership missing: %s/README.md\n' "$dir" >&2
    exit 1
  fi
done

if ! grep -Fq 'system/ is not a second data engine' "$ROOT/work/spines/source-surface.md"; then
  printf 'source-surface doctrine missing system/engine boundary\n' >&2
  exit 1
fi

if ! grep -Fq 'engine/ is the Rust data spine target' "$ROOT/work/spines/source-surface.md"; then
  printf 'source-surface doctrine missing Rust engine ownership\n' >&2
  exit 1
fi

printf 'check-source-surface-clean: ok\n'
