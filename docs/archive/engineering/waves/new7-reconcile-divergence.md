Historical/superseded engineering record. Not an active source of truth.

# NEW.7 Reconcile / Divergence

NEW.7 introduces the first reconciliation layer. It detects mismatch between
decisions, receipts, graph residue, subject state and memory candidates, then
persists divergence and reconciliation records.

This is detection and review posture only. It does not repair, compensate,
execute recovery or add new carriers.

## Old-Yai Audit

Read-only audit scope:

```text
../yai/src/orchestrator/recovery/*
../yai/src/runtime/execution/recovery_hint.c
../yai/src/runtime/execution/runtime_enforcement_recovery.c
../yai/src/substrate/signals/*
../yai/src/decision/conflict/*
../yai/src/orchestrator/recovery/reconcile.c
```

Extracted concepts:

```text
missingness and contradiction are explicit residue
recovery posture is separate from repair execution
signals are derived indicators, not truth owners
decision conflicts need traceable records
orchestrator recovery remains scenario material, not a core workflow engine
```

No old source was copied.

## Contract

Initial divergence kinds:

```text
denied_but_executed
receipt_without_decision
claim_without_receipt
missing_target_subject
stale_subject_state
memory_without_basis
graph_chain_missing
```

Initial severity:

```text
info
warning
error
critical
```

Initial reconcile posture:

```text
observed
requires_review
recoverable
unrecoverable
compensated
```

## Derivation V0

Divergence detection scans existing residue:

```text
decision records
receipt records
graph edge records
subject state records
memory candidate records
store refs
```

V0 detects:

```text
decision:deny with receipt:executed
receipt records without decision_id
attempt records without target subject
stale subject state summaries
memory candidates with zero basis counts
```

The first smoke persists `divergence` and `reconciliation` records in the same
`yai.store.record.v0` envelope.

## Smoke

`tests/smoke/reconcile-divergence/test_reconcile_divergence.c` proves:

```text
deny decision plus executed receipt produces denied_but_executed
receipt without decision produces receipt_without_decision
divergence records persist
reconciliation records persist
projection counts divergence and reconciliation residue
```

Debug command:

```text
yai reconcile summary --journal build/tmp/new7/reconcile-divergence-<pid>/journal.jsonl
```

## Out Of Scope

```text
no automatic repair
no compensation execution
no process carrier
no network carrier
no model carrier
no daemon IPC
no graph database
no memory consolidation engine
```
