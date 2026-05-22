# Naked Model Case Test Plan

Status: SPINE.2 planning doctrine for NEW.26.

This plan defines the first canonical AI behavior test wave. It does not create
a runner, carrier, policy pack, provider registry or environment repo.

## Position

The first real naked model test is NEW.26. It comes after:

```text
NEW.13-NEW.21 filesystem/data-spine refactor
NEW.22 ingest material model v0
NEW.23 model/provider subject posture v0
NEW.24 model output claim import v0
NEW.25 policy pack skeleton + model projection v0
```

It comes before:

```text
NEW.28 model carrier v0
NEW.29 agent trace/tool call import v0
NEW.30 policy compliance experiment harness v0
```

## Test Shape

NEW.26 tests a naked local model inside a case:

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
but they are not canonical core validation. When promoted into NEW.26, L0
material must be converted into fixtures with explicit provenance.

Useful L0-to-NEW.26 fields:

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

Agent frameworks are not part of NEW.26. Agent layer tests come after naked
model behavior is measurable, at NEW.29/NEW.30. Agents enter later as external
subjects, actors or sources whose traces and tool attempts are imported and
controlled.

## Non-Goals

NEW.26 does not:

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
