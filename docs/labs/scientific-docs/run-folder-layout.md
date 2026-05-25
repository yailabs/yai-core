# Run Folder Layout

Each numbered test run uses this layout:

```text
docs/labs/<lab>/
  tests/
    T001-<test-slug>/
      test.md
      runs/
        R001-<run-slug>/
          run.md
          manifest.json
          raw/
          normalized/
          figures/
          diagrams/
          tables/
          exports/
```

## Directory Contracts

`raw/`: data, logs and outputs captured from the run before cleanup.

`normalized/`: derived data cleaned for analysis and visual generation.

`figures/`: generated static charts and figures.

`diagrams/`: schematics, topologies and flow diagrams.

`tables/`: Markdown or CSV tables.

`exports/`: final report exports or bundles.

## Rules

Raw is never edited.

Normalized is derived.

Figures, diagrams and tables are derived from normalized data or declared manual
sources.

Each run produces new clean assets inside its own folder.

No shared graphic is overwritten by later runs.
