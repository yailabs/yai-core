# Filesystem Move Matrix

Status: NEW.17 applied system move matrix.

This matrix is the operational checklist for NEW.14 through NEW.21. NEW.14
moved the Rust engine crates, NEW.15 moved the Rust command, NEW.16 moved the
daemon entrypoint/support and NEW.17 moved remaining C implementation to
`system/`; data bridge thinning remains planned for NEW.18.

## Root Moves

| Current | Target | Wave | Notes |
|---|---|---|---|
| `crates/` | `engine/` plus `cmd/yai/` | NEW.14, NEW.15 | Completed; `crates/` removed after NEW.15. |
| `daemon/` | `cmd/yaid/` plus `system/daemon/` | NEW.16 | Completed; top-level `daemon/` removed. |
| `lib/` | `system/` plus `system/engine_bridge` | NEW.17, NEW.18 | NEW.17 physical move done; data bridge split second. |
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
| `system/store/rust_engine_backend.c` | `system/engine_bridge/rust_engine_backend.c` | `keep_as_bridge` |
| `system/store/journal.c` | `system/engine_bridge/journal.c`, then `engine/yai-engine/src/journal` | `keep_temporarily` |
| `system/store/journal_file.c` | `system/engine_bridge/journal_file.c`, then `engine/yai-engine/src/journal` | `replace_with_rust` |
| `system/store/record.c` | `system/engine_bridge/record.c`, then `engine/yai-engine/src/record` | `replace_with_rust` |
| `system/store/record_codec.c` | `system/engine_bridge/record_codec.c`, then `engine/yai-engine/src/record` | `replace_with_rust` |
| `system/graph/edge.c` | `system/engine_bridge/graph_edge.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `system/graph/graph.c` | `system/engine_bridge/graph.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `system/graph/reconstruct.c` | `system/engine_bridge/reconstruct.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `system/index/query.c` | `system/engine_bridge/query.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `system/index/query_filter.c` | `system/engine_bridge/query_filter.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `system/index/query_result.c` | `system/engine_bridge/query_result.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `system/memory/memory_candidate.c` | `system/engine_bridge/memory_candidate.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `system/memory/memory_kind.c` | `system/engine_bridge/memory_kind.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `system/memory/memory_scope.c` | `system/engine_bridge/memory_scope.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `system/projection/freshness.c` | `system/engine_bridge/freshness.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/projection/projection.c` | `system/engine_bridge/projection.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/projection/projection_kind.c` | `system/engine_bridge/projection_kind.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/projection/projection_request.c` | `system/engine_bridge/projection_request.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/projection/projection_result.c` | `system/engine_bridge/projection_result.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/projection/redaction.c` | `system/engine_bridge/redaction.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `system/reconcile/divergence.c` | `system/engine_bridge/divergence.c`, then `engine/yai-engine/src/reconcile` | `replace_with_rust` |
| `system/reconcile/reconcile.c` | `system/engine_bridge/reconcile.c`, then `engine/yai-engine/src/reconcile` | `replace_with_rust` |
| `system/reconcile/recovery.c` | `system/engine_bridge/recovery.c`, then `engine/yai-engine/src/reconcile` | `delete_after_engine` |

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

After NEW.17, `.rs` is allowed under `engine/` and `cmd/yai/`; `.c` is allowed
under `system/`, `cmd/yaid/` and `tests/`. No implementation source remains
under `crates/`, top-level `daemon/`, `lib/` or `ctl/`.
