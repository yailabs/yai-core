# Test Cases

This page is the public entrypoint for YAI validation paths that can be run or
inspected without reading internal engineering history.

These test cases document existing validation surfaces. They are not
benchmarks, provider compatibility claims, or production readiness claims.

## Test Status Legend

- `current`: can be run from the current repository surface.
- `manual`: requires local setup or a manually prepared runtime.
- `experimental`: exists, but output or behavior may change.
- `internal`: primarily maintainer or engineering validation.
- `planned`: documented target, not a current runnable test.

## Test Case Matrix

| Test | Status | Requires model | Requires daemon | Manual setup | Primary commands | What it proves |
|---|---|---:|---:|---:|---|---|
| `00-repository-health` | current | no | no | no | `make info`, `make check` | Repository status, layout, docs, build, and smoke validation posture. |
| `01-local-runtime-inspection` | manual | no | optional | yes | `yai doctor`, `yai hot status`, `yai store status`, `yai store summary` | Runtime path, hot-state, store, and local daemon inspection surfaces. |
| `02-filesystem-loop-lab` | lab | no | yes | yes | filesystem loop lab runbook | Existing daemon-backed filesystem loop, journal, receipts, projection, and hot-state evidence. |
| `03-model-behavior-lab` | experimental | yes | yes | yes | model-behavior lab artifact | Optional model/provider behavior evidence over a YAI case projection. |

## Test 00 -- Repository Health

Status: current

Purpose:
Verify that the repository can report its current surface and run the broad
repository check path.

Commands:

```sh
make info
make check
```

Expected behavior:

- `make info` prints the current repository status and main paths.
- `make check` runs layout checks, documentation checks, build steps, and smoke
  tests.

Failure interpretation:

- A layout or documentation failure usually means the repository surface is out
  of sync with guard expectations.
- A build or smoke failure should be read from the failing target before
  assuming the local runtime is broken.
- Failures caused by unrelated dirty work should be reported, not hidden.

Deeper reference:
[tests/cases/00-repository-health](../tests/cases/00-repository-health/README.md).

## Test 01 -- Local Runtime Inspection

Status: manual

Purpose:
Verify that local runtime, hot-state, store, and daemon inspection surfaces are
reachable without involving a model.

Commands, after building or installing the local binaries as described by the
current engineering command surface:

```sh
yai doctor
yai hot status
yai store status
yai store summary
```

Daemon status, info, loop, and shutdown commands are also part of the current
engineering command surface when a local `yaid` process and socket are prepared.

Expected behavior:

- `yai doctor` reports runtime paths and local surface status.
- `yai hot status` reports whether a hot-state snapshot is active or
  unavailable.
- `yai store status` reports record-store backend readiness.
- `yai store summary` reports aggregate record-store counts when the store is
  ready, or status information when it is missing or uninitialized.

This test does not require a provider, model, API key, or model server.

Deeper reference:
[tests/cases/01-local-runtime-inspection](../tests/cases/01-local-runtime-inspection/README.md).

## Test 02 -- Filesystem Loop Lab Validation

Status: manual

Purpose:
Use the filesystem-loop lab to inspect a daemon-backed operational
path with case material, policy material, filesystem receipts, projection, and
hot-state evidence.

Primary reference:
[Filesystem loop lab validation](labs/filesystem-loop/runbook.md).

Expected evidence includes:

- daemon status and info responses;
- a filesystem loop result with journal path, record count, receipt count, and
  decision/receipt outcomes;
- hot-state status showing active case/session/context material;
- projection inspection showing freshness material for a model consumer.

This test should remain manual until its local setup and output expectations are
split into a smaller public wrapper.

Deeper reference:
[tests/cases/02-filesystem-loop-manual](../tests/cases/02-filesystem-loop-manual/README.md).

## Test 03 -- Model Behavior Lab

Status: experimental

Purpose:
Reference the optional model-behavior lab generated through a local provider
and YAI case projection.

Primary reference:
[Model behavior lab artifacts](labs/model-behavior/README.md).

This path is not part of the default public validation baseline. It requires
external provider/model setup, and model output is non-deterministic. Captures
may differ between runs.

Deeper reference:
[tests/cases/03-model-behavior-lab](../tests/cases/03-model-behavior-lab/README.md).

## Internal And Smoke Tests

The low-level smoke tests live under `tests/smoke/`. They remain the current
implementation-level validation owner for many runtime, daemon, hot-state,
projection, and record-store paths.

Use [work/spines/testing.md](engineering/testing.md) and
[work/spines/command-surface.md](engineering/command-surface.md) for the
current deeper engineering view. Those references may still include internal or
historical material.

## What These Tests Do Not Prove

- They do not prove production readiness.
- They do not prove provider breadth.
- They do not prove model quality.
- They do not make model behavior deterministic.
- They do not replace legal review for public or commercial launch use.
- They do not imply that every older engineering command is a stable public
  interface.

## Deeper References

- [Quickstart](quickstart.md)
- [Testing doctrine](engineering/testing.md)
- [Command surface](engineering/command-surface.md)
- [Filesystem loop lab](labs/filesystem-loop/runbook.md)
- [Model behavior lab artifacts](labs/model-behavior/README.md)
