Historical/superseded engineering record. Not an active source of truth.

# NEW.13 Filesystem Refactor Plan

Status: NEW.13 planning wave.

NEW.13 creates the executable plan for moving the repository from bootstrap
roots to the mature filesystem doctrine. It does not move files, rename crates,
edit C/Rust source, change build paths, alter tests or change installed
commands.

## Target

```text
include/ = public C ABI contracts
system/  = C host/system/control/carrier/daemon/FFI boundary
engine/  = Rust operational data spine
cmd/     = executable entrypoints: yai and yaid
```

Current transitional roots:

```text
lib/
crates/
ctl/
daemon/
```

Target roots:

```text
include/
system/
engine/
cmd/
proto/
tests/
docs/
tools/
packaging/
examples/
vendor/
```

## Current-To-Target Map

System C implementation:

| Current | Target |
|---|---|
| `lib/base` | `system/base` |
| `lib/case` | `system/case` |
| `lib/subject` | `system/subject` |
| `lib/op` | `system/op` |
| `lib/control` | `system/control` |
| `lib/effect` | `system/effect` |
| `lib/daemon` | `system/daemon` |
| `lib/internal` | `system/internal` |

Data-spine C surfaces:

| Current | Target |
|---|---|
| `lib/store` | split: `system/engine_bridge` + `engine/yai-engine/src/store` |
| `lib/graph` | split: `system/engine_bridge` + `engine/yai-engine/src/graph` |
| `lib/index` | split: `system/engine_bridge` + `engine/yai-engine/src/index/query` |
| `lib/memory` | split: `system/engine_bridge` + `engine/yai-engine/src/memory` |
| `lib/projection` | split: `system/engine_bridge` + `engine/yai-engine/src/projection` |
| `lib/reconcile` | split: `system/engine_bridge` + `engine/yai-engine/src/reconcile` |

Rust and command surfaces:

| Current | Target |
|---|---|
| `crates/yai-core-engine` | `engine/yai-engine` |
| `crates/yai-core-engine-sys` | `engine/yai-engine-ffi` |
| `crates/yai-ctl` | `cmd/yai` |
| `daemon/main.c` | `cmd/yaid/main.c` |
| `daemon/ipc.c` | `system/daemon/ipc.c` |
| `daemon/core_loop.c` | `system/daemon/core_loop.c` |
| `ctl/` | remove or pointer README after `cmd/yai` exists |

`include/` remains the public C ABI root. Public headers may continue to expose
store, graph, memory, projection and reconcile contracts while implementation
ownership moves behind the ABI.

## Refactor Order

| Wave | Work |
|---|---|
| NEW.14 | Move Rust engine workspace: `crates/yai-core-engine*` -> `engine/` |
| NEW.15 | Move `yai` command: `crates/yai-ctl` -> `cmd/yai` |
| NEW.16 | Move `yaid` entrypoint: `daemon/main.c` -> `cmd/yaid/main.c` |
| NEW.17 | Move C system implementation: `lib/` -> `system/` |
| NEW.18 | Split data C logic into engine bridge vs Rust engine ownership |
| NEW.19 | Realign Makefile, checks and source placement guards |
| NEW.20 | Local install layout: `yai` + `yaid` |
| NEW.21 | Filesystem refactor milestone freeze |

Do not collapse these moves. Each wave must preserve smoke behavior before the
next wave starts.

## Source Placement Target

Target placement after NEW.21:

```text
.c allowed under:
  system/
  cmd/yaid/
  tests/

.h allowed under:
  include/yai/
  system/internal/ if needed for private headers

.rs allowed under:
  engine/
  cmd/yai/
```

Forbidden after NEW.21:

```text
No Rust under crates/
No C under lib/
No top-level daemon/ source root
No ctl/ implementation root
```

Temporary exception: NEW.14 through NEW.18 may leave bootstrap paths alive while
the staged moves are in progress. NEW.14 allowed Rust under both `engine/` and
`crates/`; NEW.15 moves the command to `cmd/yai` and removes `crates/`. NEW.19
must update guards to reflect the current stage, and NEW.21 must freeze the
target placement.

