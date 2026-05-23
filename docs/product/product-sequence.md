# Product Sequence

The commercial path follows the maturity of the core.

The product begins as `yai-core`: a local AI operational control core.
`interfaces` and `console` are not the first product root. They are surfaces
around the core: interfaces project primitive truth, and console gives humans an
operator surface over projections and review flows.

NEW.15 places the technical command at `cmd/yai`. That command is a core
control surface for development, diagnostics and daemon interaction. It is not
the Console product UX, and it must not absorb operator command surfaces.

## Sequence

| Stage | Product | Core Proof |
|---|---|---|
| 1 | Core Developer / Local | Local `yai` command, `yaid` daemon, case, subject, attempt, decision, receipt, store, graph and projection. |
| 2 | Model/Provider Behavior Lab | L0 provider scouting notes, naked model case tests and model carrier evidence after the data-plane foundation, without agent-framework ownership. |
| 3 | Integration Kit | Interfaces, SDK, conformance fixtures and adapters around core primitives. |
| 4 | Operational Memory / Audit Pack | Memory candidates, receipt reconstruction, divergence and redaction posture. |
| 5 | Professional / Team | Multi-case store, policy packs, team subject registry and audit export. |
| 6 | Enterprise / Embedded | Custom carriers, subject models, policy materialization and support. |
| 7 | Platform / Cloud | Sync, dashboards, managed policy registry and organization control plane. |

## Rule

Do not sell the platform before the local core proves the loop.
Do not make interfaces or console the product root before `yai-core` reaches a
core milestone freeze.
Do not lead with an agent framework before naked model behavior is measurable.

L0 provider scouting can start immediately, but it is product research and
fixture material, not canonical core validation. SPINE.3R places the data-plane
foundation before canonical model/provider implementation; agent-framework
tests begin only after naked model and model-carrier behavior are measurable.

YAI does not sell memory as chat history. YAI produces operational memory:

```text
governance
audit
model behavior evaluation
human review
policy refinement
carrier safety
case continuity
legal/accounting traceability
```
