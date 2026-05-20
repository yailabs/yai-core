# Extraction Plan

The old repository is a concept mine, not a folder migration source.

SPINE.0 clarifies that old `yai` is the transition concept mine and future
`ai-environment` source. It is not the future core. Extraction is concept
mining and evidence gathering, not folder movement into `yai-core`.

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

## 3. Wave-Coupled Extraction

The old `yai` repo is not cleaned in one final pass. It is mined continuously.
Every `yai-core` implementation wave that overlaps an old-`yai` concept must
include:

```text
Old-yai audit
Residue handling
```

Required sequence:

```text
inspect relevant old-yai files
extract concepts only, not folders
implement in yai-core using new primitives
update extraction inventory
classify old residue
assign useful non-core material to future ai-environment
```

Residue classification uses the existing action enum. Material that remains
useful outside the core should use:

```text
future_repo=ai-environment
action=externalize
```

Example audit scopes:

| Core wave type | Old-yai material to inspect |
|---|---|
| Memory | `src/substrate/memory/*`, `src/lineage/episodic_summary.c`, `src/lineage/semantic_summary.c`, `src/analytics/signals/*`, `src/agents/grounding/memory_strategy.c` |
| Carrier/enforcement | `src/runtime/execution/*`, `src/runtime/carriers/*`, `src/runtime/decision/*`, `src/runtime/observation/*` |
| Projection | `src/substrate/views/*`, `src/case/surface/*`, `src/decision/projection/*`, `src/models/frame/*`, `src/agents/grounding/context_pack.c` |
| Workflow-related | `src/orchestrator/*`, especially recovery and workflow material, without recreating a workflow engine in `yai-core` |

## 4. Action Enum

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

## 5. Root Mapping

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

## 6. Compatibility Rule

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

## 7. First-Wave Extraction Order

The completed CORE.NEW line has already moved past the first executable loops:

```text
NEW.0  Skeleton + guards
NEW.1  Minimum loop
NEW.2  Persistent journal + subject state
NEW.3  Control gate skeleton
NEW.4  Filesystem carrier v0
NEW.5  Graph reconstruction v0
NEW.5A Smoke isolation hardening
```

Next core work starts at NEW.6 Operational Memory Candidate v0, but sibling repo
canonicalization must be tracked separately:

```text
ENV.CANON.*      old yai to ai-environment concept mine and lab
INTF.CANON.*     interfaces over yai-core primitive truth
CONSOLE.CANON.*  console over interfaces and projections
```

The full roadmap lives in `docs/engineering/four-repo-roadmap.md`.

NEW.6 applied wave-coupled extraction for operational memory:

```text
src/substrate/memory/*           -> memory candidate concepts and future consolidation evidence
src/lineage/episodic_summary.c   -> memory basis count concept
src/lineage/semantic_summary.c   -> memory basis count concept
src/analytics/signals/*          -> freshness/confidence posture, not truth
src/agents/grounding/*strategy.c -> future ai-environment memory experiments
```

The inventory records which material was absorbed, mined or externalized.

NEW.7 applied wave-coupled extraction for reconciliation:

```text
src/orchestrator/recovery/*                       -> recovery and review posture evidence
src/runtime/execution/recovery_hint.c             -> operator recovery hint concepts
src/runtime/execution/runtime_enforcement_recovery.c -> enforcement recovery posture
src/substrate/signals/*                           -> contradiction and missingness signals
src/decision/conflict/*                           -> conflict posture and trace requirements
```

The implementation absorbs divergence/reconciliation records only. Repair,
compensation and workflow recovery remain outside NEW.7.

NEW.8 applied wave-coupled extraction for projection hardening:

```text
src/substrate/views/*               -> projection materialization and provenance posture
src/case/surface/*                  -> split between console surface and core projection shape
src/decision/projection/*           -> control projection concepts
src/decision/explanation/*          -> audit explanation and trace concepts
src/models/frame/*                  -> model projection audience concepts
src/agents/grounding/context_pack.c -> future ai-environment context experiments
src/agents/grounding/live_context.c -> future ai-environment context experiments
```

The implementation absorbs projection request/result records, consumer kind,
projection kind, freshness and redaction posture only. It does not port old
views, model frames, agent context packs or console surfaces.

NEW.9 applied wave-coupled extraction for query boundary:

```text
src/substrate/query/*                 -> read posture and filter boundary concepts
src/substrate/indexes/*               -> future index/retrieval evidence
src/substrate/store/*                 -> store backend evidence, not backend port
src/substrate/records/*               -> record taxonomy evidence
src/substrate/views/*                 -> query/projection boundary evidence
src/analytics/features/*              -> future ranking/index features
src/agents/grounding/query_strategy.c -> future ai-environment query experiments
src/agents/grounding/retrieval_strategy.c -> future ai-environment retrieval experiments
```

The implementation absorbs only journal scan/filter query, query result counts
and `yaictl query` inspection. It does not port old query surfaces, backend
registries, vector indexes, feature stores or agent retrieval strategy.

NEW.10 applied wave-coupled extraction for Rust engine R1:

```text
src/substrate/store/*      -> store/backend posture evidence
src/substrate/query/*      -> query/read posture evidence
src/substrate/indexes/*    -> future index/retrieval evidence
src/substrate/views/*      -> projection/freshness/confidence evidence
src/substrate/records/*    -> record lifecycle and taxonomy evidence
src/substrate/memory/*     -> future memory backend evidence
vendor/cjson/*             -> compatibility codec evidence only
include/yai/substrate/*    -> compatibility include material
```

The implementation absorbs only opaque-handle Rust engine FFI, JSONL append,
counts, kind query and projection summary JSON. It does not port cJSON, old
substrate headers, external DB backends, vector indexes, graph traversal or
memory consolidation.

## 8. Old Root Non-Recreation Rule

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

## 9. Inventory Quality Gates

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
