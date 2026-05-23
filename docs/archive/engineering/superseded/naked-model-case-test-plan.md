Historical/superseded engineering record. Not an active source of truth.

# Naked Model Case Test Plan

Status: SPINE.2 planning doctrine, roadmap order superseded by SPINE.3R
data-plane rebase.

This plan defines the first canonical AI behavior test wave. It does not create
a runner, carrier, policy pack, provider registry or environment repo.

## Position

The first real naked model test comes after the data-plane foundation. SPINE.3R
rebases the old NEW.26 position because NEW.22 through NEW.30 now establish
hot, record, graph, fact, projection, memory and reconcile planes. It comes
after:

```text
NEW.13-NEW.21 filesystem/data-spine refactor
NEW.22-NEW.30 data-plane foundation
ingest material model v0
model/provider subject posture v0
model output claim import v0
policy pack skeleton + model projection v0
```

It comes before:

```text
model carrier v0
agent trace/tool call import v0
policy compliance experiment harness v0
```

## Test Shape

The naked model case wave tests a naked local model inside a case:

```text
case exists
model is bound as a subject
policy is materialized into projection and gates
projection is built for the model
local model receives the projection outside an agent framework
output is imported as claim, observation or proposed operation
control evaluates any proposed operation
receipt records the import/evaluation path
memory candidate can derive from receipt-backed behavior
```

The test proves the case/projection/control/receipt loop around model behavior.
It does not prove provider reliability or hidden model policy.

## Inputs

Minimum inputs:

```text
case_ref
model subject posture
provider/runtime note
locality
projection kind
policy summary
redaction posture
review requirement
evidence requirement
prompt/projection fixture
expected import posture
```

## Outputs

Expected outputs:

```text
model output claim
optional proposed operation
control decision for any proposed operation
receipt for output import
receipt for decision/evaluation path
model behavior trace
memory candidate refs, if allowed
projection refs used by the model
```

## L0 Fixture Intake

L0 provider scouting can start immediately. L0 notes are useful seed material,
but they are not canonical core validation. When promoted into the naked model
case wave, L0 material must be converted into fixtures with explicit
provenance.

Useful L0-to-naked-model-case fields:

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

## Policy And Projection Checks

The model projection should include:

```text
allowed actions
forbidden actions
review requirements
evidence requirements
policy summaries
memory candidates
redaction posture
```

Policy must also materialize operationally through gates, decision and later
carrier boundaries. A model obeying projected policy is useful behavior
evidence, not an enforcement guarantee.

## Agent Deferral

Agent frameworks are not part of the naked model case wave. Agent layer tests
come after naked model behavior is measurable. Agents enter later as external
subjects, actors or sources whose traces and tool attempts are imported and
controlled.

## Non-Goals

The naked model case wave does not:

```text
implement model runner
implement provider registry
create model carrier
create agent framework integration
create policy packs
create ai-environment repo
touch old yai
touch interfaces
touch console
```
