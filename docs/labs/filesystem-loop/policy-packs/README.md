# Filesystem Loop Policy-Pack Fixtures

These JSON files are lab fixtures for the filesystem-loop case. They are not an
installed policy registry and they are not a general policy engine.

Current implementation posture:

```text
yai daemon run-filesystem-loop
```

materializes equivalent residue as `subject:policy-pack`, `policy_rule`,
`projection_rule`, `authority_scope` and graph evidence.
