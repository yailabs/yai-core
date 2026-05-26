Historical/superseded engineering record. Not an active source of truth.

# SPINE.41 Graph Relation Write Path

SPINE.41 adds the first minimal graph relation write path:

```text
LMDB records
-> yai.graph_relation.v1
-> lmdb_graph_relations_v0
-> yai graph relations --case <case_ref>
```

It materializes typed relations such as `decision_controls_attempt` and
`receipt_records_effect` from existing records. RuntimeGraph remains planned
and Ladybug integration remains future.
