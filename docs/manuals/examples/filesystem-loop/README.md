# Filesystem Loop Examples

These files are example material for
`docs/manuals/manual-filesystem-loop-validation.md`.

They are not an installed policy registry and they are not a full policy engine.
They are versioned fixtures for the manual filesystem-loop case so the runbook
can copy stable inputs into `build/tmp/manual-case-001` or an installed
`YAI_HOME` test case directory.

Current implementation note:

```text
yai daemon run-filesystem-loop
```

materializes this fixture posture into journal residue as
`subject:policy-pack`, `policy_rule`, `projection_rule`, `authority_scope` and
graph evidence. There is no separate `yai pack install` or `yai pack
materialize` command in the current core.
