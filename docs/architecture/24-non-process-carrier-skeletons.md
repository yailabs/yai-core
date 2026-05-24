# Non-Process Carrier Skeletons

Status: active
Owner: control/carrier substrate
Purpose: Define SPINE.33G inspectable carrier.v1 skeletons.
Not source of truth for: real network, database, git, service, endpoint, socket, listener, model-provider or review execution

SPINE.33G turns planned non-filesystem/non-process carrier families into
concrete skeleton carrier surfaces. A skeleton carrier is inspectable,
receipt-aware and covered by `carrier.v1`, but it does not execute effects.

Core rule:

```text
planned carrier family -> inspectable skeleton -> no execution until activation wave
```

Required skeleton families:

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

Every skeleton must report:

```text
contract: carrier.v1
status: skeleton
execution_available: false
receipt_required: yes
carrier_attempted: false
```

`model_provider` is visible as an imported skeleton with planned controlled and
observed modes. `review` is visible as an imported skeleton with unsupported
controlled and observed modes.

This preserves carrier visibility without fake execution.
