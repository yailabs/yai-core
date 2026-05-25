# Model Behavior Lab

Status: canonical curated artifact lab.

Purpose: keep representative model behavior examples produced inside YAI cases
without treating them as benchmarks, conformance fixtures or security claims.

## Standard Files

| Concern | Path |
|---|---|
| Purpose and map | `docs/labs/model-behavior/README.md` |
| Local test index | `docs/labs/model-behavior/test-index.md` |
| Historical material guide | `docs/labs/model-behavior/historical-material.md` |
| Runbook | `docs/labs/model-behavior/runbook.md` |
| Historical test notes | `docs/labs/model-behavior/tests.md` |
| Numbered tests | `docs/labs/model-behavior/tests/` |
| Output contract | `docs/labs/model-behavior/outputs.md` |
| Raw placeholder | `docs/labs/model-behavior/raw/` |
| Curated reports | `docs/labs/model-behavior/reports/` |

## Curated Artifacts

| Artifact | Path |
|---|---|
| Filesystem loop Qwen3 8B Q4 | `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md` |

## Numbered Tests

The canonical local index is `docs/labs/model-behavior/test-index.md`.
New numbered tests follow the shared scientific documentation, run and visual
artifact contracts from `docs/labs/scientific-docs/`.

| Test ID | Title | Latest run |
|---|---|---|
| MODEL.T001 | Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline | `docs/labs/model-behavior/tests/MODEL.T001-filesystem-loop-qwen3-8b-q4/runs/R001-historical-baseline/run.md` |

## Historical Material

Historical lab files are documented in
`docs/labs/model-behavior/historical-material.md`. They remain available for
reference, but new work should use numbered tests/runs unless an explicit
migration wave says otherwise.

## Boundary

These files are representative local runs. They are useful for architecture and
debug review, but they are not benchmarks, security guarantees or conformance
fixtures.

Raw local logs should not be committed. Public artifacts must sanitize absolute
paths, local ports where needed, secrets or environment values and broken
encoding.

Canonical run procedures live in `docs/labs/filesystem-loop/`. Benchmark
reports live in `docs/labs/nvidia/reports/`.
