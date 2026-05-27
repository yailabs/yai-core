# NET Spine

Reference version: NET.SPINE.9.0
Authority: YAI repository
Status: Local IPC channel contract started

NET is the root-level runtime communication substrate inside YAI.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

CLORI extension:
YAI controls authority.
NET moves streams.
CLORI executes neural computation.

This spine is separate from the main YAI delivery spine.
It does not rewrite or expand the existing 120-wave YAI main spine.

NET is canonical in this repository.
External repositories may copy this file as a compatibility reference.
Copied references are not authoritative.

`interfaces/transports` remains the contract and vocabulary layer.
NET is the YAI-side runtime communication substrate that may consume, honor or
adapt those contracts.

The source path for external references is `work/spines/net-spine.md`.

## Deliveries

### NET.SPINE.0 - Root Component Scaffold + Boundary Guard

Create the first root `net/` component scaffold, public vocabulary headers and
boundary guard. This wave established NET as a root runtime substrate, but its
initial layout was still repository-shaped and is corrected by NET.SPINE.0R.

### NET.SPINE.0R - Integrated Module Topology Realignment

Move NET from repository-shaped scaffold to integrated YAI module topology.
Move public NET headers to `include/yai/net`, flatten `net/src` into
`net/<area>`, remove `net/docs` and repo-shaped placeholder folders, move NET
planning/docs into `work/`, preserve root `net/` as runtime module and preserve
CLORI as an external repository.

### NET.SPINE.1 - Canonical Terms + File/Header Discipline

Add canonical NET protocol vocabulary, normalize public NET header naming and
include discipline, add module area README discipline and update guard coverage.
NET.SPINE.3R moves the protocol vocabulary to `proto/net.md`. Keep NET as an
integrated YAI module and do not implement transport, discovery or runtime
behavior.

### NET.SPINE.2 - Stream Envelope Types + Fixtures

Add canonical stream envelope types, stream event kind discipline, JSON
fixtures and static stream envelope checks. NET.SPINE.3R moves the fixtures to
`proto/fixtures/net/stream/`. This wave does not implement live streaming or
transport.

### NET.SPINE.3 - Node Identity Types + Local Machine Projection

Add canonical NET node identity types, safe local machine projection
vocabulary, node fixtures and schema/contract, and a node fixture validator.
Link node identity with stream origin/target fields. Do not implement discovery
or machine probing.

### NET.SPINE.3R - Proto Topology Realignment

Move NET protocol vocabulary, fixtures and schemas from `work/protocols` to
`proto`. Remove `work/protocols` as an invalid protocol surface. Update guards
and validators so future NET protocol material lands under `proto`.

### NET.SPINE.4 - Capability Advertisement Types + Registry Seed

Add canonical NET capability advertisement types, capability fixtures and
schema under `proto/`, and a static capability fixture validator. Seed registry
vocabulary only, link capabilities to node fixture references and do not
implement live registry, routing or discovery.

### NET.SPINE.5 - Local Endpoint Registry Skeleton

Add endpoint descriptor types, endpoint fixtures and schema under `proto/`, and
a static endpoint fixture validator. Link endpoint fixtures to node and
capability fixture ids. Create registry seed material only; do not implement
discovery, probing, routing or transport.

### NET.SPINE.6 - Health / Readiness / Liveness Probe Skeleton

Add health report types, health fixtures and schema under `proto/`, and a
static health fixture validator. Link health reports to node, endpoint and
capability fixture ids. Create probe result vocabulary only; do not perform
live probing, discovery, routing or transport.

### NET.SPINE.7 - Local Service Lifecycle Contract

Add lifecycle request/report types, lifecycle fixtures and schemas under
`proto/`, and a static lifecycle fixture validator. Link lifecycle material to
node, endpoint and capability fixture ids where applicable. Create lifecycle
vocabulary only; do not start, stop, supervise, probe, route or transport
services.

### NET.SPINE.8 - Localhost Transport Adapter Skeleton

Add transport adapter descriptor types, transport fixtures and schema under
`proto/`, and a static transport fixture validator. Link transport adapter
descriptors to endpoint fixture ids. Create localhost, local IPC, LAN, remote
and future transport vocabulary only; do not implement network IO, HTTP,
probing, routing or live streaming.

### NET.SPINE.9 - Local IPC / Unix Socket Adapter Skeleton

Add local IPC channel descriptor types, IPC fixtures and schema under `proto/`,
and a static IPC fixture validator. Link IPC channel descriptors to transport
adapter and endpoint fixture ids. Create abstract local IPC, Unix socket future,
named pipe future and platform IPC future vocabulary only; do not implement
IPC, Unix sockets, named pipes, filesystem socket paths, network IO, probing,
routing or live streaming.

### NET.SPINE.10 - LAN Discovery Adapter Skeleton

