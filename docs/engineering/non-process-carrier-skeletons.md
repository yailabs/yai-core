# Non-Process Carrier Skeletons

Status: active
Owner: system/effect and command surface
Purpose: Track SPINE.33G skeleton carrier implementation.
Not source of truth for: adapter execution, hosted provider calls or review queue execution

Implemented files:

```text
include/yai/effect/carrier_skeleton.h
system/effect/carrier_skeleton.c
tests/smoke/non-process-carrier-skeletons/test_non_process_carrier_skeletons.c
tools/checks/check-non-process-carrier-skeletons.sh
```

Skeleton carriers:

```text
network_http
database
repository_git
service
endpoint
socket
listener
model_provider
review
```

Commands:

```bash
yai carrier inspect network_http
yai carrier inspect database
yai carrier inspect repository_git
yai carrier inspect service
yai carrier inspect endpoint
yai carrier inspect socket
yai carrier inspect listener
yai carrier inspect model_provider
yai carrier inspect review
yai carrier coverage --family database
yai carrier coverage --family model_provider
```

Expected skeleton fields:

```text
contract: carrier.v1
status: skeleton
execution_available: false
receipt_required: yes
supports_inspect: yes
non_execution_reason:
carrier_attempted: false
```

Default non-execution reason is `adapter_not_implemented`. `model_provider`
uses `model_provider_carrier_not_implemented`. `review` uses
`review_lane_not_implemented`.
The model-provider future activation path is `SPINE.93A+`.

Smoke:

```bash
make smoke-spine33g
```

Guard:

```bash
make check-non-process-carrier-skeletons
```

This wave does not execute real network requests, database queries, git
commands, socket operations, listener operations, service lifecycle control,
model invocation or review queue execution.
