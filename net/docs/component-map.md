# NET Component Map

```text
include/yai/net/  public vocabulary and boundary headers
src/core/         future shared component primitives
src/registry/     future endpoint and node registries
src/stream/       future stream runtime
src/node/         future node identity implementation
src/capability/   future capability registry
src/endpoint/     future endpoint registry
src/health/       future probes
src/transport/    future transport adapters
src/route/        future routing and selection
src/observe/      future metrics and tracing
src/compat/       future external node compatibility
tests/            future component tests
fixtures/         future schemas and example payloads
benches/          future transport benchmark harness
examples/         future operator examples
```

NET.SPINE.0 keeps `src/` README-only. Future waves add code where the component
map already provides stable roots.
