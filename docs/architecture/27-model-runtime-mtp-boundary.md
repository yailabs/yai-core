# Model Runtime / MTP Boundary

Status: SPINE.33K roadmap correction.

Purpose: place MTP and speculative decoding under model runtime / runner
doctrine, not under retrieval or data-plane truth.

## Boundary

MTP belongs to model runtime / runner / decoding strategy.

MTP is:

```text
runner capability
backend capability
decoding strategy
performance feature
measured behavior
```

MTP is not:

```text
case truth
retrieval truth
memory truth
policy truth
authority
```

MTP is not retrieval. MTP is not policy authority.

```text
HNSW makes the model read less.
MTP makes the model write faster.
```

More formally:

```text
HNSW reduces input/context selection cost.
MTP reduces output token generation latency.
```

## Capability Manifest

Future runner capability probes should track:

```text
supports_streaming
supports_tools
supports_json_schema
supports_mtp
supports_speculative_decoding
mtp_depth
mtp_mode
requires_draft_model
requires_mtp_weights
backend_support
context_limit
kv_cache_policy
structured_output_risk
```

## MTP Policy And Metrics

Future MTP policy must define when MTP is allowed, when it is disabled and how
the runner falls back to normal decoding.

Metrics:

```text
acceptance_rate
latency_delta
tokens_per_second_delta
schema_failure_rate
tool_call_failure_rate
quality_regression_marker
fallback_mode
```

Rule:

```text
MTP may accelerate generation.
It must not change output authority.
```
