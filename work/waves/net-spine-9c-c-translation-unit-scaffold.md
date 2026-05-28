# NET.SPINE.9C - C Translation Unit Scaffold

Status: implemented

## Why This Wave Exists

NET had contracts, public headers, proto fixtures, schemas, validators and
guards, but the integrated `net/` module still had no compiled C translation
units. That was correct while the boundary was forming, but it risked leaving
NET as a ritual folder rather than an integrated C module.

NET.SPINE.9C adds pure C implementation files for stable vocabulary helpers.
It does not implement network behavior.

## Files Changed

- `include/yai/net/version.h`
- `include/yai/net/stream.h`
- `include/yai/net/node.h`
- `include/yai/net/capability.h`
- `include/yai/net/endpoint.h`
- `include/yai/net/health.h`
- `include/yai/net/lifecycle.h`
- `include/yai/net/transport.h`
- `Makefile`
- `tools/checks/check-net-c-sources.sh`
- `tools/checks/check-net-boundary.sh`
- `tools/checks/check-net-headers.sh`
- `tests/net/test_net_enum_names.c`
- `net/README.md`
- `net/core/README.md`
- `net/stream/README.md`
- `net/node/README.md`
- `net/capability/README.md`
- `net/endpoint/README.md`
- `net/health/README.md`
- `net/lifecycle/README.md`
- `net/transport/README.md`
- `work/spines/net-spine.md`

## C Files Added

- `net/core/net.c`
- `net/stream/stream.c`
- `net/node/node.c`
- `net/capability/capability.c`
- `net/endpoint/endpoint.c`
- `net/health/health.c`
- `net/lifecycle/lifecycle.c`
- `net/transport/transport.c`

No `net/src` tree was created.

## Helper Functions Added

- `yai_net_component_name`
- `yai_net_component_status`
- `yai_net_component_version`
- `yai_net_stream_event_kind_name`
- `yai_net_stream_event_kind_is_valid`
- `yai_net_node_kind_name`
- `yai_net_node_kind_is_valid`
- `yai_net_capability_kind_name`
- `yai_net_capability_kind_is_valid`
- `yai_net_endpoint_kind_name`
- `yai_net_endpoint_kind_is_valid`
- `yai_net_health_state_name`
- `yai_net_health_state_is_valid`
- `yai_net_lifecycle_state_name`
- `yai_net_lifecycle_state_is_valid`
- `yai_net_transport_kind_name`
- `yai_net_transport_kind_is_valid`

All helpers return stable literals or simple validity integers. Unknown enum
values return `unknown` and invalid validity status.

## Build And Check Integration

`Makefile` now includes NET C sources in the standard C library build and adds:

```sh
make build-net-c
make check-net-c-sources
```

`build-net-c` compiles and runs the minimal enum helper test.

## Guard Additions

`tools/checks/check-net-c-sources.sh` verifies integrated NET C files exist,
`net/src` remains absent, repo-shaped NET islands remain absent and NET C
sources contain no forbidden network, process, discovery or runtime behavior.

`check-net-boundary.sh` now requires NET.SPINE.9C.0 and the core C source
surface. `check-net-headers.sh` now requires the pure helper declarations.

## Validation Commands

```sh
git diff --check
make info
make net-info
tools/checks/check-net-headers.sh
make check-net-headers
tools/checks/check-net-c-sources.sh
make check-net-c-sources
make build-net-c
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
test ! -e net/src
```

Existing NET fixture validators remain part of the validation surface.

## Non-Goals

- No LAN discovery.
- No socket use.
- No HTTP.
- No IPC implementation.
- No live stream movement.
- No routing.
- No registry mutation.
- No service lifecycle execution.
- No CLI behavior.
- No CLORI code.
- No `system/` extraction.
- No `engine/` extraction.
