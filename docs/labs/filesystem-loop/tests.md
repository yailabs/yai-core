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
```
