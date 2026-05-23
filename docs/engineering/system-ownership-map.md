# System Ownership Map

Status: NEW.17 active C implementation root.

`system/` is the C host, daemon, carrier, control and FFI-boundary plane. It
keeps the machine boundary explicit while Rust owns the operational data spine.

## Target Shape

```text
system/
├── base/
├── case/
├── subject/
├── op/
├── control/
├── effect/
│   └── carriers/
├── daemon/
├── host/
├── engine_bridge/
├── internal/
└── README.md
```

## Owned Areas

| System area | Owns | Does not own |
|---|---|---|
| `base/` | C ids, status and error implementation | data-spine algorithms |
| `case/` | C case ABI implementation | external case UX |
| `subject/` | C subject ABI implementation and binding surface | full external subject state |
| `op/` | C operation attempt ABI implementation | execution engine |
| `control/` | policy/control boundary, gates, decisions and obligations | model-provider policy internals |
| `effect/` | effect boundary and carrier request/receipt shell | unbounded host execution |
| `effect/carriers/` | host carriers with explicit receipt posture | hidden side effects |
| `daemon/` | resident local daemon implementation, IPC and bounded loop orchestration | product UX |
| `host/` | host integration boundary and future observation hooks | data persistence |
| `engine_bridge/` | C ABI/FFI bridge into Rust data spine | long-term duplicate data logic |
| `internal/` | private C helpers | public ABI contracts |

## Current Bootstrap Sources

| Current source | Target system area | Wave |
|---|---|---|
| `system/base/*` | `system/base/` | NEW.17 done |
| `system/case/*` | `system/case/` | NEW.17 done |
| `system/subject/*` | `system/subject/` | NEW.17 done |
| `system/op/*` | `system/op/` | NEW.17 done |
| `system/control/*` | `system/control/` | NEW.17 done |
| `system/effect/*` | `system/effect/` | NEW.17 done |
| `system/effect/carriers/*` | `system/effect/carriers/` | NEW.17 done |
| `lib/daemon/daemon_status.c` | `system/daemon/daemon_status.c` | NEW.16 done |
| `system/internal/*` | `system/internal/` | NEW.17 done |
| `daemon/ipc.c` | `system/daemon/ipc.c` | NEW.16 done |
| `daemon/core_loop.c` | `system/daemon/core_loop.c` | NEW.16 done |
| `system/engine_bridge/rust_engine_backend.c` | `system/engine_bridge/` | NEW.18 done |

## Command Boundary

`cmd/` owns executable entrypoints:

```text
cmd/yai/        Rust technical command
cmd/yaid/main.c C daemon entrypoint
```

`system/daemon` owns daemon implementation. `cmd/yaid/main.c` should stay thin:
parse process-level arguments, initialize daemon configuration and call into
`system/daemon`.

## Old-YAI Evidence Routes

| Old-yai material | System route | Classification |
|---|---|---|
| `../yai/src/runtime/execution/*` | `system/effect`, `system/control`, `system/daemon` | mine envelopes, control bridge and receipt posture |
| `../yai/src/runtime/activation/*` | `system/daemon` | mine boot, mode selection and startup evidence |
| `../yai/src/runtime/connections/*` | `system/daemon/ipc` | mine local client connection evidence |
| `../yai/src/runtime/carriers/*` | `system/effect/carriers` | mine host-mediated carrier posture |
| `../yai/src/runtime/machine/*` | `system/daemon` | mine resident loop evidence |
| `../yai/src/runtime/lifecycle/*` | `system/daemon` | mine status/readiness/fail-closed posture |
| `../yai/tools/runtime/*` | `system/daemon/tooling` | mine tooling posture only |
| `../yai/src/case/subjects/*` | `system/subject` plus engine residue refs | mine subject binding and evidence trace |
| `../yai/src/case/materialization/*` | `system/case` plus engine residue | mine materialization contracts |
| `../yai/src/case/surface/*` | projection/console future | do not absorb as system UX |

## System / Engine Split

System code can ask the engine to persist, reconstruct, query, project or
detect mismatch. System code must not become the long-term owner of store,
journal, record codec, graph, index, query, memory, projection, reconcile,
retention or integrity logic.

After NEW.18, `system/engine_bridge` is the active bridge into Rust. The folders
`system/{store,graph,index,memory,projection,reconcile}` are physical C system
paths but still transitional data-spine logic classified `keep_temporarily`.
They preserve smoke behavior while Rust engine ownership reaches parity.

## Acceptance Target

By NEW.21:

```text
C system source lives under system/
yaid entrypoint lives under cmd/yaid/
Rust command source lives under cmd/yai/
Rust engine source lives under engine/
top-level daemon/ is gone as a source root
lib/ is gone as a C implementation root
ctl/ is gone as an implementation root
```
