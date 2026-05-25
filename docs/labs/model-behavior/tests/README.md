# Model Behavior Numbered Tests

New numbered tests for this lab should be created under this directory using
the `scientific-docs` test/run layout.

Existing historical reports remain in their current paths until explicitly
migrated.

Local navigation lives in `docs/labs/model-behavior/test-index.md`.
Historical material rules live in
`docs/labs/model-behavior/historical-material.md`.

## Registered Tests

| Test ID | Title | Status | Latest run | New execution | Quantitative benchmark |
|---|---|---|---|---|---|
| MODEL.T001 | Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline | active | `MODEL.T001-filesystem-loop-qwen3-8b-q4/runs/R001-historical-baseline/` | No | No |

## Rule

Each numbered test owns its own `runs/` directory. Each run owns its own
`raw/`, `normalized/`, `figures/`, `diagrams/`, `tables/` and `exports/`
folders.

Future agents should create new run folders instead of editing historical loose
files or extending an old run folder for a distinct execution.
