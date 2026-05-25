# Model Behavior Historical Material

Status: historical material guide.

This file explains how to treat model-behavior artifacts that existed before
the numbered test/run layout.

## Historical Surfaces

The following paths are historical lab material:

| Path | Role | Handling |
|---|---|---|
| `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md` | Historical curated lab note | Keep in place unless a migration wave explicitly changes it. |
| `docs/labs/model-behavior/outputs.md` | Historical output contract/reference material | Keep in place unless a migration wave explicitly changes it. |
| `docs/labs/model-behavior/raw/` | Historical or local raw material area | Do not copy, clean or rewrite by default. |
| `docs/labs/model-behavior/reports/` | Historical report area | Do not rewrite by default. |
| `docs/labs/model-behavior/tests.md` | Historical test notes | Keep separate from numbered tests. |

## Canonical New Work

New work must use numbered tests and runs:

```text
docs/labs/model-behavior/tests/
  MODEL.T###-<test-slug>/
    test.md
    runs/
      R###-<run-slug>/
        run.md
        manifest.json
        raw/
        normalized/
        figures/
        diagrams/
        tables/
        exports/
```

The local index for numbered work is:

```text
docs/labs/model-behavior/test-index.md
```

## Agent Rule

Agents must not edit historical files unless the user explicitly requests a
migration or correction wave for those files. When creating new evidence,
reports or derived artifacts, agents must create or update a numbered run
instead of appending to historical loose files.

## Migration Boundary

Historical material is not deleted, moved or normalized by default. A future
migration wave must name the source files, target numbered test/run, expected
derived artifacts and validation steps before changing historical files.
