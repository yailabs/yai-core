Historical/superseded engineering record. Not an active source of truth.

Historical engineering record. Not an active source of truth.

# SPINE.24A - Retroactive Command Surface Recovery

Status: complete.

Purpose:

```text
Recover the operator command surface for SPINE.20-SPINE.24 so primitives map to
views, commands, manual tests, expected output and active documentation.
```

Scope:

```text
YAI_HOME runtime layout
install-local / uninstall-local / doctor-local / print-install-paths
yai doctor
yai info
yaid version and daemon lifecycle commands
hot-state status and snapshot edge cases
pack doctrine guard
foundation/layout guards
```

Command surface:

```text
Added commands: none
Changed commands: none
Removed/deprecated commands: none
New smoke target: make smoke-spine24a
New active command inventory: docs/engineering/command-surface.md
```

Old-yai audit:

```text
../yai/src/runtime/operator/*
../yai/src/runtime/lifecycle/*
../yai/src/runtime/machine/*
../yai/tools/runtime/*
../yai/Documentation/manuals/*
../yai/Documentation/reference/*
```

No old-yai files were modified.
