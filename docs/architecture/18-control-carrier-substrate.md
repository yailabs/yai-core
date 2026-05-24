# Control / Carrier Substrate

Status: active doctrine
Owner: control/effect/observation
Purpose: Define the first low-level control/carrier posture primitives.
Not source of truth for: carrier execution, dispatch runtime or receipt storage

SPINE.33A introduces the substrate vocabulary that sits between a proposed
operation and any host/external effect.

Core rule:

```text
Nothing that comes from model/operator/API/tool toward host/external world may
be treated as direct execution.
```

It must first become an op/control/carrier candidate with explicit carrier
family, gate outcome, dispatch status and receipt guarantee mode. Observation
is not enforcement. Decision is not execution. Carrier is not free tool
invocation. Receipt is not logging.

```text
observation is not enforcement
decision is not execution
carrier is not free tool invocation
receipt is not logging
```

## Carrier Family

Carrier family names the boundary class that would be involved if an operation
became routable:

```text
filesystem
process
network_http
database
repository_git
model_provider
observation
review
unknown
```

SPINE.33A does not implement process, network, database, repository, model,
observation or review carriers. The filesystem carrier remains the only
minimal implemented family.

## Gate Outcome

Gate outcome is the control posture before dispatch:

```text
allow
deny
defer
observe_only
require_review
require_evidence
require_redaction
allow_with_constraints
quarantine
unknown
```

Gate outcome is not carrier execution status.

## Dispatch Status

Dispatch status says whether a candidate can be routed:

```text
pending
routable
dispatched
blocked
deferred
failed
not_supported
unknown
```

Dispatch status is not decision outcome and not receipt status. SPINE.33A adds
the vocabulary and a zeroed dispatch initializer. SPINE.33B adds dispatch lanes
and route planning, still without adding a queue or execution.

See [19-operation-dispatch-multiplex.md](19-operation-dispatch-multiplex.md)
for the lane grammar.

SPINE.33C adds [20-carrier-contract-v1.md](20-carrier-contract-v1.md), which
defines the common carrier.v1 contract and adapts filesystem as the first
active-minimal carrier adapter.

## Receipt Guarantee Mode

Receipt guarantee mode says how strongly YAI can attest evidence:

```text
observed
interposed
carrier_owned
embedded
external_import
unknown
```

Receipt guarantee mode does not create receipts by itself.

## Host Observation Posture

Host observation posture records observation of host/external state:

```text
not_observed
observed
mismatch
bypass_suspected
untracked_effect
unknown
```

Observation can produce evidence or divergence. Observation is not enforcement.

## CLI Surface

SPINE.33A adds:

```text
yai carrier families
```

The command is inspectable vocabulary/status only. It does not execute a
carrier and does not claim readiness for planned carrier families.
