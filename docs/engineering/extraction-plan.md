# Extraction Plan

The old `yai` repository is a concept mine, not a folder migration source.

## Rule

```text
inspect concepts
do not copy old source
do not recreate old roots
update extraction inventory
classify residue
externalize non-core material to future ai-environment
```

Every relevant SPINE.N implementation wave must include:

```text
Old-yai audit
Extraction decision table
YAI-core implementation
Old-yai residue normalization
Residue handling
Inventory update
```

A wave is not complete until the corresponding old-yai residue has been
classified. If the concept already exists in old `yai`, the wave must absorb
it, rewrite it, split it, externalize it to future `ai-environment`, quarantine
it, mark it `compat_only`, mark it `delete_later`, leave it untouched with a
reason, or explicitly defer it.

## Inventory

Extraction decisions are tracked in:

```text
docs/internal/extraction-inventory.tsv
```

Required posture:

```text
wave
old_repo_path
old_concept
target_repo
target_root
action
future_repo
status
notes
```

## Action Enum

```text
absorb
rewrite
quarantine
externalize
delete_later
compat_only
split
migrated_concept
leave_untouched
```

Status values:

```text
planned
inspected
imported
rewritten
externalized
archived
blocked_by_dirty_worktree
deferred
complete
```

## Common Audit Sources

Data and memory waves:

```text
../yai/src/substrate/store/*
../yai/src/substrate/records/*
../yai/src/substrate/graph/*
../yai/src/substrate/indexes/*
../yai/src/substrate/query/*
../yai/src/substrate/memory/*
../yai/src/substrate/views/*
../yai/src/substrate/signals/*
../yai/src/lineage/*
../yai/src/analytics/*
```

Model/provider/policy waves:

```text
../yai/src/models/*
../yai/src/runtime/provider/*
../yai/src/case/policy/*
../yai/src/agents/grounding/*
```

Host/carrier waves:

```text
../yai/src/runtime/execution/*
../yai/src/runtime/carriers/*
../yai/src/runtime/observation/*
../yai/src/capabilities/mcp/*
../yai/src/capabilities/external/*
```

Case/session/thread waves:

```text
../yai/src/case/identity/*
../yai/src/case/materialization/*
../yai/src/case/attachments/*
../yai/src/case/subjects/*
../yai/src/case/surface/*
../yai/src/case/continuity/*
../yai/src/runtime/sessions/*
```

SPINE.21 Pack Materialization Doctrine must read:

```text
../yai/src/case/packs/*
../yai/src/case/materialization/*
../yai/src/case/policy/*
../yai/src/case/subjects/*
../yai/src/case/scope/*
../yai/src/capabilities/*
../yai/src/agents/roles/*
../yai/src/agents/composition/*
../yai/src/models/frame/*
../yai/src/substrate/sources/*
../yai/src/substrate/views/*
```

SPINE.21 must classify:

```text
case/packs              -> pack/materialization
case/materialization    -> case_world/pack_materialization
case/policy             -> control/policy_pack
case/scope              -> pack/domain_scope
capabilities            -> ai-environment or pack/tools, split
agents/roles            -> ai-environment or pack/actors, externalize
agents/composition      -> ai-environment, externalize
models/frame            -> pack/projection/model_frame
substrate/sources       -> pack/materials
substrate/views         -> pack/projections
```

## Non-Core Material

Agent frameworks, scenario harnesses, release rehearsal, provider labs and QA
engines belong to future `ai-environment` unless a SPINE.N wave explicitly
imports a primitive residue shape into `yai-core`.

## Dirty Worktree Rule

Before modifying old `yai`, run:

```text
git -C ../yai status --short
```

If the wave would touch a path with user or unknown changes, do not edit that
path. Record `blocked_by_dirty_worktree` or a clear caveat and choose a
non-conflicting inventory/docs update only if safe.

## Commit Rule

If both repos are modified, keep commit boundaries separate:

```text
yai-core implementation commit
old-yai residue normalization commit
```

Do not mix unrelated old-yai cleanup into a yai-core commit.
