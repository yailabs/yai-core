# Host Observation Probe

Status: active
Owner: observation/reconcile substrate
Purpose: Define SPINE.33E host observation probe v0 and bypass/mismatch posture.
Not source of truth for: kernel enforcement, process sandboxing or repair runtime

SPINE.33E adds independent host probe vocabulary and process observation.
observation is not enforcement.

Core rule:

```text
expected state + observed state + receipt posture -> matched | mismatch
```

A carrier receipt is not enough by itself. YAI must be able to inspect host
state independently and compare it to expected state, decision and receipt
posture. If the values do not align, the output is a `divergence_candidate`.
There is no silent repair.

The process observer uses `kill(pid, 0)` as a POSIX probe:

```text
kill(pid, 0) == 0 -> running
errno == ESRCH    -> not_found
errno == EPERM    -> permission_denied
else              -> unknown
```

This POSIX probe checks existence/permission posture only. It does not send a
signal, enforce policy, stop a process or repair state.

## Vocabulary

Observation target family:

```text
process
filesystem
network
database
repository
unknown
```

Observation result:

```text
matched
mismatch
not_found
permission_denied
not_observed
unknown
```

Divergence candidate kinds:

```text
expected_stopped_but_running
expected_running_but_not_found
receipt_claimed_executed_but_not_observed
decision_denied_but_effect_observed
receipt_missing_for_observed_effect
unknown
```

SPINE.33E implements process host probe v0. Filesystem, network, database and
repository observation remain future work.

## Command Posture

```text
yai observe process --pid <pid>
yai observe compare-process --pid <pid> --expected running
yai observe compare-process --pid <pid> --expected stopped
```

All commands must say:

```text
enforcement: none
observation_is_enforcement: false
```

`yai process observe` remains the process carrier surface. `yai observe process`
is the independent host probe surface.
