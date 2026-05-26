#!/usr/bin/env bash
# YAI - labs layout guard
#
# Purpose:
#   Ensure labs use the canonical README/runbook/notebook/run compact layout.
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
legacy_manuals_dir="docs/""manuals"
legacy_standards_dir="docs/labs/scientific-doc""s"
legacy_lab_tools="tools/""labs"
legacy_policy_packs="policy-""packs"
forbid_path "$legacy_manuals_dir"
forbid_path "$legacy_standards_dir"
forbid_path "$legacy_lab_tools"

require_file "docs/labs/README.md"
require_file "docs/labs/registry.md"
require_dir "docs/labs/_shared/bin"

for shared_tool in docs/labs/_shared/bin/*; do
  [ -f "$shared_tool" ] || continue
  case "$shared_tool" in
    *.sh)
      [ -x "$shared_tool" ] || fail "shared lab tool is not executable: $shared_tool"
      bash -n "$shared_tool" || fail "shared lab tool has invalid shell syntax: $shared_tool"
      ;;
  esac
done

found_lab=0

for lab_dir in docs/labs/*; do
  [ -d "$lab_dir" ] || continue
  [ "$lab_dir" != "docs/labs/_shared" ] || continue
  case "$lab_dir" in
    docs/labs/filesystem-loop|docs/labs/external-runtime) ;;
    *) fail "unexpected lab directory: $lab_dir" ;;
  esac
  found_lab=1

  forbid_path "$lab_dir/manual.md"
  forbid_path "$lab_dir/test.md"
  forbid_path "$lab_dir/analytics"
  forbid_path "$lab_dir/reports"
  forbid_path "$lab_dir/$legacy_policy_packs"

  require_file "$lab_dir/README.md"
  require_file "$lab_dir/runbook.md"
  require_file "$lab_dir/notebook.ipynb"
  require_file "$lab_dir/run.sh"
  require_file "$lab_dir/prompts.json"
  require_dir "$lab_dir/runs"

  [ -x "$lab_dir/run.sh" ] || fail "run script is not executable: $lab_dir/run.sh"
  bash -n "$lab_dir/run.sh" || fail "run script has invalid shell syntax: $lab_dir/run.sh"
  python3 -m json.tool "$lab_dir/prompts.json" >/dev/null || fail "invalid prompt catalog JSON: $lab_dir/prompts.json"

  if [ -e "$lab_dir/pack-fixture" ]; then
    require_dir "$lab_dir/pack-fixture"
    require_file "$lab_dir/pack-fixture/README.md"
  fi
done

[ "$found_lab" -eq 1 ] || fail "no lab directories found under docs/labs"

if find docs/labs -path "*/${legacy_policy_packs}" -print -quit | grep -q .; then
  fail "legacy policy pack directory exists under docs/labs"
fi

if find docs/labs \( -name manual.md -o -name test.md -o -name run.md \) -print -quit | grep -q .; then
  fail "legacy manual/test/run surface exists under docs/labs"
fi

if find docs/labs -path '*/analytics/*' -print -quit | grep -q .; then
  fail "legacy analytics directory exists under docs/labs"
fi

if grep -R "tools/""labs" README.md docs Makefile tools configs >/dev/null 2>&1; then
  fail "active legacy lab tool reference exists"
fi

printf 'check-labs-layout: ok\n'
