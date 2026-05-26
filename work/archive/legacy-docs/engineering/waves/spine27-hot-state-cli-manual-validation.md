Historical/superseded engineering record. Not an active source of truth.

Historical engineering record. Not an active source of truth.

# SPINE.27 - Hot State CLI + Manual Validation

Status: complete.

Purpose:

```text
Make the hot-state and projection freshness command surface stable,
inspectable and manually testable before the hot-state freeze.
```

Command surface:

```text
Added commands: none
Changed commands:
  yai info
  yai doctor
  yai hot status
  yai projection inspect --journal <path> --consumer <consumer>
Removed/deprecated commands: none
New smoke target: make smoke-spine27
```

Canonical fields:

```text
hot_state_schema_status: valid|invalid|missing
freshness_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
freshness_source: hot_state|projection_record|journal|unknown
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
