# NET / YAI Boundary

Status: Boundary foundation
Authority: YAI repository

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## YAI Owns

- case authority
- policy resolution
- operator approval
- carrier authority
- action eligibility
- memory truth
- graph truth
- fact truth
- journal truth
- receipt import and reconciliation

## NET Owns

- node identity
- endpoint registries
- discovery posture
- transport adapters
- stream envelopes
- invocation transport
- health/readiness/liveness probes
- capability advertisement
- route selection inputs
- trace and receipt transport
- transport metrics

## interfaces/transports Owns

- transport contract vocabulary
- transport schemas
- stream envelope contract language
- readiness and handoff matrices
- client/runtime/provider boundary terms

## Integration Rule

YAI may invoke a capability through NET only after YAI authority has selected
and authorized the operation. NET may report route health, node capabilities,
latency and transport failures, but those facts do not become policy authority.

NET metrics can inform YAI decisions. They do not decide YAI decisions.

`interfaces/transports/` remains the contract layer. `yai/net` is the internal
runtime substrate that implements YAI-side communication behavior without
moving or rewriting the interface contracts.

## Extraction Rule

Future delivery passes may move or quarantine network, endpoint and discovery
assumptions from `system/` and `engine/` into NET. Those passes must preserve
YAI authority and must not rewrite the main YAI spine.
