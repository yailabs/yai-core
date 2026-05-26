# Architecture Glossary

- Active case graph: case-scoped graph data loaded for bounded diagnostics.
- Carrier: a boundary-specific executor or observer.
- Case: operational scope for subjects, attempts, decisions, receipts and
  durable residue.
- Control decision: the recorded decision posture for an attempted operation.
- DuckDB facts: planned analytical fact plane.
- Graph relation: durable typed edge stored as `yai.graph_relation.v1`.
- Hot state: live cache and snapshot, not durable truth.
- Journal: append-only replay and audit residue.
- LMDB record plane: durable indexed record lookup.
- Operator reviewer: local review authority subject.
- Prompt surface: terminal or interface that renders review choices, not
  authority by itself.
- RuntimeGraph: ephemeral in-memory working set built from active-case graph
  relations.
- Subject: actor, artifact, file, process, provider, operator, model or system
  boundary bound to a case.

