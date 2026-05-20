# NEW.9 Store / Index / Query Boundary

NEW.9 introduces the first query boundary over persisted core residue.

This is a journal scan/filter boundary. It is not a database, graph traversal
engine, vector index, RAG layer, ranking system or Rust production backend.

## Query Request

The C request carries:

```text
query_id
case_ref
record_kind
subject_ref
decision_id
receipt_id
memory_id
projection_id
limit
include_summary
```

All filters are optional. An empty filter returns all records up to `limit`.

## Query Result

The C result carries:

```text
result_id
query_id
matched_count
returned_count
truncated
summary
```

The first result summary is intentionally count-oriented:

```text
query_result:<id> query:<id> matched:<N> returned:<N> truncated:<true|false>
```

## Filter Behavior

```text
case_ref       -> record case
record_kind    -> record kind
subject_ref    -> record subject
decision_id    -> record decision id
receipt_id     -> record receipt id
memory_id      -> record id or memory marker in summary
projection_id  -> record id or projection marker in summary
limit          -> maximum returned count
```

Records are returned in journal order. There are no boolean expressions,
sorting rules, persistent index files, vector search or graph traversal.

## yai

```text
yai query summary --journal <path>
yai query records --journal <path> [--kind <record_kind>] [--case <case_ref>] [--limit <N>]
```

Minimum output:

```text
records: N
matched: M
returned: R
truncated: true|false
```

## Smoke Proof

`tests/smoke/query-boundary/test_query_boundary.c` creates a case, subject,
attempt, decision, receipt, graph edge, memory candidate and projection result.
It then queries all records, receipt records, memory candidates and case-scoped
records. Query result records are appended and projection confirms they are
visible as derived residue.

## Old-YAI Audit

NEW.9 inspected old query/index/store material without copying source:

```text
src/substrate/query/*
src/substrate/indexes/*
src/substrate/store/*
src/substrate/records/*
src/substrate/views/*
src/analytics/features/*
src/agents/grounding/query_strategy.c
src/agents/grounding/retrieval_strategy.c
```

Extracted concepts:

```text
query is read posture, not authority
case-scoped reads must avoid leakage
query does not decide or execute
index/retrieval material is future work
agent query strategies belong to ai-environment experiments
```
