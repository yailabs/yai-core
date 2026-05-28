# lifecycle

Owns lifecycle request/report vocabulary and future service lifecycle material.

Lifecycle request/report v1 is represented in `include/yai/net/lifecycle.h`.
Fixtures live under `proto/fixtures/net/lifecycle/`.

Lifecycle requests are intent, not execution.

Lifecycle reports are state material, not authority.

This area does not implement process management, supervision, probing, routing
or transport.

`net/lifecycle/lifecycle.c` provides pure lifecycle-state name and validity
helpers only.

Current status: C translation unit scaffold started.

Next expected wave: NET.SPINE.8 localhost transport adapter skeleton.
