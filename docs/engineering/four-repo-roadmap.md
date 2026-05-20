# Four-Repo Roadmap

SPINE.0 makes the multi-repo spine explicit before CORE.NEW.6 adds operational
memory. The goal is to keep `yai-core` correct without letting sibling repos
drift around older runtime and agent assumptions.

## Current Status

| Repo | Role | Status | Next |
|---|---|---|---|
| `yai-core` | Canonical new core. | Completed NEW.0, NEW.1, NEW.2, NEW.3, NEW.4, NEW.5 and NEW.5A. | NEW.6 Operational Memory Candidate v0. |
| `yai` | Old/current repo. | Not the future core; transition concept mine and future ai-environment material. Must not be migrated folder-by-folder into `yai-core`. | ENV.CANON.0. |
| `interfaces` | Projection/API/SDK/conformance repo. | Must become a consumer of `yai-core` primitive truth. Must not define core semantics independently. | INTF.CANON.0. |
| `console` | Operator client / TUI / human UX. | Must consume projections and interfaces. Must not own daemon, store, control, memory or carrier semantics. | CONSOLE.CANON.0. |

## Rename And Role Doctrine

Canonical future workspace:

```text
YAI/
├── yai-core/
├── ai-environment/
├── interfaces/
└── console/
```

Current transition workspace:

```text
YAI/
├── yai-core/
├── yai/
├── interfaces/
└── console/
```

`yai` is not renamed in SPINE.0. It is declared transition concept mine and
future `ai-environment` source material. The physical rename to
`ai-environment` is a later dedicated wave.

## Wave Families

| Family | Owning repo | Purpose |
|---|---|---|
| CORE.NEW.* | `yai-core` | Implementation of the local AI operational control core. |
| ENV.CANON.* | `yai`, later `ai-environment` | Canonicalize the old repo as environment, lab and scenario material. |
| INTF.CANON.* | `interfaces` | Align schemas, transports, SDK and conformance to `yai-core` truth. |
| CONSOLE.CANON.* | `console` | Align operator UX to projections and interfaces. |
| PLATFORM.LATER.* | Future platform repos/services | Defer cloud, team and managed-platform concerns. |

## Wave-Coupled Extraction

The old `yai` repo is not cleaned in one final pass. It is mined continuously.
Every `yai-core` implementation wave that overlaps with an existing old-`yai`
concept must inspect the corresponding old-`yai` material, extract only what is
needed, implement the new primitive in `yai-core` using the new grammar, and
record what old material is kept, quarantined, externalized or assigned to
future `ai-environment`.

Each relevant `yai-core` delivery must include:

```text
Old-yai audit
Residue handling
```

Required work:

```text
inspect relevant old-yai files
extract concepts only, not folders
implement in yai-core using new primitives
update extraction inventory
classify old residue as absorb/quarantine/externalize/delete_later/compat_only/mine_concept/split
assign useful non-core material to future ai-environment with future_repo=ai-environment and action=externalize
```

This avoids both blind migration and blind rewrite. `yai-core` gets the useful
semantics, while old material is normalized as soon as its concept is touched.

Examples:

| Future wave family | Old-yai audit examples | Expected handling |
|---|---|---|
| Memory waves | `src/substrate/memory/*`, `src/lineage/episodic_summary.c`, `src/lineage/semantic_summary.c`, `src/analytics/signals/*`, `src/agents/grounding/memory_strategy.c` | Absorb reusable memory semantics into `yai-core/memory`; externalize agent-specific strategy to future `ai-environment`; classify old substrate memory residue. |
| Carrier/enforcement waves | `src/runtime/execution/*`, `src/runtime/carriers/*`, `src/runtime/decision/*`, `src/runtime/observation/*` | Absorb carrier, enforcement and receipt concepts into `effect`, `control` and `store`; quarantine operator shell UX; externalize old runtime loop to an environment harness. |
| Projection waves | `src/substrate/views/*`, `src/case/surface/*`, `src/decision/projection/*`, `src/models/frame/*`, `src/agents/grounding/context_pack.c` | Absorb projection and redaction posture into `projection`; externalize model/context-pack UX material. |
| Workflow-related waves | `src/orchestrator/*` and workflow-specific recovery material | Mine scenarios and procedure records; do not recreate a workflow engine in `yai-core`. |

