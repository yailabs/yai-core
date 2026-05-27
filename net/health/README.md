# health

Owns health/readiness/liveness vocabulary and future health report material.

Health report v1 is represented in `include/yai/net/health.h`.
Fixtures live under `proto/fixtures/net/health/`.

Health reports can refer to node, endpoint and capability fixture ids.

Health is observation/declaration, not authority.

Health reports and lifecycle reports are separate; health describes
readiness/liveness, lifecycle describes service state.

This area does not implement live probing, routing, discovery or transport.

Current status: scaffold.

Next expected wave: NET.SPINE.7 local service lifecycle contract.
