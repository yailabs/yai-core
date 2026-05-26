# Run Artifact Contract

Every numbered run must be self-contained.

Required compact run layout:

```text
transcript.md
report.md
manifest.json
metrics.json
assets/
```

## Human and Tool Surfaces

`transcript.md` is the chronological execution record. It contains raw
commands, output, prompts, responses, errors and notes in order. It must not
contain scientific frontmatter.

`report.md` is the analytical report for the single run.

`manifest.json` is the machine-readable run artifact map.

`metrics.json` is the machine-readable metric package.

All run documents must point to the same run ID or run slug.

## Directory Roles

`assets/` contains logs, prompt artifacts, normalized JSON, figures, diagrams,
tables or generated report derivatives. Notebook copies do not belong in run
assets.

## Rules

- Prompted runs must record the prompt or message payload used.
- Run assets must be listed in `manifest.json`.
- Run metrics must be listed in `metrics.json`.
- Figures, diagrams and tables are run assets when they belong to a single run.
- The lab-root `notebook.ipynb` may read run metrics and regenerate charts or
  tables.
- No report should point to loose images without a manifest entry.
