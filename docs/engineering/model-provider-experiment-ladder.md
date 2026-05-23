# Model Provider Experiment Ladder

Status: SPINE.2 experiment maturity doctrine, roadmap order superseded by
SPINE.3R data-plane rebase.

YAI Core can learn from model/provider experiments before it owns model
invocation. The core must not confuse early provider scouting with canonical
validation, and it must not import agent-framework assumptions before naked
model behavior is measurable.

## Doctrine

```text
A model is not inside the core.
A model is a case-bound subject with locality, provider, posture and observed behavior.
Agent frameworks are not first.
Agents are later subjects, actors or sources, not core owners.
```

The first AI behavior experiment is a naked model inside a case. The model
receives a projection. Its output returns as a claim, observation or proposed
operation. Control evaluates the result, and receipt-backed memory can derive
from what happened.

## Maturity Levels

| Level | Name | Meaning |
|---|---|---|
| L0 | Provider scouting | Outside core. The user can start immediately, install and run models manually, collect behavior samples and record notes. There is no YAI guarantee. |
| L1 | Observed model subject | The model is represented as a subject. Output is imported as a claim or observation. There is no model carrier yet. |
| L2 | Naked model case experiment | A case builds a projection, a local model receives that projection, output becomes a claim or proposed op, control evaluates it, and receipt/memory residue is generated. |
| L3 | Model carrier v0 | Core invokes a model through a carrier. `model.invoke` has a decision and receipt. |
| L4 | Agent layer experiment | Agent framework, tool calls or MCP enters as an external subject or actor. Agent trace is imported and tool attempts are controlled. |
| L5 | Multi-model / multi-agent comparison | Compare providers, local/open/closed models, projection variants and policy compliance across cases. |

## L0 Provider Scouting

L0 can begin immediately. It is useful because it tells the project how models
behave with candidate projections, policies and local runtimes. It is not
canonical core validation because YAI Core did not bind the model as a subject,
materialize policy into gates, issue a decision, record a receipt or derive
memory.

L0 outputs should be stored as notes or fixtures for future
`ai-environment` work or future naked model case tests after the data-plane
foundation.

Suggested L0 artifacts:

```text
model name
provider/runtime
locality: local/remote
hardware
prompt/projection variant
policy text
output
refusal/unsafe/proposal notes
latency
observed limitations
```

## Model Subject Posture

A model subject records the model as a case-bound operational entity. It is not
the model's internal mind, training source, provider policy engine or runtime
implementation.

Minimum posture dimensions:

```text
model identity
provider/runtime
locality
declared provider policy
observed behavior
projection posture
redaction posture
review posture
receipt posture
```

Local model posture:

```text
more controllable runtime
better privacy
not automatically reliable
observed behavior still required
```

Remote closed model posture:

```text
unknown internal policy
provider-declared policy only
observed behavior only
projection should default stricter
```

## Policy And Projection

Policy is materialized twice:

```text
cognitively through projection
operationally through gates, decision and carrier
```

For models, projection can include:

```text
allowed actions
forbidden actions
review requirements
evidence requirements
policy summaries
memory candidates
redaction posture
```

Projection is not enforcement by itself. It shapes model cognition. Control
still owns operational admission, decision, obligations and receipts.

## Roadmap Position

SPINE.3R keeps the maturity order but rebases the wave numbers. The data-plane
foundation now occupies NEW.22 through NEW.30, so model/provider implementation
comes after that foundation.

```text
NEW.13-NEW.21 filesystem/data-spine refactor and freeze
NEW.22-NEW.30 hot, record, graph, fact, projection, memory and reconcile planes
then ingest/model/provider/policy/model-carrier experiments
```

## Non-Goals

SPINE.2 must not:

```text
implement model runner
implement provider registry
create model carrier
create policy packs
create ai-environment repo
touch old yai
touch interfaces
touch console
```
