# Filesystem Move Matrix

Status: NEW.14 partially applied move matrix.

This matrix is the operational checklist for NEW.14 through NEW.21. NEW.14 has
moved the Rust engine crates; later rows remain planned.

## Root Moves

| Current | Target | Wave | Notes |
|---|---|---|---|
| `crates/` | `engine/` plus `cmd/yai/` | NEW.14, NEW.15 | Engine crates moved in NEW.14; command remains for NEW.15. |
| `daemon/` | `cmd/yaid/` plus `system/daemon/` | NEW.16 | Split entrypoint from daemon implementation. |
| `lib/` | `system/` plus `system/engine_bridge` | NEW.17, NEW.18 | System C first, data bridge split second. |
| `ctl/` | removed or pointer README | NEW.15 | No implementation root after `cmd/yai` exists. |
| `include/` | `include/` | unchanged | Remains public ABI root. |

## Exact Current-To-Target Map

| Current path | Target path | Wave |
|---|---|---|
| `lib/base` | `system/base` | NEW.17 |
| `lib/case` | `system/case` | NEW.17 |
| `lib/subject` | `system/subject` | NEW.17 |
| `lib/op` | `system/op` | NEW.17 |
| `lib/control` | `system/control` | NEW.17 |
| `lib/effect` | `system/effect` | NEW.17 |
| `lib/daemon` | `system/daemon` | NEW.17 |
| `lib/internal` | `system/internal` | NEW.17 |
| `lib/store` | split: `system/engine_bridge` + `engine/yai-engine/src/store` | NEW.18 |
| `lib/graph` | split: `system/engine_bridge` + `engine/yai-engine/src/graph` | NEW.18 |
| `lib/index` | split: `system/engine_bridge` + `engine/yai-engine/src/index/query` | NEW.18 |
| `lib/memory` | split: `system/engine_bridge` + `engine/yai-engine/src/memory` | NEW.18 |
| `lib/projection` | split: `system/engine_bridge` + `engine/yai-engine/src/projection` | NEW.18 |
| `lib/reconcile` | split: `system/engine_bridge` + `engine/yai-engine/src/reconcile` | NEW.18 |
| `crates/yai-core-engine` | `engine/yai-engine` | NEW.14 done |
| `crates/yai-core-engine-sys` | `engine/yai-engine-ffi` | NEW.14 done |
| `crates/yai-ctl` | `cmd/yai` | NEW.15 |
| `daemon/main.c` | `cmd/yaid/main.c` | NEW.16 |
| `daemon/ipc.c` | `system/daemon/ipc.c` | NEW.16 |
| `daemon/core_loop.c` | `system/daemon/core_loop.c` | NEW.16 |
| `ctl/` | remove or pointer README after `cmd/yai` exists | NEW.15 |

## C System Files

| Current file | Target file | Disposition |
|---|---|---|
| `lib/base/error.c` | `system/base/error.c` | move |
| `lib/base/id.c` | `system/base/id.c` | move |
| `lib/case/case_ref.c` | `system/case/case_ref.c` | move |
| `lib/subject/subject_ref.c` | `system/subject/subject_ref.c` | move |
| `lib/subject/subject_binding.c` | `system/subject/subject_binding.c` | move |
| `lib/subject/subject_state.c` | `system/subject/subject_state.c` | move |
| `lib/op/attempt.c` | `system/op/attempt.c` | move |
| `lib/control/*.c` | `system/control/*.c` | move |
| `lib/effect/carrier.c` | `system/effect/carrier.c` | move |
| `lib/effect/effect_hash.c` | `system/effect/effect_hash.c` | move |
| `lib/effect/receipt.c` | `system/effect/receipt.c` | move |
| `lib/effect/carriers/filesystem_carrier.c` | `system/effect/carriers/filesystem_carrier.c` | move |
| `lib/daemon/daemon_status.c` | `system/daemon/daemon_status.c` | move |
| `lib/internal/string_util.c` | `system/internal/string_util.c` | move |

