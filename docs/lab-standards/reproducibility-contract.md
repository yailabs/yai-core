# Reproducibility Contract

Every document with experiments must identify enough material to rerun, audit
or falsify the work.

Required reproducibility fields:

```text
commit
hardware/software environment when relevant
commands
prompts when a model/provider/agent is called
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
assets paths
prompt assets when applicable
metrics.json
lab-root notebook when applicable
```

## Rules

- Use exact commands when possible.
- Record prompt text or message payloads for prompted runs as assets.
- Record `pack-fixture/` files used by a run when fixture input material affects
  the case setup or interpretation.
- Link config files instead of copying large content.
- Identify generated outputs separately from curated reports.
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
