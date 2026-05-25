# Figure Generation Contract

This contract defines how generated figures should be produced and tracked.

## Rules

No manual random image is valid unless marked manual.

Every generated chart must declare source data.

Every generated chart must be reproducible from normalized data or a script.

Every run must keep its own generated visuals.

Generated visuals must not overwrite previous run visuals.

Every figures directory must include `figure-manifest.json` when figures exist.

## Required Provenance

Each figure manifest entry must state:

```text
id
type
title
source_run
source_data
generation_method
output_path
format
created_at
```

If the source is manual, `source_data` must identify the manual source or state
`manual_source_declared`.
