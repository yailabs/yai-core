<!--
YAI Core

Copyright (c) 2026 Francesco Maiomascio.
All rights reserved.

This file is part of the source-available YAI Core repository.
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

# YAI Core

YAI is a local control runtime for case-bound AI operation. It gives model,
provider, tool, operator, and system activity an operational boundary: what
case it belongs to, what subjects it affects, what policy applies, what was
allowed or blocked, what receipt was produced, and what record, projection, or
memory can be derived from the result.

## Status

YAI Core is an early source-available technical repository. It is public for
technical evaluation and review, not production use, unless a specific written
permission or component license says otherwise.

The command and test surface is still stabilizing. The legal posture is defined
in [LICENSE.md](LICENSE.md) and summarized in [docs/legal.md](docs/legal.md).

## Why This Exists

Model output increasingly crosses into tools, files, services, memory,
workflow state, operator decisions, and provider calls. Once that happens, the
technical problem is no longer only whether a model generated a good answer.

The runtime needs to know which case the activity belongs to, which subject or
provider boundary it touched, which policy applied, what was allowed or
blocked, what evidence was produced, and what state can safely be projected
back into future work.

YAI exists to make that boundary explicit and inspectable on a local machine.

## What YAI Is

YAI treats work as bounded cases. A case is the operational frame that gives
subjects, providers, attempted operations, policy decisions, receipts, records,
projections, and memory a shared context.

Subjects are things inside or around a case: files, services, repositories,
providers, operators, tools, models, processes, or external systems. Providers
are treated as participants at a boundary, not as authority over the case.

An attempted operation is evaluated through control material: policy gates,
decisions, obligations, and effect or observation boundaries. The result should
leave a receipt and durable record that can be inspected later.

Projection and memory are derived from operational residue. They are views over
case state, not substitutes for the records and receipts that explain what
happened.

## Design Bet

- The model is not the system boundary; the case is the operational boundary.
- Provider and model output is candidate material, not authority.
- Receipts and durable records are first-class runtime material.
- Projection and memory should be derived from operational residue, not free
  text alone.
- Provider engines remain separate and can be local, remote, custom, or mocked.
- Enforcement strength depends on the boundary YAI actually owns or observes.

## What YAI Is Not

YAI is not:

- an inference engine
- a model provider
- a chatbot framework
- a generic agent framework
- a workflow builder
- a cloud platform
- a TUI or client application
- a generic policy engine
- a generic audit logger

Inference engines and model servers remain external providers. `llama.cpp`,
Ollama, vLLM, MLX, DS4, custom servers, and remote APIs may sit around a YAI
case, but YAI does not replace them and this README does not claim tested
support for each provider.

## Execution Boundary

When a subject, provider, operator, model, tool, or system attempts work inside
a case, YAI tries to turn that activity into inspectable operational material:

```text
input/proposal -> case binding -> subject/effect boundary -> control decision -> effect or observation -> receipt -> record -> projection/memory
```

The important steps are concrete: bind the activity to a case, identify the
subject and effect boundary, evaluate control material, allow, block,
constrain, observe, or import the result, write a receipt, update durable
records, and derive the projection or memory that later participants may see.

## Core Model

```text
case -> subject/provider -> attempted operation -> control decision -> effect/observation -> receipt -> record -> projection/memory
```

- `case`: bounded operational context.
- `subject/provider`: entity or boundary participating in the case.
- `attempted operation`: proposed action or observation.
- `control decision`: allow, block, constrain, or require obligations.
- `effect/observation`: local execution, boundary call, import, or observation.
- `receipt`: structured evidence of what happened.
- `record`: durable material used for reconstruction and inspection.
- `projection/memory`: controlled views and derived operational memory.

## Current Validation

Repository-level entrypoints:

```sh
make info
make check
```

Deeper runtime and manual validation lives in the engineering and manual docs.
The README is not the full command reference.

The current command surface is documented in
[docs/engineering/command-surface.md](docs/engineering/command-surface.md).
It includes local runtime and inspection commands such as `yai doctor`,
`yai hot status`, `yai store status`, daemon status/info calls, and smoke-test
paths. Treat that document as the current command reference rather than this
README.

## Current Implementation Surface

This repository currently contains the local command and daemon source, the C
system boundary, the Rust operational data engine under consolidation, and
smoke/manual validation material.

- `cmd/` contains local binaries such as `yai` and `yaid`.
- `system/` contains the C daemon, host-boundary, control, carrier, bridge, and
  transitional shim surface.
- `engine/` contains the Rust operational data engine being consolidated.
- Hot-state and record-store inspection are documented in the current command
  surface.
- Smoke and manual validation exist, but command and test surfaces are still
  being stabilized.

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

Some C data-plane paths are transitional while Rust engine ownership is being
consolidated. The current source boundary is described in
[docs/engineering/source-surface.md](docs/engineering/source-surface.md).

## Documentation

- [Documentation index](docs/index.md)
- [Legal posture](docs/legal.md)
- [Command surface](docs/engineering/command-surface.md)
- [Source surface](docs/engineering/source-surface.md)
- [Testing](docs/engineering/testing.md)
- [Filesystem loop manual](docs/manuals/manual-filesystem-loop-validation.md)
- [Architecture notes](docs/architecture/README.md)

These documents still include older internal planning and implementation
material. The public documentation surface is being split into shorter focused
pages after this README pass.

## License And Contributions

YAI Core is source-available, not open source by default. Source access is for
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

- Early repository; not production-ready unless explicitly stated.
- Command and test surfaces are still stabilizing.
- Public docs are being separated from older architecture, manual, and planning
  material.
- Provider/backend mentions should not be read as tested provider breadth.
- Data-plane ownership is still being consolidated between transitional C paths
  and the Rust engine.
- Commercial or public launch use still requires legal review and explicit
  permission under the source-available posture.
