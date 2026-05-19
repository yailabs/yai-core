# Repository Split

The YAI workspace is split by ownership, not by UI or product packaging.

```text
YAI/
├── yai/        # old/current concept mine, future ai-environment material
├── yai-core/   # canonical local AI operational control core
├── interfaces/ # projection/API/SDK/conformance
└── console/    # operator terminal/client UX
```

`yai-core` is a sibling to `yai`, not nested inside `yai`.

## Ownership

```text
yai-core   = canonical local AI operational control core
yai        = old/current repo, concept mine and future ai-environment material
interfaces = schemas, transports, SDK, API and conformance over core primitives
console    = operator terminal/client UX over projections and operations
```

## Order

```text
core first
interfaces second
console later
platform last
```

`interfaces` projects protocol and SDK truth from core operations. `console`
consumes operations and projections. Neither owns core semantics.

## Non-Goal

Do not recreate `yai-core` as a subfolder of `yai`. Do not keep two active copies
of New Core docs.
