# stream

Owns stream event/envelope vocabulary and future stream runtime surfaces.

Stream envelope v1 is represented in `include/yai/net/stream.h`. Fixtures live
under `proto/fixtures/net/stream/`.

Stream envelopes may later be moved through transport adapters, but NET.SPINE.9C
does not implement live streaming.

`net/stream/stream.c` provides pure event-kind name and validity helpers only.

Must not implement live transport, approve invocations, decide policy or make
receipts authoritative.

Current status: C translation unit scaffold started.

Next expected wave: NET.SPINE.13 streaming runtime skeleton.
