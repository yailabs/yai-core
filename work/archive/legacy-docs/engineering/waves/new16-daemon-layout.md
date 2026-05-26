Historical/superseded engineering record. Not an active source of truth.

# NEW.16 Daemon Layout

Status: NEW.16 physical refactor wave.

NEW.16 moves the C daemon entrypoint and daemon support code into the target
command/system layout:

```text
daemon/main.c              -> cmd/yaid/main.c
daemon/ipc.c               -> system/daemon/ipc.c
daemon/core_loop.c         -> system/daemon/core_loop.c
lib/daemon/daemon_status.c -> system/daemon/daemon_status.c
```

`cmd/yaid` is the canonical daemon entrypoint location. `system/daemon` owns
daemon support, IPC and bounded loop implementation. The top-level `daemon/`
source root is removed.

## Current Layout

```text
cmd/
  yai/
  yaid/
    main.c

system/
  daemon/
    ipc.c
    core_loop.c
    daemon_status.c
```

`yai` remains the Rust technical command in `cmd/yai`. `yaid` remains the C
daemon binary and still builds to `build/yaid`.

## Boundaries

NEW.16 does not move the rest of `lib/`; general C implementation migration is
NEW.17. NEW.16 does not change daemon IPC protocol, public headers, install
layout, engine code or command behavior.

## Audit Result

Read-only old-yai runtime audit classified activation, lifecycle, machine and
connection material as daemon concept evidence only. Runtime tooling may inform
future daemon tooling, but no old source is copied.
