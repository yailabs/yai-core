# NET.SPINE.4 - Capability Advertisement Types + Registry Seed

## Purpose

Create the canonical NET capability advertisement contract without implementing
a live capability registry, routing, discovery or policy resolution.

## Files Changed

- `include/yai/net/capability.h`
- `proto/net.md`
- `proto/fixtures/net/capability/*.json`
- `proto/schemas/net-capability-advertisement.v1.schema.json`
- `tools/checks/check-net-capability-fixtures.py`
- `tools/checks/check-net-node-fixtures.py`
- `tools/checks/check-net-boundary.sh`
- `tools/checks/check-net-headers.sh`
- `Makefile`
- `net/capability/README.md`
- `net/node/README.md`
- `net/README.md`
- `work/spines/net-spine.md`

## Capability Advertisement Fields

`schema_version`, `capability_id`, `node_id`, `capability_label`,
`capability_kind`, `capability_family`, `capability_scope`,
`capability_state`, `constraints`, `metadata` and `advertised_at_unix_ms`.

## Fixture Set

`neural-llm-decode.json`, `neural-embedding-encode.json`,
`metrics-stream.json`, `receipt-emit.json` and `generic-external.json`.

## Schema Location

`proto/schemas/net-capability-advertisement.v1.schema.json`

## Guard Additions

- `tools/checks/check-net-capability-fixtures.py` validates capability fixture
  shape, allowed values, fixture hygiene and forbidden implementation claims.
- `tools/checks/check-net-node-fixtures.py` now checks node `capability_refs`
  against capability advertisement fixture ids when those fixtures exist.
- `tools/checks/check-net-headers.sh` now verifies public capability
  advertisement symbols and blocks registry/discovery/routing/policy/runtime
  function declarations.
- `tools/checks/check-net-boundary.sh` now requires NET.SPINE.4.0, capability
  fixtures, capability schema and the Capability Advertisement protocol section.

## Node/Capability Reference Relation

Node fixture `capability_refs` resolve to capability advertisement fixture ids.
This is static fixture validation only, not live registry resolution.

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
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no live capability registry
- no discovery
- no endpoint registry
- no health probing
- no transport
- no router
- no policy resolution
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
