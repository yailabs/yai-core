Historical/superseded engineering record. Not an active source of truth.

# SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics

Status: delivered.

SPINE.44 adds bounded RuntimeGraph query diagnostics: fanout, fanin,
neighborhood, causal path search and edge-kind filter support. The commands
load the per-command ephemeral RuntimeGraph working set and run bounded
traversal over active-case graph data.

RuntimeGraph is not a generic graph database. RuntimeGraph is not durable
truth. Plain output remains parseable. Color-aware graph inspection is
documented for future console/studio graph inspection.

Validation:

```bash
make check-runtimegraph-query-causal-path
make smoke-spine44
```

The smoke verifies fanout, fanin, neighborhood, causal path found, causal path
not_found, edge-kind filter, bounded traversal, max-depth clamp and empty-case
behavior under `docs/labs/filesystem-loop`.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.
