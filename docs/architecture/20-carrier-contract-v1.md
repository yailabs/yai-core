# Carrier Contract v1

Status: active
Owner: control/carrier substrate
Purpose: Define carrier.v1 and the filesystem carrier adapter boundary.
Not source of truth for: process signals, network/database/git/model carriers or runtime queues

SPINE.33C defines the first carrier execution contract. A carrier is not a free
tool. A carrier is a controlled effect realizer that must expose:

```text
carrier.v1
pre_state_observation
controlled_execution_or_observation
post_state_observation
evidence_capture
receipt_assembly
receipt_validation
residue_recording
```

The filesystem carrier is the first carrier.v1 adapter. Process, network_http,
database, repository_git, model_provider, observation and review lanes remain
planned or non-executing.

SPINE.33D adds process carrier v0 as the second active-minimal carrier.v1
adapter. Its real signal effects are limited to test-owned child processes;
arbitrary PID signals are blocked.

## Outcome Boundary

Carrier outcome values:

```text
executed
blocked
deferred
observed
failed
quarantined
waiting_operator
waiting_agent
not_attempted
unknown
```

carrier outcome is not gate outcome. Carrier outcome is not dispatch status.
Carrier outcome is not receipt guarantee mode.

## Receipt Posture

Minimum carrier receipt posture:

```text
carrier_family
carrier_name
contract_version
attempt_id
decision_id
receipt_id
outcome
guarantee_mode
pre_state_observed
post_state_observed
evidence_captured
receipt_validated
residue_recorded
```

## Filesystem Mapping

Filesystem read maps to observed:

```text
carrier_family: filesystem
carrier_name: filesystem
contract_version: carrier.v1
operation: fs.read
outcome: observed
guarantee_mode: interposed
pre_state_observed: true
post_state_observed: true
evidence_captured: true
receipt_validated: true
residue_recorded: true
```

Blocked filesystem write maps to blocked:

```text
operation: fs.write
outcome: blocked
controlled_execution_or_observation: not_executed
guarantee_mode: interposed
receipt_required: yes
```

Allowed filesystem write maps to executed:

```text
operation: fs.write
outcome: executed
controlled_execution_or_observation: executed
post_state_observation: supported
guarantee_mode: interposed
receipt_required: yes
```

`interposed` is the honest guarantee mode for SPINE.33C because YAI mediates
the filesystem read/write path and assembles evidence/receipt material through
the filesystem carrier. It is not claiming kernel-level embedded enforcement.

SPINE.33F records filesystem as `controlled: active_minimal` in the carrier
coverage matrix and keeps other non-filesystem families visible without fake
execution.
