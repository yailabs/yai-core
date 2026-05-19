# Carrier Protocol

Purpose: invoke, observe or import evidence for an effect over one or more subjects.

## Carrier Request

```text
carrier_ref
case_ref
op_ref
decision_ref
target_subject_ref
effect_class
constraints
receipt_requirements
requested_at
```

## Carrier Result

```text
carrier_ref
effect_status
receipt_ref optional
observed_state_ref optional
failure_class optional
completed_at
```

## Rule

Carriers do not own control. Control decides. Carriers execute, observe or import evidence and produce receipts or failure records.
