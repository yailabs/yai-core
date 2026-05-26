# RuntimeGraph Query / Causal Path

Status: SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics.

RuntimeGraph query is bounded diagnostic traversal over active-case graph data.
RuntimeGraph is not a generic graph database and not durable graph truth. Each
CLI command loads the per-command ephemeral RuntimeGraph working set from graph
relations, then runs fanout, fanin, neighborhood or causal path diagnostics.

## Command Surface

```bash
yai graph fanout --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph fanin --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph neighborhood --case <case_ref> --node <ref> --depth 1 [--edge-kind <kind>]
yai graph path --case <case_ref> --from <ref> --to <ref> --max-depth 4
```

Limits default to 20 and clamp at 200. Neighborhood depth defaults to 1 and
clamps at 2. Causal path max-depth defaults to 4 and clamps at 6. If no path is
found, output uses `path_status: not_found`.

The edge-kind filter applies to fanout, fanin and neighborhood. Plain output
remains parseable. Color-aware graph inspection is documented as future visual
semantics only; it must not replace stable text output.

## Validation

```bash
make check-runtimegraph-query-causal-path
make smoke-spine44
```

The smoke covers runtime graph rebuild/load, fanout, fanin, neighborhood,
causal path found, causal path not_found, edge-kind filter, bounded traversal
clamping and empty-case behavior.

The active lab path is `docs/labs/filesystem-loop`.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.

SPINE.45 freezes these diagnostics inside the graph/runtimegraph block. The
freeze includes path found, path not_found, empty case and review/control graph
visibility for `review_request`, `review_decision` and `control_pending`.
