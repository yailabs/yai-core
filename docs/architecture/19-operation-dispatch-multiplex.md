# Operation Dispatch Multiplex

Status: active
Owner: control/carrier substrate
Purpose: Define SPINE.33B dispatch lanes and no-execution route planning.
Not source of truth for: carrier execution, process signals, network calls or receipt hardening

SPINE.33B moves from carrier vocabulary to dispatch planning. An operation
attempt can be classified by carrier family, mapped to a carrier lane and
reported as routable or not supported without executing an effect.

Core doctrine:

```text
dispatch is separate from decision
dispatch is separate from execution
carrier lanes are inspectable route targets
no single global queue owns all operation attempts
execution_performed: false
```

Decision says whether an operation may proceed. Dispatch says where it would
go if allowed. Execution is the later carrier-owned effect path. These planes
must stay separate so a future process, network or repository carrier cannot be
introduced as a free tool invocation.

## Lane Grammar

SPINE.33B defines these carrier lanes:

```text
filesystem_lane
process_lane
network_http_lane
database_lane
repository_git_lane
model_provider_lane
observation_lane
review_lane
unknown_lane
```

Every lane carries:

```text
lane_ref
carrier_family
ordering_policy
capacity_policy
lock_policy
timeout_policy
retry_policy
receipt_requirement
failure_mode
status
```

## Policies

Ordering policies:

```text
serial_per_case
parallel_by_subject
exclusive_global
unordered
unknown
```

Capacity policies:

```text
single_inflight
bounded_parallel
unbounded_planned
disabled
unknown
```

Lock policies:

```text
case_lock
subject_lock
target_lock
carrier_lock
none
unknown
```

Timeout policies:

```text
short
normal
long
manual
unknown
```

Retry policies:

```text
none
safe_retry
manual_retry
requires_review
unknown
```

## Route Posture

Filesystem is the only active-minimal family because the existing filesystem
carrier smoke path already exists:

```text
carrier_family: filesystem
lane: filesystem_lane
dispatch_status: routable
lane_status: active_minimal
execution_performed: false
receipt_requirement: required
```

Process, network_http, database, repository_git, model_provider, observation
and review lanes are routable but planned:

```text
carrier_family: process
lane: process_lane
dispatch_status: routable
lane_status: planned
execution_performed: false
receipt_requirement: required
```

Unknown families remain unsupported:

```text
carrier_family: unknown
lane: unknown_lane
dispatch_status: not_supported
execution_performed: false
```

SPINE.33B does not implement a queue, worker, process signal, host probe,
network request, database call, git mutation, model invocation or receipt
hardening.

SPINE.33C builds on this route plan with `carrier.v1`. Filesystem remains the
only active-minimal lane and becomes the first carrier contract adapter.

SPINE.33D activates process_lane minimally for process observation, dry-run
signal planning and test-owned signal control. It remains guarded by the no
arbitrary kill rule.
