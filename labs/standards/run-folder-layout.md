# Run Folder Layout

Each compact test run uses this layout:

```text
labs/<lab>/
  README.md
  runbook.md
  notebook.ipynb
  prompts.json
  run.sh
  pack-fixture/
  runs/
    YYYYMMDD-<run-slug>/
      transcript.md
      report.md
      manifest.json
      metrics.json
      assets/
```

## Directory Contracts

`transcript.md`: complete chronological execution record. It contains raw
commands, terminal output, prompts, responses, errors and ordered notes. It
does not carry scientific frontmatter.

`report.md`: analytical report for the single run. It is the final human
artifact and composes summary, execution, evidence tables, generated figures,
artifact index, missing measurements, interpretation, limitations and next
run.

`manifest.json`: run artifact map.

`metrics.json`: normalized metrics for the single run.

`notebook.ipynb`: canonical guided notebook for the lab/test. It uses short
explanations and official commands; it does not contain long orchestration
scripts.

`prompts.json`: canonical prompt/query catalog for the lab/test.

`README.md`: lab entry point and protocol.

`runbook.md`: complete executable terminal procedure.

`run.sh`: local helper that creates a compact run folder under this lab.

`pack-fixture/`: optional lab/test input material shaped toward pack doctrine.
It is not a pack runtime, installer or production source root.

`assets/`: logs, prompts, JSON, SVG, HTML, CSV or generated files used by the
run. Notebook copies do not belong here.

## Rules

- A run must be understandable from `transcript.md` and `report.md`.
- A test must be understandable from `README.md`, `runbook.md`,
  `notebook.ipynb` and `run.sh`.
- The notebook is a human-guided demo; the runbook is the complete terminal
  procedure.
- Prompt text authority lives in `prompts.json`; runbooks and notebooks may
  display prompts for human readability.
- Canonical labs do not use separate manual or protocol files.
- Assets are attachments; they are not the primary report surface.
- `report.md` is the per-run analytical artifact.
- Generated figures are valid only when they are derived from run evidence,
  listed in `manifest.json` and included in `report.md`.
- No separate aggregate directory is canonical.
- Do not create lab-level outputs, tests, reports or analytics directories for
  canonical lab evidence.
- Runs must identify `pack-fixture/` material when fixture input affects setup,
  materialization posture or interpretation.
- New runs must not use mandatory `raw/`, `normalized/`, `figures/`,
  `diagrams/`, `tables` or `exports/` nesting.