## Duplicate Data Logic Classification

| Current file | Target route | Initial classification |
|---|---|---|
| `lib/store/rust_engine_backend.c` | `system/engine_bridge/rust_engine_backend.c` | `keep_as_bridge` |
| `lib/store/journal.c` | `system/engine_bridge/journal.c`, then `engine/yai-engine/src/journal` | `keep_temporarily` |
| `lib/store/journal_file.c` | `system/engine_bridge/journal_file.c`, then `engine/yai-engine/src/journal` | `replace_with_rust` |
| `lib/store/record.c` | `system/engine_bridge/record.c`, then `engine/yai-engine/src/record` | `replace_with_rust` |
| `lib/store/record_codec.c` | `system/engine_bridge/record_codec.c`, then `engine/yai-engine/src/record` | `replace_with_rust` |
| `lib/graph/edge.c` | `system/engine_bridge/graph_edge.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `lib/graph/graph.c` | `system/engine_bridge/graph.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `lib/graph/reconstruct.c` | `system/engine_bridge/reconstruct.c`, then `engine/yai-engine/src/graph` | `replace_with_rust` |
| `lib/index/query.c` | `system/engine_bridge/query.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `lib/index/query_filter.c` | `system/engine_bridge/query_filter.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `lib/index/query_result.c` | `system/engine_bridge/query_result.c`, then `engine/yai-engine/src/query` | `replace_with_rust` |
| `lib/memory/memory_candidate.c` | `system/engine_bridge/memory_candidate.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `lib/memory/memory_kind.c` | `system/engine_bridge/memory_kind.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `lib/memory/memory_scope.c` | `system/engine_bridge/memory_scope.c`, then `engine/yai-engine/src/memory` | `replace_with_rust` |
| `lib/projection/freshness.c` | `system/engine_bridge/freshness.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/projection/projection.c` | `system/engine_bridge/projection.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/projection/projection_kind.c` | `system/engine_bridge/projection_kind.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/projection/projection_request.c` | `system/engine_bridge/projection_request.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/projection/projection_result.c` | `system/engine_bridge/projection_result.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/projection/redaction.c` | `system/engine_bridge/redaction.c`, then `engine/yai-engine/src/projection` | `replace_with_rust` |
| `lib/reconcile/divergence.c` | `system/engine_bridge/divergence.c`, then `engine/yai-engine/src/reconcile` | `replace_with_rust` |
| `lib/reconcile/reconcile.c` | `system/engine_bridge/reconcile.c`, then `engine/yai-engine/src/reconcile` | `replace_with_rust` |
| `lib/reconcile/recovery.c` | `system/engine_bridge/recovery.c`, then `engine/yai-engine/src/reconcile` | `delete_after_engine` |

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
| `crates/Cargo.toml` | transitional command workspace until NEW.15 | NEW.14 temporary |
| `crates/yai-ctl/Cargo.toml` | `cmd/yai/Cargo.toml` | NEW.15 |
| `crates/yai-ctl/src/main.rs` | `cmd/yai/src/main.rs` | NEW.15 |

## Daemon Files

| Current file | Target file | Wave |
|---|---|---|
| `daemon/main.c` | `cmd/yaid/main.c` | NEW.16 |
| `daemon/ipc.c` | `system/daemon/ipc.c` | NEW.16 |
| `daemon/core_loop.c` | `system/daemon/core_loop.c` | NEW.16 |
| `daemon/README.md` | `cmd/yaid/README.md` or `system/daemon/README.md` | NEW.16 |

## Final Placement Gate

After NEW.21:

```text
.c -> system/, cmd/yaid/, tests/
.h -> include/yai/, system/internal/ when private
.rs -> engine/, cmd/yai/
```

During NEW.14, `.rs` is allowed under `engine/` and under `crates/yai-ctl/`.
After NEW.21, no implementation source remains under `lib/`, `crates/`,
top-level `daemon/` or `ctl/`.
