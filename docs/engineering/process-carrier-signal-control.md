# Process Carrier Signal Control

Status: active
Owner: system/effect and command surface
Purpose: Track SPINE.33D process carrier v0 and safe signal control.
Not source of truth for: arbitrary process management or process workflow APIs

Implemented files:

```text
include/yai/effect/process_carrier.h
include/yai/effect/process_signal.h
include/yai/effect/process_state.h
system/effect/carriers/process_carrier.c
system/effect/process_signal.c
system/effect/process_state.c
```

Commands:

```bash
yai carrier inspect process
yai process observe --pid <pid>
yai process signal --pid <pid> --signal TERM --dry-run
yai process signal --pid <pid> --signal TERM
yai process signal --pid <pid> --signal KILL --dry-run
```

The CLI is intentionally conservative: dry-run produces an inspectable dispatch
posture and real signals to arbitrary PIDs are blocked with
`carrier_attempted: false`.

Real signal execution is validated in C smoke only against a test-owned child
process spawned by the smoke test. No process spawn API is exposed for user
workflows in this wave.
