# NET.SPINE.7 - Local Service Lifecycle Contract

Status: completed

## Purpose

NET.SPINE.7 adds canonical local service lifecycle request and report
vocabulary so future NET waves can represent start, stop, restart and status
material without performing process control.

This wave creates contract, fixture, schema and guard material only. It does not
spawn processes, supervise services, check live status, perform network IO,
route invocations or control CLORI.

## Files Changed

- `include/yai/net/lifecycle.h`
- `include/yai/net/net.h`
- `proto/net.md`
- `proto/schemas/net-lifecycle-request.v1.schema.json`
- `proto/schemas/net-lifecycle-report.v1.schema.json`
- `proto/fixtures/net/lifecycle/*.json`
- `tools/checks/check-net-lifecycle-fixtures.py`
- `tools/checks/check-net-headers.sh`
- `tools/checks/check-net-boundary.sh`
- `Makefile`
- `net/README.md`
- `net/lifecycle/README.md`
- `net/health/README.md`
- `net/endpoint/README.md`
- `work/spines/net-spine.md`

## Lifecycle Request And Report Fields

Lifecycle request v1 contains:

- `lifecycle_request_id`
- `subject_id`
- `subject_kind`
- `requested_action`
- `requested_by_node_id`
- `reason`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `requested_at_unix_ms`

Lifecycle report v1 contains:

- `lifecycle_report_id`
- `subject_id`
- `subject_kind`
- `lifecycle_state`
- `observed_by_node_id`
- `reason`
- `metadata_json` in the public C descriptor
- `metadata` in JSON fixtures
- `observed_at_unix_ms`

## Fixture Set

Lifecycle fixtures live under `proto/fixtures/net/lifecycle/`:

- `request-start.json`
- `request-stop.json`
- `request-status.json`
- `report-declared.json`
- `report-running.json`
- `report-stopped.json`
- `report-failed.json`
- `report-retired.json`

All fixtures are synthetic canonical examples. They do not contain real
hostnames, IP addresses, ports, filesystem paths, usernames, MAC addresses,
secrets, process ids, real service names, provider/model support claims or
performance claims.

## Schema Locations

Lifecycle schemas live at:

```text
proto/schemas/net-lifecycle-request.v1.schema.json
proto/schemas/net-lifecycle-report.v1.schema.json
```

## Validator And Guard Additions

`tools/checks/check-net-lifecycle-fixtures.py` validates lifecycle fixtures,
required fields, enum values, safe metadata, timestamps and reference
resolution.

`tools/checks/check-net-headers.sh` now requires `include/yai/net/lifecycle.h`,
checks umbrella inclusion, validates lifecycle symbols and prevents lifecycle
header runtime declarations.

`tools/checks/check-net-boundary.sh` now checks lifecycle fixture presence,
schema presence, Local Service Lifecycle protocol vocabulary, NET.SPINE.7.0
spine version and process/service manager implementation symbols in active NET
implementation surfaces.

## Relation To Health

Health reports and lifecycle reports are separate. Health describes readiness,
liveness and availability posture. Lifecycle describes requested action intent
and declared or observed service state.

Lifecycle material may reference node, endpoint and capability fixture ids where
applicable. Service and external subject ids may remain unresolved in future
fixtures.

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
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
test ! -e work/protocols
```

## Non-Goals

- no process spawning
- no service supervision
- no start/stop implementation
- no live status check
- no network IO
- no HTTP request
- no socket use
- no endpoint probing
- no discovery
- no transport
- no routing
- no CLI behavior
- no CLORI code
- no system/ extraction
- no engine/ extraction
