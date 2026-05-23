# Data Plane Roadmap

This is the active data-plane roadmap. It replaces older active-looking data
spine and operational data-plane notes, which are archived under
`docs/archive/engineering/roadmaps/` and `docs/archive/engineering/superseded/`.

## Doctrine

YAI Core is residue-first.

```text
journal = replay/audit
hot state = live session state, not truth
record plane = durable lookup
graph plane = causality and reconstruction
fact plane = analytics and diagnostics
memory plane = consolidated operational experience
projection plane = controlled live cognitive view
reconcile plane = mismatch detection
observability/evaluation = quality facts, not truth
```

## Planes

| Plane | Target | Role |
|---|---|---|
| hot | shared memory / hot state | active case, session, projection frame, locks, obligations and deltas |
| journal | append-only files | replay, audit, debug and rebuild |
| record | LMDB | durable normalized record lookup |
| graph | Ladybug | persistent operational causality |
| fact | DuckDB | analytical facts, reports and model behavior analysis |
| memory | Rust engine memory | receipt-backed, graph-derived operational continuity |
| projection | Rust engine projection | live, versioned, delta-aware cognitive view |
| reconcile | Rust engine reconcile | expected-vs-observed mismatch posture |
| observability/evaluation | facts and diagnostics | freshness, trace, provenance, rebuild and quality posture |

## Linear Delivery

```text
SPINE.23 Hot State / Shared Memory Plane v0
SPINE.24 LMDB Record Backend v0
SPINE.25 Journal Replay to LMDB v0
SPINE.26 Ladybug Graph Backend v0
SPINE.27 DuckDB Fact Backend v0
SPINE.28 Projection Delta / Live View v0
SPINE.29 Memory Consolidation over Record + Graph + Facts v0
SPINE.30 Reconcile over Hot / Store / Graph Consistency v0
SPINE.31 Observability / Evaluation Facts v0
SPINE.32 Data Plane Milestone Freeze
```

## Rules

The journal must remain in the system, but it must recede from the primary
operational path.

SPINE.20 creates the local host root for future data planes:

```text
YAI_HOME/store
```

Future backend-specific subdirectories may be introduced by their owning waves:

```text
YAI_HOME/store/journal
YAI_HOME/store/lmdb
YAI_HOME/store/ladybug
YAI_HOME/store/duckdb
```

Models do not access the case directly. They receive a controlled live
projection assembled with the active interaction thread and authorized memory
or delta material.

Memory is not chat history. It is scoped operational experience derived from
residue.
