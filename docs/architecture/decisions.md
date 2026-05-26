# Decisions

This is the active decision compendium. Detailed ADR history is archived under
`work/archive/adr/`.

## Current Decisions

- YAI is a local case-bound operational control runtime.
- `system/` owns operational control boundaries and local carrier surfaces.
- `engine/` owns durable Rust data-plane behavior.
- Hot state is not durable truth.
- Journal replay and LMDB records are durable reconstruction surfaces.
- Graph relations are durable typed relations.
- RuntimeGraph is ephemeral, per-command active-case computation.
- RuntimeGraph is not durable truth.
- Bounded graph diagnostics include fanout, fanin, neighborhood and causal
  path checks; this is not a generic graph database.
- Review, defer and quarantine are active control states, not passive labels.
- The terminal prompt surface is not review authority.
- `docs/` is curated architecture documentation.
- `labs/` contains experiments, runbooks, outputs and reports.
- `work/` contains spines, waves, inventories, execution notes and archives.

## Future Decisions

DuckDB facts, HNSW/retrieval, Context Compiler, memory consolidation, native
model execution and broader provider runtime behavior remain planned decision
areas.

