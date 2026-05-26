# ADR 0037: Graph Persistence / RuntimeGraph

## Status

Accepted for SPINE.40.

## Context

SPINE.34 froze LMDB record lookup and SPINE.35-SPINE.39 froze journal replay
into LMDB. The next data-plane block is graph work. The prior roadmap language
could be misread as choosing between persistent graph storage and in-memory
graph computation.

## Decision

YAI separates graph persistence from RuntimeGraph.

Graph persistence owns durable typed relations and rebuild truth. RuntimeGraph
is the in-memory active case working set and owns active in-memory case
computation. HNSW finds candidate nodes. HNSW later attaches to retrieval units
and runtime nodes as candidate retrieval, not graph truth. HNSW is not graph truth.
Context Compiler later renders controlled projection/model context.
Projection does not disappear.

The governing rule is:

```text
Persistent truth on disk.
Computational shape in memory.
Vector proximity as candidate discovery.
Compiled projection as model-facing view.
```

## Consequences

SPINE.40 exposes the schema and runtime-status surface:

```bash
yai graph schema
yai graph runtime-status
```

The CLI must not claim graph persistence, Ladybug integration, RuntimeGraph
working-set implementation, HNSW or Context Compiler implementation.

## Non-Goals

```text
No full graph persistence engine.
No Ladybug integration.
No RuntimeGraph working set.
No graph rebuild.
No HNSW.
No context compiler.
No DuckDB facts.
No memory consolidation.
```
