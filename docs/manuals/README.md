# Manuals

This folder contains operator-facing runbooks for validating and debugging
`yai-core` behavior.

Manuals do not define architecture, protocol contracts or product claims. They
describe concrete local procedures for running the current core implementation
and preserving evidence from those runs.

## Manual Map

- `manual-filesystem-loop-validation.md` - local daemon filesystem loop,
  command-surface baseline, hot-state validation, case entry, provider
  attachment and vendored prompt-surface runbook.
- `manual-filesystem-loop-validation.ipynb` - notebook companion for the common
  command-surface and LAN-provider validation path.
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
docs/manuals/manual-filesystem-loop-validation.md
docs/manuals/manual-filesystem-loop-validation.ipynb
```

The filesystem loop manual is the single active manual test path. It owns both
the command-surface baseline and the provider/case prompt workflow.
