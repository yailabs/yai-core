# External Runtime Lab

Status: canonical lab for LAN provider and external runtime readiness checks.

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

- LAN endpoint probes.
- Provider status classification.
- Authentication boundary classification.
- Model inventory when `/v1/models` is reachable.
- Small HNSW smoke panels when present.
- Prompt/request payload preservation for provider calls.
- Root notebook summaries over compact run folders.

## Protocol

Research question: can the operator host observe a LAN provider endpoint,
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

## Current Runs

| Run | Status | Notes | Report |
|---|---|---|---|
| `20260525-lan-provider` | completed | Includes `43117` live evidence and `44317` negative evidence. | `runs/20260525-lan-provider/report.md` |
| `20260526-lan-provider` | completed | Includes `43117` live evidence and `44317` negative evidence. | `runs/20260526-lan-provider/report.md` |
| `20260526-43117-provider-only` | completed | Current clean provider-port run; no `44317` probe in the curated set. | `runs/20260526-43117-provider-only/report.md` |

## Current Summary

| Metric | Value |
|---|---:|
| Runs counted | 3 |
| Completed runs | 3 |
| Current live provider port | 43117 |
| Current clean run includes obsolete port | No |
| Current `/v1/models` status | reachable |
| Current `/v1/chat/completions` status | auth_blocked |
| Current HNSW query ms/query | 2.911083 |

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
