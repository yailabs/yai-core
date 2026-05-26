Historical/superseded engineering record. Not an active source of truth.

# Filesystem Move Matrix

Status: NEW.18 applied engine bridge split matrix.

This matrix is the operational checklist for NEW.14 through NEW.21. NEW.14
moved the Rust engine crates, NEW.15 moved the Rust command, NEW.16 moved the
daemon entrypoint/support, NEW.17 moved remaining C implementation to
`system/` and NEW.18 centralized the Rust engine C bridge under
`system/engine_bridge`.

## Root Moves

| Current | Target | Wave | Notes |
|---|---|---|---|
| `crates/` | `engine/` plus `cmd/yai/` | NEW.14, NEW.15 | Completed; `crates/` removed after NEW.15. |
| `daemon/` | `cmd/yaid/` plus `system/daemon/` | NEW.16 | Completed; top-level `daemon/` removed. |
| `lib/` | `system/` plus `system/engine_bridge` | NEW.17, NEW.18 | Completed; C implementation moved and Rust bridge centralized. |
| `ctl/` | removed | NEW.17 | Retired pointer removed after C system move. |
| `include/` | `include/` | unchanged | Remains public ABI root. |

## Exact Current-To-Target Map

| Current path | Target path | Wave |
|---|---|---|
| `lib/base` | `system/base` | NEW.17 done |
| `lib/case` | `system/case` | NEW.17 done |
| `lib/subject` | `system/subject` | NEW.17 done |
| `lib/op` | `system/op` | NEW.17 done |
| `lib/control` | `system/control` | NEW.17 done |
| `lib/effect` | `system/effect` | NEW.17 done |
| `lib/daemon` | `system/daemon` | NEW.16 done |
| `lib/internal` | `system/internal` | NEW.17 done |
| `lib/store` | `system/store`, then split: `system/engine_bridge` + `engine/yai-engine/src/store` | NEW.17 done, NEW.18 |
| `lib/graph` | `system/graph`, then split: `system/engine_bridge` + `engine/yai-engine/src/graph` | NEW.17 done, NEW.18 |
| `lib/index` | `system/index`, then split: `system/engine_bridge` + `engine/yai-engine/src/index/query` | NEW.17 done, NEW.18 |
| `lib/memory` | `system/memory`, then split: `system/engine_bridge` + `engine/yai-engine/src/memory` | NEW.17 done, NEW.18 |
| `lib/projection` | `system/projection`, then split: `system/engine_bridge` + `engine/yai-engine/src/projection` | NEW.17 done, NEW.18 |
| `lib/reconcile` | `system/reconcile`, then split: `system/engine_bridge` + `engine/yai-engine/src/reconcile` | NEW.17 done, NEW.18 |
| `crates/yai-core-engine` | `engine/yai-engine` | NEW.14 done |
| `crates/yai-core-engine-sys` | `engine/yai-engine-ffi` | NEW.14 done |
| `crates/yai-ctl` | `cmd/yai` | NEW.15 done |
| `daemon/main.c` | `cmd/yaid/main.c` | NEW.16 done |
| `daemon/ipc.c` | `system/daemon/ipc.c` | NEW.16 done |
| `daemon/core_loop.c` | `system/daemon/core_loop.c` | NEW.16 done |
| `ctl/` | removed | NEW.17 done |

## C System Files

| Current file | Target file | Disposition |
|---|---|---|
| `system/base/error.c` | `system/base/error.c` | moved in NEW.17 |
| `system/base/id.c` | `system/base/id.c` | moved in NEW.17 |
| `system/case/case_ref.c` | `system/case/case_ref.c` | moved in NEW.17 |
| `system/subject/subject_ref.c` | `system/subject/subject_ref.c` | moved in NEW.17 |
| `system/subject/subject_binding.c` | `system/subject/subject_binding.c` | moved in NEW.17 |
| `system/subject/subject_state.c` | `system/subject/subject_state.c` | moved in NEW.17 |
| `system/op/attempt.c` | `system/op/attempt.c` | moved in NEW.17 |
| `system/control/*.c` | `system/control/*.c` | moved in NEW.17 |
| `system/effect/carrier.c` | `system/effect/carrier.c` | moved in NEW.17 |
| `system/effect/effect_hash.c` | `system/effect/effect_hash.c` | moved in NEW.17 |
| `system/effect/receipt.c` | `system/effect/receipt.c` | moved in NEW.17 |
| `system/effect/carriers/filesystem_carrier.c` | `system/effect/carriers/filesystem_carrier.c` | moved in NEW.17 |
| `lib/daemon/daemon_status.c` | `system/daemon/daemon_status.c` | moved in NEW.16 |
| `system/internal/string_util.c` | `system/internal/string_util.c` | moved in NEW.17 |

