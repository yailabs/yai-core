#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
FORMAT="$ROOT/docs/engineering/pack-format.md"
ARCH="$ROOT/docs/architecture/15-pack-materialization.md"
ROADMAP="$ROOT/docs/engineering/four-repo-roadmap.md"
ADR="$ROOT/docs/adr/0017-packs-as-case-materialization-units.md"
INVENTORY="$ROOT/docs/internal/extraction-inventory.tsv"

for file in "$FORMAT" "$ARCH" "$ROADMAP" "$ADR" "$INVENTORY"; do
  if [ ! -f "$file" ]; then
    printf 'pack doctrine file missing: %s\n' "$file" >&2
    exit 1
  fi
done

for section in \
  identity kind owner version status compatibility domains materials policies \
  projection_rules authority_scopes procedures templates output_schemas \
  memory_seeds actors tools fixtures limits provenance validation retirement
do
  if ! grep -q "^$section$" "$FORMAT"; then
    printf 'pack manifest section missing from pack-format.md: %s\n' "$section" >&2
    exit 1
  fi
done

for kind in native_yai user organization vertical pilot reference experimental compat; do
  if ! grep -q "^$kind$" "$ROADMAP" && ! grep -q "^\\`$kind\\`" "$ARCH"; then
    printf 'pack kind missing from roadmap/architecture doctrine: %s\n' "$kind" >&2
    exit 1
  fi
done

for mapping in \
  'pack.domain        -> case_domain' \
  'pack.material      -> case_attachment' \
  'pack.binding       -> case_binding' \
  'pack.policy        -> policy_material / policy_rule' \
  'pack.projection    -> projection_rule' \
  'pack.authority     -> authority_scope'
do
  if ! grep -q "$mapping" "$ARCH"; then
    printf 'pack materialization mapping missing: %s\n' "$mapping" >&2
    exit 1
  fi
done

for old_path in \
  '../yai/src/case/packs/*' \
  '../yai/src/case/materialization/*' \
  '../yai/src/case/policy/*' \
  '../yai/src/case/scope/*' \
  '../yai/src/capabilities/*' \
  '../yai/src/agents/roles/*' \
  '../yai/src/agents/composition/*' \
  '../yai/src/models/frame/*' \
  '../yai/src/substrate/sources/*' \
  '../yai/src/substrate/views/*'
do
  if ! grep -q "SPINE.21	$old_path" "$INVENTORY"; then
    printf 'SPINE.21 extraction inventory row missing: %s\n' "$old_path" >&2
    exit 1
  fi
done

printf 'check-pack-doctrine: ok\n'
