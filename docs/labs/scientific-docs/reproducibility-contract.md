# Reproducibility Contract

Every document with experiments must identify enough material to rerun, audit
or falsify the work.

Required reproducibility fields:

```text
commit
hardware/software environment when relevant
commands
inputs
configs
outputs
known nondeterminism
artifact paths
```

For numbered test runs, reproducibility also requires:

```text
test_id
run_id
manifest.json
raw paths
normalized paths
visual manifests when visuals exist
```

## Rules

- Use exact commands when possible.
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
environment notes
limitations
```
