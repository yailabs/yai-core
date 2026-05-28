# SPINE.50 Fact Reports + CLI Manual Validation

Status: completed.

SPINE.50 adds compact read-only fact reports over the first operational
DuckDB fact set.

Commands:

```bash
yai facts report --case <case_ref>
yai facts report --case <case_ref> --section receipts
yai facts report --case <case_ref> --section decisions
yai facts report --case <case_ref> --section projections
yai facts report --case <case_ref> --section policy
yai facts report --case <case_ref> --section carriers
yai facts report --case <case_ref> --section divergence
yai facts report --case <case_ref> --section memory
yai facts report --case <case_ref> --section model
```

`facts summary` is counts. `facts report` is interpreted compact sections over
existing DuckDB facts only.

Boundaries:

```text
facts reports are not truth
facts report != case truth
facts report != audit packet
facts report != reconcile action
facts report != memory consolidation
```

Section posture:

```text
none_observed
no_model_records
memory_is_truth: false
```

Zero divergence/model facts are valid when source records are absent. SPINE.50
does not add new fact extraction, fact revision, report file artifacts, audit
packets, reconcile actions, memory consolidation, model invocation, retrieval
facts, provider runtime facts, CLORI, HNSW or Context Compiler.

Validation:

```bash
make check-fact-reports-cli
make smoke-spine50
```
