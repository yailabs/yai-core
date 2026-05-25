# Data Context Runtime / RuntimeGraph

Status: SPINE.33M.

Purpose: define how durable data-plane truth becomes fast runtime computation.

## Doctrine

Truth lives on durable planes. Computation happens in runtime working sets.

YAI does not choose between Ladybug and an in-memory graph. YAI separates:

```text
graph truth persistente
runtime graph computazionale
vector retrieval index
compiled projection / model context
```

The canonical split is:

```text
LMDB stores records.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
```

## Plane Roles

| Plane | Role |
|---|---|
| Journal | append-only replay / recovery / audit chronology |
| LMDB | durable record truth and lookup |
| Ladybug | persistent graph truth and relation rebuild contract |
| DuckDB | analytical facts, metrics, evaluation and quality signals |
| RuntimeGraph | in-memory active case graph / working set |
| HNSW | rebuildable vector candidate retrieval index |
| Context Compiler | controlled projection/model context compiler |

RuntimeGraph is not truth. It is loaded or rebuilt from durable truth and kept
current while work is active.

## RuntimeGraph Contents

RuntimeGraph is a YAI-specific working set, not a generic graph database.

It may contain:

```text
node arena
edge arena
adjacency indexes
reverse adjacency indexes
case indexes
subject indexes
policy indexes
authority/scope indexes
retrieval unit cache
vector bindings
dirty delta overlay
snapshot/replay cursor
generation counters
```

It is optimized for:

```text
case-local lookup
subject-local lookup
bounded traversal
causal path expansion
policy filtering
evidence/support traversal
receipt -> decision -> op reconstruction
retrieval candidate expansion
context assembly
```

## Graph And HNSW

YAI graph:

```text
G_yai = (V, E, tauV, tauE, A)
```

where:

```text
V    = records, receipts, claims, evidence, subjects, policies, ops, memory
E    = typed semantic edges
tauV = node type
tauE = edge type
A    = case, subject, authority, timestamp and scope attributes
```

HNSW graph:

```text
G_hnsw = (V_vec, E_near)
```

where:

```text
V_vec  = embedding vectors
E_near = geometric proximity edges
```

The bridge is:

```text
vector_id -> retrieval_unit_id -> runtime_node_id -> record_ref / graph_ref / payload_ref
```

HNSW finds candidates. RuntimeGraph gives those candidates operational meaning.

## Write Pipeline

```text
1. append journal event
2. write/update LMDB record
3. write/update graph relation persistence
4. update RuntimeGraph in memory
5. if retrieval-eligible, create/update retrieval unit
6. update HNSW or mark vector index dirty
7. derive facts into DuckDB if needed
8. refresh projection/context if affected
```

Rule:

```text
Truth first.
Runtime acceleration second.
```

## Read Pipeline

```text
1. Hot state identifies current case/thread/op
2. RuntimeGraph loads the active case working set
3. Context Compiler formulates retrieval query
4. HNSW finds candidate vector ids
5. vector ids map to retrieval units and runtime nodes
6. RuntimeGraph expands causal, policy and evidence paths
7. LMDB retrieves payload truth
8. DuckDB contributes cost, quality, frequency and risk facts
9. Policy/redaction/freshness filters apply
10. Context Compiler emits compiled projection/model context
```

The model sees the compiled projection/model context, not LMDB, Ladybug,
DuckDB, HNSW or RuntimeGraph directly.

## ContextFrame

Projection remains the architectural concept of a controlled view.

Context Compiler is the machine that builds the view.

ContextFrame is a temporary compiled artifact produced by the compiler. It is
not truth, memory, storage or graph.

## Non-Goals

SPINE.33M does not implement RuntimeGraph, HNSW, Ladybug persistence,
DuckDB facts or Context Compiler execution. It corrects the roadmap and
architecture vocabulary before those implementation waves.
