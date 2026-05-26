# Reconciliation Model

Reconciliation handles mismatch between expected, decided, executed, observed,
projected and remembered reality.

Reconcile is not recovery execution. It is expected-vs-observed mismatch
detection and recovery posture.

## Divergence Classes

```text
expected_vs_observed
claim_without_receipt
receipt_without_decision
external_bypass
policy_conflict
stale_memory
subject_state_mismatch
carrier_partial_failure
projection_stale
hot_state_mismatch
record_graph_mismatch
fact_derivation_mismatch
```

## Reconciliation Flow

```text
detect divergence
classify risk
link case-world material / records / receipts / graph edges / facts
produce recovery or compensation posture
project finding
```

## Rule

Reconciliation must not hide gaps. A missing receipt, bypass or conflicting policy is an explicit artifact, not an exception swallowed by projection.

## NEW.5 Reconstruction Input

NEW.5 does not implement reconciliation yet, but it creates the first input
needed by reconciliation: graph edges that connect case, subject, operation,
decision and receipt.

Receipt chain reconstruction can expose whether a receipt lacks:

```text
case edge
operation edge
decision edge
subject update edge
```

Later reconciliation can turn those gaps into explicit divergence artifacts.

## NEW.7 Divergence V0

NEW.7 adds two store record kinds:

```text
divergence
reconciliation
```

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

The first detector is residue-derived. It does not execute recovery or
compensation. It only records mismatch for projection and operator review.

SPINE.1 target owner: Rust data spine detects and materializes reconcile
records; C system code may enforce or carry later recovery decisions only
through explicit control and carrier boundaries.

SPINE.3R target owner: Rust engine reconciles across hot, journal, record,
graph, fact, memory and projection planes. SPINE.29 is the planned wave for
reconcile over hot/store/graph consistency. SPINE.3R does not implement that
backend.

## NEW.13 Route

```text
system/reconcile -> split: system/engine_bridge + engine/yai-engine/src/reconcile
../yai-dev/src/substrate/signals/* -> reconcile or memory concept evidence
../yai-dev/src/analytics/* -> reconcile/index/memory evidence or yai-dev
```

No reconciliation implementation moves in NEW.13.

NEW.14 moved the Rust engine crate to `engine/yai-engine`, making that the
physical owner for future reconcile mechanics. C reconcile code remains under
`system/reconcile` as `keep_temporarily` after the NEW.18 bridge split.

## Projection Interaction

NEW.8 makes divergence visibility explicit in projection results. A projection
result carries `source_divergence_count` and a redaction posture, so audit,
operator and model views can expose mismatch without allowing projection to
rewrite or hide reconciliation truth. Live projection must expose delta and
freshness posture when divergence affects the view.

## Observability Interaction

SPINE.4 makes divergence exposure part of Case View Quality. Reconcile feeds
observability with:

```text
critical divergence flags
divergence_exposure
case_context_consistency
record_graph_mismatch
fact_derivation_mismatch
projection_stale
```

Observability measures whether mismatch is visible quickly enough for controlled
behavior. It does not repair mismatch and it does not own reconcile truth.
