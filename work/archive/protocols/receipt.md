# Receipt Protocol

Purpose: make an effect or observation durable and reconstructable.

## Fields

```text
receipt_ref
case_ref
op_ref optional
decision_ref optional
carrier_ref optional
subject_refs
effect_status
observed_state_ref optional
result_hash optional
external_receipt_ref optional
created_at
```

## Receipt Classes

```text
executed
blocked
observed
imported
partial_failure
missing
```

## Rule

Receipt gaps are first-class divergence artifacts. They must not be hidden by projections.
