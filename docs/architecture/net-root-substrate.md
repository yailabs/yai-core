# NET Root Substrate

Status: Boundary foundation
Authority: YAI repository

NET is the root-level communication substrate inside YAI. It exists to discover
nodes, describe their capabilities, move invocation streams and carry transport
metrics and receipts without taking ownership of YAI authority.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## Root Boundary

NET is planned as a root component parallel to existing YAI authority and data
planes. Its role is transport and node coordination, not policy, memory, graph,
fact, journal or execution truth.

`interfaces/transports/` remains the contract vocabulary and schema surface.
NET is the runtime substrate inside YAI: discovery, node identity, streams,
health, routing inputs, transport metrics and capability advertisement.

NET-owned substrate responsibilities:

- local node identity
- local endpoint registry
- LAN discovery
- remote endpoint registry
- node health probes
- capability advertisements
- stream envelopes
- transport adapters
- invocation transport
- trace and receipt transport
- transport metrics

## Non-Goals

This delivery does not implement NET transport, LAN discovery, IPC, routing or
CLI commands. It only establishes the spine, doctrine and boundary documents
for later delivery.

NET must not turn `system/` into a network supervisor. NET may expose lifecycle
contracts and node readiness, but authority and approval remain in YAI.

## Compatibility Posture

External repositories may become NET-compatible nodes. CLORI is the first named
compatibility target, but NET does not depend on CLORI and does not encode CLORI
as a special authority source.

The authoritative NET roadmap is `docs/spines/net-spine.md`.