## Duplicate Data Logic Classification

| Current file | Target route | Initial classification |
|---|---|---|
| `system/engine_bridge/rust_engine_backend.c` | `system/engine_bridge/rust_engine_backend.c` | `keep_as_bridge` |
| `system/store/journal.c` | `engine/yai-engine/src/journal` | `keep_temporarily` |
| `system/store/journal_file.c` | `engine/yai-engine/src/journal` | `keep_temporarily` |
| `system/store/record.c` | `engine/yai-engine/src/record` | `keep_temporarily` |
| `system/store/record_codec.c` | `engine/yai-engine/src/record` | `keep_temporarily` |
| `system/graph/edge.c` | `engine/yai-engine/src/graph` | `keep_temporarily` |
| `system/graph/graph.c` | `engine/yai-engine/src/graph` | `keep_temporarily` |
| `system/graph/reconstruct.c` | `engine/yai-engine/src/graph` | `keep_temporarily` |
| `system/index/query.c` | `engine/yai-engine/src/query` | `keep_temporarily` |
| `system/index/query_filter.c` | `engine/yai-engine/src/query` | `keep_temporarily` |
| `system/index/query_result.c` | `engine/yai-engine/src/query` | `keep_temporarily` |
| `system/memory/memory_candidate.c` | `engine/yai-engine/src/memory` | `keep_temporarily` |
| `system/memory/memory_kind.c` | `engine/yai-engine/src/memory` | `keep_temporarily` |
| `system/memory/memory_scope.c` | `engine/yai-engine/src/memory` | `keep_temporarily` |
| `system/projection/freshness.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/projection/projection.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/projection/projection_kind.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/projection/projection_request.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/projection/projection_result.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/projection/redaction.c` | `engine/yai-engine/src/projection` | `keep_temporarily` |
| `system/reconcile/divergence.c` | `engine/yai-engine/src/reconcile` | `keep_temporarily` |
| `system/reconcile/reconcile.c` | `engine/yai-engine/src/reconcile` | `keep_temporarily` |
| `system/reconcile/recovery.c` | `engine/yai-engine/src/reconcile` | `keep_temporarily` |

`keep_as_bridge` means the C file is an intentional ABI/FFI bridge.
`keep_temporarily` means it stays only until the Rust engine has parity and the
guard wave can remove bootstrap assumptions.

## Rust Files

| Current file | Target file | Wave |
|---|---|---|
| `engine/Cargo.toml` | `engine/Cargo.toml` | NEW.14 done |
| `engine/yai-engine/Cargo.toml` | `engine/yai-engine/Cargo.toml` | NEW.14 done |
| `engine/yai-engine/src/*.rs` | `engine/yai-engine/src/` | NEW.14 done |
| `engine/yai-engine-ffi/Cargo.toml` | `engine/yai-engine-ffi/Cargo.toml` | NEW.14 done |
| `engine/yai-engine-ffi/src/lib.rs` | `engine/yai-engine-ffi/src/lib.rs` | NEW.14 done |
| `cmd/yai/Cargo.toml` | `cmd/yai/Cargo.toml` | NEW.15 done |
| `cmd/yai/src/main.rs` | `cmd/yai/src/main.rs` | NEW.15 done |

## Daemon Files

| Current file | Target file | Wave |
|---|---|---|
| `cmd/yaid/main.c` | `cmd/yaid/main.c` | NEW.16 done |
| `system/daemon/ipc.c` | `system/daemon/ipc.c` | NEW.16 done |
| `system/daemon/core_loop.c` | `system/daemon/core_loop.c` | NEW.16 done |
| `system/daemon/daemon_status.c` | `system/daemon/daemon_status.c` | NEW.16 done |

## Final Placement Gate

After NEW.21:

```text
.c -> system/, cmd/yaid/, tests/
.h -> include/yai/, system/internal/ when private
.rs -> engine/, cmd/yai/
```

After NEW.18, `.rs` is allowed under `engine/` and `cmd/yai/`; `.c` is allowed
under `system/`, `cmd/yaid/` and `tests/`. No implementation source remains
under `crates/`, top-level `daemon/`, `lib/` or `ctl/`.
