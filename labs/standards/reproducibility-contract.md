# Reproducibility Contract

Every document with experiments must identify enough material to rerun, audit
or falsify the work.

Required reproducibility fields:

```text
commit
hardware/software environment when relevant
commands
prompts when a model/provider/agent is called
prompt catalog path and prompt ID when applicable
inputs
configs
pack fixture paths when used
outputs
known nondeterminism
artifact paths
```

For numbered test runs, reproducibility also requires:

```text
test_id
run_id
manifest.json
transcript.md
report.md
assets paths
prompts.json
prompt assets when applicable
metrics.json
lab-root notebook when applicable
```

## Rules

- Use exact commands when possible.
- Keep `transcript.md` raw and chronological.
- Use `report.md` for interpretation, tables, figure references, missing
  measurements and limitations.
- Keep notebooks as human-guided demos. Complete rerun procedure belongs in
  `runbook.md`; run capture belongs in `run.sh`; reusable implementation
  belongs in `labs/shared/bin/`.
- Record prompt text or message payloads for prompted runs as assets.
- Record `pack-fixture/` files used by a run when fixture input material affects
  the case setup or interpretation.
- Link config files instead of copying large content.
- Identify generated outputs separately from curated reports.
- Generated figures must be reproducible from committed run metrics/assets and
  referenced from both `report.md` and `manifest.json`.
- State known nondeterminism, including random seeds, remote services,
  scheduler effects and timing variance.
- If a required field is unavailable, write `Not recorded`.

## Minimum Package

```text
method description
command list
input refs
config refs
output refs
run manifest
run transcript
run metrics
environment notes
limitations
```
