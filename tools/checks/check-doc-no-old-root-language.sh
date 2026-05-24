#!/bin/sh
# YAI - old root language guard
#
# Purpose:
#   Prevent active target docs from reintroducing retired root names.
#
# Scope:
#   Checks selected active architecture/filesystem docs for path-like old roots.
#
# Non-goals:
#   Does not scan historical archives.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
FILES="$ROOT/docs/architecture/00-spine.md $ROOT/docs/engineering/filesystem-target.md"

for term in agents analytics capabilities decision lineage models orchestrator runtime substrate governance knowledge state providers workflow; do
  if grep -n "$term/" $FILES >/tmp/yai-doc-old-root-hit 2>/dev/null; then
    cat /tmp/yai-doc-old-root-hit >&2
    printf 'old root appears as path-like target: %s/\n' "$term" >&2
    exit 1
  fi
done

rm -f /tmp/yai-doc-old-root-hit
printf 'check-doc-no-old-root-language: ok\n'
