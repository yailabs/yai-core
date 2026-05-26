# NET Architecture

NET exists as a root component because communication with local, localhost, LAN
and remote capability nodes is a runtime substrate concern, not a scattered
property of `system/`, `engine/` or interface contract documents.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## Relationship To system/

`system/` owns authority-facing behavior: case binding, policy/control,
operator review, carrier eligibility, receipt import and operational records.
NET may later expose node health, endpoint status and transport receipts to
`system/`, but it does not become the authority layer.

NET.SPINE.0 does not mutate `system/`.

## Relationship To engine/

`engine/` owns operational data-plane material. NET may later carry transport
metrics, invocation ids and receipt envelopes that become engine-readable
records after YAI authority imports them. NET does not write graph truth,
memory truth, fact truth or journal truth directly.

NET.SPINE.0 does not mutate `engine/`.

## Relationship To interfaces/transports

`interfaces/transports` remains the contract, vocabulary, readiness and handoff
surface. NET is the runtime substrate inside YAI that may consume, honor or
adapt those contracts later. NET does not duplicate interfaces as source of
truth.

## External Nodes

NET prepares a stable place for external capability nodes such as CLORI. CLORI
lives in `yailabs/clori`, is external to YAI, and may become NET-compatible.
NET does not execute CLORI internals.

## Transport Posture

NET prepares local, localhost, LAN and remote transport boundaries without
implementing them in NET.SPINE.0. No discovery, socket, HTTP, server or routing
behavior is implemented in this wave.
