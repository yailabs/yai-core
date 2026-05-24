#!/bin/sh
# YAI - old source root guard
#
# Purpose:
#   Prevent retired source roots from reappearing in the active repository.
#
# Scope:
#   Checks top-level, include and system roots for forbidden old names.
#
# Non-goals:
#   Does not inspect archived historical docs.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

if [ -e "$ROOT/src" ]; then
  printf 'forbidden source root found: %s/src\n' "$ROOT" >&2
  exit 1
fi

for forbidden in agents analytics capabilities decision lineage models orchestrator runtime substrate governance knowledge state providers workflow; do
  for path in "$ROOT/$forbidden" "$ROOT/include/yai/$forbidden" "$ROOT/system/$forbidden"; do
    if [ -e "$path" ]; then
      printf 'forbidden core root found: %s\n' "$path" >&2
      exit 1
    fi
  done
done

printf 'check-no-old-roots: ok\n'
