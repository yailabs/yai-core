# Filesystem Loop Lab

Status: canonical operator lab for filesystem-loop validation.

Purpose: validate that YAI can run a bounded filesystem case through
command-surface checks, daemon smoke paths, pack fixture material, case
residue, prompt evidence and provider-boundary posture.

This lab replaces `docs/manuals/manual-filesystem-loop-validation.md` as the
source of truth. The executable notebook has been restored from the historical
Italian manual notebook and now lives at `notebook.ipynb`; `runbook.md` is the
static Markdown copy of that notebook.

## Canonical Files

| Concern | Path |
|---|---|
| Entry point and protocol | `docs/labs/filesystem-loop/README.md` |
| Runbook | `docs/labs/filesystem-loop/runbook.md` |
| Notebook | `docs/labs/filesystem-loop/notebook.ipynb` |
| Run script | `docs/labs/filesystem-loop/run.sh` |
| Runs | `docs/labs/filesystem-loop/runs/` |
| Pack fixture | `docs/labs/filesystem-loop/pack-fixture/` |

## Scope

- Command-surface documentation checks.
- Filesystem-loop smoke paths.
- Pack fixture selection and case materialization posture.
- Case boundary and provider-boundary posture.
- Prompt labs that preserve exact prompt text used against the case view.
- Root notebook summaries over available compact runs.

## Protocol

Research question: can the filesystem-loop lab preserve command-surface,
daemon smoke and policy-boundary evidence in a repeatable compact run package?

Each run records the selected fixture material, case preparation or
materialization posture, commands or prompt interactions, observed outputs,
receipts or projection notes when present, metrics, limitations and supporting
assets.

Expected run package:

```text
runs/YYYYMMDD-<slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

## Inputs / Pack Fixture

The lab uses `pack-fixture/` as operational input material. Runs should record
which fixture files were used, distinguish fixture material from materialized
case state, and keep unimplemented runtime materialization behavior explicit.

## Boundaries

This lab is not a benchmark and is not provider support. It validates that YAI
keeps provider output as claim/proposal material inside a case boundary.

This lab does not treat model output as authoritative state. Provider output,
when present, is candidate material only. Any proposed action must pass through
the control and carrier boundary.

Non-goals:

- No provider benchmark claim.
- No model quality claim.
- No production safety claim.
- No migration of historical manuals into active truth.

## Current Runs

| Run | Status | Report |
|---|---|---|
| `20260526-smoke` | completed | `runs/20260526-smoke/report.md` |
| `20260526-qwen3-filesystem-loop-historical` | historical_baseline | `runs/20260526-qwen3-filesystem-loop-historical/report.md` |

## Current Summary

| Metric | Value |
|---|---:|
| Runs counted | 2 |
| Completed runs | 1 |
| Historical baseline runs | 1 |
| Commands executed | 3 |
| Commands passed | 3 |
| Commands failed | 0 |
| Prompted calls | 6 |
| Model outputs | 6 |

The root notebook contains the full filesystem-loop execution path, including
the historical provider prompt labs. It also reads `runs/*/metrics.json` and can
regenerate local tables from run metrics.

## RuntimeGraph Compatibility Notes

Older compatibility material for RuntimeGraph rebuild is preserved here as a
lab note, not as a separate lab surface.

RuntimeGraph is an in-memory working set for active case computation. The
working set is per-command ephemeral in this lab surface; resident service planned remains future work.

Relevant command pattern:

```bash
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
yai graph rebuild --case <case_ref> --from graph-relations
```

Expected output posture includes `runtime_graph_rebuild`,
`yai.runtime_graph_rebuild_report.v1`, completed journal replay, completed
graph materialization, completed runtime graph status, relation/node/edge
counts and `rebuild_status: completed`.

Boundary posture: RuntimeGraph is not durable truth. Negative checks include
bad journal failure, missing case zero-count behavior and the journal to LMDB to
graph relations to runtime-summary path.

Working-set posture remains per-command and ephemeral:

```text
runtime-status
runtime-load
per-command ephemeral
resident_service: planned
durable_truth: graph_persistence
hnsw: future_candidate_index
context_compiler: future_consumer
```

Graph relation materialization emits `yai.graph_relation.v1` records from LMDB
record truth. Expected relation posture includes `graph materialize`, `graph
relations`, `decision_controls_attempt`, `receipt_records_effect`,
`relation_id` and `source_record_id`.

Bad journals must fail cleanly with failed replay and rebuild status, zero
node/edge totals and an invalid input reason when available.

## How To Execute

Use `runbook.md` for the full terminal procedure. Use `notebook.ipynb` for the
manual analytical notebook path. Use `run.sh` to create a new compact run
folder under this lab.
