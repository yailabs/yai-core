#!/usr/bin/env sh
set -eu

# YAI - Case Runtime Discovery guard
#
# Purpose:
#   Keep SPINE.51A as a discovery-only audit of ref, binding, authority,
#   resource scope, visibility and capability boundaries.
#
# Non-goals:
#   No CaseHandle, CapabilityLease, ResourceScope, projection, control,
#   carrier, facts, graph or provider implementation.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-case-runtime-discovery: %s\n' "$1" >&2
  exit 1
}

for file in \
  work/discovery/case-runtime/case-runtime-discovery.md \
  work/discovery/case-runtime/ref-binding-authority-map.tsv \
  work/discovery/case-runtime/yai-dev-extraction-candidates.tsv \
  work/discovery/case-runtime/implementation-gap-list.md \
  work/discovery/case-runtime/next-wave-recommendation.md
do
  [ -f "$file" ] || fail "missing file: $file"
done

ref_map="work/discovery/case-runtime/ref-binding-authority-map.tsv"
gap_list="work/discovery/case-runtime/implementation-gap-list.md"
extract_map="work/discovery/case-runtime/yai-dev-extraction-candidates.tsv"
discovery="work/discovery/case-runtime/case-runtime-discovery.md"

for phrase in \
  "case_ref" \
  "subject_ref" \
  "authority_scope"; do
  grep -F -- "$phrase" "$ref_map" >/dev/null || fail "ref map missing: $phrase"
done

if ! grep -F -- "resource scope" "$ref_map" >/dev/null &&
   ! grep -F -- "filesystem sandbox" "$ref_map" >/dev/null; then
  fail "ref map missing resource scope or filesystem sandbox"
fi

for phrase in \
  "CaseHandle" \
  "CapabilityLease" \
  "VisibilityScope" \
  "AuthorityScope"; do
  grep -F -- "$phrase" "$gap_list" >/dev/null || fail "gap list missing: $phrase"
done

grep -E -- "candidate_to_extract_from_yai_dev|obsolete_yai_dev|duplicate_yai_dev|future_case_handle|future_capability_lease|leave_archived" "$extract_map" >/dev/null ||
  fail "yai-dev extraction file contains no required classification"

for phrase in \
  "No implementation" \
  "Do not mutate yai-dev" \
  "future_resource_scope" \
  "future_visibility_scope" \
  "future_authority_scope" \
  "future_carrier_boundary"; do
  grep -F -- "$phrase" "$discovery" "$extract_map" "$gap_list" >/dev/null ||
    fail "missing discovery phrase: $phrase"
done

case "$(cat Makefile)" in
  *"check-case-runtime-discovery"* ) ;;
  *) fail "Makefile missing check-case-runtime-discovery" ;;
esac

printf 'check-case-runtime-discovery: ok\n'
