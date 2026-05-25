# Model Behavior Lab Runbook

Status: canonical lab runbook.

Purpose: curate representative model behavior artifacts from YAI case runs.

## Source Run

Use the owning operational lab to produce the case state. For filesystem-loop
model behavior, run:

```text
docs/labs/filesystem-loop/runbook.md
```

## Capture

Capture model-facing projections, provider readiness posture and selected model
responses. Keep raw logs outside the repository until sanitized.

## Sanitize

Before committing a curated artifact:

- replace absolute local paths with placeholders;
- replace local ports where needed;
- remove secrets and environment values;
- state that model output is claim/proposal material;
- state that the artifact is not a benchmark and not a conformance fixture.

## Publish

Curated behavior artifacts live as Markdown files in this directory. Larger
supporting reports should live under `reports/`.
