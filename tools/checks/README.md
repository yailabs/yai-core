# Checks

Status: active guard surface
Owner: tools/checks
Purpose: Shell guards for source layout, documentation doctrine and wave freeze checks.
Not source of truth for: runtime behavior or feature semantics.

Guard scripts should carry a short `YAI - ...` purpose header. They should
fail with direct messages and avoid silently changing repository state.
