# NET Root Substrate

Status: NET.SPINE.0R integrated module topology
Authority: YAI repository

NET is the root-level runtime communication substrate inside YAI.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

The concrete component lives at `net/`. Public headers live at
`include/yai/net/`. Internal planning and spine material lives under `work/`.

## Boundary

NET owns node identity, endpoint registry, stream envelopes, transport adapter
boundaries, health/readiness/liveness vocabulary, capability advertisement,
routing boundaries, invocation transport, trace/receipt transport and transport
metrics.

NET does not own case authority, policy resolution, facts truth, graph truth,
memory truth, journal truth, neural execution, model loading, model decoding,
operator approval or action eligibility.

## interfaces/transports

`interfaces/transports` remains contract, vocabulary, readiness and handoff.
NET is the YAI runtime substrate that may later consume or align with those
contracts.

## Current State

NET.SPINE.0R realigns NET from repository-shaped scaffold to integrated YAI
module topology. It does not implement discovery, transport, routing, server
behavior or external node execution.
