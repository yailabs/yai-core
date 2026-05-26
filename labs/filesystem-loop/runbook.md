# Filesystem Loop Runbook

Status: canonical CLI procedure.

This runbook is the complete terminal procedure for the filesystem-loop lab.
The notebook is a guided human demo; automation belongs to `run.sh` and shared
helpers under `labs/shared/bin/`.

## Prerequisites

Run commands from the repository root:

```bash
pwd
test -f labs/filesystem-loop/README.md
test -x labs/filesystem-loop/run.sh
python3 -m json.tool labs/filesystem-loop/prompts.json >/tmp/yai-fsloop-prompts.json
```

Use the repository binary when you want the local build:

```bash
test -x target/debug/yai
```

The real prompt path may require case/provider setup first. The current
evidence run shows the failure mode when `subject:llm-provider` has not entered
`case:new12-filesystem`.

## Prompt Listing

Prompt text authority is `labs/filesystem-loop/prompts.json`.

```bash
labs/filesystem-loop/run.sh --list-prompts
```

The runbook references prompt IDs. It does not duplicate prompt bodies.

## Real Filesystem-Loop Prompt Run

Use one prompt at a time:

```bash
YAI=target/debug/yai labs/filesystem-loop/run.sh \
  --slug real-fsloop \
  --prompt-id lab-a-orientamento-01
```

Expected run package:

```text
labs/filesystem-loop/runs/YYYYMMDD-real-fsloop/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

If the command stops before model output, keep the run. The transcript and
report must state the exact precondition or error. A failed prompt run is still
evidence.

## Safe Structural Fallback

Use this when the case/provider runtime is not attached:

```bash
labs/filesystem-loop/run.sh \
  --slug canon6-smoke \
  --command "make check-docs"
```

This is structural evidence only. It is not a model-quality evaluation and not
a benchmark.

## Report and Figure Regeneration

After a run exists, compose report-first tables and figures:

```bash
python3 labs/shared/bin/generate-run-figures.py \
  --run-dir labs/filesystem-loop/runs/<run-dir> \
  --update-report \
  --overwrite
```

Generated figures are supporting evidence inside `report.md`. The final human
artifact is `report.md`; `transcript.md` is raw chronological execution
evidence; `metrics.json` is machine-readable data; `assets/` contains
attachments.

## Output Inspection

```bash
ls labs/filesystem-loop/runs/<run-dir>
sed -n '1,200p' labs/filesystem-loop/runs/<run-dir>/transcript.md
sed -n '1,220p' labs/filesystem-loop/runs/<run-dir>/report.md
python3 -m json.tool labs/filesystem-loop/runs/<run-dir>/manifest.json
python3 -m json.tool labs/filesystem-loop/runs/<run-dir>/metrics.json
```

## Validation

```bash
python3 -m json.tool labs/filesystem-loop/notebook.ipynb
bash -n labs/filesystem-loop/run.sh
tools/checks/check-lab-notebooks.sh
tools/checks/check-labs-layout.sh
tools/checks/check-lab-run-contract.sh
make check-lab-notebooks
make check-labs
make check-lab-runs
make check-docs
```

## Failure Interpretation

- Missing case/provider attachment is a precondition failure, not a model
  failure.
- Nonzero command exit is recorded in `metrics.json` and interpreted in
  `report.md`.
- Missing measurements are written as `Not measured`.
- Do not infer model quality, production readiness or benchmark superiority
  from this lab.

## Operator Review Automation Contract

Automation mode does not block for interactive input. A review-required write
prints scriptable evidence and exits:

```text
review_required: yes
status: pending_operator
carrier_attempted: false
execution_performed: false
next_commands:
```

Follow-up review commands:

```bash
yai control pending --case case:new12-filesystem
yai control show review:new12-fs-write-review
yai control approve review:new12-fs-write-review --reason "test approval"
yai control deny review:new12-fs-write-review --reason "test denial"
yai control defer review:new12-fs-write-review --reason "test defer"
yai control quarantine review:new12-fs-write-review --reason "test quarantine"
yai control review --interactive
yai control watch --case case:new12-filesystem
yai control wait review:new12-fs-write-review --timeout 1
```

Guard vocabulary: `require_review`, `pending_operator`, `approve`, `deny`,
`defer`, `quarantine`, `next_commands`, `wait timeout`, `watch`, `not_a_tty`,
`carrier_attempted: false`, `execution_performed: false`,
`subject:linenoise-terminal is prompt surface`,
`subject:operator-reviewer is review authority`, operator reviewer authority,
model proposal observed, model cannot approve, automatic proposed-op gate
import is future work, `labs/filesystem-loop`.

## Replay and Graph Compatibility Notes

These notes preserve CLI compatibility anchors used by documentation guards.
They are not notebook orchestration.

### Journal Replay to LMDB

Journal Replay to LMDB uses `journal replay`, `dry-run`, idempotent replay and
diagnostic counters: `records_written`, `records_duplicate`,
`invalid_entries`, no silent skip and no journal fallback.

### Journal Replay Freeze

Journal Replay Freeze keeps `journal inspect`, `journal replay`,
`replay-status`, `replay-report`, `yai.replay_report.v1`, `journal_identity`,
`record_schema`, `compatibility`, `cursor_line`, `idempotent`,
`schema_mismatch`, `invalid_json`, `records_written`, `records_duplicate`,
`invalid_entries` and failed report evidence visible.

Replay diagnostics use `yai.replay_report.v1`, `replay-report`,
`journal_identity`, `compatibility`, `cursor_line`, `records_written`,
`records_duplicate`, `invalid_entries` and failed report fields.

### Graph Relation Write Path

Graph materialization emits `yai.graph_relation.v1` records. Expected terms:
`graph materialize`, `graph relations`, `decision_controls_attempt`,
`receipt_records_effect`, `relation_id`, `source_record_id`,
`RuntimeGraph remains planned` and Ladybug integration remains future.

### RuntimeGraph Working Set

RuntimeGraph is an in-memory working set. It is per-command ephemeral:

```text
runtime-status
runtime-load
runtime-summary
resident_service: planned
durable_truth: graph_persistence
hnsw: future_candidate_index
context_compiler: future_consumer
```

RuntimeGraph rebuild uses `journal`, LMDB, `graph relations`,
`runtime-summary`, `runtime_graph_rebuild`,
`yai.runtime_graph_rebuild_report.v1`, resident service planned and
RuntimeGraph is not durable truth.

RuntimeGraph query inspection supports `fanout`, `fanin`, `neighborhood`,
bounded traversal, causal path, `max-depth`, edge-kind filter,
color-aware graph inspection and plain output remains parseable.
RuntimeGraph is not a generic graph database.

Graph freeze vocabulary: graph schema, RuntimeGraph, runtime graph rebuild,
HNSW future, Context Compiler future, Ladybug future persistence integration,
`review_request`, `review_decision` and `control_pending`.
