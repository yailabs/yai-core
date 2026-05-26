# NET Boundary

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

## Ownership Table

NET owns:

- node identity
- endpoint registry
- stream envelopes
- transport adapters
- health/readiness/liveness
- capability advertisement
- routing boundary
- invocation transport
- trace/receipt transport
- transport metrics

NET does not own:

- case authority
- policy resolution
- facts truth
- graph truth
- memory truth
- journal truth
- neural execution
- model loading
- model decoding
- operator approval
- action eligibility

NET does not own case authority.
NET does not own policy resolution.
NET does not own neural execution.
NET does not own graph truth.
NET does not own memory truth.
NET does not own journal truth.

## Guardrail

NET transports requests, chunks, metrics, traces and receipts. YAI decides what
is authorized, what is imported, what becomes durable record material and what
is rejected or quarantined.