Create LAN discovery adapter skeleton and docs that reserve discovery posture
inside NET. Add guards preventing LAN discovery assumptions from spreading into
`system/` or `engine/`.

### NET.SPINE.11 - Remote Endpoint Registry Skeleton

Add remote endpoint registry skeleton, static configuration fixtures and tests
for configured, unavailable and unsupported remote endpoints. Do not implement
remote network calls.

### NET.SPINE.12 - Transport Adapter Interface

Implement the public transport adapter interface and adapter status types.
Connect localhost, IPC, LAN and remote adapter skeletons to the interface
without adding live transport behavior.

### NET.SPINE.13 - Streaming Runtime Skeleton

Add streaming runtime skeleton, event queue types and fixture validation for
ordered stream events. Do not implement background runtime services.

### NET.SPINE.14 - Invocation Envelope + Request Correlation

Create invocation envelope types, request correlation ids and fixtures tying
invocation, stream, trace and receipt ids together. Add tests for missing and
malformed correlation material.

### NET.SPINE.15 - Router Skeleton + Selection Inputs

Implement router skeleton types for capability match, node health, latency and
policy handoff inputs. Add tests that prove router output is advisory until YAI
authority accepts it.

### NET.SPINE.16 - Queue / Retry / Timeout / Backpressure Skeleton

Add queue, retry, timeout and backpressure types plus fixture examples for
waiting, retryable, expired and rejected invocations. No worker scheduler is
implemented in this wave.

### NET.SPINE.17 - Trace / Receipt Transport Types

Create trace and receipt transport types and tests for propagation through
stream envelopes. Guard the rule that NET transports receipts but YAI decides
authority.

### NET.SPINE.18 - Trust / Security Boundary Enforcement Guard

Add enforcement guard coverage for trust boundary language, forbidden authority
claims and accidental writes into YAI graph, memory, facts or journal from NET.

### NET.SPINE.19 - NET Metrics Collector Skeleton

Implement metrics collector skeleton types and fixtures for discovery latency,
connect latency, handshake latency, first chunk latency, stream bytes, queue
wait and transport error counts.

### NET.SPINE.20 - NET CLI Read-Only Control Surface

Add read-only `yai net` command skeletons for nodes, health, capabilities and
trace inspection. Commands may report scaffold/planned status only until backing
runtime behavior exists.

### NET.SPINE.21 - system/ Network Assumption Extraction

Audit `system/` for provider health, endpoint checks, carrier transport,
LAN/discovery and service lifecycle assumptions. Extract or quarantine findings
behind NET docs, guards or adapter skeletons.

### NET.SPINE.22 - engine/ Endpoint Assumption Extraction

Audit `engine/` for endpoint state, external service status, live transport
residue and provider runtime assumptions. Extract or quarantine findings so
engine remains data-plane, not transport owner.

### NET.SPINE.23 - External Capability Node Contract

Create the generic external capability node contract with fixtures, tests and
compatibility docs. Keep external execution separate from YAI authority.

### NET.SPINE.24 - CLORI Node Compatibility Contract

Add CLORI-specific compatibility contract, fixture examples and receipt
boundary tests. CLORI remains external and is not vendored into YAI.

### NET.SPINE.25 - Transport Benchmark Harness v0

Create benchmark harness v0 for transport overhead fixtures and repeatable
measurement shape. Do not publish benchmark claims until real runs exist.

### NET.SPINE.26 - Local / Localhost / LAN / Remote Test Matrix

Add a test matrix covering local, localhost, LAN and remote postures across
registry, health, routing inputs, streaming and receipt transport boundaries.

### NET.SPINE.27 - YAI Carrier Invocation Through NET Boundary

Connect YAI carrier invocation docs and skeleton handoff types to NET transport
boundaries. Preserve the rule that carriers and YAI authority own execution
eligibility while NET owns stream movement.

### NET.SPINE.28 - External Reference Copy Sync Contract

Add sync contract and guard instructions for external repositories that copy
`work/spines/net-spine.md`, including CLORI's non-authoritative compatibility
reference.

### NET.SPINE.29 - NET v0 Freeze

Freeze NET v0 layout, headers, docs, guard coverage, fixture schema posture and
integration boundaries before runtime transport work expands beyond skeletons.

## External Reference Copy

External repositories should copy:

```text
work/spines/net-spine.md
```

into:

```text
<clori-docs>/spines/yai-net-spine-reference.md
```

with this header:

```text
# YAI NET Spine Reference

Source repository: yai
Source path: work/spines/net-spine.md
Reference version: NET.SPINE.9.0
Authority: YAI repository
Mode: copied compatibility reference

This file is copied into CLORI to keep CLORI aligned with YAI NET contracts.
This file is not authoritative.
When this reference diverges from YAI, YAI wins.
```
