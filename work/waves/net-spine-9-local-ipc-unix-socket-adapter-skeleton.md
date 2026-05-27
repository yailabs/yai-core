# NET.SPINE.9 - Local IPC / Unix Socket Adapter Skeleton

Status: completed

## Purpose

NET.SPINE.9 adds canonical local IPC channel descriptor vocabulary so future
NET waves can represent local process communication surfaces without
implementing Unix sockets, named pipes, platform IPC, filesystem socket paths,
process control or live transport.

This wave creates contract, fixture, schema and guard material only. It does
not create IPC channels, open sockets, expose real paths, probe endpoints, move
stream data, route invocations or perform lifecycle actions.

## Files Changed

- `include/yai/net/transport.h`
- `proto/net.md`
- `proto/schemas/net-local-ipc-channel.v1.schema.json`
- `proto/fixtures/net/ipc/*.json`
- `tools/checks/check-net-ipc-fixtures.py`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `Makefile`
- `net/README.md`
- `net/transport/README.md`
- `net/endpoint/README.md`
- `work/spines/net-spine.md`

## IPC Channel Descriptor Fields

IPC channel descriptor v1 contains:

- `ipc_channel_id`
- `transport_adapter_id`
- `endpoint_id`
- `channel_label`
- `channel_kind`
- `channel_scope`
- `channel_state`
- `channel_security`
- `channel_ref`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `declared_at_unix_ms`

## Fixture Set

IPC fixtures live under `proto/fixtures/net/ipc/`:

- `abstract-local.json`
- `unix-socket-future.json`
- `named-pipe-future.json`
- `platform-ipc-future.json`
- `memory-channel-future.json`

All fixtures use abstract channel references. They do not contain real
hostnames, IP addresses, ports, filesystem paths, usernames, MAC addresses,
process ids, secrets, provider/model support claims or performance claims.

## Schema Location

IPC channel schema lives at:

```text
proto/schemas/net-local-ipc-channel.v1.schema.json
```

## Validator And Guard Additions

`tools/checks/check-net-ipc-fixtures.py` validates IPC fixtures, required
fields, enum values, safe metadata, declared timestamps, endpoint references
and transport adapter references.

`tools/checks/check-net-headers.sh` now checks IPC channel kind, scope, state,
security and descriptor symbols in `include/yai/net/transport.h`.

`tools/checks/check-net-boundary.sh` now checks IPC fixture presence, schema
presence, Local IPC Channel protocol vocabulary, NET.SPINE.9.0 spine version
and IPC/network implementation signals in active NET surfaces.

## Relation To Transport Adapters And Endpoints

IPC channel descriptors reference transport adapter fixture ids from
`proto/fixtures/net/transport/` and endpoint fixture ids from
`proto/fixtures/net/endpoint/`.

The relation is static descriptor material. It is not a live IPC channel,
filesystem socket path, connection, probe, route or stream movement.

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
python3 tools/checks/check-net-ipc-fixtures.py
make check-net-ipc-fixtures
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no Unix socket implementation
- no socket file paths
- no filesystem IPC paths
- no named pipe implementation
- no platform IPC implementation
- no network IO
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
