# endpoint

Owns endpoint descriptor vocabulary and future endpoint registry material.

Endpoint descriptor v1 is represented in `include/yai/net/endpoint.h`.
Fixtures live under `proto/fixtures/net/endpoint/`.

Endpoint descriptors can refer to node fixture ids and capability fixture ids.

An endpoint is not authority.

This area does not implement discovery, probing, routing or transport.

Current status: scaffold.

Next expected wave: NET.SPINE.6 health/readiness/liveness probe skeleton.
