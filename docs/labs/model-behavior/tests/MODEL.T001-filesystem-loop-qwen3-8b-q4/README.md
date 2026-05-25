# MODEL.T001 - Filesystem Loop / Qwen3 8B Q4

Status: active numbered model-behavior test.

This directory contains the first numbered model-behavior test. It follows the
shared test/run layout defined in `docs/labs/scientific-docs/`.

## Test Files

| Item | Path | Notes |
|---|---|---|
| Test definition | `test.md` | Defines MODEL.T001 and its scope. |
| Latest run | `runs/R001-historical-baseline/run.md` | Historical normalization only. |
| Run manifest | `runs/R001-historical-baseline/manifest.json` | Machine-readable run artifact map. |

## Runs

| Run ID | Folder | Status | New execution | Quantitative benchmark |
|---|---|---|---|---|
| MODEL.T001.R001 | `runs/R001-historical-baseline/` | historical baseline | No | No |

## Future Runs

Future runs must be added under:

```text
runs/R###-<run-slug>/
```

Each future run must keep its own `run.md`, `manifest.json`, `raw/`,
`normalized/`, `figures/`, `diagrams/`, `tables/` and `exports/` folders. Do
not overwrite or extend `R001-historical-baseline` for a new execution.
