# RuntimeGraph Query / Causal Path

Status: SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics.

SPINE.44 starts using RuntimeGraph as a graph. RuntimeGraph query supports
bounded traversal over active-case graph data loaded into the per-command
working set. RuntimeGraph is not a generic graph database. It is not durable
truth, model memory, HNSW or a Context Compiler.

The active lab path is `docs/labs/filesystem-loop`.

## Boundary

```text
durable truth = journal + LMDB + graph relations
runtime computation = RuntimeGraph per-command ephemeral working set
query = bounded traversal diagnostics
```

## Commands

```bash
yai graph fanout --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph fanin --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph neighborhood --case <case_ref> --node <ref> --depth 1 [--edge-kind <kind>]
yai graph path --case <case_ref> --from <ref> --to <ref> --max-depth 4
```

Fanout lists outgoing RuntimeGraph edges. Fanin lists incoming RuntimeGraph
edges. Neighborhood uses bounded traversal with max depth 2. Causal path
diagnostics use bounded directed traversal with default max-depth 4 and max
6. The edge-kind filter applies to fanout, fanin and neighborhood.

Plain output remains parseable. Future color-aware graph inspection should
keep stable semantics: case/world blue, subject cyan, policy/authority yellow,
decision/gate orange, carrier/effect purple, receipt green, divergence red,
memory magenta and projection/view gray/blue.

## Non-Goals

No generic graph query language.
No resident RuntimeGraph service.
No HNSW.
No Context Compiler.
No memory consolidation.
No the removed manuals surface update.
No docs/labs/nvidia cleanup.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.

## Operator Review Input

SPINE.44A adds review/control records that graph materialization can later see
as active-case records. `require_review` creates `pending_operator`; approve,
deny, defer and quarantine write review decisions and receipts. Deny, defer
and quarantine keep `carrier_attempted: false` and `execution_performed:
false`. `subject:linenoise-terminal is prompt surface`; operator reviewer
authority is separate. The active lab path is `docs/labs/filesystem-loop`.

SPINE.45 freezes fanout, fanin, neighborhood and causal path as bounded
traversal diagnostics. The edge-kind filter remains supported and plain output
remains parseable. Review/control graph visibility now covers
`review_request`, `review_decision` and `control_pending`.
