# ADR 0031: Data Context Runtime / RuntimeGraph

Status: accepted

## Context

The graph, retrieval and context compiler roadmap needs to avoid treating
Ladybug, RuntimeGraph, HNSW and projections as interchangeable stores.

## Decision

YAI separates durable truth from runtime computation.

Journal, LMDB, Ladybug and DuckDB preserve replay, records, relations and
facts. RuntimeGraph is the in-memory working graph for the active case/session.
HNSW is a rebuildable vector index over retrieval units bound to RuntimeGraph
nodes. Context Compiler builds temporary controlled projections/model contexts
from RuntimeGraph, retrieval candidates, policy, facts and token budgets.

## Consequences

- Ladybug remains graph persistence and relation truth.
- RuntimeGraph becomes the in-memory active case graph.
- HNSW remains candidate retrieval only.
- ContextFrame remains a temporary compiled artifact, not truth.
- Future graph and retrieval waves must name the durable/runtime split.

## Non-Goals

No RuntimeGraph implementation, HNSW index, Ladybug integration, DuckDB facts
or Context Compiler implementation is added by this ADR.
