# Agent Operating Appendix

Status: active doctrine  
Owner: docs/engineering  
Purpose: Define minimum operating rules for agents editing this repository.  
Not source of truth for: product roadmap details or legal policy.

## Before Editing

An agent must do this before changing behavior:

```text
1. Read docs/engineering/current-status.md.
2. Read docs/engineering/four-repo-roadmap.md.
3. Read docs/engineering/command-surface.md.
4. Read docs/engineering/source-surface.md.
5. Check git status.
6. Identify whether the touched concept exists in yai-dev.
7. Update docs/internal/extraction-inventory.tsv if old residue is touched.
8. Do not mix unrelated legal/docs/source changes.
9. Do not add a primitive without a command/view/lab test unless internal-only is explicit.
10. Do not add long-term data-plane logic to transitional C shim roots.
```

## Agent Rule

A file is not ready for agent modification unless its ownership and boundary
are visible either in the file header or in the owning README.

If ownership is unclear, add or update the header/README before changing
behavior.

## Editing Rules

```text
1. Do not edit a file before identifying its ownership.
2. If ownership is unclear, add or update the header/README before changing behavior.
3. Do not add long-term data-plane logic to transitional C shim roots.
4. Do not mix roadmap, legal, source and feature changes in one commit.
5. Do not create a primitive without a view/command/lab test unless internal-only is explicit.
6. Do not use yai-dev as source code to copy wholesale.
7. Extract concepts from yai-dev, rewrite them in yai grammar, classify residue.
8. If a file is historical, archive it or mark it; do not leave it active-looking.
9. If command output changes, update command-surface.md and testing.md.
10. If a source root changes role, update source-surface.md.
```

## Commit Boundaries

Keep commits explainable:

```text
feature behavior
docs/status/roadmap
legal/repo-readiness
source movement
yai-dev residue cleanup
```

When a delivery touches more than one category, split commits unless the wave
explicitly scopes them together.

## Transitional Shim Rule

The transitional C data shim roots are:

```text
system/store
system/graph
system/index
system/memory
system/projection
system/reconcile
```

They may keep current ABI and smoke paths working. They must not become the
long-term owner of record, graph, fact, memory, projection or reconcile data
planes. Future durable data-plane ownership belongs under `engine/yai-engine`
unless a later roadmap wave changes that explicitly.
