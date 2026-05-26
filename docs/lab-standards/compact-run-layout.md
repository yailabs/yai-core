# Compact Run Layout

Status: canonical compact layout.

New lab work should use compact lab-root notebooks and run-local reports.

## Test Folder

```text
docs/labs/<test>/
  README.md
  runbook.md
  notebook.ipynb
  run.sh
  pack-fixture/
  runs/
```

## Run Folder

```text
runs/YYYYMMDD-<run-slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

## Meanings

`transcript.md` is the raw chronological execution record. It contains
commands, output, prompts, responses, errors and notes in order. It must not
contain scientific frontmatter.

`report.md` is the analytical report for one run.

`manifest.json` is the artifact map for one run.

`metrics.json` is the normalized machine-readable metric package for one run.

`notebook.ipynb` is the canonical executable notebook for the lab/test. It
contains operational cells, prompt/workload cells when applicable, and local
metric review cells.

`README.md` is the lab/test entry point and protocol.

`runbook.md` is the terminal procedure. It may be a static Markdown copy of the
notebook in the same order.

`run.sh` is the lab-local helper for creating new compact run folders.

`pack-fixture/` is optional lab/test input material shaped toward pack
doctrine. It is not a pack runtime, installer or production source root.

`assets/` contains logs, prompts, JSON, SVG, HTML, CSV or other files needed by
that run. Notebook copies do not belong in run assets.

## Rules

- Do not create separate aggregate directories.
- Do not create permanent lab archives; git history is the archive.
- Do not use `raw/`, `normalized/`, `figures/`, `diagrams/`, `tables` or
  `exports/` as mandatory run subfolders for new runs.
- Do not place README files inside run folders or run assets.
- Keep exactly one canonical notebook at the lab/test root.
- Do not use separate manual or protocol files in canonical labs.
- Keep `runbook.md` aligned with the lab-root notebook.
- Runs must identify which `pack-fixture/` material was used when applicable.
