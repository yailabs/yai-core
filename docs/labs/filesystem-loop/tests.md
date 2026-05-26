# Filesystem Loop Lab Tests

Status: canonical lab test expectation map.

## Repository Checks

```bash
make check-docs
make smoke-spine24a
make smoke-spine27
```

## Lab Assertions

| Area | Expected signal |
|---|---|
| Runtime layout | `yai doctor` reports paths and hot-state status. |
| Hot state | Missing snapshot reports `missing_snapshot`; daemon loops produce active status. |
| Carrier planning | Carrier route reports `execution_performed: false` for planning-only paths. |
| Journal replay to LMDB | `yai journal replay --dry-run` writes nothing, real replay creates or confirms LMDB records, second replay is idempotent. |
| Replay idempotency/schema | `yai journal replay-status` reports `cursor_line`, `journal_identity`, `record_schema` and `compatibility`; schema mismatch writes no records. |
| Policy fixtures | JSON fixtures validate and materialize as `subject:policy-pack` residue. |
| Provider boundary | Provider attachment records base URL/model, not provider authority. |
| Model output | Model output is `claim_or_proposal` and not authoritative case state. |

## Related Benchmark Tests

Provider reachability and latency are benchmark concerns, not filesystem-loop
correctness. Use:

```bash
tools/labs/probe-lan-provider.sh --host 172.20.10.3 --port 43117 --model qwen-local
python3 tools/labs/generate-benchmark-report.py \
  --root benchmarks/nvidia \
  --curation configs/labs/nvidia-report-curation.yaml \
  --out docs/labs/nvidia/reports/20260525-lan-provider
```

## Journal Replay Checks

These checks validate journal replay to LMDB for the filesystem-loop lab.

```bash
yai journal inspect --path "$JOURNAL"
yai journal replay --path "$JOURNAL" --dry-run
yai journal replay --path "$JOURNAL"
yai journal replay-status --path "$JOURNAL"
yai store summary
yai store record list --case case:new12-filesystem --limit 10
```

Expected signals:

```text
invalid_entries: 0
replay_ready: yes
records_written:
records_duplicate:
record_store_status: ready
journal_identity:
record_schema: yai.record.v1
compatibility: compatible
cursor_line:
replay_status: completed
```

## Replay Schema Checks

These checks validate SPINE.37 replay idempotency and schema handling.

```bash
yai journal replay --path "$JOURNAL" --dry-run
yai journal replay --path "$JOURNAL"
yai journal replay-status --path "$JOURNAL"
yai journal replay --path "$JOURNAL"
```

Expected signals:

```text
second replay records_written: 0
second replay records_duplicate: N
schema mismatch records_written: 0
schema mismatch compatibility: schema_mismatch
no false completed status for invalid input
```
## Replay Diagnostics / Rebuild Report

SPINE.38 validates durable replay diagnostics for the canonical
docs/labs/filesystem-loop path.

```bash
yai journal replay --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
```

Expected evidence includes `yai.replay_report.v1`, `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries` and a failed report for corrupt replay input.

## Journal Replay Freeze

SPINE.39 freezes the replay checks under `docs/labs/filesystem-loop`.

```bash
yai journal inspect --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
yai store summary
yai store record list --case <case_ref> --limit 10
```

Expected signals:

```text
journal_identity:
record_schema: yai.record.v1
journal_schema: yai.store.record.v0
compatibility: compatible
cursor_line:
replay_status: completed
replay_report_schema: yai.replay_report.v1
records_written:
records_duplicate:
invalid_entries: 0
idempotent: yes
```

Negative checks:

```text
schema_mismatch writes no records
invalid_json writes no records
no false complete
no journal fallback
```

## Graph Schema Checks

SPINE.40 adds schema/status checks that are useful before graph relation write
work begins.

```bash
yai graph schema
yai graph runtime-status
```

Expected signals:

```text
graph_schema:
node_kinds:
edge_kinds:
relation_write_path: active_minimal
runtime_graph:
status: planned
role: in_memory_active_case_working_set
```

## Graph Relation Write Path Checks

SPINE.41 checks graph relation materialization from replayed LMDB records.
RuntimeGraph remains planned. Ladybug integration remains future.

```bash
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
```

Expected signals:

```text
schema: yai.graph_relation.v1
relations_written: N
second materialize relations_written: 0
second materialize relations_duplicate: N
edge_kind: decision_controls_attempt
edge_kind: receipt_records_effect
runtime_graph_updated: false
```
