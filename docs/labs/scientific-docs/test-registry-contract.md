# Test Registry Contract

The central lab test registry lives at:

```text
docs/labs/test-registry.md
```

It prevents numbered tests from becoming detached from their lab, latest run or
report.

## Required Columns

```text
| Test ID | Lab | Title | Status | Latest run | Report |
|---|---|---|---|---|---|
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

- Every numbered test should appear in the registry.
- `Latest run` may be empty for draft tests.
- `Report` may be empty when no report exists yet.
- Historical reports remain in place until explicitly migrated.
- The registry records location and state; it does not validate scientific
  truth.
