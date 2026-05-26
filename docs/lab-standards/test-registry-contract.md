# Test Registry Contract

The central lab test registry lives at:

```text
docs/labs/registry.md
```

It prevents compact lab/test folders from becoming detached from their manual,
runbook, protocol, notebook and latest run.

## Required Columns

```text
| Lab | Purpose | Manual | Runbook | Test | Notebook | Latest run |
|---|---|---|---|---|---|---|
```

## Status Values

```text
draft
active
frozen
archived
superseded
```

## Rules

- Every canonical lab/test folder should appear in the registry.
- `Latest run` may be empty for draft labs.
- Historical reports remain in place until explicitly migrated.
- The registry records location and state; it does not validate scientific
  truth.
- The registry does not own run output.
