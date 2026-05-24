Historical/superseded engineering record. Not an active source of truth.

# SPINE.33D Process Carrier Signal Control

Status: completed
Owner: control/carrier substrate
Purpose: Archive the SPINE.33D implementation record.
Not source of truth for: active command output or current roadmap

SPINE.33D added:

```text
process carrier.v1
process observe
process signal dry-run
test-owned TERM/KILL smoke
unsafe target blocking
make smoke-spine33d
make check-process-carrier-signal-control
```

No arbitrary PID kill, process workflow spawn API, network/database/git/model
carrier or host observation hardening was added.
