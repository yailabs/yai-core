# NEW.10 Rust Operational Data Engine R1

NEW.10 introduces the first real Rust operational data engine integration behind
the C ABI.

This is not a rewrite of the core in Rust. C still owns public ABI and semantic
contracts. Rust owns an internal operational data engine path behind opaque FFI
handles and serialized record input.

## R1 Capabilities

```text
create/free engine handle
append record JSON
count records
count by record kind
query by record kind
build projection summary JSON
```

## FFI Boundary

Rust exports:

```text
yai_engine_new
yai_engine_free
yai_engine_append_record_json
yai_engine_record_count
yai_engine_count_kind
yai_engine_query_kind
yai_engine_projection_summary_json
yai_engine_last_error
yai_engine_string_free
```

The ABI rules are:

```text
opaque handle only
explicit free
no Rust structs across ABI
const char* JSON/JSONL input
caller-owned output buffers
integer status codes
deterministic error strings
```

## C Shim

The C-facing shim is:

```text
include/yai/store/rust_engine_backend.h
lib/store/rust_engine_backend.c
```

It exposes `yai_rust_engine_*` wrapper functions and keeps the raw Rust symbols
behind the backend boundary.

## Default Truth Path

The existing C file-backed journal remains active and tested. Rust R1 consumes
the same `yai.store.record.v0` JSONL residue as an internal backend/mirror path.
It does not replace the C journal as default truth.

## yai

`yai engine summary --journal <path>` loads an existing JSONL journal through
the Rust engine path and prints:

```text
records
receipts
graph_edges
memory_candidates
projections
divergences
```

## Smoke Proof

`tests/smoke/rust-engine-r1/test_rust_engine_r1.c` creates a Rust engine through
the C shim, appends eight JSON records, counts records, counts receipts, queries
memory candidates and builds a projection summary JSON.

## Old-YAI Audit

NEW.10 inspected old substrate and cJSON material without copying source:

```text
src/substrate/store/*
src/substrate/query/*
src/substrate/indexes/*
src/substrate/views/*
src/substrate/records/*
src/substrate/memory/*
vendor/cjson/*
include/yai/substrate/*
```

Extracted concepts:

```text
store is persistence/backend posture, not semantic owner
query is read posture, not authority
views carry projection refs, freshness and confidence evidence
cJSON is compatibility evidence only
old substrate headers are compatibility material, not future include roots
```

Deferred:

```text
external database backend
vector/RAG retrieval
graph traversal backend
memory consolidation
backend switch
dual-write parity
```
