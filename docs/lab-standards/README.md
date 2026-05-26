# Lab Standards

Status: LAB.DOCS.0.

`lab-standards` defines the documentation rules used by YAI lab work.

It does not define a product surface. It does not define public narrative. It
does not validate scientific truth. It defines structure, metadata, evidence
discipline, reproducibility and review rules.

This directory is not a lab. It is not a test. It is methodology, contracts and
templates for authoring lab, test, run and report documents.

## Contents

| File | Purpose |
|---|---|
| `document-taxonomy.md` | Supported document types and when to use them. |
| `document-structure-contract.md` | Common section spine. |
| `metadata-contract.md` | Shared frontmatter fields and statuses. |
| `claims-evidence-results-contract.md` | Claim, evidence, result and interpretation discipline. |
| `reproducibility-contract.md` | Required reproducibility fields for experimental work. |
| `artifact-contract.md` | Valid artifact classes and artifact link rules. |
| `prompt-artifact-contract.md` | Required prompt provenance for prompted runs. |
| `pack-fixture-contract.md` | Lab input fixture rules aligned with pack doctrine. |
| `run-numbering-contract.md` | Test, run and visual artifact numbering. |
| `compact-run-layout.md` | Canonical compact lab notebook and run layout. |
| `run-folder-layout.md` | Standard per-run folder structure. |
| `visual-artifact-contract.md` | Required provenance for figures, diagrams, charts and tables. |
| `test-registry-contract.md` | Central test registry shape. |
| `agent-authoring-protocol.md` | Rules for agent-authored lab documents. |
| `templates/` | Placeholder-driven document templates. |
| `schemas/` | Initial v0 JSON schemas. |
| `examples/` | Minimal examples of valid document shapes. |

## Non-Goals

- No website route.
- No public narrative.
- No social material.
- No hardware-vendor-specific positioning.
- No rewriting existing lab reports.
- No benchmark execution.
- No PDF export.
- No external archive integration.
- No validation CLI.
- No scientific truth claims.

## LAB.DOCS.1 Additions

Test runs and generated visual artifacts must follow the compact run contract.
Each lab/test owns one canonical root `notebook.ipynb`. Each execution lives
under `runs/YYYYMMDD-<run-slug>/` with `transcript.md`, `report.md`,
`manifest.json`, `metrics.json` and `assets/`.

`README.md` owns the lab purpose, scope and protocol. `runbook.md` is the
terminal procedure and may be a static Markdown copy of the canonical notebook.
`run.sh` is the local helper that creates new compact run folders.

No separate aggregate directory is canonical.

Prompted runs must also keep prompt artifacts as run assets. A benchmark that
calls a provider or model without recording its prompt is incomplete.

## Layout Guards

Run these checks after editing lab structure or run packages:

```bash
make check-labs
make check-lab-runs
```

The first command validates lab root files, optional pack fixtures and local
run scripts. The second validates compact run folders, transcripts, reports,
manifests, metrics and run assets.
