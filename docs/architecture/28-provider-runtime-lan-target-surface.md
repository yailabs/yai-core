# Provider Runtime / LAN Target Surface

Status: SPINE.33L.

Purpose: define the first inspectable provider-runtime surface for runtime
targets, device registry, dry-run provider start plans, model catalog posture
and model runtime status.

## Runtime Targets

SPINE.33L defines three runtime target modes:

```text
local
lan
external
```

`local` means the provider process would run on the same machine as the local
`yai` / `yaid` runtime.

`lan` means the provider process would run on a paired LAN device with a future
YAI runtime agent or `yaid`-compatible provider supervisor.

`external` means YAI attaches to an existing endpoint and does not supervise the
process.

LAN device is not a provider. LAN device is a runtime target.

Canonical chain:

```text
device
-> provider supervisor
-> provider instance
-> model attachment
-> case
```

## Device Registry

The v0 registry lives at:

```text
YAI_HOME/config/devices.jsonl
```

Each line records:

```text
device_id
name
target_mode
host
port
pairing_status
trust_status
os
arch
gpu_backend
provider_supervisor_status
notes
```

SPINE.33L does not probe devices. `provider_supervisor_status` is recorded as
declared posture, usually `not_checked`.

## Provider Start Plans

`yai provider start --dry-run` creates a plan view only. It must not start a
provider process, contact a LAN device, scan models or invoke a model.

The provider start dry-run surface is the only start posture in SPINE.33L.

LAN plans require a paired device and trusted device. Untrusted LAN devices are
blocked with:

```text
reason: device_not_trusted
execution_performed: false
```

External targets are attach-only:

```text
action: attach_only
execution_performed: false
```

## Runtime Status

Provider logs are path-visible through `yai provider logs-path`.

SPINE.33L exposes status for model routing, context compiler, retrieval/HNSW
and decoding acceleration so these concepts are visible from CLI before
implementation:

```text
model_routing: planned
context_compiler: planned
retrieval_hnsw: planned
decoding_acceleration: planned
normal_decoding: baseline
native_mtp: opportunistic
fallback: normal_decoding
```

## Non-Execution Boundary

No remote start in SPINE.33L. No provider process is started. Manual provider
launch remains a development fallback, but no manual provider launch as normal
workflow is the future doctrine.

Canonical guard phrase: no remote start in SPINE.33L.
Canonical guard phrase: no manual provider launch as normal workflow.
