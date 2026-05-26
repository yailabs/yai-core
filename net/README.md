# NET

NET is the integrated YAI runtime communication module for local, localhost,
LAN and remote capability nodes.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## Status

NET is an integrated scaffold. Discovery is not implemented. Transport is not
implemented. Routing is not implemented. External nodes are planned.

NET is not a standalone repository. It has no local Makefile, docs tree, test
tree, fixture tree, benchmark tree or examples tree. Public headers live under
`include/yai/net/`. Internal module areas live directly under `net/`.

## Module Layout

```text
core/        future shared NET primitives
registry/    future node and endpoint registries
stream/      future stream runtime
node/        future node identity implementation
capability/  future capability registry
endpoint/    future endpoint registry
health/      future health/readiness/liveness probes
transport/   future transport adapters
route/       future routing and selection
observe/     future metrics, trace and receipt observation
compat/      future external node compatibility
```

## Validation

```sh
make net-info
make check-net-boundary
```

## Non-Goals

NET.SPINE.0R does not implement LAN discovery, remote transport, local socket
transport, HTTP behavior, routing behavior, server behavior, CLI behavior or
CLORI support. It realigns the scaffold to YAI's integrated module topology.
