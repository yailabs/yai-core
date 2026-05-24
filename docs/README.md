# YAI Documentation Spine

Status: DOC.CORE.0 canonical documentation freeze.

This folder is the active documentation spine for `yai`, the canonical local
AI operational control system.

Workspace siblings:

```text
yai-dev/
interfaces/
studio/
console/
```

Not owned by this repository:

```text
cloud/team/
public SDK truth
product client UX
```

This package does not move `yai-dev` source files, change `interfaces`, change
`studio`, change `console`, or migrate lab roots into the core.

DOC.NEW.0 created the canonical package. DOC.NEW.1 turned the core outline into
machine contracts. DOC.CORE.0 freezes naming, cross-links, guard checks and
final spine compression before NEW.1 implementation.

## Reading Order

1. `architecture/00-spine.md`
2. `architecture/01-terminology.md`
3. `architecture/02-integration-modes.md`
4. `architecture/04-subject-model.md`
5. `architecture/06-control-policy-model.md`
6. `engineering/target-layout.md`
7. `engineering/extraction-plan.md`
8. `engineering/export-matrices.md`
9. `engineering/rust-engine.md`
10. `engineering/c-abi.md`
11. `product/positioning.md`
12. `engineering/testing.md`
13. `adr/0000-decision-compendium.md`
14. `manuals/manual-filesystem-loop-validation.md`

## Ownership Rule

`architecture/` owns core doctrine.
`protocols/` owns first protocol shapes.
`engineering/` owns extraction, target layout, validation and implementation order.
`manuals/` owns operator-facing runbooks for local validation.
`product/` owns positioning and commercial wedges.
`adr/` owns decisions and consequences.
