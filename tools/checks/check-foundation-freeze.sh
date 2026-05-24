#!/bin/sh
# YAI - foundation freeze guard
#
# Purpose:
#   Keep the post-foundation source and docs surface compact and intentional.
#
# Scope:
#   Checks required roots, forbidden roots, active engineering docs and wave
#   template contract terms.
#
# Non-goals:
#   Does not build or run runtime tests.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_dir() {
  if [ ! -d "$ROOT/$1" ]; then
    printf 'foundation freeze required directory missing: %s\n' "$1" >&2
    exit 1
  fi
}

for dir in include system engine cmd/yai cmd/yaid proto tests docs tools packaging examples vendor; do
  require_dir "$dir"
done

for forbidden in src lib crates ctl daemon agents runtime substrate orchestrator models capabilities lineage analytics governance knowledge state workflow; do
  if [ -e "$ROOT/$forbidden" ]; then
    printf 'foundation freeze forbidden root found: %s\n' "$forbidden" >&2
    exit 1
  fi
done

allowed_docs='
README.md
agent-operating-appendix.md
carrier-contract-v1.md
carrier-coverage-matrix.md
command-surface.md
control-carrier-rebase.md
current-status.md
data-plane-roadmap.md
extraction-plan.md
file-header-standard.md
filesystem-target.md
four-repo-roadmap.md
hot-state-plane.md
host-observation-probe.md
lmdb-record-plane.md
operation-dispatch-multiplex.md
operational-extraction-contract.md
pack-format.md
pack-roadmap.md
process-carrier-signal-control.md
source-surface.md
testing.md
wave-template.md
'

for path in "$ROOT"/docs/engineering/*; do
  name=$(basename "$path")
  if [ -f "$path" ]; then
    case "$allowed_docs" in
      *"
$name
"*) ;;
      *)
        printf 'unexpected active engineering doc: %s\n' "$name" >&2
        exit 1
        ;;
    esac
  fi
done

if ! grep -q 'SPINE.23.*Hot State Doctrine + ABI.*done' "$ROOT/docs/engineering/four-repo-roadmap.md"; then
  printf 'roadmap does not name SPINE.23 as completed hot-state doctrine sequence\n' >&2
  exit 1
fi

if ! grep -q 'SPINE.27 Hot State CLI + Manual Validation' "$ROOT/docs/engineering/current-status.md"; then
  printf 'current-status does not name SPINE.27 hot state cli manual validation\n' >&2
  exit 1
fi

for term in \
  'yai-dev source audit' \
  'Extraction decision table' \
  'YAI implementation' \
  'Command Surface' \
  'yai-dev residue normalization' \
  'Inventory update' \
  'Case-world impact' \
  'Data-plane impact' \
  'Projection/model impact' \
  'Observability/freshness impact' \
  'Tests:' \
  'Docs:' \
  'Validation:' \
  'Commit boundaries'
do
  if ! grep -q "$term" "$ROOT/docs/engineering/wave-template.md"; then
    printf 'wave-template missing operational contract term: %s\n' "$term" >&2
    exit 1
  fi
done

if ! grep -q 'An implementation wave must not be only additive inside `yai`' "$ROOT/docs/engineering/operational-extraction-contract.md"; then
  printf 'operational extraction additive-only guard missing\n' >&2
  exit 1
fi

printf 'check-foundation-freeze: ok\n'
