---
id: MODEL.T001
type: model_behavior_test
title: Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline
status: active
version: 0.1.0
date: 2026-05-25
authors: []
project: YAI
lab: model-behavior
tags:
  - model-behavior
  - filesystem-loop
  - qwen3
  - historical-baseline
summary: Numbered test definition for the historical filesystem-loop Qwen3 8B Q4 model behavior baseline.
latest_run: MODEL.T001.R001
report: runs/R001-historical-baseline/run.md
claims: []
artifacts:
  - runs/R001-historical-baseline/manifest.json
  - runs/R001-historical-baseline/normalized/normalized-summary.json
  - runs/R001-historical-baseline/diagrams/D001-filesystem-loop-flow.md
  - runs/R001-historical-baseline/tables/TBL001-run-summary.md
related:
  - ../../filesystem-loop-qwen3-8b-q4.md
supersedes:
superseded_by:
---

# Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline

## Test Identity

| Field | Value |
|---|---|
| Test ID | MODEL.T001 |
| Title | Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline |
| Status | active |
| Lab | model-behavior |
| Type | model behavior test |

## Purpose

Define the first numbered model-behavior test for the historical filesystem-loop
Qwen3 8B Q4 lab material. This test creates a stable place for future runs to
record prompts, policy constraints, outputs, derived summaries and visual
artifacts without rewriting the historical report.

The initial run, `MODEL.T001.R001`, is a historical baseline normalization. It
is not a new execution and does not make a quantitative benchmark claim.

## Research Question

Can a local language model participate in a bounded filesystem-loop task while
preserving observable inputs, outputs, policy constraints and run artifacts?

## Hypothesis

A bounded filesystem-loop run can be documented as a reproducible
model-behavior test if prompts, policy constraints, outputs, raw artifacts and
derived summaries are separated by run.

## Scope

- Local model behavior in a bounded filesystem-loop task.
- Historical Qwen3 8B Q4 lab material already present in this repository.
- Documentation structure, artifact separation and run traceability.
- Future compatibility with numbered test and run contracts under
  `docs/labs/scientific-docs/`.

## Non-Goals

- No claim of model general intelligence.
- No claim of production safety.
- No benchmark claim.
- No comparison against other models.
- No migration of historical raw files.

## Expected Artifacts

Each run for this test is expected to keep its own:

- `run.md`
- `manifest.json`
- `raw/`
- `normalized/`
- `figures/`
- `diagrams/`
- `tables/`
- `exports/`

## Run List

| Run ID | Folder | Status | Notes |
|---|---|---|---|
| MODEL.T001.R001 | `runs/R001-historical-baseline/` | historical_baseline | Normalizes the current historical lab state without executing a new model run. |

## Historical Boundary

Historical files such as `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md`,
`docs/labs/model-behavior/outputs.md`, `docs/labs/model-behavior/raw/` and
`docs/labs/model-behavior/reports/` are references for this test. They are not
part of the numbered run output unless a future migration wave explicitly moves
or derives artifacts from them.

## References

- Historical lab note: `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md`
- Local test index: `docs/labs/model-behavior/test-index.md`
- Historical material guide: `docs/labs/model-behavior/historical-material.md`
- Shared contracts: `docs/labs/scientific-docs/`
