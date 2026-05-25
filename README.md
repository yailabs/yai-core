<!--
YAI

Copyright (c) 2026 Francesco Maiomascio.
All rights reserved.

This file is part of the source-available YAI repository.
Use, copying, modification, distribution, and production operation
are governed by the repository licensing documents, including LICENSE.md and
docs/legal.md.

Public source access is provided for technical evaluation and review. Commercial
use, redistribution, hosted use, sublicensing, incorporation into third-party
products, or production use requires explicit written permission unless another
file or component license says otherwise.
-->

<div align="center">
  <img src="docs/reference/figures/brand/yai-transp.png" alt="YAI" width="220" />

  <strong>local control runtime for case-bound AI operation</strong>
  <br />
  <span>Cases, subjects, policy gates, receipts, records, projections, and operational memory.</span>

  <br />

  ![Runtime](https://img.shields.io/badge/runtime-local-0f766e?style=flat&labelColor=1f2937)
  ![Core](https://img.shields.io/badge/core-case%2Fcontrol-475569?style=flat&labelColor=1f2937)
  ![License](https://img.shields.io/badge/license-source--available-374151?style=flat&labelColor=1f2937)
</div>

# YAI

YAI is a local runtime for binding AI-mediated activity to operational
cases. It does not run models. It controls the boundary around model, provider,
tool, operator, and system activity: which case it belongs to, which subjects
it touches, which policy applies, what was allowed or blocked, what receipt was
produced, and what record, projection, or memory can be derived from the
result.

## Status

YAI is an early source-available repository. It is public for technical
evaluation and review, and is not production-ready unless explicitly stated.

The command and test surface is still stabilizing. The legal posture is defined
in [LICENSE.md](LICENSE.md) and [docs/legal.md](docs/legal.md).

## Why This Exists

Model output can cross into tools, files, services, provider calls, memory,
workflows, and operator decisions. Once that happens, generation quality is not
enough.

The runtime needs case binding, policy and control decisions, receipts,
records, projection, and recovery material. It needs to know which subject or
provider boundary was touched, what was allowed or blocked, what evidence was
produced, and what state can safely be derived for future work.

YAI exists to make that boundary explicit and inspectable on a local machine.

## What YAI Is

YAI treats work as bounded cases. A case is the operational frame that gives
subjects, providers, attempted operations, policy decisions, receipts, records,
projections, and memory a shared context.

## Current Validation

Repository-level entrypoints:

```sh
make info
make check
```

Deeper runtime and manual validation lives in the engineering and manual docs.
The README is not the full command reference.

Agent-facing ownership rules live in:

```text
docs/engineering/file-header-standard.md
docs/engineering/agent-operating-appendix.md
```

The current command surface is documented in
[docs/engineering/command-surface.md](docs/engineering/command-surface.md).
Treat that document as the current command reference until `docs/commands.md`
is split out. Failures from unrelated dirty work should be reported, not
hidden.

## Current Implementation Surface

This repository currently contains:

- `cmd/` contains local binaries such as `yai` and `yaid`.
- `system/` contains the C daemon, host-boundary, control, carrier, bridge, and
  transitional shim surface.
- `engine/` contains the Rust operational data engine being consolidated.
- `include/` contains public and system ABI headers.
- `tests/` contains smoke and validation tests.
- Current command docs cover hot-state, record-store, control/carrier, process,
  and observation surfaces.
- Some C data-plane paths remain transitional while Rust engine ownership is
  consolidated.

## Repository Layout

```text
include/    public and system ABI headers
system/     C system boundary: daemon, host boundary, carriers, control, FFI bridges
engine/     Rust operational data engine
cmd/        local binaries: yai and yaid
proto/      schemas, fixtures, and protocol material
docs/       architecture, engineering notes, ADRs, legal notes
tests/      smoke and validation tests
tools/      checks and developer utilities
vendor/     vendored support code
examples/   examples when present
packaging/  packaging material when present
```

The current source boundary is described in
[docs/engineering/source-surface.md](docs/engineering/source-surface.md).

## Documentation

- [Documentation index](docs/index.md)
- [Technical brief](docs/technical-brief.md)
- [Quickstart](docs/quickstart.md)
- [Test cases](docs/test-cases.md)
- [Provider boundary](docs/providers.md)
- [Architecture summary](docs/architecture.md)
- [Glossary](docs/glossary.md)
- [Legal posture](docs/legal.md)
- [Current engineering command surface](docs/engineering/command-surface.md)
- [Current engineering source surface](docs/engineering/source-surface.md)
- [Testing](docs/engineering/testing.md)
- [Filesystem loop manual](docs/manuals/manual-filesystem-loop-validation.md)

Engineering references may still include internal or historical material. The
public documentation surface is being split into shorter focused pages.

## License And Contributions

YAI is source-available, not open source by default. Source access is for
technical evaluation and review unless another file or component explicitly
grants different rights.

- [LICENSE.md](LICENSE.md)
- [NOTICE.md](NOTICE.md)
- [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [SECURITY.md](SECURITY.md)
- [docs/legal.md](docs/legal.md)

Technical feedback is welcome. Broad external contribution is not open yet
unless maintainers explicitly scope the change.

## Current Limitations

- Early source-available repository.
- Not production-ready unless explicitly stated.
- Command and test surfaces are still stabilizing.
- Public docs are being separated from older architecture, manual, and planning
  material.
- Provider/backend mentions should not be read as tested provider breadth.
- Data-plane ownership is still being consolidated between transitional C paths
  and the Rust engine.
- Commercial or public launch use still requires legal review and explicit
  permission under the source-available posture.