## Build Target Plan

Expected Makefile transition:

```text
build-c:
  compiles system/ and cmd/yaid

build-rust:
  cargo build workspace over engine/ and cmd/yai

build:
  build-c + build-rust

smoke:
  unchanged behavior after each move
```

NEW.14 and NEW.15 should keep Rust workspace behavior stable while changing
paths. NEW.16 and NEW.17 should keep the C build graph stable while moving
entrypoints and implementation files. NEW.18 should keep ABI behavior stable
while moving ownership from duplicated C data logic toward Rust engine paths.

NEW.15 replaces the transitional split form with `engine/Cargo.toml` for the
engine workspace and standalone `cmd/yai/Cargo.toml` for the command crate.

## Guard Plan

| Guard | Plan |
|---|---|
| `check-no-old-roots` | unchanged; old legacy roots stay forbidden |
| `check-required-layout` | NEW.19 must expect `system/`, `engine/` and `cmd/` |
| `check-source-placement` | NEW.19 must reject C under `lib/`, Rust under `crates/`, top-level `daemon/` sources and `ctl/` implementation after the staged moves |
| docs checks | continue requiring canonical docs and no old-root language |

Docs must explain the temporary exception until NEW.21: bootstrap roots are
allowed only because refactor waves have not moved them yet.

## Data Spine Ownership

`engine/yai-engine` owns:

```text
residue
store
journal
record
codec
graph
index
query
memory
projection
reconcile
retention
integrity
```

`system` owns:

```text
daemon
host
carriers
control boundary
effect boundary
subject/case/op ABI implementation
FFI bridge
```

## Duplicate Logic Quarantine

C data logic currently in `lib/store`, `lib/graph`, `lib/index`, `lib/memory`,
`lib/projection` and `lib/reconcile` is transitional. NEW.18 must classify each
file as one of:

```text
keep_as_bridge
move_to_engine
replace_with_rust
delete_after_engine
keep_temporarily
```

The initial file-level classification is maintained in
`filesystem-move-matrix.md`.

## Old-YAI Audit Summary

NEW.13 inspected old-yai areas as concept evidence only. No old source is
copied and no folder is migrated.

| Old-yai area | NEW.13 route |
|---|---|
| `../yai/src/substrate/store/*` | `engine/store` or `system/engine_bridge` |
| `../yai/src/substrate/records/*` | `engine/record` |
| `../yai/src/substrate/graph/*` | `engine/graph` |
| `../yai/src/substrate/indexes/*` | `engine/index` |
| `../yai/src/substrate/query/*` | `engine/query` |
| `../yai/src/substrate/memory/*` | `engine/memory` |
| `../yai/src/substrate/views/*` | `engine/projection` |
| `../yai/src/substrate/signals/*` | `engine/reconcile` or `engine/memory` |
| `../yai/src/runtime/execution/*` | `system/effect`, `system/control` and `system/daemon` |
| `../yai/src/runtime/carriers/*` | `system/effect/carriers` |
| `../yai/src/runtime/machine/*` | `system/daemon` |
| `../yai/src/runtime/lifecycle/*` | `system/daemon` |
| `../yai/src/case/subjects/*` | `system/subject` and engine residue refs |
| `../yai/src/case/materialization/*` | `system/case` and engine residue |
| `../yai/src/case/surface/*` | projection/console future, not system UX |
| `../yai/src/lineage/*` | `engine/graph` and `engine/memory` |
| `../yai/src/analytics/*` | `engine/index`, `engine/memory`, `engine/reconcile` or `ai-environment` |

## Stub And Incomplete Verticalization

Any touched README or doc saying `placeholder`, `future`, `TODO` or `stub`
must be rewritten to name:

```text
transitional role
target role
owning future wave
```

or the material must be removed or quarantined. Stale bootstrap language must
not survive beside the new doctrine.

## Non-Implementation Rule

NEW.13 must not:

```text
move directories
rename crates
edit C/Rust source
change Makefile build paths
alter tests
alter installed commands
copy old-yai source
```