## Stub And Stale Surface Rule

If a wave touches code or docs that contain stubs, placeholder-only sections,
TODO-only surfaces, half-implemented files or stale command/path language, the
wave must either verticalize the material into the current doctrine or remove
or quarantine it. Do not leave old command names or half-updated install paths
beside the new canonical surface.

## Policy Direction

Policy is case material, not just text, a deterministic gate or model prompt.
The CORE.NEW policy line must mature policy across source material, machine
claims/rules/gates, binding to case/subject/op/carrier/projection, decision
basis, projection to models/agents/operators, memory influence and hard
enforcement. YAI does not claim to control a model provider's internal policy;
it controls what the model sees, what operation is accepted, which carrier may
execute, which receipt remains and which memory derives.

## CORE.NEW Roadmap

| Wave | Name | Status |
|---|---|---|
| NEW.0 | Skeleton + guards | done |
| NEW.1 | Minimum loop | done |
| NEW.2 | Persistent journal + subject state | done |
| NEW.3 | Control gate skeleton | done |
| NEW.4 | Filesystem carrier v0 | done |
| NEW.5 | Graph reconstruction v0 | done |
| NEW.5A | Smoke isolation hardening | done |
| SPINE.0 | Wave-coupled extraction rule | done |
| NEW.6 | Operational Memory Candidate v0 | done |
| NEW.7 | Reconcile / Divergence v0 | done |
| NEW.8 | Projection hardening and redaction posture v0 | done |
| NEW.9 | Store/index/query boundary v0 | done |
| NEW.10 | Rust operational data engine R1 integration | done |
| NEW.11 | Daemon IPC v0 | done |
| NEW.12 | Daemon-backed core loop v0 | done |
| NEW.13 | Local command + install layout v0 | current |
| NEW.14 | YAI_HOME host layout + daemon run/store dirs | planned |
| NEW.15 | Daemon lifecycle locks + pid/socket discipline | planned |
| NEW.16 | Policy source/material/claim ingestion v0 | planned |
| NEW.17 | Policy binding to case/subject/op/carrier/projection | planned |
| NEW.18 | Policy projection to models/agents/operators v0 | planned |
| NEW.19 | Policy test corpus + adversarial policy cases | planned |
| NEW.20 | Filesystem carrier hardening | planned |
| NEW.21 | Process carrier v0 with strict host-control safeguards | planned |
| NEW.22 | Host observation/ingest probes v0 | planned |
| NEW.23 | Recovery/compensation posture v0 | planned |
| NEW.24 | Memory consolidation v0 in Rust engine | planned |
| NEW.25 | Store/index/query Rust backend R2 | planned |
| NEW.26 | Daemon op execution over IPC | planned |
| NEW.27 | yai command grammar completion | planned |
| NEW.28 | Local protocol fixtures from core smoke journals | planned |
| NEW.29 | Interfaces handoff package prep | planned |
| NEW.30 | Core conformance harness + milestone freeze | planned |

## ENV.CANON Roadmap

Purpose: turn old `yai` into `ai-environment`, concept mine and lab without
pretending it is still the core.

| Wave | Name |
|---|---|
| ENV.CANON.0 | Role declaration and old-core freeze |
| ENV.CANON.1 | Inventory old roots as experiment/lab material |
| ENV.CANON.2 | Agents as external subjects/adapters, not core |
| ENV.CANON.3 | Capabilities as effect/op fixtures, not core catalog |
| ENV.CANON.4 | Orchestrator/workflow as external workflow lab |
| ENV.CANON.5 | Models/providers as external model lab and carrier test subjects |
| ENV.CANON.6 | Substrate material as store/graph/memory extraction evidence |
| ENV.CANON.7 | Old tests converted to ai-environment scenarios |
| ENV.CANON.8 | ai-environment harness against yai-core |
| ENV.CANON.8A | Agentic QA / Release Rehearsal lab boundary |
| ENV.CANON.9 | Rename/package decision |
| ENV.CANON.10 | Environment milestone freeze |

