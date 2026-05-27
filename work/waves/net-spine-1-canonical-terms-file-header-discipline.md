# NET.SPINE.1 - Canonical Terms + File/Header Discipline

## Purpose

Establish canonical NET terminology, public header naming discipline, module area README discipline and guard coverage before runtime NET work begins.

## Files Changed

- `include/yai/net/*.h`
- `net/README.md`
- `net/*/README.md`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `work/protocols/net.md`
- `work/spines/net-spine.md`
- `Makefile`

## Header Discipline Established

Public NET headers use stable `YAI_NET_*_H` include guards, YAI-style header blocks, C-compatible declarations, `yai_net_` type names and `YAI_NET_` enum or macro names. They expose vocabulary only and do not include system, engine, socket, HTTP, discovery or runtime transport implementation surfaces.

## Terminology Source

Canonical NET vocabulary lives at `work/protocols/net.md`.

## Guard Additions

- `tools/checks/check-net-headers.sh` verifies required public headers, guards, public symbol prefixes, forbidden internal includes, forbidden network includes and forbidden runtime function declarations.
- `tools/checks/check-net-boundary.sh` now checks NET.SPINE.1.0 source truth, protocol vocabulary presence, integrated module topology, header presence, no CLORI vendoring and no old copied spine path.

## Non-Goals

- no discovery
- no transport
- no sockets
- no server
- no CLI behavior
- no CLORI code
- no YAI system extraction
- no YAI engine extraction

## Validation Commands

```sh
git diff --check
make info
make net-info
tools/checks/check-net-headers.sh
make check-net-headers
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
```

## Known Unrelated Dirty Baseline

At wave start, `.vscode/PythonImportHelper-v2-Completion.json` was already dirty and is not part of this wave. Known unrelated failures may still include trailing whitespace in `work/agents/agent-operating-appendix.md` and an unexpected active engineering doc `spine45a-documentation-root-canon-collapse.md`; these are not fixed by NET.SPINE.1.
