# NEW.0 Package

NEW.0 is the first physical `yai-core` package. It creates a small, correct
skeleton and guardrails. It does not implement the full system.

## Scope

```text
docs/
include/yai/
lib/
daemon/
ctl/
proto/
tests/
tools/checks/
Makefile
README.md
VERSION
```

## First Roots

```text
base
ingest
subject
case
op
control
effect
store
index
graph
memory
projection
reconcile
daemon
ctl
```

These roots exist as placeholders in NEW.0 so the layout is stable. NEW.0 does
not implement them.

## Guard

NEW.0 must include:

```text
tools/checks/check-no-old-roots.sh
tools/checks/check-required-layout.sh
tools/checks/check-doc-canonical-location.sh
tools/checks/check-doc-required-files.sh
tools/checks/check-doc-no-old-root-language.sh
```

The guard forbids old root names in the future core.

## Non-Goals

```text
no old folder migration
no product Console
no cloud
no agent framework
no advanced memory engine
no runtime loop
no old source import
```
