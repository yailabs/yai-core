# Pack Fixture Contract

Status: structural lab standard.

## Purpose

A pack fixture is lab/test input material shaped according to existing pack
doctrine. It is used to prepare, represent or inspect materialization into a case
during a lab test.

A pack fixture is not a pack runtime, installer, registry backend,
materialization API, command implementation or source-root production pack.
It must not imply any production command such as a pack installer or runtime
loader.

## Allowed Material Areas

A pack fixture may contain material areas aligned with pack doctrine:

```text
materials
policies
procedures
projections
templates
outputs
memory
actors
tools
fixtures
tests
```

Presence of a directory does not imply runtime support. Empty optional areas
should not be created unless they help the specific test.

## Required Posture

- Keep fixture material local to the lab/test that uses it.
- Declare whether the fixture is complete, partial or historical.
- Distinguish fixture material from materialized case state.
- Record which fixture files were used by each run.
- Mark unknown materialization behavior as `unknown` or `not implemented`.
- Do not infer production pack behavior from fixture files.
- Do not create a separate repository-root fixture registry for lab inputs.

## Filesystem-Loop Pilot

The current filesystem-loop fixture primarily represents:

- policies;
- projection posture;
- authority posture;
- validation/evidence fixture material.

It is a partial lab input fixture. It is not a complete pack and does not prove
that policies are required for every pack.
