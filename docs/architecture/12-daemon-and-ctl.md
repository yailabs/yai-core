# Daemon And Ctl

`yaid` and `yai` are technical core surfaces.

SPINE.1 classifies top-level `daemon/`, retired `ctl/` and the former
`crates/yai-ctl` as transitional bootstrap locations. NEW.15 moved the Rust
technical command to `cmd/yai` and removed `crates/`. NEW.16 moved the daemon
entrypoint to `cmd/yaid`, daemon support to `system/daemon` and removed the
top-level `daemon/` source root. NEW.17 removed the retired `ctl/` pointer
root. The command shape is:

```text
cmd/
├── yai/
│   ├── Cargo.toml
│   └── src/main.rs
└── yaid/
    └── main.c
```

NEW.18B adds the live case session boundary. The daemon and CLI may still pass
`case_ref`, `subject_ref` and journal paths across IPC or shell boundaries, but
an active case is represented as a `case_session` with a materialized
`case_context`.

The target system plane keeps daemon implementation and host/control boundary
code under `system/`. After NEW.16, `system/daemon` owns daemon support:

```text
system/daemon/ipc.c
system/daemon/core_loop.c
system/daemon/daemon_status.c
```

## `yaid`

`yaid` is the resident local process.
It is C and its entrypoint lives in `cmd/yaid/main.c`.

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

## `yai`

`yai` is the embedded technical control CLI for the core. It now lives in
`cmd/yai`.

In NEW.1 `yai` is implemented in Rust as a client over core primitives. It
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

User-facing command UX belongs to Console. `yai` exists for technical inspection, smoke tests, local control and development diagnostics. It must not become the product CLI.

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

`yai` can call:

```text
yai daemon status --socket <path>
yai daemon info --socket <path>
yai daemon shutdown --socket <path>
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
multi-client runtime. It is the first proof that `yai -> yaid -> core loop ->
journal/projection -> response` works locally.

## Local Command Layout Timing

Local install layout is no longer NEW.13. It is scheduled as SPINE.20 so the
repo does not install a filesystem shape already known to be transitional.

SPINE.20 is expected to make `yai` the canonical local developer command and
install `yaid` beside it:

```text
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
```

The local host layout is:

```text
$(YAI_HOME)/run/
$(YAI_HOME)/store/
$(YAI_HOME)/log/
$(YAI_HOME)/tmp/
```

Default daemon socket:

```text
$(YAI_HOME)/run/yaid.sock
```

This will not make Console the owner of the `yai` command. Console remains the
operator UX surface. `yai` remains the technical core command for local
development, inspection and controlled daemon interaction.
