# NEW.11 Daemon IPC

NEW.11 turns `yaid` from a bootstrap stub into the first local daemon endpoint.

This is not the full daemon runtime. It is the first IPC contract between
`yai` and `yaid`.

## IPC Mode

NEW.11 uses a Unix domain socket on macOS/Linux.

Smoke tests use isolated paths:

```text
build/tmp/new11/daemon-ipc-<pid>/yaid.sock
```

## Requests

The initial protocol is line-oriented:

```text
status
info
shutdown
```

No case, op, control, effect, store, memory or projection execution is exposed
over IPC yet.

## Response

Responses are JSON lines with:

```text
ok
version
status
message
```

Example:

```text
{"ok":true,"version":"0.0.0-newcore","status":"ok","message":"yaid local ipc ready"}
```

## yaid

```text
build/yaid --version
build/yaid --once status
build/yaid --socket <path> --foreground
```

`--foreground` is accepted for the resident local process posture. NEW.11 does
not implement background service management or persistence supervision.

## yai

```text
yai daemon status --socket <path>
yai daemon info --socket <path>
yai daemon shutdown --socket <path>
```

`yai` remains the Rust technical client. Console remains a sibling product
boundary and does not inherit protocol ownership from `yai`.

## Smoke Proof

`tests/smoke/daemon-ipc/test_daemon_ipc.sh` starts `yaid`, waits for the socket,
calls status/info/shutdown through `yai`, then asserts that the daemon exits.

## Old-YAI Audit

NEW.11 inspected runtime activation, lifecycle, machine, connection, transport
and runtime tooling material without copying source:

```text
src/runtime/activation/*
src/runtime/lifecycle/*
src/runtime/machine/*
src/runtime/connections/*
src/runtime/boundary/transport/*
tools/runtime/*
Documentation/manuals/runtime-service-operations.md
```

Extracted concepts:

```text
daemon has local lifecycle/readiness posture
runtime access must fail closed
local IPC is a boundary, not semantic ownership
transport normalization/security are future work
operator/service tooling remains separate from core semantics
```

Deferred:

```text
case/op execution over IPC
HTTP or remote transport
auth
daemon persistence manager
service supervision
runtime carrier loop
```
