# NET.SPINE.8 - Localhost Transport Adapter Skeleton

Status: completed

## Purpose

NET.SPINE.8 adds canonical localhost transport adapter descriptor vocabulary so
future NET waves can describe how stream envelopes may move through local,
localhost, LAN, remote or future transport surfaces.

This wave creates contract, fixture, schema and guard material only. It does not
open sockets, call HTTP, probe endpoints, move stream data, route invocations or
perform service lifecycle actions.

## Files Changed

- `include/yai/net/transport.h`
- `include/yai/net/net.h`
- `proto/net.md`
- `proto/schemas/net-transport-adapter.v1.schema.json`
- `proto/fixtures/net/transport/*.json`
- `tools/checks/check-net-transport-fixtures.py`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `Makefile`
- `net/README.md`
- `net/transport/README.md`
- `net/endpoint/README.md`
- `net/stream/README.md`
- `work/spines/net-spine.md`

## Transport Adapter Descriptor Fields

Transport adapter descriptor v1 contains:

- `transport_adapter_id`
- `endpoint_id`
- `transport_label`
- `transport_kind`
- `transport_scope`
- `transport_state`
- `transport_security`
- `stream_profile`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `declared_at_unix_ms`

## Fixture Set

Transport fixtures live under `proto/fixtures/net/transport/`:

- `localhost-http.json`
- `local-ipc.json`
- `lan-http.json`
- `remote-http.json`
- `future-transport.json`

All fixtures are synthetic canonical examples. They do not contain real
hostnames, IP addresses, ports, filesystem paths, usernames, MAC addresses,
secrets, provider/model support claims or performance claims.

## Schema Location

Transport adapter schema lives at:

```text
proto/schemas/net-transport-adapter.v1.schema.json
```

## Validator And Guard Additions

`tools/checks/check-net-transport-fixtures.py` validates transport fixtures,
required fields, enum values, safe metadata, declared timestamps and endpoint
reference resolution.

`tools/checks/check-net-headers.sh` now requires `include/yai/net/transport.h`,
checks umbrella inclusion, validates transport symbols and prevents transport
header runtime declarations.

`tools/checks/check-net-boundary.sh` now checks transport fixture presence,
schema presence, Transport Adapter protocol vocabulary, NET.SPINE.8.0 spine
version and network/process implementation signals in active NET surfaces.

## Relation To Endpoints And Streams

Transport adapter descriptors reference endpoint fixture ids from
`proto/fixtures/net/endpoint/`.

Stream envelopes may later be moved through transport adapters. This wave only
declares the adapter vocabulary and fixture shape; it does not implement live
stream movement.

## Validation Commands

```sh
git diff --check
make info
make net-info
tools/checks/check-net-headers.sh
make check-net-headers
python3 tools/checks/check-net-stream-fixtures.py
make check-net-stream-fixtures
python3 tools/checks/check-net-node-fixtures.py
make check-net-node-fixtures
python3 tools/checks/check-net-capability-fixtures.py
make check-net-capability-fixtures
python3 tools/checks/check-net-endpoint-fixtures.py
make check-net-endpoint-fixtures
python3 tools/checks/check-net-health-fixtures.py
make check-net-health-fixtures
python3 tools/checks/check-net-lifecycle-fixtures.py
make check-net-lifecycle-fixtures
python3 tools/checks/check-net-transport-fixtures.py
make check-net-transport-fixtures
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
test ! -e work/protocols
make check-docs
```

## Known Unrelated Check-Docs Failure

At the start of this wave, `make check-docs` was expected to fail on unrelated
SPINE.46 fact-plane material if `work/waves/spine46-duckdb-fact-plane-bitemporal-schema.md`
was still absent. That baseline issue is not part of NET.SPINE.8.

## Non-Goals

- no localhost HTTP implementation
- no socket use
- no HTTP request
- no endpoint probing
- no discovery
- no routing
- no live stream movement
- no service lifecycle execution
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
