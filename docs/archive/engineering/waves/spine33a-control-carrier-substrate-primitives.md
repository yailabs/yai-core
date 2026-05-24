Historical/superseded engineering record. Not an active source of truth.

# SPINE.33A Control / Carrier Substrate Primitives

Status: completed
Owner: control/effect/observation
Purpose: Archive the SPINE.33A implementation wave.
Not source of truth for: current ABI or command output

SPINE.33A adds minimal C ABI/types and helpers for:

```text
carrier family
gate outcome
dispatch status
receipt guarantee mode
host observation posture
```

It also adds:

```text
yai carrier families
make smoke-spine33a
make check-control-carrier-substrate
```

No carrier execution, queue, scheduler or LMDB behavior changed.
