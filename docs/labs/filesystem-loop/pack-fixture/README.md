# Filesystem Loop Pack Fixture

This directory is the filesystem-loop lab input fixture shaped toward existing
pack materialization doctrine. It is not a complete pack runtime, installer,
registry backend or source-root production pack.

The current useful material is in `policies/`. It represents policy,
projection and authority posture for the bounded filesystem-loop case. See
`pack-doctrine-mapping.md` for the fixture-to-doctrine mapping.

Current implementation posture:

```text
yai daemon run-filesystem-loop
```

materializes equivalent residue as `subject:policy-pack`, `policy_rule`,
`projection_rule`, `authority_scope` and graph evidence.

## Files

| Path | Role |
|---|---|
| `pack.yaml` | Minimal fixture identity and limitations. |
| `policies/filesystem-sandbox-policy.json` | Filesystem sandbox policy posture. |
| `policies/linenoise-terminal-interface-policy.json` | Terminal/interface authority posture. |
| `policies/model-case-projection-policy.json` | Model projection and authority posture. |
| `pack-doctrine-mapping.md` | Mapping to existing pack doctrine. |
