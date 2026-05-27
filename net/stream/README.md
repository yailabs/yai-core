# stream

Owns stream event/envelope vocabulary and future stream runtime surfaces.

Stream envelope v1 is represented in `include/yai/net/stream.h`. Fixtures live
under `proto/fixtures/net/stream/`.

Must not implement live transport, approve invocations, decide policy or make
receipts authoritative.

Current status: scaffold.

Next expected wave: NET.SPINE.13 streaming runtime skeleton.
