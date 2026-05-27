# transport

Owns transport adapter vocabulary and future stream movement material.

Transport adapter v1 is represented in `include/yai/net/transport.h`.
Fixtures live under `proto/fixtures/net/transport/`.

Transport descriptors refer to endpoint descriptors.

Transport adapters may later move stream envelopes.

This area does not implement network IO, HTTP, probing, routing or lifecycle
behavior.

Current status: scaffold.

Next expected wave: NET.SPINE.9 local IPC / Unix socket adapter skeleton.
