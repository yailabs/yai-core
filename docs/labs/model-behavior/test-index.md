# Model Behavior Test Index

Status: local numbered-test index.

This index is the canonical entry point for numbered model-behavior tests.
Historical lab material remains available, but new work should be added under
numbered tests and runs.

| Test ID | Title | Status | Latest run | New execution | Quantitative benchmark | Notes |
|---|---|---|---|---|---|---|
| MODEL.T001 | [Filesystem Loop / Qwen3 8B Q4 Model Behavior Baseline](tests/MODEL.T001-filesystem-loop-qwen3-8b-q4/test.md) | active | [MODEL.T001.R001](tests/MODEL.T001-filesystem-loop-qwen3-8b-q4/runs/R001-historical-baseline/run.md) | No | No | historical baseline normalization |

## Local Rules

- Use `docs/labs/model-behavior/tests/` for new numbered tests.
- Use `runs/R###-slug/` for new runs under an existing numbered test.
- Keep raw, normalized, figures, diagrams, tables and exports separated per
  run.
- Do not edit historical lab material unless an explicit migration wave asks
  for it.
- Do not treat historical baseline runs as benchmark evidence.

## Related Files

- Numbered tests root: `docs/labs/model-behavior/tests/`
- Historical material guide: `docs/labs/model-behavior/historical-material.md`
- Shared documentation contracts: `docs/labs/scientific-docs/`
- Central registry: `docs/labs/test-registry.md`
