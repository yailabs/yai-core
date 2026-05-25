# Run Artifact Contract

Every numbered run must be self-contained.

Required run layout:

```text
run.md
manifest.json
raw/
normalized/
figures/
diagrams/
tables/
exports/
```

## Human and Tool Surfaces

`run.md` is the human-readable run report.

`manifest.json` is the machine-readable run manifest.

Both must point to the same run ID.

## Directory Roles

`raw/` contains raw data, logs and uncleaned command output.

`normalized/` contains cleaned data for analysis and visual generation.

`figures/` contains generated static figures and charts.

`diagrams/` contains schematics, flows and topology diagrams.

`tables/` contains readable tables in Markdown or CSV.

`exports/` contains final exported bundles or report derivatives.

## Rules

- Raw data is never edited in place.
- Normalized data is derived.
- Figures, diagrams and tables are derived from normalized data or declared
  manual sources.
- Each run owns its own generated visuals.
- No report should point to loose images without a manifest.
