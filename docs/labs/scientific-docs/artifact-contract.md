# Artifact Contract

Artifacts are files or references that allow a reader to inspect the basis of a
document.

Valid artifact classes:

```text
raw data
normalized data
scripts
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
```

## Artifact Rules

Public interpretation must point back to artifacts or explicitly declare that no
artifact exists yet.

Raw data and normalized data must not be conflated.

Generated figures must identify their source data when the source exists.

Logs may be artifacts, but logs alone do not create a claim.

Reports may cite other reports, but a report is not a substitute for raw or
normalized evidence when a claim depends on measured data.

No artifact may include secrets.

## Run-Scoped Artifacts

Test runs and generated visual artifacts must follow the shared run and visual
artifact contracts.

Each numbered run owns:

```text
raw/
normalized/
figures/
diagrams/
tables/
exports/
```

Raw data is never edited in place. Normalized data is derived. Figures,
diagrams and tables are derived from normalized data or explicitly declared
manual sources.

Generated visuals must be listed in a manifest before a report treats them as
report artifacts.
