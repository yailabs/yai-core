# Scientific Documentation Spine

Status: LAB.DOCS.0.

`scientific-docs` defines the documentation rules used by YAI lab work.

It does not define a product surface. It does not define public narrative. It
does not validate scientific truth. It defines structure, metadata, evidence
discipline, reproducibility and review rules.

This directory is not a lab experiment. It is the shared documentation spine
that lab directories may use when creating notes, experiment reports, technical
reports, papers and benchmark reports.

## Contents

| File | Purpose |
|---|---|
| `document-taxonomy.md` | Supported document types and when to use them. |
| `document-structure-contract.md` | Common section spine. |
| `metadata-contract.md` | Shared frontmatter fields and statuses. |
| `claims-evidence-results-contract.md` | Claim, evidence, result and interpretation discipline. |
| `reproducibility-contract.md` | Required reproducibility fields for experimental work. |
| `artifact-contract.md` | Valid artifact classes and artifact link rules. |
| `run-numbering-contract.md` | Test, run and visual artifact numbering. |
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

Test runs and generated visual artifacts must follow the shared run and visual
artifact contracts. New lab tests use `<LABCODE>.T###`; runs use
`<TEST-ID>.R###`; visual artifacts use `<RUN-ID>.F###`, `<RUN-ID>.C###`,
`<RUN-ID>.D###` or `<RUN-ID>.TBL###`.
