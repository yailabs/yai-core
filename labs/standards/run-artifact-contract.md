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

`report.md` is the analytical report for the single run. It interprets the
transcript, metrics and assets; it is the primary human artifact.

`manifest.json` is the machine-readable run artifact map.

`metrics.json` is the machine-readable metric package.

All run documents must point to the same run ID or run slug.

## Directory Roles

`assets/` contains logs, prompt artifacts, normalized JSON, figures, diagrams,
tables or generated report derivatives. Notebook copies do not belong in run
assets.

## Rules

- Prompted runs must record the prompt or message payload used.
- Transcript output should be raw and chronological. If output is too large for
  inline reading, the transcript must still point to complete assets.
- If prompts/queries are used, the transcript must include prompt ID and
  payload path.
- If model responses are produced, the transcript must include the response
  artifact path and either a bounded excerpt or a pointer.
- For current labs, prompt/query text authority lives in lab-root
  `prompts.json`; run assets keep the resolved prompt record used by that run.
- Run assets must be listed in `manifest.json`.
- Run metrics must be listed in `metrics.json`.
- Figures, diagrams and tables are run assets when they belong to a single run.
- Generated figures must be referenced from both `report.md` and
  `manifest.json`.
- `report.md` should include summary, what was executed, evidence table,
  metrics table, generated figures, artifact index, missing measurements,
  interpretation, limitations and next run.
- The lab-root `notebook.ipynb` may guide a human through official commands and
  inspect generated reports. It must not become the hidden automation script.
- No report should point to loose images without a manifest entry.
- A compact run must not use `run.md` as an active run surface.
