# C Data Logic Thinning Map

Status: NEW.18 classification map.

This map classifies current C data-spine folders after NEW.17 and NEW.18.
The folders remain active smoke paths for now, but Rust `engine/yai-engine` is
the target owner of durable data-spine logic.

| Current folder | Current role | Target role | Immediate status | Future waves |
|---|---|---|---|---|
| `system/store` | C journal, record and codec path used by smokes | bridge or replaced by engine store/journal/record | `keep_temporarily` | NEW.23, NEW.24 |
| `system/graph` | C graph edge and reconstruction smoke path | engine graph | `keep_temporarily` | NEW.25 |
| `system/index` | C query/filter smoke path | engine query/index | `keep_temporarily` | NEW.24 |
| `system/memory` | C memory candidate smoke path | engine memory | `keep_temporarily` | NEW.28 |
| `system/projection` | C projection materialization and counts | engine projection | `keep_temporarily` | NEW.27 |
| `system/reconcile` | C divergence and reconcile smoke path | engine reconcile | `keep_temporarily` | NEW.29 |
| `system/engine_bridge` | C bridge into Rust engine | only C location for Rust engine FFI integration | active | NEW.18 onward |

## File Classification

| File | Classification | Notes |
|---|---|---|
| `system/engine_bridge/rust_engine_backend.c` | `keep_as_bridge` | Active C shim over Rust engine FFI. |
| `system/store/journal.c` | `keep_temporarily` | Smoke-backed C journal path until Rust parity. |
| `system/store/journal_file.c` | `keep_temporarily` | File-backed JSONL path used by smoke/manual flows. |
| `system/store/record.c` | `keep_temporarily` | C record construction path until engine record parity. |
| `system/store/record_codec.c` | `keep_temporarily` | C JSON codec path until engine codec parity. |
| `system/graph/edge.c` | `keep_temporarily` | C graph edge taxonomy smoke path. |
| `system/graph/graph.c` | `keep_temporarily` | C graph summary path. |
| `system/graph/reconstruct.c` | `keep_temporarily` | C reconstruction smoke path. |
| `system/index/query.c` | `keep_temporarily` | C query scan path. |
| `system/index/query_filter.c` | `keep_temporarily` | C filter path. |
| `system/index/query_result.c` | `keep_temporarily` | C result path. |
| `system/memory/memory_candidate.c` | `keep_temporarily` | C memory candidate construction path. |
| `system/memory/memory_kind.c` | `keep_temporarily` | C memory taxonomy path. |
| `system/memory/memory_scope.c` | `keep_temporarily` | C scope/freshness path. |
| `system/projection/freshness.c` | `keep_temporarily` | C freshness taxonomy path. |
| `system/projection/projection.c` | `keep_temporarily` | C projection summary path. |
| `system/projection/projection_kind.c` | `keep_temporarily` | C projection taxonomy path. |
| `system/projection/projection_request.c` | `keep_temporarily` | C request path. |
| `system/projection/projection_result.c` | `keep_temporarily` | C result path. |
| `system/projection/redaction.c` | `keep_temporarily` | C redaction taxonomy path. |
| `system/reconcile/divergence.c` | `keep_temporarily` | C divergence taxonomy path. |
| `system/reconcile/reconcile.c` | `keep_temporarily` | C reconcile record path. |
| `system/reconcile/recovery.c` | `keep_temporarily` | C recovery posture taxonomy path; candidate for deletion after engine parity. |

`keep_temporarily` means the file is intentionally retained to preserve current
smoke behavior. It is not target ownership.
