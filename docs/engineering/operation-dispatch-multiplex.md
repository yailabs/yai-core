# Operation Dispatch Multiplex

Status: active
Owner: system/effect and command surface
Purpose: Track SPINE.33B no-execution carrier lane routing.
Not source of truth for: real queues, carrier execution or process control

SPINE.33B adds the first operation dispatch model. A dispatch plan names the
carrier family, carrier lane, dispatch status, lane policies and receipt
requirement. It is deliberately inspectable before execution exists.

Implemented files:

```text
include/yai/effect/dispatch_lane.h
include/yai/effect/dispatch_plan.h
include/yai/effect/dispatch_multiplex.h
system/effect/dispatch_lane.c
system/effect/dispatch_plan.c
system/effect/dispatch_multiplex.c
```

CLI:

```bash
yai carrier lanes
yai carrier route --family filesystem
yai carrier route --family process
yai carrier route --family unknown
```

Important boundary:

```text
dispatch plan != decision
dispatch plan != execution
execution_performed: false
```

The CLI may report `dispatch_status: routable` for planned lanes. That only
means the lane is known and a future carrier contract can target it. It does
not mean a process, network, database, repository, model or observation effect
can be executed.

Manual filesystem-loop validation must stay aligned with this path. Any future
carrier/control wave that changes CLI, dispatch, gate, carrier or receipt
behavior must update:

```text
docs/manuals/manual-filesystem-loop-validation.it.ipynb
docs/manuals/manual-filesystem-loop-validation.md
```
