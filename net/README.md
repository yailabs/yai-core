# NET

NET is the YAI runtime communication substrate for local, localhost, LAN and
remote capability nodes.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## Status

NET is a root component scaffold. Discovery is not implemented. Transport is
not implemented. Routing is not implemented. External nodes are planned.

## What NET Is

NET is the YAI-side substrate for node identity, endpoint registries, stream
envelopes, transport adapters, health/readiness/liveness vocabulary,
capability advertisement, routing boundaries, invocation transport,
trace/receipt transport and transport metrics.

## What NET Is Not

NET is not the YAI authority system.
NET is not an inference engine.
NET is not a provider implementation.
NET is not `interfaces/transports`.

## Component Layout

```text
docs/      component architecture, boundaries and extraction plan
include/   public NET vocabulary headers
src/       future implementation roots, README-only in NET.SPINE.0
tests/     future tests
fixtures/  future stream, node and registry fixtures
benches/   future transport benchmark harness
examples/  future operator and integration examples
```

## Validation

```sh
make -C net info
make -C net check
```

Root-level validation is exposed as:

```sh
make net-info
make check-net-boundary
```

## Non-Goals

NET.SPINE.0 does not implement LAN discovery, remote transport, local socket
transport, HTTP behavior, routing behavior, server behavior or CLORI support.
It mounts the component boundary, vocabulary headers, documentation, checks and
future extension points.
