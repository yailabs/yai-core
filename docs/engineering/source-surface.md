# Source Surface

SPINE.28B freezes the active source surface after the repository identity
cutover.

## Rule

```text
system/ is not a second data engine.
engine/ is the Rust data spine target.
system/ may keep C ABI, host, carrier, control, daemon and transitional shim
code while current smoke paths depend on it.
```

File ownership and boundary must be visible in a file header or owning README
before an agent makes semantic changes. The standard is
`file-header-standard.md`; the agent checklist is `agent-operating-appendix.md`.

## Active C System Roots

```text
system/base          active C system support
system/case          active C case/session/world/context
system/control       active C control/gate/decision
system/daemon        active C daemon support
system/effect        active C carrier/effect/receipt
system/engine_bridge active C bridge to Rust engine
system/hot           active C hot-state bridge/cache
system/internal      active C internals
system/op            active C operation attempt
system/subject       active C subject/binding/state
```

These roots may contain current implementation logic.

## Transitional Data Shims

```text
system/store         transitional C data shim until LMDB/engine migration
system/graph         transitional C graph shim until Ladybug/engine graph
system/index         transitional C query/index shim
system/memory        transitional C memory candidate shim
system/projection    transitional C projection shim
system/reconcile     transitional C reconcile shim
```

These directories keep current ABI and smoke paths working. They are not the
future home of durable data-plane logic. New long-term data-plane logic belongs
under `engine/yai-engine`.

REPO.HYGIENE.1 applies explicit transitional shim banners to these roots.

## Public ABI Roots

```text
include/yai/*
```

Headers are public ABI declarations. A header root is not data-plane ownership.

## Archived Placeholder Roots

`system/ingest` and `include/yai/ingest` were README-only placeholders. They
are archived under:

```text
docs/archive/engineering/placeholders/
```

Ingest returns as an implementation surface only when the linear roadmap reaches
the ingest wave.

## Local Environment Roots

Local Python environments do not belong in the repository:

```text
venv/
.venv/
env/
ENV/
```

They are ignored and blocked by `check-source-surface-clean`.
