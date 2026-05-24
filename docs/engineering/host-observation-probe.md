# Host Observation Probe

Status: active
Owner: system/observation and system/reconcile
Purpose: Track SPINE.33E host probe v0 and divergence candidate posture.
Not source of truth for: enforcement, repair, eBPF/auditd/inotify or sandboxing

Implemented files:

```text
include/yai/observation/host_probe.h
include/yai/observation/observation_result.h
include/yai/observation/observation_target.h
include/yai/reconcile/divergence_candidate.h
system/observation/host_probe.c
system/observation/observation_result.c
system/observation/observation_target.c
system/reconcile/divergence_candidate.c
```

Commands:

```bash
yai observe process --pid <pid>
yai observe compare-process --pid <pid> --expected running
yai observe compare-process --pid <pid> --expected stopped
```

The probe uses `kill(pid, 0)` on POSIX systems. It reports host-visible state
and never sends a signal. Mismatch produces a `divergence_candidate` and leaves
repair to later reconcile waves.

SPINE.33E keeps process effect control in the process carrier and keeps host
observation independent:

```text
yai process observe = process carrier surface
yai observe process = host_probe observation surface
```
