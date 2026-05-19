# Extraction Plan

The old repository is a concept mine, not a folder migration source.

## 1. Core Rule

Do not migrate old roots as roots. Extract primitive concepts into future roots.

```text
no extraction without inventory row
no migration by folder
no old root recreated
every old concept gets action enum
compatibility is explicit, not assumed
```

## 2. Inventory-Driven Extraction

Every extraction candidate must have a row in:

```text
../internal/extraction-inventory.tsv
```

Required TSV columns:

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

If a concept is not in the inventory, it is not ready for extraction. If a row
does not name a future primitive, the work is still mining, not migration.

## 3. Action Enum

```text
absorb
quarantine
externalize
delete_later
compat_only
mine_concept
split
```

Action meanings:

| Action | Meaning |
|---|---|
| `absorb` | concept enters future `yai-core` primitive |
| `quarantine` | keep out until ownership is resolved |
| `externalize` | belongs in `ai-environment`, `interfaces`, `console` or later platform |
| `delete_later` | keep only until compatibility drains |
| `compat_only` | old API remains wrapper, not new authority |
| `mine_concept` | read for ideas, do not migrate shape |
| `split` | old root decomposes into multiple primitives |

## 4. Root Mapping

| Old root | Future destination | Action |
|---|---|---|
| `src/agents` | `subject`, `ingest`, `projection`, `ai-environment` | mine_concept |
| `src/capabilities` | `op`, `effect`, `control` | absorb |
| `src/case` | `case`, `subject`, `projection` | absorb |
| `src/decision` | `control` | absorb |
| `src/lineage` | `graph`, audit projection | absorb |
| `src/models` | `subject/model`, `effect/model carrier`, `projection/model`, `ai-environment` | mine_concept |
| `src/orchestrator` | `case/procedure`, `reconcile`, `ai-environment` | mine_concept |
| `src/runtime` | `daemon`, `effect`, `ctl`, `interfaces` | split |
| `src/substrate` | `store`, `index`, `graph`, `memory`, `projection` | split |
| `src/analytics` | `index`, `memory`, `reconcile`, `projection`, `ai-environment` | mine_concept |

This map is not permission to copy folders. It is a routing table for concepts.

## 5. Compatibility Rule

Existing include compatibility surfaces may remain in the current repo until
consumers are drained. The future `yai-core` must not recreate old root names.

Compatibility work must name:

```text
compat surface
consumer
planned drain point
future primitive
test proving behavior
```

Compatibility wrappers cannot become semantic owners.

## 6. First-Wave Extraction Order

Order for future code waves:

```text
NEW.0: docs, guards, empty roots
NEW.1: case_ref, subject_binding, attempt, decision, receipt, record, projection
NEW.2: carrier request and null/filesystem carrier
NEW.3: observed mode import and receipt gaps
NEW.4: graph edges and reconstruction projection
NEW.5: Rust store dual-write experiment
```

This order keeps subject/control/effect/store residue ahead of memory and
advanced retrieval.

## 7. Old Root Non-Recreation Rule

Forbidden future core roots:

```text
agents
analytics
capabilities
decision
lineage
models
orchestrator
runtime
substrate
governance
knowledge
state
providers
workflow
```

The concept can survive only under the new primitive owner. The old root name
must not become a directory, include root, package namespace or subsystem owner
inside future `yai-core`.

## 8. Inventory Quality Gates

Before extracting a file or symbol:

```text
inventory row exists
future_repo is named
future_root is one of approved roots or external repo
future_primitive is concrete
action is one of the enum values
compat_required is yes/no
first_wave is named
notes explain ownership risk
```

If any gate fails, the extraction remains documentation work.
