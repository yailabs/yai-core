# External Runtime Lab

Status: canonical lab for local provider and external runtime readiness checks.

Purpose: record provider reachability, authentication state, model inventory and
runtime smoke evidence without centering the lab on a vendor-specific folder.

The executable source is `notebook.ipynb`; `runbook.md` is the static Markdown
copy of that notebook.

## Canonical Files

| Concern | Path |
|---|---|
| Entry point and protocol | `docs/labs/external-runtime/README.md` |
| Runbook | `docs/labs/external-runtime/runbook.md` |
| Notebook | `docs/labs/external-runtime/notebook.ipynb` |
| Prompt catalog | `docs/labs/external-runtime/prompts.json` |
| Run script | `docs/labs/external-runtime/run.sh` |
| Runs | `docs/labs/external-runtime/runs/` |
| Pack fixture | Not defined |

## Concepts

- Provider readiness: whether a provider endpoint accepts a request at the
  network and HTTP layers.
- Authentication state: whether a reachable endpoint blocks generation because
  credentials are missing or invalid.
- Model inventory: model metadata returned by provider discovery endpoints.
- Smoke workload: a small check used to confirm that the measurement path works.

## Scope

- Local endpoint probes.
- Provider status classification.
- Authentication boundary classification.
- Model inventory when `/v1/models` is reachable.
- Small HNSW smoke panels when present.
- Prompt/request payload preservation for provider calls.
- Prompt catalog execution through `run.sh --prompt-id <id>`.
- Root notebook summaries over compact run folders.

## Protocol

Research question: can the operator host observe a local provider endpoint,
classify model discovery and chat-generation readiness, and preserve the
resulting evidence without mixing raw runs and publishable reports?

Each run records endpoint targets, request or prompt payloads when available,
status classification, model inventory, smoke metrics when present, generated
figures or tables, limitations and supporting assets.

Expected run package:

```text
runs/YYYYMMDD-<slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

Prompt text authority:

```text
docs/labs/external-runtime/prompts.json
```

`run.sh --list-prompts` lists catalog entries. `run.sh --prompt-id
local-provider-ready` resolves one provider prompt, calls
`docs/labs/_shared/bin/probe-local-provider.sh`, and stores the request payload,
curl metadata and provider response under run `assets/`.

## Current Runs

| Run | Status | Notes | Report |
|---|---|---|---|
| `20260526-canon6-provider-probe` | current | Prompt-catalog local provider probe using `local-provider-ready`; payload, curl metadata and provider response preserved. | `runs/20260526-canon6-provider-probe/report.md` |

## Current Summary

| Metric | Value |
|---|---:|
| Runs counted | 1 |
| Completed runs | 1 |
| Structural smoke runs | 0 |
| Prompt catalog runs | 1 |
| Provider endpoint attached | `127.0.0.1:43117` |
| `/v1/models` status | HTTP 200 |
| `/v1/chat/completions` status | HTTP 200 |
| Prompt payload preserved | Yes |
| Benchmark executed | No |

The root notebook follows the same structure as the filesystem-loop notebook:
environment setup first, explicit workload/probe cells next, then compact run
metrics review.

## Boundary

This lab reports local runtime evidence. It does not make hardware scaling,
production readiness or provider quality claims unless a run records the
required measurements and limitations.

No `pack-fixture/` is currently defined for this lab. If a future test needs
imported operational material, create it using
`docs/lab-standards/pack-fixture-contract.md`.

Non-goals:

- No production readiness claim.
- No hardware scaling claim.
- No provider quality claim.
- No hidden authentication material in generated outputs.

## How To Execute

Use `runbook.md` for the full terminal procedure. Use `notebook.ipynb` for the
manual analytical notebook path. Use `run.sh` to create a new compact run
folder under this lab.

Create the current provider-probe package with:

```bash
docs/labs/external-runtime/run.sh --slug canon6-provider-probe --prompt-id local-provider-ready
```

List provider prompt catalog entries with:

```bash
docs/labs/external-runtime/run.sh --list-prompts
```

For structural smoke only, use:

```bash
docs/labs/external-runtime/run.sh --slug <slug> --command "true"
```

Each run writes `transcript.md`, `report.md`, `metrics.json`,
`manifest.json` and supporting attachments under `assets/`.

After a run exists, compose report-first tables and figures with:

```bash
python3 docs/labs/_shared/bin/generate-run-figures.py --run-dir docs/labs/external-runtime/runs/<run> --update-report --overwrite
```
