# Extraction Plan

The `yai-dev` repository is a concept mine, not a folder migration source.

## Rule

```text
inspect concepts
do not copy old source
do not recreate old roots
update extraction inventory
classify residue
externalize non-core material to yai-dev lab
```

Every relevant SPINE.N implementation wave must include:

```text
yai-dev audit
Extraction decision table
YAI implementation
yai-dev residue normalization
Residue handling
Inventory update
```

A wave is not complete until the corresponding yai-dev residue has been
classified. If the concept already exists in `yai-dev`, the wave must absorb
it, rewrite it, split it, externalize it to `yai-dev` lab, quarantine
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
../yai-dev/src/substrate/store/*
../yai-dev/src/substrate/records/*
../yai-dev/src/substrate/graph/*
../yai-dev/src/substrate/indexes/*
../yai-dev/src/substrate/query/*
../yai-dev/src/substrate/memory/*
../yai-dev/src/substrate/views/*
../yai-dev/src/substrate/signals/*
../yai-dev/src/lineage/*
../yai-dev/src/analytics/*
```

Model/provider/policy waves:

```text
../yai-dev/src/models/*
../yai-dev/src/runtime/provider/*
../yai-dev/src/case/policy/*
../yai-dev/src/agents/grounding/*
```

Host/carrier waves:

```text
../yai-dev/src/runtime/execution/*
../yai-dev/src/runtime/carriers/*
../yai-dev/src/runtime/observation/*
../yai-dev/src/capabilities/mcp/*
../yai-dev/src/capabilities/external/*
```

Case/session/thread waves:

```text
../yai-dev/src/case/identity/*
../yai-dev/src/case/materialization/*
../yai-dev/src/case/attachments/*
../yai-dev/src/case/subjects/*
../yai-dev/src/case/surface/*
../yai-dev/src/case/continuity/*
../yai-dev/src/runtime/sessions/*
```

SPINE.21 Pack Materialization Doctrine must read:

```text
../yai-dev/src/case/packs/*
../yai-dev/src/case/materialization/*
../yai-dev/src/case/policy/*
../yai-dev/src/case/subjects/*
../yai-dev/src/case/scope/*
../yai-dev/src/capabilities/*
../yai-dev/src/agents/roles/*
../yai-dev/src/agents/composition/*
../yai-dev/src/models/frame/*
../yai-dev/src/substrate/sources/*
../yai-dev/src/substrate/views/*
```

SPINE.21 must classify:

```text
case/packs              -> pack/materialization
case/materialization    -> case_world/pack_materialization
case/policy             -> control/policy_pack
case/scope              -> pack/domain_scope
capabilities            -> yai-dev or pack/tools, split
agents/roles            -> yai-dev or pack/actors, externalize
agents/composition      -> yai-dev, externalize
models/frame            -> pack/projection/model_frame
substrate/sources       -> pack/materials
substrate/views         -> pack/projections
```

## Non-Core Material

Agent frameworks, scenario harnesses, release rehearsal, provider labs and QA
engines belong to `yai-dev` lab unless a SPINE.N wave explicitly
imports a primitive residue shape into `yai`.

## Dirty Worktree Rule

Before modifying `yai-dev`, run:

```text
git -C ../yai-dev status --short
```

If the wave would touch a path with user or unknown changes, do not edit that
path. Record `blocked_by_dirty_worktree` or a clear caveat and choose a
non-conflicting inventory/docs update only if safe.

## Commit Rule

If both repos are modified, keep commit boundaries separate:

```text
yai implementation commit
yai-dev residue normalization commit
```

Do not mix unrelated yai-dev cleanup into a yai commit.
