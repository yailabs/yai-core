# File Header Standard

Status: active doctrine  
Owner: docs/engineering  
Purpose: Define concise ownership and boundary headers for active files.  
Not source of truth for: legal notices, product positioning, or generated code.

## Rule

Every important active file should make its ownership and boundary visible
either in the file header or in the owning directory README.

Headers must stay short. They exist to prevent future agents from assigning
truth to the wrong file, not to narrate the whole architecture.

## C Headers

```c
/*
 * YAI - <module name>
 *
 * Purpose:
 *   <one or two lines>
 *
 * Ownership:
 *   <what this file owns>
 *
 * Boundary:
 *   <what this file must not own>
 *
 * Status:
 *   stable | active | transitional_shim | test_only | planned_stub
 */
```

## C Sources

```c
/*
 * YAI - <module name>
 *
 * Implements:
 *   <public/private contract implemented here>
 *
 * This file owns:
 *   <specific behavior>
 *
 * This file does not own:
 *   <explicit non-goals>
 *
 * Notes:
 *   <only if needed: transitional, bridge, future drain>
 */
```

## Rust Modules

```rust
//! YAI - <module name>
//!
//! Purpose:
//!   <what this module does>
//!
//! Ownership:
//!   <what this module owns>
//!
//! Boundary:
//!   <what this module must not own>
//!
//! Status:
//!   stable | active | transitional | experimental
```

## Shell Guards

```sh
# YAI - <check name>
#
# Purpose:
#   <what this guard prevents>
#
# Scope:
#   <what it checks>
#
# Non-goals:
#   <what it intentionally does not check>
```

## Active Markdown Docs

```md
# <Title>

Status: active | historical | superseded | roadmap | doctrine | runbook
Owner: <repo area>
Purpose: <short purpose>
Not source of truth for: <if applicable>
```

## Transitional C Shim Banner

Use this in README files or headers for `system/store`, `system/graph`,
`system/index`, `system/memory`, `system/projection` and `system/reconcile`:

```text
Transitional C shim.

This directory keeps the current ABI/smoke path working until the Rust engine
owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.
```

## Density

Prefer four fields:

```text
Purpose
Ownership
Boundary
Status
```

If a file is tiny and obvious, one ownership sentence in the file or owning
README is enough. If ownership changes, update the header in the same commit as
the behavior change.
