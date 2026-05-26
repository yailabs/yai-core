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
carrier-outcome-harness.md
carrier-receipt-divergence.md
carrier-contract-v1.md
carrier-coverage-matrix.md
cli-review-interaction-surface.md
command-surface.md
control-carrier-rebase.md
current-status.md
data-context-runtime-runtimegraph.md
data-plane-roadmap.md
external-clori-boundary.md
extraction-plan.md
file-header-standard.md
filesystem-target.md
four-repo-roadmap.md
graph-persistence-runtimegraph.md
graph-runtimegraph-freeze.md
graph-relation-write-path.md
hot-state-plane.md
host-observation-probe.md
journal-replay-parser-hardening.md
journal-replay-to-lmdb.md
journal-replay-freeze.md
lmdb-record-plane.md
lmdb-record-plane-freeze.md
net-yai-boundary.md
non-process-carrier-skeletons.md
operation-dispatch-multiplex.md
operator-review-deferred-loop.md
operational-extraction-contract.md
pack-format.md
pack-roadmap.md
process-carrier-signal-control.md
provider-runtime-lan-target-surface.md
replay-idempotency-schema-version.md
replay-diagnostics-rebuild-report.md
review-loop-test-matrix.md
runtimegraph-rebuild.md
runtimegraph-query-causal-path.md
runtimegraph-working-set.md
source-surface.md
testing.md
wave-template.md
'

for path in "$ROOT"/work/archive/engineering-snapshots/*; do
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

if ! grep -q 'SPINE.23.*Hot State Doctrine + ABI.*done' "$ROOT/work/spines/yai-spine.md"; then
  printf 'roadmap does not name SPINE.23 as completed hot-state doctrine sequence\n' >&2
  exit 1
fi

if ! grep -q 'SPINE.27 Hot State CLI + Manual Validation' "$ROOT/work/spines/current-status.md"; then
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
  if ! grep -q "$term" "$ROOT/work/waves/wave-template.md"; then
    printf 'wave-template missing operational contract term: %s\n' "$term" >&2
    exit 1
  fi
done

if ! grep -q 'An implementation wave must not be only additive inside `yai`' "$ROOT/work/waves/operational-extraction-contract.md"; then
  printf 'operational extraction additive-only guard missing\n' >&2
  exit 1
fi

printf 'check-foundation-freeze: ok\n'
