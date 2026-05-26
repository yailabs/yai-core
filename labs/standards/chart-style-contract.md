# Chart Style Contract

Charts should be technical, legible and reproducible.

Generated charts should be produced from run data with reproducible code,
preferably Python using numpy, pandas and matplotlib for current lab figure
work. Hand-edited charts must be marked as manual visual artifacts.

If a run does not contain the source data needed for a chart, do not create the
chart. Missing evidence is a limitation, not an invitation to infer values.

Figures are not the report. They are visual evidence inside `report.md`.
Every generated figure must be linked from `report.md`, listed in
`manifest.json` and indexed from `assets/generated-figures.json`. If a figure
does not appear in the report, it should not be generated.

Use markdown tables before charts when the evidence is only two or three
categorical or binary values. Presence/absence charts are allowed as diagnostic
figures, not as primary benchmark figures.

## Evidence Categories

Report-first figures and tables should fit one of these categories:

- Execution evidence: command exit status, duration when measured, stdout or
  stderr availability, prompt/request count, response count and error count.
- Runtime/model/provider evidence: endpoint status, model list availability,
  chat/completion status, response token or timing fields and provider
  readiness when measured.
- Filesystem-loop evidence: prompt sequence, command sequence, model response
  sequence, receipt/output presence, pack-fixture material used and mutative
  operation posture when observed.
- Benchmark/measurement evidence: latency, throughput, tokens/sec, VRAM, HNSW
  query latency or similar metrics only when quantitative measurements exist.
- Report tables: small categorical summaries, artifact indexes, metrics tables
  and missing-measurement tables.

## Chart Rules

- Title is brief.
- Subtitle is optional and technical.
- Axes are always labeled.
- Units are always declared.
- Colors are not random.
- No decorative 3D for quantitative charts.
- Legend appears only when necessary.
- Source data is declared.
- Run ID is visible in metadata or caption.
- The generation command or notebook cell is reproducible from committed run
  data.
- Generated chart files are referenced from both `report.md` and
  `manifest.json`.
- Unsupported charts are omitted and reported as `Not measured`.
- SVG is the default static figure format.

## Diagram Rules

- Use few nodes.
- Name relationships.
- Show direction clearly.
- Avoid random icons.
- Do not include a claim that is not supported by the text.

## Caption Rules

Captions should state:

```text
what was measured or represented
source run
source data
units when relevant
known limitations
```
