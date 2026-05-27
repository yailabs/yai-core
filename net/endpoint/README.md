# endpoint

Owns endpoint descriptor vocabulary and future endpoint registry material.

Endpoint descriptor v1 is represented in `include/yai/net/endpoint.h`.
Fixtures live under `proto/fixtures/net/endpoint/`.

Endpoint descriptors can refer to node fixture ids and capability fixture ids.

Endpoint fixture ids may be referenced by health reports.

Endpoint descriptors may be lifecycle subjects, but endpoint lifecycle is not
probed here.

Endpoint fixture ids may be referenced by transport adapter descriptors.

Local IPC endpoint descriptors may be referenced by IPC channel descriptors.

An endpoint is not authority.

This area does not implement discovery, probing, routing or transport.

Current status: scaffold.

Next expected wave: NET.SPINE.6 health/readiness/liveness probe skeleton.
