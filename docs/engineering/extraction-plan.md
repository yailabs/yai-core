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

Filesystem / data-spine refactor waves must inspect:

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
../yai/src/models/*
../yai/src/agents/grounding/*
../yai/src/capabilities/mcp/*
../yai/src/capabilities/external/*
../yai/src/runtime/provider/*
../yai/src/runtime/execution/*
../yai/src/runtime/carriers/*
../yai/src/runtime/machine/*
../yai/src/runtime/lifecycle/*
../yai/src/case/policy/*
../yai/src/case/subjects/*
../yai/src/case/materialization/*
../yai/src/case/surface/*
```

In NEW.13 there is no code extraction, source movement or file migration.

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
| Model/provider | `src/models/*`, `src/runtime/provider/*`, `src/case/policy/*`, `src/substrate/views/*`, `src/substrate/memory/*` |
| Agent/tool | `src/agents/grounding/*`, `src/capabilities/mcp/*`, `src/capabilities/external/*`, `src/runtime/execution/*` |

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

## 7. Stub And Incomplete Rule

When a refactor wave touches a stub, TODO-only file, placeholder, bootstrap
bridge or half-implemented surface, it must:

```text
verticalize it into the new doctrine
or quarantine it
or mark delete_later
```

It must not leave stale bootstrap language alive.

## 8. Refoundation Extraction Order

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

Next core work is NEW.13 target filesystem doctrine/refactor plan. Local
install layout is delayed to NEW.20. Sibling repo canonicalization remains
tracked separately:

```text
ENV.CANON.*      old yai to ai-environment concept mine and lab
INTF.CANON.*     interfaces over yai-core primitive truth
CONSOLE.CANON.*  console over interfaces and projections
```

The full roadmap lives in `docs/engineering/four-repo-roadmap.md`.

NEW.13 applies wave-coupled extraction only as audit and inventory planning:

```text
src/substrate/store/*          -> engine/store or system/engine_bridge
src/substrate/records/*        -> engine/record
src/substrate/graph/*          -> engine/graph
src/substrate/indexes/*        -> engine/index
src/substrate/query/*          -> engine/query
src/substrate/memory/*         -> engine/memory
src/substrate/views/*          -> engine/projection
src/substrate/signals/*        -> engine/reconcile or engine/memory
src/runtime/execution/*        -> system/effect + system/control + system/daemon
src/runtime/carriers/*         -> system/effect/carriers
src/runtime/machine/*          -> system/daemon
src/runtime/lifecycle/*        -> system/daemon
src/case/subjects/*            -> system/subject + engine residue refs
src/case/materialization/*     -> system/case + engine residue
src/case/surface/*             -> projection/console future, not system UX
src/lineage/*                  -> engine/graph + engine/memory
src/analytics/*                -> engine/index/memory/reconcile or ai-environment
```

NEW.15 applies wave-coupled extraction only as command-boundary audit:

```text
../yai/src/runtime/operator/*                   -> split: cmd/yai diagnostics or console future
../yai/tools/runtime/*                          -> cmd/yai or system/daemon tooling concepts
../yai/Documentation/manuals/operator-manual.md -> console future
../yai/Documentation/manuals/runtime-service-operations.md -> daemon docs
../console/Documentation/commands/*             -> console future
../console/Documentation/runtime-attachment/*   -> console/interfaces future
```

No old source is copied. `cmd/yai` remains the technical core command, not
operator Console.

The inventory records these routes. No old-yai source is copied.

Future model/provider waves must audit and classify:

```text
src/models/*                  -> future model subject/provider posture, ai-environment, model carrier concepts
src/agents/grounding/*        -> future ai-environment, model projection experiments
src/runtime/provider/*        -> future model carrier/provider subject
src/capabilities/mcp/*        -> future agent/tool adapter tests
src/capabilities/external/*   -> future external adapter tests
src/substrate/views/*         -> projection concepts
src/substrate/memory/*        -> memory concepts
src/case/policy/*             -> policy materialization and model projection evidence
```

SPINE.2 is documentation only. It defines L0 provider scouting and the NEW.26
through NEW.30 model/provider experiment sequence, but it does not touch old
`yai`, copy source, create a model carrier, create a provider registry, create
policy packs or create `ai-environment`.

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
and `yai query` inspection. It does not port old query surfaces, backend
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

NEW.11 applied wave-coupled extraction for daemon IPC:

```text
src/runtime/activation/*             -> daemon entry and startup posture
src/runtime/lifecycle/*              -> service status/readiness posture
src/runtime/machine/*                 -> future resident loop evidence
src/runtime/connections/*             -> client connection boundary evidence
src/runtime/boundary/transport/*      -> local IPC transport evidence
tools/runtime/*                       -> future daemon tooling evidence
Documentation/manuals/runtime-service-operations.md -> runtime operations posture
```

The implementation absorbs only local Unix socket IPC, status/info/shutdown
messages and daemon status JSON. It does not port runtime loop, transport
normalization/security, API dispatch, service manager integration or carrier
execution.

NEW.12 applied wave-coupled extraction for daemon-backed core execution:

```text
src/runtime/activation/*                         -> daemon startup posture
src/runtime/lifecycle/*                          -> resident lifecycle evidence
src/runtime/machine/*                            -> future loop/machine evidence
src/runtime/connections/*                        -> local request/response boundary
src/runtime/execution/request_envelope.c         -> request envelope concept
src/runtime/execution/runtime_envelope_validation.c -> envelope validation posture
src/runtime/execution/runtime_control_bridge.c   -> control/effect bridge concept
src/runtime/execution/runtime_control_enforcement.c -> enforcement boundary concept
src/runtime/execution/operational_receipt.c      -> receipt response evidence
src/runtime/decision/*                           -> daemon decision context evidence
src/runtime/observation/*                        -> observation posture evidence
src/case/surface/*                               -> console/projection split evidence
src/case/identity/*                              -> case identity evidence
src/case/subjects/*                              -> subject binding evidence
src/case/policy/*                                -> control policy evidence
src/case/decision/*                              -> case decision evidence
src/substrate/records/*                          -> append semantics evidence
src/substrate/views/*                            -> projection/read-model posture
src/substrate/query/*                            -> summary/query posture
```

The implementation absorbs only daemon request/response envelopes, two bounded
daemon-served loops and journal/projection summary commands. It does not port
old runtime roots, case surface UX, transport compatibility layers, HTTP,
auth, service management, process/model carriers or interfaces/console code.

NEW.20 will apply wave-coupled extraction for local command and install layout:

```text
../yai/packaging/*                         -> install path roles and package adapter evidence
../yai/tools/runtime/*                     -> local runtime tooling evidence
../yai/Documentation/manuals/runtime-service-operations.md -> daemon operation posture
../yai/Documentation/product/distribution.md -> local-first distribution posture
../yai/Documentation/reference/repository/* -> repository ownership boundary
../console/Documentation/commands/*        -> console command UX externalization
../console/Documentation/runtime-attachment/* -> console runtime attachment externalization
```

That implementation should absorb only local command, daemon binary and local
`YAI_HOME` directory concepts after filesystem / data-spine refoundation. It
must not port old packaging scripts, service managers, distro packages, console
command UX, authorization, launchd/systemd, Homebrew or cloud distribution.

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
