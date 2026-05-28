#!/usr/bin/env sh
set -eu

# YAI - CaseHandle / CapabilityLease Boundary guard
#
# Purpose:
#   Keep SPINE.51B as the first runtime-resolved case/scope/capability
#   boundary before Live Projection Frame Schema.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-casehandle-capability-boundary: %s\n' "$1" >&2
  exit 1
}

for file in \
  include/yai/case/case_handle.h \
  include/yai/subject/subject_handle.h \
  include/yai/control/authority_scope.h \
  include/yai/projection/visibility_scope.h \
  include/yai/effect/resource_scope.h \
  include/yai/control/capability_lease.h \
  system/case/case_handle.c \
  system/subject/subject_handle.c \
  system/control/authority_scope.c \
  system/projection/visibility_scope.c \
  system/effect/resource_scope.c \
  system/control/capability_lease.c \
  tests/smoke/casehandle-capability-boundary/test_casehandle_capability_boundary.sh \
  work/waves/spine51b-casehandle-capability-boundary.md
do
  [ -f "$file" ] || fail "missing file: $file"
done

for phrase in \
  "yai case resolve --case" \
  "yai case scope --case" \
  "yai capability derive --case" \
  "refs_are_authority: false" \
  "subject_lacks_execute_authority" \
  "resource_outside_scope" \
  "carrier_dispatch_allowed"; do
  grep -F -- "$phrase" cmd/yai/src/main.rs tests/smoke/casehandle-capability-boundary/test_casehandle_capability_boundary.sh >/dev/null ||
    fail "command surface missing: $phrase"
done

for phrase in \
  "CaseHandle" \
  "SubjectHandle" \
  "CapabilityLease" \
  "ResourceScope" \
  "VisibilityScope" \
  "AuthorityScope" \
  "refs are identifiers, not authority" \
  "bindings are relations, not capabilities" \
  "case resolve" \
  "case scope" \
  "capability derive" \
  "carrier dispatch allowed" \
  "resource_outside_scope" \
  "subject_lacks_execute_authority"; do
  grep -F -- "$phrase" \
    work/waves/spine51b-casehandle-capability-boundary.md \
    work/spines/command-surface.md \
    work/spines/testing.md \
    work/spines/current-status.md \
    work/spines/yai-spine.md \
    docs/architecture/system.md \
    docs/architecture/control-runtime.md \
    docs/architecture/data-runtime.md \
    labs/filesystem-loop/runbook.md \
    labs/filesystem-loop/tests.md \
    labs/filesystem-loop/outputs.md >/dev/null ||
      fail "docs missing: $phrase"
done

for label in \
  "case_handle:resolve llm-provider ok" \
  "case_scope:model no_execute ok" \
  "capability:model_write denied ok" \
  "capability:filesystem_write requires_review ok" \
  "capability:filesystem_read minted ok" \
  "capability:outside_sandbox denied ok" \
  "refs:not_authority ok"; do
  grep -F -- "$label" tests/smoke/casehandle-capability-boundary/test_casehandle_capability_boundary.sh >/dev/null ||
    fail "smoke missing label: $label"
done

case "$(cat Makefile)" in
  *"check-casehandle-capability-boundary"* ) ;;
  *) fail "Makefile missing check-casehandle-capability-boundary" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine51b"* ) ;;
  *) fail "Makefile missing smoke-spine51b" ;;
esac

printf 'check-casehandle-capability-boundary: ok\n'
