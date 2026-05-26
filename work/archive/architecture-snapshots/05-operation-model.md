# Operation Model

An operation attempt is a normalized attempted effect over one or more case-bound subjects.

The core does not begin from capability names. It begins from attempts and effect classes.

## Attempt Fields

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
control_mode
requested_at
```

## Effect Classes

Initial effect classes:

```text
read
write
execute
invoke
emit
delete
mutate
export
import
observe
```

## Capability Absorption

Legacy capability taxonomy becomes operation shape, effect class, mutability, sensitivity and locality. Capability is not a future core root.

## Valid Attempt Rule

An attempt is admissible for control only when it has a case, actor or principal posture, target subject, effect class and expected receipt posture.
