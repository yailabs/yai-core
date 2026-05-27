# NET.SPINE.3 - Node Identity Types + Local Machine Projection

## Purpose

Create the canonical NET node identity contract and safe local machine
projection surface, without probing the actual machine or implementing
discovery.

## Files Changed

- `include/yai/net/node.h`
- `proto/net.md`
- `proto/fixtures/net/node/*.json`
- `proto/schemas/net-node-identity.v1.schema.json`
- `tools/checks/check-net-node-fixtures.py`
- `tools/checks/check-net-boundary.sh`
- `tools/checks/check-net-headers.sh`
- `Makefile`
- `net/node/README.md`
- `net/README.md`
- `work/spines/net-spine.md`

## Node Identity Fields

`schema_version`, `node_id`, `node_label`, `node_kind`, `node_scope`,
`node_state`, `identity_source`, `safe_metadata`, `observed_at_unix_ms`,
`endpoint_refs` and `capability_refs`.

## Fixture Set

`local-process.json`, `localhost-service.json`, `lan-node.json`,
`remote-node.json` and `external-node.json`.

## Schema Location

`proto/schemas/net-node-identity.v1.schema.json`

Corrective note: NET.SPINE.3R moved protocol material from `work/protocols` to
`proto`.

## Guard Additions

- `tools/checks/check-net-node-fixtures.py` validates node fixture fields,
  allowed values, list shapes, safe metadata shape and private metadata hygiene.
- `tools/checks/check-net-headers.sh` now verifies public node identity symbols
  and blocks discovery/probing/network function declarations.
- `tools/checks/check-net-boundary.sh` now requires NET.SPINE.3.0, node
  fixtures, node schema and the Node Identity protocol section.

## Stream/Node Relation

Stream envelopes use `origin_node_id` and `target_node_id` as references to NET
node identities. NET.SPINE.3 does not add registry resolution.

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
```

## Non-Goals

- no LAN discovery
- no remote discovery
- no local machine probing
- no endpoint registry implementation
- no capability registry implementation
- no transport adapter
- no router
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
