#!/bin/sh
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
current-status.md
data-plane-roadmap.md
extraction-plan.md
filesystem-target.md
four-repo-roadmap.md
hot-state-plane.md
operational-extraction-contract.md
pack-format.md
pack-roadmap.md
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

if ! grep -q 'SPINE.25 Hot State Case Session / Context Integration' "$ROOT/docs/engineering/current-status.md"; then
  printf 'current-status does not name SPINE.25 hot state session/context integration\n' >&2
  exit 1
fi

for term in \
  'Old-yai source audit' \
  'Extraction decision table' \
  'YAI-core implementation' \
  'Command Surface' \
  'Old-yai residue normalization' \
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

if ! grep -q 'An implementation wave must not be only additive inside `yai-core`' "$ROOT/docs/engineering/operational-extraction-contract.md"; then
  printf 'operational extraction additive-only guard missing\n' >&2
  exit 1
fi

printf 'check-foundation-freeze: ok\n'
