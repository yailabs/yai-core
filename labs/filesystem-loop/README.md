# Filesystem Loop Lab

Status: canonical operator lab for filesystem-loop validation.

Purpose: validate that YAI can run a bounded filesystem case through
command-surface checks, daemon smoke paths, pack fixture material, case
residue, prompt evidence and provider-boundary posture.

This lab is the source of truth for filesystem-loop validation. `notebook.ipynb`
is the complete manual operator notebook: build, install, daemon, case
materialization, command-surface sweeps and hand-written prompt cells.
`runbook.md` is the static terminal copy of the same flow.

## Canonical Files

| Concern | Path |
|---|---|
| Entry point and protocol | `labs/filesystem-loop/README.md` |
| Runbook | `labs/filesystem-loop/runbook.md` |
| Notebook | `labs/filesystem-loop/notebook.ipynb` |
| Prompt catalog | `labs/filesystem-loop/prompts.json` |
| Run script | `labs/filesystem-loop/run.sh` |
| Runs | `labs/filesystem-loop/runs/` |
| Pack fixture | `labs/filesystem-loop/pack-fixture/` |

## Scope

- Command-surface documentation checks.
- Filesystem-loop smoke paths.
- Pack fixture selection and case materialization posture.
- Case boundary and provider-boundary posture.
- Prompt labs that preserve exact prompt text used against the case view.
- Prompt/query catalog execution through `run.sh --prompt-id <id>`.
- Notebook-guided manual execution over the full command surface.
- Hand-written prompt cells that produce model output when a provider is bound.

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

Prompt catalog authority for scripted compact runs:

```text
labs/filesystem-loop/prompts.json
```

`run.sh --list-prompts` lists catalog entries. `run.sh --prompt-id <id>`
executes one prompt/query at a time, stores the resolved `assets/prompt.json`
and `assets/prompt.txt`, then captures stdout, stderr and exit status in the
run package. The notebook keeps the important prompt text written directly in
cells so an operator can run and inspect model output without opening the JSON.

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
| `20260526-real-fsloop` | current prompt-catalog execution attempt | `runs/20260526-real-fsloop/report.md` |
| `20260526-canon6-smoke` | retained structural smoke | `runs/20260526-canon6-smoke/report.md` |

## Current Summary

| Metric | Value |
|---|---:|
| Runs counted | 2 |
| Completed run packages | 2 |
| Structural smoke runs | 1 |
| Prompt-catalog runs | 1 |
| Commands executed | 2 |
| Commands passed | 1 |
| Commands failed | 1 |
| Prompted calls | 1 |
| Model outputs | 0 |

The root notebook is the full manual path with official commands and prompt
cells. Use `runbook.md` when the same flow is needed as terminal text.

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

Create the current structural smoke package with:

```bash
labs/filesystem-loop/run.sh --slug canon6-smoke --command "make check-docs"
```

Create the current prompt-catalog execution attempt with:

```bash
YAI=target/debug/yai labs/filesystem-loop/run.sh --slug real-fsloop --prompt-id lab-a-orientamento-01
```

List prompt catalog entries with:

```bash
labs/filesystem-loop/run.sh --list-prompts
```

Execute one catalog prompt at a time with:

```bash
labs/filesystem-loop/run.sh --slug <slug> --prompt-id <prompt-id>
```

Each run writes `transcript.md`, `report.md`, `metrics.json`,
`manifest.json` and supporting attachments under `assets/`.

After a run exists, compose report-first tables and figures with:

```bash
python3 labs/shared/bin/generate-run-figures.py --run-dir labs/filesystem-loop/runs/<run> --update-report --overwrite
```

Automation/test review mode is non-interactive. `yai daemon
run-filesystem-review-loop --socket <socket>` records a `require_review`
pending item, prints `review_required: yes`, `status: pending_operator`,
`carrier_attempted: false`, `execution_performed: false` and `next_commands`,
then exits. `subject:linenoise-terminal is prompt surface`; operator reviewer
authority belongs to `subject:operator-reviewer`. The follow-up commands are
`approve`, `deny`, `defer` and `quarantine`.

CLI review automation uses `control pending`, `control show`, `control review --interactive`,
`control watch` and `control wait`. `not_a_tty` is the
expected non-interactive response for interactive review. Guard vocabulary:
operator reviewer authority, `subject:operator-reviewer is review authority`,
`next_commands`.

The review-loop matrix belongs to this lab, not to a separate model-behavior
lab. It keeps approve, deny, defer, quarantine, `pending_operator`,
`next_commands`, `wait timeout`, bounded `watch`, model proposal observed,
model cannot approve and automatic proposed-op gate import is future work in
one filesystem-loop evidence surface. Model output remains claim/proposal
material; it cannot approve, mutate decisions or execute a carrier.

## Review Loop Test Matrix

SPINE.44C aligns the review loop test matrix inside
`labs/filesystem-loop`. The matrix covers
blocked outside-sandbox writes, `pending_operator` review-required writes,
`next_commands`, approve, deny, defer, quarantine, `wait timeout`, bounded
`watch`, resolved wait and non-TTY interactive handling.

Non-execution states keep `carrier_attempted: false` and
`execution_performed: false`. `approve` may execute only after the local
operator reviewer resolves the pending request; `deny`, `defer` and
`quarantine` do not execute the filesystem carrier.

Model proposal observed, model cannot approve, and automatic proposed-op gate
import is future work. Model-facing posture is documented here so the lab
distinguishes projection boundary, model proposal, control pending/review,
carrier execution and receipt.
