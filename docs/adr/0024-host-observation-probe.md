# ADR 0024: Host Observation Probe

Status: accepted

## Decision

Observation is not enforcement.

YAI uses an independent host probe to compare expected state with observed
state. A mismatch produces a divergence candidate and no silent repair.

## Consequences

- Carrier receipts are not treated as sufficient host truth.
- Process observation uses POSIX `kill(pid, 0)` in v0.
- The process carrier owns signal effects; the host probe owns observation.
- eBPF, auditd, inotify and sandboxing remain future work.
