Historical/superseded engineering record. Not an active source of truth.

# SPINE.33L Provider Runtime / LAN Target Surface v0

Status: completed

SPINE.33L added the first implementative provider runtime surface.

Implemented:

```text
runtime target vocabulary: local, lan, external
device registry v0 at YAI_HOME/config/devices.jsonl
yai device list/add/inspect/trust/untrust
yai provider runtime status
yai provider targets
yai provider start --dry-run
yai provider logs-path
yai model catalog status
yai model runtime status
smoke-spine33l
```

Non-goals preserved:

```text
No actual provider process start.
No actual remote LAN execution.
No mDNS/zeroconf discovery.
No SSH integration.
No remote yaid protocol implementation.
No GPU probing.
No CUDA/Metal detection.
No model scanning.
No DS4 integration.
No HNSW implementation.
No decoding acceleration implementation.
No case model attach implementation.
No model carrier changes.
No interfaces/studio changes.
No yai-dev mutation.
```
