# Operational Extraction Contract

Status: SPINE.6B active contract.

This contract applies to every future implementation wave.

Core rule:

```text
A wave is not complete until the corresponding old-yai residue has been classified.
```

If the concept already exists in old `yai`, the wave must either absorb it,
rewrite it, split it, externalize it to future `ai-environment`, quarantine it,
mark it `compat_only`, mark it `delete_later`, or explicitly defer it. Leaving
old material ambiguous is not allowed.

## Required Operating Sequence

```text
OLD-YAI READ
EXTRACTION
YAI-CORE IMPLEMENTATION
OLD-YAI RESIDUE NORMALIZATION
INVENTORY UPDATE
VALIDATION
```

An implementation wave must not be only additive inside `yai-core` when the
touched concept already exists in old `yai`.

## Repository Scope

Primary repo:

```text
yai-core
```

Secondary repo when the wave requires it:

```text
../yai
```

Do not touch `interfaces` or `console` unless the delivery explicitly says so.

## Dirty Worktree Protection

Before touching old `yai`, run:

```text
git -C ../yai status --short
```

If there are user or unknown changes in paths the wave would modify:

```text
do not touch those files
record the caveat
choose a non-conflicting inventory or docs update only if safe
```

## Extraction Decision Table

Every relevant wave must include an extraction decision table with:

```text
old path
extracted concept
target yai-core root
action
future_repo
status
notes
```

Allowed actions:

```text
absorb
rewrite
split
externalize
quarantine
compat_only
delete_later
migrated_concept
leave_untouched
```

Allowed future repos:

```text
yai-core
ai-environment
interfaces
console
none
```

Allowed statuses:

```text
planned
inspected
imported
rewritten
externalized
archived
blocked_by_dirty_worktree
deferred
complete
```

## Old-Yai Residue Normalization

Useful old material absorbed into `yai-core` must be normalized in old `yai` as
one of:

```text
externalized
migrated_concept
historical
compat_only
delete_later
quarantine
leave_untouched
```

Material that is agentic, experimental, harness-oriented, provider-lab,
QA-lab, scenario-lab, workflow-lab or model-lab should usually be assigned to:

```text
future_repo=ai-environment
action=externalize
```

Do not delete silently. Deletion requires an inventory row and explicit
`delete_later` or removal posture.

## Commit Boundaries

If a wave changes both repos, prefer separate commits:

```text
commit 1: yai-core implementation
commit 2: old-yai residue normalization
```

Do not hide old-yai cleanup inside a yai-core commit.

## Lineage

Every yai-core primitive derived from old material should name its source
lineage when applicable:

```text
derived_from_old_yai:
  src/case/attachments/*
  src/runtime/execution/operational_receipt.c
```

The old code is not copied wholesale. Semantics are extracted and rewritten in
the new grammar.

## Non-Goals

```text
no blind folder migration
no old root recreation inside yai-core
no unrelated old-yai cleanup
no interface or console edits unless explicitly scoped
no mixed unrelated commits
```
