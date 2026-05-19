# YAI Core Spine

Status: DOC.CORE.0 canonical spine.

YAI Core is a local AI operational control core.

It binds operational subjects to cases, captures operation attempts,
materializes policy into machine gates, emits control decisions, executes or
observes effects through carriers, records receipts, derives operational memory
and serves controlled projections.

Core truth is residue-based. YAI Core owns operational truth only through
case-bound records, subject bindings, attempts, decisions, receipts, graph
relations, memory and projections. It does not own external system truth unless
the external system binds or reports through YAI protocols.

## Reading Map

| Need | Read |
|---|---|
| Frozen terms | [01-terminology.md](01-terminology.md) |
| Integration guarantees | [02-integration-modes.md](02-integration-modes.md) |
| Case boundary | [03-case-domain.md](03-case-domain.md) |
| Subjects | [04-subject-model.md](04-subject-model.md) |
| Operations | [05-operation-model.md](05-operation-model.md) |
| Control and policy | [06-control-policy-model.md](06-control-policy-model.md) |
| Carriers and receipts | [07-effect-carrier-model.md](07-effect-carrier-model.md) |
| Store/index/graph | [08-store-index-graph-model.md](08-store-index-graph-model.md) |
| Memory | [09-memory-model.md](09-memory-model.md) |
| Projection | [10-projection-model.md](10-projection-model.md) |
| Reconciliation | [11-reconciliation-model.md](11-reconciliation-model.md) |
| Daemon and ctl | [12-daemon-and-ctl.md](12-daemon-and-ctl.md) |
| C/Rust boundary | [13-c-rust-boundary.md](13-c-rust-boundary.md) |

## Machine Spine

```text
world material
-> ingest
-> subject binding
-> case boundary
-> operation attempt
-> control / policy gate
-> effect / carrier
-> receipt
-> store
-> graph / index
-> memory
-> projection
-> reconcile
```

The system starts from subjects, attempts, policy, effects, receipts,
operational memory and controlled projections. It does not start from agents,
workflows, prompts, UI or model providers.

## Core Invariants

```text
no operation without a case boundary
no target without a subject reference
no enforcement claim without a control decision
no effect truth without receipt or observation provenance
no memory without record, receipt or graph basis
no projection without provenance, freshness and redaction posture
no reconciliation without expected-vs-observed comparison
```

These invariants are stronger than folder layout. If an implementation violates
them, it is outside the core doctrine even if it uses the right names.

## Primitive Ownership

| Primitive | Owns | Does not own |
|---|---|---|
| `base` | ids, time, status, errors, byte refs | domain semantics |
| `ingest` | normalization of material and claims | belief or execution |
| `subject` | operational entities bound to cases | full external system state |
| `case` | boundary, posture, subject binding, evidence refs | abstract world governance |
| `op` | normalized attempted operation | policy decision or execution |
| `control` | policy materialization, gates, decisions, obligations | carrier side effects |
| `effect` | carrier requests, execution/observation, receipts | policy authority |
| `store` | durable records, journals, blobs, integrity | intelligent recall |
| `index` | retrieval structures and query access | truth ownership |
| `graph` | relationships, causality, reconstruction paths | raw record persistence |
| `memory` | scoped operational experience from residue | hidden prompt context |
| `projection` | controlled read models | source-of-truth state |
| `reconcile` | mismatch detection and recovery posture | silent repair |
| `daemon` | local residency, IPC, loop, supervision | product UX |
| `ctl` | technical core command surface | console experience or semantics |

## Operational Truth Model

| Level | Meaning | Example |
|---|---|---|
| claim | unverified material entered the system | external tool says it wrote a file |
| observation | YAI observed material or state | filesystem state changed |
| decision | YAI evaluated an attempt | write allowed with constraints |
| receipt | effect or observation was recorded with provenance | carrier receipt for file write |
| reconstruction | records and graph edges explain sequence | decision -> carrier -> receipt |
| memory | residue has been consolidated under scope | repo write pattern remembered for case |
| projection | controlled view is served to a consumer | audit packet or model context |

YAI Core does not convert claims into receipts by wording. Claims become
evidence only through ingest, binding, provenance and store records.

## Evidence, Receipt And Memory

Evidence is any case-bound record or reference that can support a decision,
reconstruction or projection.

A receipt is durable evidence connecting an attempted operation, decision
posture, carrier or observation path, affected subjects and result status.
Imported receipts keep external provenance and weaker guarantee labels.

Memory is scoped operational experience derived from records, receipts, graph
paths and decisions. It is not raw storage and not a context pack.

## Control Guarantee Model

YAI Core must never overclaim control.

| Mode | Core can claim | Core cannot claim |
|---|---|---|
| observed | observed, recorded, reconstructed | prevented, blocked or enforced |
| advisory | recommended or warned | blocked, enforced or executed |
| interposed | decided before forwarding effect request | real effect unless receipt returns |
| enforced | host/carrier is expected to honor decision | bypass impossible outside declared boundary |
| carrier_owned | executed or blocked through YAI carrier | external bypass prevention outside carrier |
| embedded | host-integrated decision and receipt path | control outside declared embed boundary |

The same primitives exist in every mode, but their guarantees differ. Projection
and product language must preserve the guarantee label.

## Repository Split

```text
yai-core   = canonical local AI operational control core
yai        = old/current concept mine, future ai-environment material
interfaces = projection/API/SDK/conformance
console    = operator terminal/client UX
```

`yai-core` is a workspace sibling to `yai`, not nested inside `yai`.

## Implementation Order

```text
DOC.CORE.0: canonical docs freeze
NEW.1: minimum loop implementation
integration adapters
Rust operational data engine behind C FFI
```

NEW.1 proves:

```text
open case
bind subject
submit op attempt
control decision
null carrier / observed receipt
append store record
query projection
```

## Non-Goals

YAI Core is not an agent framework, workflow engine, product CLI, TUI, general
assistant, cloud platform, model provider, SDK surface, compliance certificate
or dashboard.

Do not migrate old roots as folders. Do not make Console or Interfaces the
product root for core truth.
