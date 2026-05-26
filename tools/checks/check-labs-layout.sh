#!/usr/bin/env bash
# YAI - labs layout guard
#
# Purpose:
#   Ensure labs use the canonical runbook/notebook/run compact layout.
#
# Scope:
#   Checks docs/labs root, lab entries, run scripts and legacy lab paths.
#
# Non-goals:
#   Does not execute labs or validate run results.
set -euo pipefail

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$repo_root"

fail() {
  printf 'check-labs-layout: %s\n' "$*" >&2
  exit 1
}

require_file() {
  [ -f "$1" ] || fail "missing required file: $1"
}

require_dir() {
  [ -d "$1" ] || fail "missing required directory: $1"
}

forbid_path() {
  [ ! -e "$1" ] || fail "forbidden path exists: $1"
}

require_dir "docs/lab-standards"
legacy_standards_dir="docs/labs/scientific-doc""s"
forbid_path "$legacy_standards_dir"

require_file "docs/labs/README.md"
require_file "docs/labs/registry.md"

found_lab=0

for lab_dir in docs/labs/*; do
  [ -d "$lab_dir" ] || continue
  found_lab=1

  forbid_path "$lab_dir/manual.md"
  forbid_path "$lab_dir/test.md"
  forbid_path "$lab_dir/analytics"
  forbid_path "$lab_dir/reports"
  forbid_path "$lab_dir/outputs.md"
  forbid_path "$lab_dir/tests.md"
  forbid_path "$lab_dir/policy-packs"

  require_file "$lab_dir/README.md"
  require_file "$lab_dir/runbook.md"
  require_file "$lab_dir/notebook.ipynb"
  require_file "$lab_dir/run.sh"
  require_dir "$lab_dir/runs"

  [ -x "$lab_dir/run.sh" ] || fail "run script is not executable: $lab_dir/run.sh"
  bash -n "$lab_dir/run.sh" || fail "run script has invalid shell syntax: $lab_dir/run.sh"

  if [ -e "$lab_dir/pack-fixture" ]; then
    require_dir "$lab_dir/pack-fixture"
    require_file "$lab_dir/pack-fixture/README.md"
  fi
done

[ "$found_lab" -eq 1 ] || fail "no lab directories found under docs/labs"

if find docs/labs -path '*/policy-packs' -print -quit | grep -q .; then
  fail "policy-packs directory exists under docs/labs"
fi

printf 'check-labs-layout: ok\n'
