Historical engineering record. Not an active source of truth.

# SPINE.23 Hot State / Shared Memory Plane v0

SPINE.23 introduces hot state as the first live data plane.

Delivery summary:

```text
hot state ABI
system/hot implementation
Rust engine hot module
daemon-owned YAI_HOME/run/hot-state.json snapshot
yai doctor hot-state diagnostics
yai hot status
smoke-spine23
hot-state doctrine guard
```

Doctrine:

```text
hot state is not truth
journal remains replay/audit
projection freshness and stale reason are live cache posture
```

True OS shared memory or mmap optimization remains future work.
