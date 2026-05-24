# Test 00 -- Repository Health

Status: current

## Purpose

Verify that the repository can report its current status and run the broad
repository check path.

## Prerequisites

- Work from the repository root.
- Use the local toolchain expected by the Makefile.

No model, provider, daemon, API key, or manual runtime setup is required.

## Commands

```sh
make info
make check
```

## Expected Behavior

`make info` prints repository status and main path information.

`make check` runs the current layout checks, documentation checks, build steps,
and smoke tests.

## Failure Interpretation

- Layout failures usually mean repository paths or required files are out of
  sync with current guards.
- Documentation failures usually mean public or engineering docs no longer meet
  guard expectations.
- Build and smoke failures should be read from the failing target output.
- Failures from unrelated dirty work should be reported separately.

## Deeper References

- [Public test cases](../../../docs/test-cases.md)
- [Testing doctrine](../../../docs/engineering/testing.md)
- [Repository README](../../../README.md)
