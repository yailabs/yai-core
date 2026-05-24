# Process Carrier Signal Control

Status: active
Owner: control/carrier substrate
Purpose: Define SPINE.33D process carrier v0 and signal safety boundary.
Not source of truth for: arbitrary process management or host observation hardening

SPINE.33D introduces the first controlled process carrier path. SIGTERM and
SIGKILL are controlled effects, not shell commands.

The process carrier declares carrier.v1.

Core rule:

```text
op -> gate -> process_lane -> process carrier -> signal -> observation -> receipt
```

No process effect is valid without case scope, target process subject,
op_attempt, decision/gate outcome, process_lane dispatch, process carrier,
pre-state observation, controlled signal effect, post-state observation and
receipt.

Safety rule:

```text
no arbitrary kill
```

Real signal effects are limited to a test-owned process spawned by smoke or
manual test. Arbitrary or external PID signal attempts report:

```text
carrier_attempted: false
outcome: blocked
reason: unsafe_process_target
```

## Vocabulary

Process owner scope:

```text
test_owned
yai_owned
external_observed
unknown
```

Process state:

```text
unknown
running
stopped
exited
signaled
not_found
permission_denied
```

Process signal:

```text
TERM
KILL
INT
HUP
UNKNOWN
```

SPINE.33D supports real TERM and KILL only in test-owned scope. CLI dry-run
does not send a signal. CLI non-dry-run on arbitrary PIDs is blocked.

## Mapping

```text
observe running -> observed
TERM sent + exited/signaled -> executed
KILL sent + exited/signaled -> executed
unsafe target denied -> blocked
signal failed -> failed
```

Process receipts use `guarantee_mode: interposed` because YAI mediates the
signal path in tests. This does not claim kernel-level embedded enforcement.

SPINE.33E separates this from the independent host probe surface:

```text
yai process observe = process carrier surface
yai observe process = independent host observation surface
```

SPINE.33F records process as `controlled: active_minimal` and
`observed: active_minimal` in the carrier coverage matrix. That coverage remains
limited to safe/test-owned signal paths and independent process observation.
