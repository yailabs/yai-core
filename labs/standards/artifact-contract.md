# Artifact Contract

Artifacts are files or references that allow a reader to inspect the basis of a
document.

Valid artifact classes:

```text
raw data
normalized data
scripts
prompts
notebooks
figures
diagrams
tables
logs
configs
reports
snapshots
release refs
DOI refs when available
pack fixture material
```

## Artifact Rules

Public interpretation must point back to artifacts or explicitly declare that no
artifact exists yet.

Prompted interpretation must point back to a prompt artifact or explicitly
declare that the prompt is `Not available`.

In compact runs, artifacts are grouped under `assets/`; `manifest.json` and
`metrics.json` provide the machine-readable surface.

The lab-root `README.md`, `runbook.md`, `notebook.ipynb`, `run.sh` and
`prompts.json` are protocol/execution surfaces, not run attachments. A run may
copy the resolved prompt payload into `assets/`, but it must not copy the
lab-root notebook into run assets.

`transcript.md` is raw chronological execution evidence. `report.md` is the
final human analytical artifact for a run. `metrics.json` is normalized
machine-readable data used by the report and figures. `assets/` contains
attachments, including prompt payloads, logs, generated tables and SVG figures.

Raw data and normalized data must not be conflated.

Generated figures must identify their source data when the source exists.
Generated figures are supporting evidence inside `report.md`; they are not the
report itself.

Logs may be artifacts, but logs alone do not create a claim.

Reports may cite other reports, but a report is not a substitute for raw or
normalized evidence when a claim depends on measured data.

No artifact may include secrets.

Pack fixture material is an artifact when it is used as lab/test input. Reports
must distinguish fixture files from materialized case state and observed run
outputs.

## Compact Run-Scoped Artifacts

Test runs and generated visual artifacts must follow the shared run and visual
artifact contracts.

Each compact run owns:

```text
transcript.md
report.md
manifest.json
metrics.json
assets/
```

Raw data, normalized data, figures, diagrams, tables, prompts, logs and
notebooks are attachments under `assets/` only when explicitly declared by a
lab-specific profile. The canonical notebook is at lab root.

Generated visuals must be listed in `manifest.json` before a report treats them
as report artifacts.

Canonical labs do not preserve separate manuals, lab-level outputs files or
lab-level tests files as active artifacts.

Generated figure indexes, such as `assets/generated-figures.json`, are run
artifacts. They must be based on existing run evidence and should state when a
candidate chart was omitted because source data was unavailable.

Generated report tables, such as `assets/generated-tables.md`, are run
artifacts. They should be referenced from `report.md` when they summarize the
same evidence used by generated figures.
