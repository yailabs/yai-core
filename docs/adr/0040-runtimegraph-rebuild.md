# ADR 0040: RuntimeGraph Rebuild

## Status

Accepted for SPINE.43.

## Context

SPINE.42 introduced RuntimeGraph as an active-minimal in-memory working set
loaded from graph relations. That proves inspection, but not rebuildability
from the durable/replayable chain.

## Decision

RuntimeGraph must be rebuildable from journal, LMDB and graph relations.
RuntimeGraph rebuild produces a diagnostic report using
`yai.runtime_graph_rebuild_report.v1` and does not make RuntimeGraph durable
truth.

The canonical chain is:

```text
journal -> LMDB -> graph relations -> RuntimeGraph rebuild -> runtime-summary
```

RuntimeGraph remains per-command ephemeral and resident service planned.
RuntimeGraph is not durable truth.
The active lab path is `docs/labs/filesystem-loop`.

## Non-Goals

No resident RuntimeGraph service.
No Ladybug integration.
No HNSW.
No Context Compiler.
No docs/labs/nvidia cleanup.
