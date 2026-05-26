Historical/superseded engineering record. Not an active source of truth.

# SPINE.43 RuntimeGraph Rebuild

SPINE.43 adds RuntimeGraph rebuild:

```text
journal
-> LMDB
-> graph relations
-> runtime_graph_rebuild
-> runtime-summary
```

The report schema is `yai.runtime_graph_rebuild_report.v1`.

RuntimeGraph is not durable truth. RuntimeGraph remains per-command ephemeral,
resident service planned. HNSW, Ladybug and Context Compiler remain future.

Canonical lab references stay under `labs/filesystem-loop`. This wave
does not touch `labs/nvidia`.
