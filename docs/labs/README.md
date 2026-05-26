# Labs

Status: canonical home for real lab procedures, notebooks and runs.

`docs/labs/` owns material that can be run, repeated, curated or summarized.
Root stays light: shared contracts live in `docs/lab-standards/`, and each lab
owns its own README/protocol, runbook, notebook, run script, optional pack
fixture and runs.

## Canonical Root

```text
docs/labs/
  README.md
  registry.md
  filesystem-loop/
  external-runtime/
```

## Canonical Lab Shape

```text
docs/labs/<lab>/
  README.md
  runbook.md
  notebook.ipynb
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
docs/labs/registry.md
```

Root registry entries point to each lab purpose, runbook, notebook, pack
fixture posture and latest run.

## Lab Standards

Shared rules for lab notes, experiment reports, technical reports, papers,
benchmark reports, artifacts and agent-authored lab documents live in:

```text
docs/lab-standards/
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
- `runbook.md` is the executable terminal procedure.
- Each lab/test owns one canonical executable notebook at lab root.
- `runbook.md` is the static Markdown copy of the lab-root notebook.
- `run.sh` creates a compact run folder under its own lab.
- `pack-fixture/` is optional lab/test input material shaped toward pack
  doctrine. It is not a pack runtime or production source root.
- Every compact run has `transcript.md`, `report.md`, `manifest.json`,
  `metrics.json` and `assets/`.
- Historical expanded layouts should be folded into compact runs before use.
- Raw local benchmark output may be referenced, but the committed lab surface is
  the compact run package.
- Failed probes remain evidence and must not be hidden.
- No lab artifact may include API keys, authorization headers or secrets.
