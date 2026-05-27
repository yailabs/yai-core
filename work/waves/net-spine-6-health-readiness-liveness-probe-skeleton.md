# NET.SPINE.6 - Health / Readiness / Liveness Probe Skeleton

Status: completed

## Purpose

NET.SPINE.6 adds canonical health, readiness and liveness report vocabulary so
future NET waves can represent node, endpoint and capability status without
performing live probes.

This wave creates contract, fixture, schema and guard material only. It does not
call localhost, open sockets, perform HTTP requests, discover LAN nodes, route
invocations or run endpoint probes.

## Files Changed

- `include/yai/net/health.h`
- `proto/net.md`
- `proto/schemas/net-health-report.v1.schema.json`
- `proto/fixtures/net/health/*.json`
- `tools/checks/check-net-health-fixtures.py`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `Makefile`
- `net/README.md`
- `net/health/README.md`
- `net/node/README.md`
- `net/endpoint/README.md`
- `net/capability/README.md`
- `work/spines/net-spine.md`

## Health Report Fields

Health report v1 contains:

- `health_report_id`
- `subject_id`
- `subject_kind`
- `check_kind`
- `health_state`
- `observed_by_node_id`
- `reason`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `observed_at_unix_ms`

## Fixture Set

Health fixtures live under `proto/fixtures/net/health/`:

- `node-health.json`
- `endpoint-liveness.json`
- `endpoint-readiness.json`
- `capability-readiness.json`
- `degraded.json`
- `unavailable.json`

All fixtures are synthetic canonical examples. They do not contain real
hostnames, IP addresses, ports, filesystem paths, usernames, MAC addresses,
secrets, provider/model support claims or performance claims.

## Schema Location

Health report schema lives at:

```text
proto/schemas/net-health-report.v1.schema.json
```

## Validator And Guard Additions

`tools/checks/check-net-health-fixtures.py` validates health fixtures, required
fields, enum values, safe metadata, observed timestamps and reference
resolution.

`tools/checks/check-net-headers.sh` now checks health state, check kind and
subject kind symbols and prevents health header runtime declarations.

`tools/checks/check-net-boundary.sh` now checks health fixture presence, schema
presence, Health / Readiness / Liveness protocol vocabulary and NET.SPINE.6.0
spine version.

## Reference Relation

Health reports reference:

- node fixture ids from `proto/fixtures/net/node/`
- endpoint fixture ids from `proto/fixtures/net/endpoint/`
- capability fixture ids from `proto/fixtures/net/capability/`

Transport and external subject ids may remain unresolved in future fixtures,
but this wave uses already represented node, endpoint and capability ids.

The relation is static health report seed material. It is not a live probe,
health check runner, routing decision, trust proof or authority signal.

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
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no live probe implementation
- no network IO
- no HTTP request
- no socket use
- no endpoint probing
- no discovery
- no transport
- no routing
- no service lifecycle
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
