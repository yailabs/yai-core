# Commands

Status: active implementation surface  
Owner: cmd  
Purpose: Local binaries for operator control and daemon execution.  
Not source of truth for: core data-plane semantics or public protocol contracts.

`cmd/yai` owns the technical control CLI.

`cmd/yaid` owns the daemon executable entrypoint.

Command output that becomes operator-facing must be reflected in
`docs/engineering/command-surface.md` and `docs/engineering/testing.md`.
