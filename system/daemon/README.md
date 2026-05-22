# system/daemon

Status: NEW.16 daemon support location.

This root owns the C daemon support implementation after the NEW.16 move:

```text
system/daemon/ipc.c
system/daemon/core_loop.c
system/daemon/daemon_status.c
```

The daemon binary entrypoint lives in `cmd/yaid/main.c`.
