# Operation Attempt Protocol

Purpose: represent a normalized attempted effect.

## Fields

```text
op_ref
case_ref
actor_subject_ref
target_subject_ref
source_subject_refs
effect_class
sensitivity
locality
mutability
expected_effect
expected_receipt
requested_at
```

## Rule

An attempt is not a command string. It is a machine object that can be admitted, denied, deferred, observed, executed or reconciled.
