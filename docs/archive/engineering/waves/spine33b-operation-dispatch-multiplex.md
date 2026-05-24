Historical/superseded engineering record. Not an active source of truth.

# SPINE.33B Operation Dispatch Multiplex

Status: completed
Owner: control/carrier substrate
Purpose: Archive the SPINE.33B implementation record.
Not source of truth for: active roadmap or command syntax

SPINE.33B added a no-execution dispatch/multiplex layer:

```text
dispatch_lane
dispatch_plan
dispatch_multiplex
yai carrier lanes
yai carrier route --family <family>
smoke-spine33b
check-operation-dispatch-multiplex
```

The wave routes carrier families to carrier lanes and exposes lane policy
metadata. It does not execute carriers, spawn queues, control processes, call
networks, mutate databases, run git commands or invoke models.

Validation target:

```bash
make smoke-spine33b
make check-operation-dispatch-multiplex
```
