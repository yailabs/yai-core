#!/bin/sh
# YAI - pack doctrine guard
#
# Purpose:
#   Ensure pack materialization remains documented before runtime expansion.
#
# Scope:
#   Checks pack docs, ADR and inventory for required doctrine terms.
#
# Non-goals:
#   Does not implement or validate a pack runtime.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
FORMAT="$ROOT/work/archive/engineering-snapshots/pack-format.md"
ARCH="$ROOT/work/archive/architecture-snapshots/15-pack-materialization.md"
ROADMAP="$ROOT/work/spines/yai-spine.md"
ADR="$ROOT/work/archive/adr/0017-packs-as-case-materialization-units.md"
INVENTORY="$ROOT/work/inventories/extraction-inventory.tsv"

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
  '../yai-dev/src/case/packs/*' \
  '../yai-dev/src/case/materialization/*' \
  '../yai-dev/src/case/policy/*' \
  '../yai-dev/src/case/scope/*' \
  '../yai-dev/src/capabilities/*' \
  '../yai-dev/src/agents/roles/*' \
  '../yai-dev/src/agents/composition/*' \
  '../yai-dev/src/models/frame/*' \
  '../yai-dev/src/substrate/sources/*' \
  '../yai-dev/src/substrate/views/*'
do
  if ! grep -q "SPINE.21	$old_path" "$INVENTORY"; then
    printf 'SPINE.21 extraction inventory row missing: %s\n' "$old_path" >&2
    exit 1
  fi
done

printf 'check-pack-doctrine: ok\n'
