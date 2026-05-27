# NET.SPINE.3R - Proto Topology Realignment

## Purpose

Correct NET protocol material placement by moving canonical protocol vocabulary,
fixtures and schemas out of `work/` and into `proto/`.

## Why `work/protocols` Was Wrong

`work/` is for repository work material: spines, waves, inventories, status,
archive and internal planning. Canonical protocol contracts do not belong there.

## Why `proto` Is Canonical

`proto/` is the canonical YAI protocol surface for protocol vocabulary, schemas,
fixtures and protocol contracts.

## Moved Paths

Corrective note: NET.SPINE.3R moved protocol material from `work/protocols` to
`proto`.

- `work/protocols/net.md` to `proto/net.md`
- `work/protocols/fixtures/net/stream/` to `proto/fixtures/net/stream/`
- `work/protocols/fixtures/net/node/` to `proto/fixtures/net/node/`
- `work/protocols/schemas/net-stream-envelope.v1.schema.json` to `proto/schemas/net-stream-envelope.v1.schema.json`
- `work/protocols/schemas/net-node-identity.v1.schema.json` to `proto/schemas/net-node-identity.v1.schema.json`

## Deleted Paths

- `work/protocols/`

## Guard/Validator Updates

- NET stream fixture validator now reads `proto/fixtures/net/stream/`.
- NET node fixture validator now reads `proto/fixtures/net/node/`.
- NET boundary guard now requires NET protocol material under `proto/`, forbids
  `work/protocols` and rejects active old path references.

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
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no stream contract changes
- no node identity contract changes
- no discovery
- no transport
- no sockets
- no server
- no CLI behavior
- no CLORI code
- no YAI runtime behavior change
