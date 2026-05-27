# Model Runtime

YAI controls model invocation boundaries. It does not treat model text as case
authority or carrier execution.

## Boundaries

- Provider runtime handles local or external model providers as bounded
  invocation targets.
- LAN target support is a provider/runtime boundary, not a remote authority
  model.
- Model carrier work binds model invocation to case, subject, policy, receipt
  import and observation.
- Runner/model runtime doctrine keeps model execution separate from operational
  authority.

Model output can become a claim, proposal, observation or result envelope. YAI
must still decide whether it can affect the case, create memory, invoke a
carrier or enter review.

## Planned Runtime Work

MTP/speculative decoding, native model runners and richer provider backends are
future runtime work. CLORI is a possible future or parallel native neural
execution component; no CLORI implementation is claimed in this repository
state.

## Context Residency

A model must not reconstruct the active case from scratch for every request.
YAI plans ContextFrame base/delta state, ContextDelta invalidation,
CaseModelSession binding and runner KV/cache boundaries.

CaseModelSession binds case, actor, model, runner, provider, context frame,
policy scope, retrieval providers and cache policy. It is not authority.

Runner KV/cache state is an optimization scoped by model, runner, tokenizer,
context hash, prefix hash and valid frame id. KV cache is not YAI memory,
truth, authority or provenance.
