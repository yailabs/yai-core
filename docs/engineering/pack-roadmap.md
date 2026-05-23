# Pack Roadmap

Status: SPINE.21 doctrine.

Packs become canonical before the data-plane foundation because pack material
is future input to hot state, records, graph, facts, projection and memory.

## Native YAI Pack Examples

Planned examples:

```text
general-case
document-dossier
coding
research
planning
audit-evidence
professional-practice
business-analysis
filesystem-safety
model-behavior-test
```

These names are planning examples. SPINE.21 does not create pack directories or
ship pack content.

## User Pack Examples

```text
personal writing style
personal prompt library
contract review checklist
invoice classification method
custom report template
personal research workflow
local script bundle
local folder/document set
custom MCP/tool hints
```

User packs materialize a user's own method inside controlled AI cases.

## Organization Pack Examples

```text
company procedure pack
internal compliance checklist
quality standard
approved provider policy
document template pack
review/escalation pack
role/authority pack
department workflow pack
customer support standard
```

Organization packs materialize company methods, standards and policies into
shared cases. They require versioning, provenance and governance posture.

## Vertical Pack Examples

```text
legal contract review
accounting practice
business administration
software release audit
AI behavior evaluation
evidence packet generation
procurement review
technical due diligence
```

Vertical packs are pilot/reference/domain bundles, not an immediate
marketplace.

## Delivery Order

```text
SPINE.21  Pack Materialization Doctrine
SPINE.22  Filesystem Refactor Milestone Freeze
SPINE.23  Hot State / Shared Memory Plane v0
SPINE.24  LMDB Record Backend v0
SPINE.25  Journal Replay to LMDB v0
SPINE.26  Ladybug Graph Backend v0
SPINE.27  DuckDB Fact Backend v0
SPINE.28  Projection Delta / Live View v0
SPINE.29  Memory Consolidation over Record + Graph + Facts v0
SPINE.30  Reconcile over Hot / Store / Graph Consistency v0
SPINE.31  Observability / Evaluation Facts v0
SPINE.32  Data Plane Milestone Freeze
```

Later product and environment work may consume pack doctrine, but core truth
stays in `yai-core`.

## Deferred Work

```text
pack runtime
pack installer
pack registry backend
marketplace
billing
cloud sync
agent pack executor
```
