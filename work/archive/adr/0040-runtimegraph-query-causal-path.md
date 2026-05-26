# ADR 0040: RuntimeGraph Query / Causal Path

Status: accepted.

## Decision

RuntimeGraph query supports bounded diagnostic traversal over active-case graph
data. It is not a generic graph database language and not durable graph truth.

The supported commands are fanout, fanin, neighborhood and causal path
diagnostics. Neighborhood is bounded traversal with max depth 2. Path
diagnostics use max-depth with an implementation maximum of 6. The edge-kind
filter applies to fanout, fanin and neighborhood.

Plain output remains parseable. Color-aware graph inspection is future-facing
console/studio semantics and must preserve stable plain output.

The active lab path is `labs/filesystem-loop`.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.
