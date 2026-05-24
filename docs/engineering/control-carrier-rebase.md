# Control / Carrier Rebase

Status: active
Owner: engineering/control-effect
Purpose: Track the SPINE.33A control/carrier substrate extraction.
Not source of truth for: carrier execution or dispatch runtime

SPINE.33A drains the first low-level concepts from old runtime carrier,
execution, observation and decision material into the new `yai` grammar.

Extracted posture:

```text
carrier family
gate outcome
dispatch status
receipt guarantee mode
host observation posture
```

Boundary rules:

```text
observation is not enforcement
decision is not execution
carrier is not free tool invocation
receipt is not logging
```

The implementation is intentionally small:

```text
include/yai/effect/carrier_family.h
include/yai/effect/dispatch.h
include/yai/effect/receipt_guarantee.h
include/yai/control/gate_outcome.h
include/yai/observation/host_observation.h
system/effect/carrier_family.c
system/effect/dispatch.c
system/effect/receipt_guarantee.c
system/control/gate_outcome.c
system/observation/host_observation.c
```

No process, network, database, git, model or observation carrier is added. No
dispatch queue is added. No LMDB behavior changes.

Manual inspection:

```text
yai carrier families
```

Validation:

```text
make check-control-carrier-substrate
make smoke-spine33a
```
