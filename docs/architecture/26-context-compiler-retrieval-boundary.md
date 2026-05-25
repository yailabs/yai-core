# Context Compiler / Retrieval Boundary

Status: SPINE.33K roadmap correction.

Purpose: define the future context compiler and retrieval boundary before
retrieval implementation begins.

## Doctrine

Projection does not disappear.

SPINE.33L does not implement the Context Compiler. It exposes
`yai model runtime status` so `context_compiler`, `retrieval_hnsw`, model
routing and decoding acceleration posture are visible from CLI before runtime
implementation.

Projection evolves into the controlled view produced by the context compiler.
A compiled model context is a temporary, scoped, freshness-aware consumer view.
It is not truth. It is not memory. It is not the record plane. It is not the
graph.

```text
Journal / LMDB / Ladybug / DuckDB
        ↓ rebuild/load
RuntimeGraph
        ↓ retrieval bindings
HNSW
        ↓ candidates
RuntimeGraph expansion + policy/facts filtering
        ↓
Context Compiler
        ↓
Compiled Projection / Model Context
        ↓
Model Runner
```

## Terms

```text
projection:
  generic controlled view

model context:
  projection rendered for a model/provider/runner

ContextFrame:
  temporary compiled artifact; not durable truth

retrieval unit:
  scoped, provenance-bearing material that may be embedded and retrieved

candidate set:
  retrieval output before hard filters, rerank and token-budget packing
```

## HNSW Boundary

HNSW is candidate retrieval. HNSW is not graph. HNSW is not memory. HNSW is not
decision authority.

HNSW is a rebuildable proximity index over retrieval units. It stores vectors
and refs, not authority. HNSW corruption must not destroy case truth.

The bridge is:

```text
vector_id -> retrieval_unit_id -> runtime_node_id -> record_ref / graph_ref / payload_ref
```

SPINE.58D is `HNSW Candidate -> RuntimeGraph Expansion`.

```text
HNSW finds vectors.
RuntimeGraph gives meaning.
```

Canonical rule:

```text
A YAI decision may use HNSW/vector-retrieval candidates,
but no YAI decision may be justified by vector proximity alone.
```

## Retrieval Unit Schema

Future retrieval units must carry:

```text
retrieval_unit_id
source_record_id
source_plane
case_ref
subject_refs
material_kind
visibility
authority_posture
redaction_policy
scope_policy
payload_hash
text_view_hash
embedding_model_ref
embedding_model_version
embedding_dimensions
distance_metric
created_at
```

No raw secret embedding. No unscoped cross-case retrieval. No embedding without
provenance.

## Context Assembly

Hard filters run before scoring:

```text
case scope
subject scope
authority
redaction
freshness
policy compatibility
operation compatibility
```

Scoring is composite, not vector-only:

```text
vector similarity
graph relevance
freshness
behavior facts
risk penalty
redundancy penalty
```

The context compiler may overfetch candidates, expand graph context, apply fact
scoring, remove redundant material, pack a token budget and render the final
model-specific view.
