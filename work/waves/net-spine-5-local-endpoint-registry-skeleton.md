# NET.SPINE.5 - Local Endpoint Registry Skeleton

Status: completed

## Purpose

NET.SPINE.5 adds canonical endpoint representation so future NET waves can
connect node identities and capability advertisements to declared communication
surfaces.

This wave creates endpoint descriptor contract material only. It does not
implement network IO, discovery, routing, probing, health checks or service
lifecycle behavior.

## Files Changed

- `include/yai/net/endpoint.h`
- `proto/net.md`
- `proto/schemas/net-endpoint-descriptor.v1.schema.json`
- `proto/fixtures/net/endpoint/*.json`
- `tools/checks/check-net-endpoint-fixtures.py`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `Makefile`
- `net/README.md`
- `net/endpoint/README.md`
- `net/node/README.md`
- `net/capability/README.md`
- `work/spines/net-spine.md`

## Endpoint Descriptor Fields

Endpoint descriptor v1 contains:

- `endpoint_id`
- `node_id`
- `endpoint_label`
- `endpoint_kind`
- `endpoint_scope`
- `endpoint_state`
- `endpoint_security`
- `address_ref`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `declared_at_unix_ms`
- `capability_refs` in JSON fixtures

## Fixture Set

Endpoint fixtures live under `proto/fixtures/net/endpoint/`:

- `local-process.json`
- `localhost-http.json`
- `local-ipc.json`
- `lan-http.json`
- `remote-http.json`
- `future-transport.json`

All fixture address references are fake and abstract. Canonical fixtures do not
contain real hostnames, IP addresses, ports, filesystem paths, usernames, MAC
addresses, secrets, model support claims or benchmark claims.

## Schema Location

Endpoint descriptor schema lives at:

```text
proto/schemas/net-endpoint-descriptor.v1.schema.json
```

## Validator And Guard Additions

`tools/checks/check-net-endpoint-fixtures.py` validates endpoint fixtures,
required fields, enum values, safe metadata, declared timestamps and reference
resolution.

`tools/checks/check-net-headers.sh` now checks endpoint kind, scope, state and
security symbols and prevents endpoint header runtime declarations.

`tools/checks/check-net-boundary.sh` now checks endpoint fixture presence,
schema presence, Endpoint Descriptor protocol vocabulary and NET.SPINE.5.0
spine version.

## Reference Relation

Endpoint fixtures reference node fixture ids from `proto/fixtures/net/node/` and
capability fixture ids from `proto/fixtures/net/capability/`.

The relation is static registry seed material. It is not a live endpoint
registry, liveness proof, routing decision or authority signal.

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
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no live endpoint registry
- no discovery
- no probing
- no health checks
- no transport
- no routing
- no sockets
- no HTTP implementation
- no service lifecycle
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
