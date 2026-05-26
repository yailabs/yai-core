Historical/superseded engineering record. Not an active source of truth.

# SPINE.26 Hot State Projection Freshness Integration

Historical engineering record. Not an active source of truth.

## Outcome

SPINE.26 makes projection freshness consumer-aware.

```text
fresh model projection -> usable
receipt/decision/memory stale model projection -> refresh_required
authority/divergence/thread/manual/unknown stale model projection -> blocked_for_model
operator/audit/debug stale projection -> refresh_recommended
```

The command surface stays small. `yai projection inspect` now reports
freshness, stale reason, policy, consumer and source. `yai hot status` reports
`projection_policy`. The prompt dry-run path reports freshness policy and the
non-dry-run path warns when the model-visible projection requires refresh or is
blocked by policy.

## Command Surface

Added commands:

```text
none
```

Changed commands:

```text
yai projection inspect
yai hot status
yai prompt --dry-run
```

Removed/deprecated commands:

```text
none
```

Expected key output:

```text
projection_freshness: fresh|stale|unknown|rebuilding
stale_reason: ...
freshness_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
source: hot_state|projection_record
```

## Validation Target

```text
make smoke-spine26
make check
```
