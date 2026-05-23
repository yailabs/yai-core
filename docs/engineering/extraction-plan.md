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
Residue handling
```

## Inventory

Extraction decisions are tracked in:

```text
docs/internal/extraction-inventory.tsv
```

Required posture:

```text
old_path
old_symbol_or_file
old_concept
future_repo
future_root
future_primitive
action
compat_required
first_wave
notes
```

## Action Enum

```text
absorb
quarantine
externalize
delete_later
compat_only
mine_concept
split
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

## Non-Core Material

Agent frameworks, scenario harnesses, release rehearsal, provider labs and QA
engines belong to future `ai-environment` unless a SPINE.N wave explicitly
imports a primitive residue shape into `yai-core`.
