# NET / YAI Boundary

Status: NET.SPINE.0 root component scaffold
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
- stream envelopes
- transport adapter boundaries
- invocation transport
- health/readiness/liveness vocabulary
- capability advertisement
- route selection inputs
- trace and receipt transport
- transport metrics

NET facts can inform YAI decisions. They do not decide YAI decisions.

See `net/docs/boundary.md` and `net/docs/yai-integration.md`.
