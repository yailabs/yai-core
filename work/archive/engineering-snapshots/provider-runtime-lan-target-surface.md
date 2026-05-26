# Provider Runtime / LAN Target Surface

Status: SPINE.33L.

Purpose: track the provider runtime / LAN target surface v0 implementation.

## Summary

SPINE.33L introduces an inspectable provider-runtime surface without starting
providers. It adds device registry commands, runtime target status, provider
start dry-run plans, provider log paths, model catalog posture and model
runtime status.

The normal future workflow should not require the operator to manually launch a
provider process. Manual provider launch remains a development fallback.

## Runtime Layout

```text
YAI_HOME/config/devices.jsonl
YAI_HOME/run/providers/
YAI_HOME/log/providers/
YAI_HOME/tmp/provider-plans/
```

## Commands

```bash
yai device list
yai device add --id workstation --name Workstation --host 192.168.1.50 --port 8777 --target lan
yai device inspect workstation
yai device trust workstation
yai device untrust workstation

yai provider runtime status
yai provider targets
yai provider start --dry-run --target local --kind llama_server --model qwen3-8b-q4
yai provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash
yai provider start --dry-run --target external --endpoint http://127.0.0.1:43117/v1/chat/completions --kind openai_compatible
yai provider logs-path

yai model catalog status
yai model runtime status
```

## Device Registry

Device registry v0 stores JSONL entries with:

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

The registry is declarative. SPINE.33L does not perform LAN discovery,
mDNS/zeroconf, remote authentication or supervisor probes.

## Provider Dry Run

Provider start plans are dry-run only:

```text
execution_performed: false
receipt_required: yes
```

LAN plans check registry posture. An untrusted LAN device blocks with:

```text
status: blocked
reason: device_not_trusted
```

A trusted LAN device produces a planned start view but still does not contact
the device or start a provider.

## Model Runtime Status

`yai model runtime status` exposes planned posture for:

```text
model_routing
context_compiler
retrieval_hnsw
decoding_acceleration
normal_decoding
speculative_draft
native_mtp
fallback
```

This is status visibility. It is not HNSW, MTP, model runner or provider
implementation.

## Validation

```bash
make check-provider-runtime-lan-target-surface
make smoke-spine33l
```
