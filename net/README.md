# NET

NET is the integrated YAI runtime communication module for local, localhost,
LAN and remote capability nodes.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

CLORI extension:
YAI controls authority.
NET moves streams.
CLORI executes neural computation.

## Status

Current status: scaffold.

NET.SPINE.1 establishes canonical terms, public header discipline and module
area README discipline. NET.SPINE.2 starts stream envelope contract material.
NET.SPINE.3 starts node identity contract material. Discovery, transport,
routing, server behavior, CLI behavior and CLORI code are not implemented.
NET.SPINE.4 starts capability advertisement contract material.
NET.SPINE.5 starts endpoint descriptor contract material.
NET.SPINE.6 starts health/readiness/liveness contract material.
NET.SPINE.7 starts local service lifecycle contract material.
NET protocol vocabulary and fixtures live under `proto/`.

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
lifecycle/   future local service lifecycle contract material
transport/   future transport adapters
route/       future routing and selection
observe/     future metrics, trace and receipt observation
compat/      future external node compatibility
```

## Validation

```sh
make net-info
make check-net-headers
make check-net-stream-fixtures
make check-net-node-fixtures
make check-net-capability-fixtures
make check-net-endpoint-fixtures
make check-net-health-fixtures
make check-net-lifecycle-fixtures
make check-net-boundary
```

## Non-Goals

NET.SPINE.7 does not implement LAN discovery, remote transport, local socket
transport, HTTP behavior, routing behavior, server behavior, CLI behavior or
CLORI support. It keeps NET as an integrated module, not a standalone
repository.
