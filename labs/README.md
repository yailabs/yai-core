# Labs

Status: canonical home for real lab procedures, notebooks and runs.

`labs/` owns material that can be run, repeated, curated or summarized.
Root stays light: shared contracts live in `labs/standards/`, and each lab
owns its own README/protocol, runbook, notebook, run script, optional pack
fixture, prompt catalog and runs.

## Canonical Root

```text
labs/
  README.md
  registry.md
  shared/bin/
  filesystem-loop/
  external-runtime/
```

## Canonical Lab Shape

```text
labs/<lab>/
  README.md
  runbook.md
  notebook.ipynb
  prompts.json
  run.sh
  pack-fixture/
  runs/
    YYYYMMDD-<slug>/
      transcript.md
      report.md
      manifest.json
      metrics.json
      assets/
```

## Registry

The canonical lab registry is:

```text
labs/registry.md
```

Root registry entries point to each lab purpose, runbook, notebook, pack
fixture posture, prompt catalog and latest run.

Current latest runs:

| Lab | Latest run |
|---|---|
| filesystem-loop | `filesystem-loop/runs/20260526-real-fsloop/` |
| external-runtime | `external-runtime/runs/20260526-canon6-provider-probe/` |

## Lab Standards

Shared rules for lab notes, experiment reports, technical reports, papers,
benchmark reports, artifacts and agent-authored lab documents live in:

```text
labs/standards/
```

Test runs and generated visual artifacts must follow the shared compact run and
visual artifact contracts.

## Layout Guard

Use these repository checks before adding or changing lab structure:

```bash
make check-labs
make check-lab-runs
```

`check-labs` validates the lab root surface. `check-lab-runs` validates each
compact run package.

## Rules

- `README.md` is the lab entry point and protocol.
- `runbook.md` is the complete executable terminal procedure.
- Each lab/test owns one canonical human-guided notebook at lab root.
- `notebook.ipynb` is a short guided demo that calls official commands; it is
  not hidden automation.
- `prompts.json` is the prompt/query authority for that lab.
- `run.sh` creates a compact run folder under its own lab.
- `run.sh --prompt-id <id>` executes exactly one catalog prompt/query and
  stores the resolved payload under run `assets/`.
- `labs/shared/bin/` contains shared lab helper tools; lab helpers do
  not live under repository-root `tools/`.
- `pack-fixture/` is optional lab/test input material shaped toward pack
  doctrine. It is not a pack runtime or production source root.
- Every compact run has `transcript.md`, `report.md`, `manifest.json`,
  `metrics.json` and `assets/`.
- `report.md` is the final human artifact for a run; generated tables and SVG
  figures support the report and must be listed in `manifest.json`.
- Historical expanded layouts should be folded into compact runs before use.
- Raw local benchmark output may be referenced, but the committed lab surface is
  the compact run package.
- Failed probes remain evidence and must not be hidden.
- No lab artifact may include API keys, authorization headers or secrets.
