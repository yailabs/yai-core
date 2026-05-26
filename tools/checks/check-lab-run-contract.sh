#!/usr/bin/env bash
# YAI - lab run contract guard
#
# Purpose:
#   Ensure committed lab runs use the compact run package contract.
#
# Scope:
#   Checks labs run folders for manifest, metrics, transcript, report and
#   asset structure.
#
# Non-goals:
#   Does not validate scientific claims or benchmark quality.
set -euo pipefail

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$repo_root"

fail() {
  printf 'check-lab-run-contract: %s\n' "$*" >&2
  exit 1
}

require_file() {
  [ -f "$1" ] || fail "missing required file: $1"
}

require_nonempty_file() {
  require_file "$1"
  [ -s "$1" ] || fail "file is empty: $1"
}

require_dir() {
  [ -d "$1" ] || fail "missing required directory: $1"
}

validate_json() {
  python3 -m json.tool "$1" >/dev/null || fail "invalid JSON: $1"
}

found_run=0

for runs_dir in labs/*/runs; do
  [ -d "$runs_dir" ] || continue

  for run_dir in "$runs_dir"/*; do
    [ -d "$run_dir" ] || continue
    found_run=1

    require_nonempty_file "$run_dir/transcript.md"
    require_nonempty_file "$run_dir/report.md"
    require_file "$run_dir/manifest.json"
    require_file "$run_dir/metrics.json"
    require_dir "$run_dir/assets"

    [ ! -e "$run_dir/run.md" ] || fail "forbidden run.md exists: $run_dir/run.md"
    [ ! -e "$run_dir/assets/notebook.ipynb" ] || fail "notebook copy found in run assets: $run_dir/assets/notebook.ipynb"

    first_line=""
    IFS= read -r first_line < "$run_dir/transcript.md" || true
    [ "$first_line" != "---" ] || fail "transcript has YAML frontmatter marker: $run_dir/transcript.md"

    validate_json "$run_dir/manifest.json"
    validate_json "$run_dir/metrics.json"
  done
done

[ "$found_run" -eq 1 ] || fail "no run directories found under labs/*/runs"

if find labs -path '*/runs/*/run.md' -print -quit | grep -q .; then
  fail "run.md exists under labs/*/runs"
fi

if find labs -path '*/runs/*/assets/notebook.ipynb' -print -quit | grep -q .; then
  fail "notebook.ipynb exists under run assets"
fi

printf 'check-lab-run-contract: ok\n'
