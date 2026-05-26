# Data Context Runtime / RuntimeGraph

Status: SPINE.40 schema boundary active.

Purpose: track the doctrine and roadmap correction that separates durable
truth from runtime graph computation.

## Summary

YAI separates durable truth from runtime computation.

```text
Truth lives on durable planes.
Computation happens in runtime working sets.
Persistent truth on disk.
Computational shape in memory.
```

```text
Journal, LMDB, Ladybug and DuckDB preserve replay, records, relations and facts.
RuntimeGraph is the in-memory working graph for the active case/session.
HNSW is a rebuildable vector index over retrieval units bound to RuntimeGraph nodes.
Context Compiler builds temporary controlled projections/model contexts from
RuntimeGraph, retrieval candidates, policy, facts and token budgets.
```

Short form:

```text
LMDB stores records.
Graph persistence stores durable typed relations.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
```

SPINE.40 adds the schema/status surface for this doctrine. Graph persistence
owns durable typed relations. RuntimeGraph is the in-memory active case working
set. HNSW finds candidate nodes and HNSW is not graph truth. Projection does
not disappear.

## Corrected Graph Block

The graph block is no longer framed as Ladybug-only runtime graph work.

```text
SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema done
SPINE.41 Graph Relation Write Path
SPINE.42 RuntimeGraph In-Memory Working Set
SPINE.43 RuntimeGraph Rebuild from Journal / LMDB / Graph Store
SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics
SPINE.45 Graph + RuntimeGraph Freeze
```

Ladybug remains graph persistence / relation truth / rebuild contract.
RuntimeGraph becomes the active in-memory computational working set.

## Corrected Retrieval Block

HNSW is not the center of retrieval. It is a proximity accelerator attached to
retrieval units and RuntimeGraph bindings.

```text
SPINE.58A Context Compiler / Retrieval Boundary Doctrine
SPINE.58B Retrieval Unit Schema + Embedding Manifest
SPINE.58C HNSW Candidate Index Build / Rebuild
SPINE.58D HNSW Candidate -> RuntimeGraph Expansion
SPINE.58E Context Assembly / Rerank / Token Budget Packing
SPINE.58F Retrieval Residue + Cost / Recall Facts
SPINE.58G Context Compiler / Retrieval Freeze
```

Rule:

```text
HNSW finds vectors.
RuntimeGraph gives meaning.
vector_id -> retrieval_unit_id -> runtime_node_id -> record_ref / graph_ref / payload_ref
```

## Boundaries

RuntimeGraph is not:

```text
truth
LMDB replacement
Ladybug replacement
DuckDB replacement
HNSW
model context
```

HNSW is not:

```text
memory
graph truth
decision authority
policy authority
```

ContextFrame is not:

```text
truth
memory
storage
graph
```

ContextFrame is a temporary compiled artifact, not durable truth.

## Validation

```bash
make check-data-context-runtime-roadmap
make check-docs
```
