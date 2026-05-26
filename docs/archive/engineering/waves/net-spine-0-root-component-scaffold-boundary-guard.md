Historical/superseded engineering record. Not an active source of truth.

# NET.SPINE.0 - Root Component Scaffold + Boundary Guard

Status: complete when NET scaffold, headers, docs and guard pass.

NET.SPINE.0 creates NET as a real root-level YAI component scaffold. It mounts
`net/`, public vocabulary headers, README-only source roots, component docs,
local validation targets and a boundary guard.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

CLORI extension:
YAI controls authority.
NET moves streams.
CLORI executes neural computation.

## Scope

- create `net/` root component layout
- create `net/include/yai/net/` header vocabulary
- create `net/docs/` component docs
- add `net/Makefile`
- add `tools/checks/check-net-boundary.sh`
- rewrite `docs/spines/net-spine.md` to NET.SPINE.0.2
- update YAI NET boundary docs as pointers to component docs

## Non-Goals

- no LAN discovery implementation
- no remote transport implementation
- no server implementation
- no fake CLI behavior
- no CLORI support implementation
- no rewrite of the existing 120-wave YAI main spine
