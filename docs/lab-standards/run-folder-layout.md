# Run Folder Layout

Each compact test run uses this layout:

```text
docs/labs/<lab>/
  README.md
  runbook.md
  notebook.ipynb
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

`report.md`: analytical report for the single run.

`manifest.json`: run artifact map.

`metrics.json`: normalized metrics for the single run.

`notebook.ipynb`: canonical executable notebook for the lab/test. It may run
operational cells, prompt/workload cells when applicable, and local metric
review cells.

`README.md`: lab entry point and protocol.

`runbook.md`: executable terminal procedure. It may mirror `notebook.ipynb`.

`run.sh`: local helper that creates a compact run folder under this lab.

`pack-fixture/`: optional lab/test input material shaped toward pack doctrine.
It is not a pack runtime, installer or production source root.

`assets/`: logs, prompts, JSON, SVG, HTML, CSV or generated files used by the
run. Notebook copies do not belong here.

## Rules

- A run must be understandable from `transcript.md` and `report.md`.
- A test must be understandable from `README.md`, `runbook.md`,
  `notebook.ipynb` and `run.sh`.
- The notebook is the executable source; the runbook is the static terminal
  copy.
- Canonical labs do not use separate manual or protocol files.
- Assets are attachments; they are not the primary report surface.
- `report.md` is the per-run analytical artifact.
- No separate aggregate directory is canonical.
- Runs must identify `pack-fixture/` material when fixture input affects setup,
  materialization posture or interpretation.
- New runs must not use mandatory `raw/`, `normalized/`, `figures/`,
  `diagrams/`, `tables` or `exports/` nesting.
