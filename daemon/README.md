# daemon

Transitional role: this top-level root is the bootstrap source location for the
current `yaid` resident process.

Target role:

```text
daemon/main.c      -> cmd/yaid/main.c
daemon/ipc.c       -> system/daemon/ipc.c
daemon/core_loop.c -> system/daemon/core_loop.c
```

Owning wave: NEW.16 splits the daemon entrypoint from daemon implementation.
After NEW.21 there should be no top-level `daemon/` source root.
