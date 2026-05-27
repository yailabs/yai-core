# NET.SPINE.2 - Stream Envelope Types + Fixtures

## Purpose

Create the first concrete NET data contract: stream envelope vocabulary, C
header types, protocol documentation, JSON fixtures, schema and static
validation.

## Files Changed

- `include/yai/net/stream.h`
- `proto/net.md`
- `proto/fixtures/net/stream/*.json`
- `proto/schemas/net-stream-envelope.v1.schema.json`
- `tools/checks/check-net-stream-fixtures.py`
- `tools/checks/check-net-boundary.sh`
- `tools/checks/check-net-headers.sh`
- `Makefile`
- `net/stream/README.md`
- `net/README.md`
- `work/spines/net-spine.md`

## Stream Envelope Fields

`schema_version`, `stream_id`, `invocation_id`, `trace_id`, `sequence`,
`event_kind`, `stream_state`, `payload_kind`, `origin_node_id`,
`target_node_id`, `timestamp_unix_ms` and `payload`.

## Fixture Set

`request.json`, `response.json`, `chunk.json`, `metric.json`, `receipt.json`,
`error.json` and `complete.json`.

## Schema Location

`proto/schemas/net-stream-envelope.v1.schema.json`

Corrective note: NET.SPINE.3R moved protocol material from `work/protocols` to
`proto`.

## Guard Additions

- `tools/checks/check-net-stream-fixtures.py` validates stream fixture fields,
  allowed values, ordering/timestamp integer shape, filename/event alignment and
  fixture hygiene.
- `tools/checks/check-net-headers.sh` now verifies stream envelope header
  symbols and blocks stream runtime function declarations.
- `tools/checks/check-net-boundary.sh` now requires NET.SPINE.2.0, stream
  fixtures and the stream envelope schema.

## Validation Commands

```sh
git diff --check
make info
make net-info
tools/checks/check-net-headers.sh
make check-net-headers
python3 tools/checks/check-net-stream-fixtures.py
make check-net-stream-fixtures
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
```

## Non-Goals

- no live streaming
- no transport adapter
- no network IO
- no router
- no discovery
- no CLORI code
- no CLI behavior
- no system/ extraction
- no engine/ extraction
