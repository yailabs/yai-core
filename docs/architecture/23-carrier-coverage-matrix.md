# Carrier Coverage Matrix

Status: active
Owner: control/carrier substrate
Purpose: Define SPINE.33F carrier coverage matrix and mode taxonomy.
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
service, endpoint, socket, listener and model provider are visible skeletons or
planned surfaces, not executable carriers.

The command surface is:

```text
yai carrier coverage
yai carrier coverage --family <carrier_family>
yai carrier coverage --mode controlled|observed|imported
```

Skeleton carrier families must report:

```text
execution_available: false
```
