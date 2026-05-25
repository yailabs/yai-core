# ADR 0028: Provider Runtime / LAN Target Surface

Status: accepted

## Context

YAI has case-bound provider attachment and model interaction surfaces, but
manual provider launch remains outside YAI. SPINE.33K clarified context
compiler, retrieval, HNSW and MTP boundaries. The next step is to expose the
provider runtime target surface without implementing real provider supervision.

## Decision

YAI must not require manual provider process launch as the normal workflow.
Provider runtime management is modeled through runtime targets, device registry,
provider start plans, logs and receipts.

LAN provider execution requires paired/trusted devices and a remote supervisor.
SPINE.33L exposes the surface but does not execute remote starts.

## Consequences

- `local`, `lan` and `external` are canonical runtime target modes.
- `YAI_HOME/config/devices.jsonl` records device registry v0.
- `yai provider start --dry-run` reports plans with
  `execution_performed: false`.
- Untrusted LAN targets block provider start plans.
- Model catalog, retrieval/HNSW, decoding acceleration and model routing status
  become inspectable without implementation claims.
