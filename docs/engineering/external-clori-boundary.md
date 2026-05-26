# External CLORI Boundary

Status: Boundary foundation
Authority: YAI repository

CLORI is external to the YAI repository. CLORI may expose a NET-compatible node
adapter, but CLORI is not part of YAI authority and NET is not a CLORI
dependency.

YAI controls authority.
NET moves streams.
CLORI executes neural computation.

## YAI Side

YAI owns case authority, policy, operator approval, carrier eligibility, memory,
graph, facts, journal and receipt reconciliation.

YAI may select CLORI as an external capability node only through a
NET-compatible contract and only after YAI authority has permitted the
invocation.

## NET Side

NET owns node identity, endpoint registries, health, capability advertisement,
stream envelopes, invocation transport, trace transport, receipt transport and
transport metrics.

`interfaces/transports/` remains the contract and vocabulary layer. NET is the
YAI runtime communication substrate and must not be moved into `interfaces/`.

CLORI appears to NET as one compatible external node type. CLORI is not required
for NET to exist.

## CLORI Side

CLORI owns neural execution concerns: model artifact inspection, descriptors,
tensor tables, quant profiles, execution IR, backend abstraction, memory
accounting, KV cache, decode/sampler policy, inference metrics, benchmark
receipts and standalone CLI/server surfaces.

CLORI must not own YAI policy, memory, graph, facts, journal, carrier authority
or action approval.

## Reference Rule

The authoritative NET spine lives in YAI at `docs/spines/net-spine.md`.

CLORI may store a copied compatibility reference at
`docs/spines/yai-net-spine-reference.md`. That copy must include the source
path, source version, authority statement and non-authoritative warning.
