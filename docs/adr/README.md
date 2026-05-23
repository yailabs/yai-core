# ADRs

Architecture decision records for DOC.NEW.1.

Each ADR records a core boundary decision, context, consequences and non-goals.
In NEW.0 these ADRs are repository doctrine only; they do not implement the
runtime loop.

Start with:

```text
0000-decision-compendium.md
```

Then read the atomic records:

```text
0001-new-core-boundary.md
0002-case-bound-subjects.md
0003-daemon-over-runtime.md
0004-control-over-decision-root.md
0005-store-index-graph-memory-projection-split.md
0006-no-agent-framework-in-core.md
0007-no-workflow-engine-in-core.md
0008-c-first-rust-operational-data-engine.md
0009-system-engine-cmd-layout.md
0010-rust-data-spine-ownership.md
0011-lib-is-transitional.md
0012-naked-model-before-agent-framework.md
0013-case-world-before-subject-binding.md
0014-operational-data-plane-stratification.md
0015-projection-as-live-cognitive-view.md
0016-operational-observability-evaluation-plane.md
```

Atomic ADR structure:

```text
Status
Context
Decision
Consequences
Non-goals
Supersedes
Related docs
```
