# Daemon And Ctl

`yaid` and `yaictl` are technical core surfaces.

## `yaid`

`yaid` is the resident local process.

Owns:

```text
boot
config
loop
local IPC
event stream
dispatch
status
supervision
host integration
```

## `yaictl`

`yaictl` is the embedded technical control CLI for the core.

In NEW.1 `yaictl` is implemented in Rust as a client over core primitives. It
does not own core semantics.

Initial command families:

```text
daemon
case
subject
op
control
decision
receipt
store
memory
projection
carrier
debug
```

## Console Boundary

User-facing command UX belongs to Console. `yaictl` exists for technical inspection, smoke tests, local control and development diagnostics. It must not become the product CLI.

## NEW.11 Local IPC V0

NEW.11 adds the first local daemon IPC contract over a Unix domain socket.

Initial request types:

```text
status
info
shutdown
```

Initial response fields:

```text
ok
version
status
message
```

`yaid` can run with:

```text
yaid --socket <path> --foreground
yaid --once status
yaid --version
```

`yaictl` can call:

```text
yaictl daemon status --socket <path>
yaictl daemon info --socket <path>
yaictl daemon shutdown --socket <path>
```

No case, operation, control, effect or store execution crosses IPC in NEW.11.
The daemon is alive and inspectable, but operational dispatch remains deferred.

## NEW.12 Daemon-Backed Loop V0

NEW.12 adds the first daemon-served core loop. The daemon can now serve:

```text
run_minimum_loop
run_filesystem_loop
journal_summary
projection_summary
```

The transport remains a local Unix socket and the request format remains
line-oriented. The daemon response is structured JSON with request id, status,
journal path and residue counts.

`run_minimum_loop` creates a bounded case/subject/op/control/receipt/store path
and appends graph, memory and projection residue. `run_filesystem_loop` proves
the same daemon path with sandboxed filesystem residue.

This is not public API, HTTP, auth, session management, service supervision or
multi-client runtime. It is the first proof that `yaictl -> yaid -> core loop ->
journal/projection -> response` works locally.
