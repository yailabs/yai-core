# Manuals

This folder contains operator-facing runbooks for validating and debugging
`yai-core` behavior.

Manuals do not define architecture, protocol contracts or product claims. They
describe concrete local procedures for running the current core implementation
and preserving evidence from those runs.

## Manual Map

- `manual-command-surface-validation.md` - single operator runbook for current
  runtime, daemon, hot-state and projection command validation.
- `manual-command-surface-validation.ipynb` - notebook companion for the
  command-surface validation runbook.
- `manual-filesystem-loop-validation.md` - local daemon filesystem loop,
  case entry, provider attachment and vendored prompt-surface runbook.
- `manual-filesystem-loop-validation.ipynb` - notebook companion for the common
  LAN-provider validation path.
- `manual-filesystem-loop-validation.it.ipynb` - Italian notebook companion
  with translated prompt labs.
- `requirements-notebook.txt` - Python packages for local notebook kernels.
- `examples/filesystem-loop/policy-packs/` - example policy pack material used
  by the filesystem loop manual.

## Command Update Rule

When a wave adds, removes or changes an operator command, update all three
surfaces in the same delivery:

```text
docs/engineering/command-surface.md
docs/manuals/manual-command-surface-validation.md
docs/manuals/manual-command-surface-validation.ipynb
```

Use the filesystem loop manual only for the provider/case prompt workflow.
Use the command-surface manual as the canonical manual test for command
availability, expected output and installed-runtime validation.
