# Carrier Coverage Matrix

Status: active
Owner: control/carrier substrate
Purpose: Define carrier coverage matrix and mode taxonomy.
Not source of truth for: adapter execution, receipt hardening or provider runtime

SPINE.33F makes every relevant effect-capable carrier family visible. The
carrier coverage matrix is a visibility and planning surface with no fake
execution.

Doctrine phrase: no fake execution.

Core rule:

```text
family + mode + adapter status + outcomes + receipt requirement -> coverage
```

Carrier modes:

```text
controlled
observed
imported
```

Mode is not outcome, carrier family or dispatch status.

Carrier outcome postures tracked by the matrix:

```text
executed
blocked
deferred
failed
mismatch
observed
quarantined
waiting_operator
waiting_agent
not_attempted
```

Outcome is not mode, gate outcome or dispatch status.

Adapter statuses:

```text
active_minimal
skeleton
planned
unsupported
deferred
```

Carrier families:

```text
filesystem
process
network_http
database
repository_git
service
endpoint
socket
listener
model_provider
observation
review
unknown
```

Filesystem and process have active-minimal controlled paths. Process observed
mode is active-minimal through the host probe. Network, database, repository,
service, endpoint, socket, listener, model provider and review are visible
carrier.v1 skeleton surfaces, not executable carriers.

The command surface is:

```text
yai carrier coverage
yai carrier coverage --family <carrier_family>
yai carrier coverage --mode controlled|observed|imported
```

Skeleton carrier families must report:

```text
execution_available: false
receipt_required: yes
carrier_contract: carrier.v1
```

SPINE.33G adds `yai carrier inspect <family>` for the non-process skeleton
families and requires the coverage matrix to agree with skeleton status.

SPINE.33H adds `yai carrier outcome-test` as a dry-run harness for outcome
posture. It tests outcomes against the matrix but does not turn skeleton
coverage into executable carrier support.
