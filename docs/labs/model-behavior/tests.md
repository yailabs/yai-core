# Model Behavior Lab Tests

Status: canonical lab test expectation map.

## Artifact Checks

| Area | Expected signal |
|---|---|
| Boundary statement | Artifact says it is not a benchmark, guarantee or conformance fixture. |
| Authority posture | Model output is described as claim/proposal material. |
| Sanitization | No secrets, raw auth headers or private local paths. |
| Source run | Artifact points to the owning runbook or source lab. |
| Raw logs | Raw captures remain outside git or in ignored local roots. |

## Suggested Review

```bash
rg -n "Authorization:|api[_-]?key|Bearer|/Users/|/home/" docs/labs/model-behavior
```

Any hit must be reviewed before publication.