### ENV.CANON Agentic QA / Release Rehearsal

Agentic QA is not built inside `yai-core` and should not become an internal
`yai` self-test engine. It belongs in future `ai-environment` as an external
lab that can put YAI systems, LidoPro and third-party software under test.

Canonical ownership:

```text
ai-environment
  qa/
    doctrine/
    scenarios/
    agents/
    harnesses/
    simulators/
    fake-systems/
    runners/
    reports/
    adapters/
      yai/
      lidopro/
      generic-web-app/
```

Product-oriented naming can also group the same capability as:

```text
ai-environment/
  release-rehearsal/
  agentic-qa/
  test-case-engines/
  evidence-lab/
```

Ownership split:

| Owner | Owns |
|---|---|
| `ai-environment` | Agentic QA engines, release rehearsal, scenario generation, simulation runtime, fake systems, accelerated time, adversarial runs, QA reports, evidence bundles and adapters. |
| YAI systems under test (`yai-core`, transition `yai`, later packaged YAI) | Invariants, contracts, protocol expectations, trace/evidence shape, conformance targets, smoke targets and release validation requirements. |
| YaiLabs / site | Public and commercial methodology: Agentic QA, Release Simulation, Test Case Generation and Evidence-Based QA. |

Rule:

```text
do not build QA inside YAI
build an agentic environment that can test YAI
```

YAI exposes verifiable surfaces:

```text
Documentation/testing/
  release-validation-contract.md
  invariant-map.md
  evidence-contract.md

protocols/testing/
  scenario-envelope.schema.json
  evidence-report.schema.json

make test
make validate-contract
make smoke
```

`ai-environment` executes scenarios against those surfaces, validates
invariants, observes traces and produces evidence reports. This keeps QA at a
healthy distance from the system it validates and makes the capability reusable
for non-YAI products.

## INTF.CANON Roadmap

Purpose: make `interfaces` project `yai-core` truth, not old Series 2
runtime/agent semantics.

| Wave | Name |
|---|---|
| INTF.CANON.0 | Interface role reaffirmation |
| INTF.CANON.1 | Primitive schema alignment: case, subject, op, decision, receipt, projection |
| INTF.CANON.2 | Transport alignment to yaid/yai/local IPC future |
| INTF.CANON.3 | Remove old root leakage from registry names |
| INTF.CANON.4 | Operation registry rebase on op/effect/control |
| INTF.CANON.5 | SDK boundary: clients consume projections, not core internals |
| INTF.CANON.6 | Conformance fixtures from yai-core smoke journals |
| INTF.CANON.7 | Interfaces milestone freeze |

## CONSOLE.CANON Roadmap

Purpose: make `console` operator UX consume `yai-core` projections and
`interfaces`, not own runtime truth.

| Wave | Name |
|---|---|
| CONSOLE.CANON.0 | Console role reaffirmation |
| CONSOLE.CANON.1 | Command taxonomy rebase: case/subject/op/decision/receipt/memory/projection |
| CONSOLE.CANON.2 | Remove old agent/governance/runtime command assumptions |
| CONSOLE.CANON.3 | Projection-first TUI model |
| CONSOLE.CANON.4 | Review/inspection UX over yai-core receipts and graph |
| CONSOLE.CANON.5 | Console integration against interfaces |
| CONSOLE.CANON.6 | Console milestone freeze |

## Commercial Spine

The product begins as `yai-core`: a local AI operational control core.
`interfaces` and `console` are not the first product root. They are surfaces
around the core.

Commercial sequence:

```text
Core Developer / Local
Integration Kit
Operational Memory / Audit Pack
Professional / Team
Enterprise / Embedded
Platform / Cloud later
```

## Non-Goals

SPINE.0 does not:

```text
create ai-environment repo
rename yai
touch interfaces
touch console
implement NEW.6
change code
move source files
migrate old yai folders
make yai-core depend on old yai
```
