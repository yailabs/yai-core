# Quickstart

This page is a minimal local orientation for technical evaluation. It is not a
full command reference and does not describe package installation or production
deployment.

## Prerequisites / Assumptions

- Work from the repository root.
- Use the local toolchain expected by the Makefile, including a C compiler,
  `make`, and the Rust toolchain used by the current command and engine code.
- Treat the repository as source-available technical evaluation material.

## Build / Check

Start with the lightweight repository status command:

```sh
make info
```

Run the full repository check when you want layout, docs, build, and smoke
coverage:

```sh
make check
```

`make check` is intentionally broader than a README quickstart. If it fails,
read the failing check output before assuming the runtime itself is the issue.

## First Repository Checks

Useful first checks:

```sh
make info
make check
```

For details on what these checks cover, see [Testing](engineering/testing.md).

## Run A First Validation Path

The public validation entrypoint is [Test cases](test-cases.md).

Start with Test 00, repository health:

```sh
make info
make check
```

Then use Test 01 for no-model runtime inspection when local `yai` and `yaid`
binaries are built or installed.

## Current Runtime Inspection

The current engineering command surface documents runtime inspection commands
such as:

```sh
yai doctor
yai hot status
yai store status
```

Those commands may require building or installing the local binaries according
to the current engineering docs. This quickstart does not duplicate that full
surface.

## Where Command Details Live

- [Command surface](engineering/command-surface.md)
- [Test cases](test-cases.md)
- [Filesystem loop manual](manuals/manual-filesystem-loop-validation.md)
- [Testing](engineering/testing.md)

## Known Limitations

- YAI is an early source-available repository for technical evaluation.
- The command and test surfaces are still stabilizing.
- Public docs are being separated from older architecture, manual, and
  engineering material.
- Provider/backend examples should not be read as tested provider breadth.
- No package availability or production deployment flow is claimed here.
