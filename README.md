<!--
YAI

Copyright (c) 2026 Francesco Maiomascio.
All rights reserved.

This file is part of the YAI Community Source Tree.
Use, copying, modification, distribution, and production operation
are governed by the repository licensing documents, including LICENSE and the
reference documents under docs/.

Development and non-production use is permitted under the applicable
YAI license terms. Production, organizational, persistent,
collaborative, customer-affecting, or business-critical use requires
a commercial license.
-->

<div align="center">
  <img src="docs/reference/figures/brand/yai-transp.png" alt="YAI" width="220" />

  <strong>local AI operational control core</strong>
  <br />
  <span>Cases, subjects, control gates, receipts, and operational memory for local AI systems.</span>

  <br />

  ![Runtime](https://img.shields.io/badge/runtime-local-0f766e?style=flat&labelColor=1f2937)
  ![Core](https://img.shields.io/badge/core-operational%20control-475569?style=flat&labelColor=1f2937)
  ![License](https://img.shields.io/badge/license-community%20source-374151?style=flat&labelColor=1f2937)
</div>

YAI is a local AI operational control core.

It binds operational subjects into cases, governs attempted operations through
machine policy gates, records receipts for effects, derives operational memory,
and serves controlled projections back to models, agents, operators, and
systems.

YAI is not another intelligence layer.
YAI is the operational control layer around intelligence.

## What YAI Is

YAI owns the local machine-level control and operational-memory engine for
AI-mediated work.

It treats work as a bounded operational domain, called a case. Inside a case,
observable or controllable subjects can be named, related, governed, affected,
observed, and remembered. An attempted operation against those subjects becomes
an `op`: material that must be bound to a case, evaluated against policy,
passed through gates, executed or observed through carriers, and recorded with
receipts.

The mature operational spine is:

```text
world formation -> operational residue -> live/durable data planes -> controlled projection -> model/operator/carrier behavior -> receipt/evidence -> graph/memory/reconcile -> observability/evaluation -> next projection/action
```

This gives local AI systems a durable operational layer for case-bound
worlds, subjects, operation attempts, policy gates, control decisions, effect
carriers, structured receipts, durable records, graph-backed reconstruction,
operational memory, controlled projections, divergence reconciliation and case
view quality.

## What YAI Is Not

YAI is not:

* an agent framework;
* a workflow engine;
* a model provider;
* a cloud platform;
* a TUI;
* a replacement for existing systems.

It does not own external agents, business workflows, model providers, cloud
platform behavior, public SDK surfaces, business systems, or client
applications. Those systems may integrate with YAI, but the repository's
identity remains the local control engine.

YAI is also not a generic policy engine, generic audit logger, or runtime
monitor. Policy, auditability, and observation matter because they are part of
operational control, not because the core is reducible to any one of them.

## Architectural Thesis

Most AI systems begin with a model loop:

```text
context -> model -> tool -> observation -> memory
```

That loop is useful, but it makes the model conversation look like the system
boundary. YAI moves the boundary to operational control:

```text
case world -> residue -> data planes -> live projection -> control -> receipt -> graph/memory/reconcile -> observability -> next action
```

Model output is candidate material, not authority. A model can propose,
classify, summarize, estimate, route, explain, or generate evidence candidates.
The core decides what an attempted operation means, what policy applies, which
gates must fire, whether an effect is executed or blocked, what receipt proves
the result, and how the operation changes memory and future projections.

Authority, execution, receipts, memory, and continuity remain outside the
model. Future work is scheduled only through the linear SPINE.20+ roadmap in
`docs/engineering/four-repo-roadmap.md`.

The canonical operator command inventory is
`docs/engineering/command-surface.md`. It maps runtime layout, daemon, hot
state, record-store status, projection, pack doctrine and foundation checks to
commands, manual tests and expected output.

The active source-surface boundary is `docs/engineering/source-surface.md`.
`system/` is not a second data engine; Rust data-plane ownership targets
`engine/yai-engine`, while current C data roots are transitional shims.

The hot-state manual surface is:

```text
yai doctor
yai hot status
yai store status
yai projection inspect --journal <path> --consumer model
```

The command-surface baseline is integrated into the filesystem loop manual and
notebook under `docs/manuals/manual-filesystem-loop-validation.md` and
`docs/manuals/manual-filesystem-loop-validation.ipynb`, with Italian companion
`docs/manuals/manual-filesystem-loop-validation.it.ipynb`.

## Computational Model

`case`
: A bounded operational control domain. A case gives subjects, operations,
policies, receipts, records, graph edges, memory, projections, and divergence a
shared operational frame. In the C ABI this now includes case-domain,
case-attachment, and case-binding residue so the case is a small operational
world, not only a `case_ref` on records.

`pack`
: A case materialization unit. A pack can materialize methods, procedures,
policies, templates, projection rules, output schemas, memory seeds,
actors/tools and validation fixtures into a case. A pack is not a marketplace
product, DLC, prompt bundle or runtime executor.

`subject`
: An observable or controllable entity bound to a case. A subject may be a file,
process, repository, service, model, agent, operator, external system, document,
resource, or domain object.

`op`
: An attempted operation against one or more subjects. An op is not trusted
because it came from a model, agent, tool, operator, script, or remote system;
it becomes operationally meaningful only after case binding and control.

`control`
: Policy materialization, gates, decisions, constraints, and obligations for an
op. Control translates applicable policy and case state into machine decisions.

`effect`
: Execution or observation of an operation through a carrier. An effect may
write, read, call, invoke, import, classify, block, observe, or reconcile.

`carrier`
: The mechanism that executes or observes an effect at a boundary. Carriers may
be local process, filesystem, provider, model, IPC, shell, adapter, or imported
receipt boundaries.

`receipt`
: Structured proof of execution, observation, block, failure, constraint,
imported result, or recovery posture. A receipt is not a generic log line; it
connects an effect to case state, subjects, policy, decision, records, and
graph.

`store`
: Durable operational records, sources, receipts, policy snapshots, journals,
and reconstruction material.

`hot state`
: Live runtime cache for the active case session, current projection frame,
case world/context lifecycle, active interaction thread, participant view,
freshness, stale reason and latest residue refs. Hot state is not truth; it is
rebuilt from durable residue and currently snapshots to
`YAI_HOME/run/hot-state.json` with schema `yai.hot_state.v1`. SPINE.28 freezes
this cache/diagnostic surface before LMDB durable record lookup starts.

`record store`
: Durable indexed record lookup under `YAI_HOME/store/lmdb`. LMDB is durable
record lookup, not hot state, journal replay, graph truth, analytical facts or
memory. SPINE.29 defines schema `yai.record.v1`, logical keys such as
`record:id`, `record:case`, `record:kind` and `record:subject`, and the
`yai store status` surface before SPINE.30 writes records.

`graph`
: Relationships between subjects, operations, decisions, receipts, policies,
records, projections, memory, and divergence. The graph preserves operational
structure for reconstruction and controlled query.

`memory`
: Operational memory derived from records, decisions, receipts, effects,
policies, projections, divergences, and graph structure.

`projection`
: A controlled view served to models, agents, operators, APIs, audit surfaces,
or external systems. A projection is scoped material, not ownership of truth.

`reconcile`
: Divergence detection, recovery, and compensation when expected state and
observed state differ.

`daemon`
: The local resident service. `yaid` is the local daemon.

`ctl`
: The local technical command surface. `yai` is the canonical local technical
command.

## Integration Modes

YAI integrates with surrounding systems through four control postures.

`observed`
: YAI sees an operation after, alongside, or around execution. It can bind the
event to a case, classify returned material, import or produce receipts, update
records, derive memory, and detect divergence. It cannot claim full enforcement
over a boundary it did not control.

`interposed`
: YAI sits on the local path before an effect is executed. It can apply gates,
decisions, constraints, obligations, blocks, and receipt requirements at that
local boundary.

`carrier-owned`
: YAI owns the carrier that executes or observes the effect. This gives the
core the strongest local enforcement posture because execution and receipt
production share a controlled boundary.

`embedded`
: YAI control logic runs inside a host system, adapter, service, or runtime
boundary. Enforcement strength depends on the host integration contract and the
material returned as receipts.

If YAI does not own the carrier or run on the remote side, it cannot claim full
enforcement. It can still control the local boundary, classify returned
material, import receipts, preserve records, update graph and memory, and
reconcile divergence.

## Models Boundary

Models are central, but not sovereign.

Model invocation is itself an operation. A model receives a controlled
projection, produces candidate material, and returns claims that may become
proposals, summaries, classifications, routes, explanations, or evidence
candidates.

The model does not own authority, execution, receipts, operational memory,
continuity, policy enforcement, or case truth.

Stronger models make YAI more useful, not obsolete. Better inference
improves classification, planning, summarization, projection, review, and
recovery, but operational authority remains in the control layer around
intelligence.

## Memory Model

YAI operational memory is distinct from other AI memory forms.

`model parametric memory`
: Information encoded in model weights. It is broad, opaque, and not
case-bound operational state.

`prompt/context memory`
: Material placed into a model invocation. It is a projection for a task, not
durable authority or continuity.

`retrieval memory`
: Retrieved documents, chunks, embeddings, or indexes. It helps assemble
context, but retrieval alone does not prove what happened.

`agent memory`
: Notes, summaries, plans, preferences, or task-local history held by an agent.
It may support behavior, but it does not define operational truth.

`YAI operational memory`
: The case-bound memory of subjects, operations, decisions, receipts, effects,
policies, records, projections, divergences, and graph structure.

YAI operational memory is not another knowledge base. It is memory derived from
controlled operation and structured residue.

## Policy And Control

Policy in a prompt is not enforcement.

YAI materializes policy into machine control through policy sources,
policy material, policy claims, policy rules, case bindings, gates, decisions,
obligations, receipts, and policy memory.

A prompt can describe policy. It cannot enforce policy. Enforcement requires a
machine boundary that can bind the attempted operation, evaluate applicable
rules, emit a control decision, constrain or block the effect, and record a
receipt.

## Repository Shape

The filesystem doctrine separates ABI, system boundary, operational data, local
binaries, schemas, documentation, validation, and developer tools.

```text
include/  public and system ABI contracts
system/   C and system plane: daemon, host boundary, carriers, control shell, FFI bridges
engine/   Rust operational data spine: hot state, store, graph, index, memory, query, projection, reconcile
cmd/      local binaries: yai and yaid
proto/    schemas, fixtures, and protocol material
docs/     architecture, protocols, engineering notes, and ADRs
tests/    unit, integration, conformance, smoke, and adversarial tests
tools/    checks, probes, validation, and developer utilities
packaging/ package source material
examples/ non-authoritative examples
vendor/   vendored support code
```

Current refactor state: NEW.18 has centralized the Rust engine C shim under
`system/engine_bridge`. The C data-spine folders under
`system/{store,graph,index,memory,projection,reconcile}` are transitional
`keep_temporarily` smoke paths; Rust `engine/yai-engine` is the target owner of
store, journal, record codec, graph, index/query, memory, projection,
reconcile, retention, and integrity logic.

SPINE.3R adds the case-world and live data-plane doctrine. NEW.19 guard
realignment is done. Case-world material precedes subject behavior:

```text
case_world -> case_domain -> case_attachment -> case_binding -> case_session -> case_context -> subject
```

Projection is not a summary. It is a versioned cognitive view over the
operational data planes of a case. SPINE.4 adds Operational Observability &
Evaluation: YAI measures whether the case view is fresh, causal, provenanced,
coherent, complete, replayable and useful enough for controlled behavior.
Shared memory, LMDB, Ladybug, DuckDB, observability records and debug commands
are planned-not-created backend waves, not current implementations.

NEW.18B adds the live case context boundary: refs identify durable material,
while sessions and contexts operate on loaded case state.

The canonical case-view quality vector is `CVQ`: freshness,
causal_completeness, provenance_sufficiency, projection_consistency,
authority_alignment, memory_basis_quality, divergence_exposure, delta_accuracy
and cost.

NEW.18C separates journal replay/audit from active model context. The prompt
surface now uses an active interaction thread plus participant view frame over
the current case projection; transcript records remain audit material, not raw
chat memory.

SPINE.6A compresses active engineering docs. SPINE.6B makes future
implementation waves bidirectional: if the touched concept exists in `yai-dev`,
the wave must extract or rewrite useful material, classify the old residue,
update inventory and keep commit boundaries clear. SPINE.20 adds local runtime
layout: `PREFIX/bin/yai`, `PREFIX/bin/yaid`, `YAI_HOME/run`, `YAI_HOME/store`,
`YAI_HOME/log`, `YAI_HOME/tmp`, `YAI_HOME/cases`, `YAI_HOME/sockets` and
`YAI_HOME/config`. SPINE.20A rebases the active linear roadmap so SPINE.21 is
Pack Materialization Doctrine before filesystem freeze and data-plane
implementation. SPINE.21 defines packs as case materialization units; no pack
runtime, installer, registry backend or marketplace is implemented. SPINE.22
freezes the filesystem/runtime foundation. SPINE.23-SPINE.26 introduce and
harden hot state as a non-authoritative live case/session/context cache with
consumer-aware projection freshness policy before record, graph, fact,
projection and memory backends begin. SPINE.29 defines LMDB as the durable
indexed record lookup plane and keeps the next implementation step scoped to
SPINE.30 LMDB Record Write Path.

### Absorbed Concepts

Earlier or adjacent concepts are absorbed into the control spine:

* agents -> external subjects and YAI-dev participants;
* capabilities -> op effect classes and control gates;
* models -> model subjects, model carriers, projections, and model output claims;
* runtime -> daemon and effect carriers;
* substrate -> store, graph, index, memory, and projection;
* lineage -> graph, receipts, and reconstruction;
* workflow -> external procedure and case progress, not the core engine.

## Local Operation

`yaid`
: The local daemon. It is the resident service boundary for local control,
carrier coordination, records, receipts, and operational memory.

`yai`
: The canonical local technical command. It is the local control and inspection
surface for operators and developer workflows.

The README is not a command reference. Operational manuals and reference
documents own detailed command behavior.

## Validation

Stable repository validation entrypoints:

```text
make info
make check
```

## Documentation Pointers

Start with:

```text
docs/architecture/00-spine.md
docs/architecture/01-terminology.md
docs/architecture/04-subject-model.md
docs/architecture/06-control-policy-model.md
docs/engineering/four-repo-roadmap.md
docs/engineering/filesystem-target.md
docs/engineering/data-plane-roadmap.md
docs/engineering/wave-template.md
docs/engineering/operational-extraction-contract.md
docs/engineering/current-status.md
docs/status/implementation-status.md
```

Implementation status and roadmap material belong in status or roadmap
documents, not in this README.
