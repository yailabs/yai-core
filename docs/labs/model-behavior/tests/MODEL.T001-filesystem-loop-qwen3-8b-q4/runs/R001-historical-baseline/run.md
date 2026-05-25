---
id: MODEL.T001.R001
type: test_run
title: Filesystem Loop / Qwen3 8B Q4 Historical Baseline
status: historical_baseline
version: 0.1.0
date: 2026-05-25
authors: []
project: YAI
lab: model-behavior
test_id: MODEL.T001
run_id: MODEL.T001.R001
tags:
  - model-behavior
  - filesystem-loop
  - qwen3
  - historical-baseline
summary: Historical baseline normalization for the first numbered model-behavior test.
claims: []
artifacts:
  - manifest.json
  - normalized/normalized-summary.json
  - diagrams/D001-filesystem-loop-flow.md
  - tables/TBL001-run-summary.md
related:
  - ../../../../filesystem-loop-qwen3-8b-q4.md
supersedes:
superseded_by:
---

# Filesystem Loop / Qwen3 8B Q4 Historical Baseline

## Run Summary

This run records the current historical filesystem-loop Qwen3 8B Q4 lab state
into the new numbered test/run layout. It does not execute a new model run.

This is a historical baseline normalization. It keeps the historical report in
place and records only derived metadata, a normalized summary, a textual
diagram and a run-summary table.

It is not benchmark evidence. Missing execution details remain marked as
`Not measured` or `Not available`.

## Test Reference

| Field | Value |
|---|---|
| Test ID | MODEL.T001 |
| Run ID | MODEL.T001.R001 |
| Test definition | `../../test.md` |
| Historical source | `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md` |

## Environment

| Field | Value |
|---|---|
| Hardware | Not measured |
| Runtime | Not available |
| Model backend | Not available |
| Model | Qwen3 8B Q4, from historical lab note |

## Inputs

- Historical artifact: `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md`
- Historical artifact: `docs/labs/model-behavior/outputs.md`
- New execution input: Not available

## Commands

No model command was executed for this run.

## Raw Outputs

No historical raw output was copied into this numbered run folder. Historical
raw files remain in their existing locations until an explicit migration wave.

## Normalized Outputs

- `normalized/normalized-summary.json`

## Figures

No generated figures are present for this historical baseline run.

No chart or SVG was generated for this run.

## Diagrams

- `diagrams/D001-filesystem-loop-flow.md`

## Tables

- `tables/TBL001-run-summary.md`

## Exports

Not available.

## Results

This run records document structure and artifact placement only. Quantitative
model measurements are not available.

| Metric | Value |
|---|---|
| Latency | Not measured |
| Tokens per second | Not measured |
| VRAM peak | Not measured |
| New model execution | No |
| Quantitative benchmark | No |

## Interpretation

The numbered layout can represent this historical model-behavior material
without mixing raw artifacts, normalized summaries, diagrams and tables. The
run does not prove model quality, safety or generality.

## Limitations

- Historical baseline only.
- No new model execution was performed.
- No quantitative benchmark is claimed.
- Historical raw files were not migrated.
- Missing environment details are recorded as `Not measured` or `Not available`.

## Reproducibility

This run is reproducible as a documentation normalization, not as a model
execution. Reproduction means regenerating the same metadata, summary, diagram
and table from the referenced historical lab note and current contracts.

## Artifact Manifest

- `manifest.json`
