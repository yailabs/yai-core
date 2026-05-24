# smoke

Status: active test surface
Owner: tests/smoke
Purpose: End-to-end smoke checks for current spine behavior.
Not source of truth for: architecture doctrine or public API semantics.

Smoke tests prove current behavior and guard against drift. They should not add
new primitives without corresponding docs and command/manual validation when
operator-facing behavior changes.
