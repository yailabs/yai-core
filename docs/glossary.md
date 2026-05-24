# Glossary

## case

A bounded operational context. A case gives subjects, providers, attempts,
decisions, receipts, records, projections, and memory a shared scope.

## subject

An entity bound to or referenced by a case. A subject may be a file, process,
repository, service, provider, model, operator, tool, document, or external
system.

## provider

A boundary that supplies model output, tool behavior, service behavior, or
mocked behavior. Providers participate in cases, but YAI does not own inference
or treat provider output as authority.

## attempted operation / op

Proposed work against a subject or boundary before the runtime treats the
result as operational residue. An attempt becomes meaningful when it is bound
to a case and evaluated by control material.

## control

The runtime plane for policy materialization, gates, decisions, obligations,
and receipt requirements. Control turns applicable case material into an
inspectable decision.

## gate

An evaluation point where control material is applied to a case, subject,
provider, attempted operation, carrier, projection, or retention posture.

## decision

The structured outcome of control for an attempted operation. A decision may
allow, block, constrain, observe, import, or require obligations.

## effect

Execution or imported consequence at a boundary. An effect should be connected
to a decision and receipt when it becomes operational material.

## observation

Material learned or imported without necessarily owning execution. Observation
can still be bound to a case and recorded with receipt posture.

## carrier

The mechanism that executes, observes, or imports an effect at a boundary. A
carrier may be local process, filesystem, provider, IPC, shell, adapter, or
another controlled integration boundary.

## receipt

Structured evidence of execution, observation, block, failure, constraint,
imported result, or recovery posture. A receipt connects boundary activity to a
case, subjects, decisions, and records.

## record

Durable operational material used for reconstruction and inspection. Records
may represent attempts, decisions, receipts, sources, policy material, or other
case residue.

## store

The durable record and lookup surface. It preserves operational residue so the
runtime can inspect, reconstruct, and derive later views.

## hot state

Live runtime cache for current case/session/projection posture. Hot state is
useful for inspection and freshness, but it is not durable truth.

## projection

A controlled view over case material served to a model, operator, API, audit,
debug, or external participant. A projection is scoped material, not ownership
of truth.

## operational memory

Case-scoped memory derived from records, receipts, decisions, effects, policies,
projections, and related structure. It is not just model context or free text.

## reconcile

Divergence detection and recovery posture when expected state and observed
state differ.

## daemon

The local resident process boundary for daemon-backed runtime behavior. In this
repository, the daemon binary is `yaid`.

## yai

The local technical command for inspection and operator/developer workflows. It
is not a client application surface.

## yaid

The local daemon binary. It owns resident daemon behavior and local IPC surfaces
documented in the current command references.
