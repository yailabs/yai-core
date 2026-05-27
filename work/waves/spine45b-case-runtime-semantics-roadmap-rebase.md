# SPINE.45B Case Runtime Semantics / Retrieval Federation / Context Residency Roadmap Rebase

Status: done.

This is a roadmap/doctrine/guard wave. It adds no runtime behavior.

SPINE.45B updates the main YAI spine before DuckDB facts so facts, retrieval,
compiled context, model-session state, graph revision and counterfactual
semantics are not bolted on later.

## Decisions

- SPINE.46 is renamed to DuckDB Fact Plane Doctrine + Bitemporal Schema.
- A fact is not just a row; it is a temporally scoped, provenance-bearing
  assertion.
- Retrieval can be native, attached or federated, but retrieval candidates are
  not evidence until resolved.
- External retrieval cannot enter model context without receipt, scope and
  provenance gating.
- ContextFrame is not truth.
- ContextDelta is not memory.
- CaseModelSession is not authority.
- KV cache is not YAI memory.
- Graph corrections are append-only patches.
- Counterfactuals are governed simulations, not historical corrections.

## Guard

```bash
make check-case-runtime-semantics-roadmap
```
