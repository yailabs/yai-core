# Record Protocol

Purpose: persist core residue in a stable record shape.

## Record Envelope

```text
record_ref
record_kind
schema_version
case_ref
subject_refs
source_refs
payload_hash
payload
created_at
integrity_ref optional
```

## Initial Record Kinds

```text
source
case_domain
case_attachment
case_binding
subject_binding
attempt
decision
receipt
policy_rule
projection_rule
authority_scope
model_interpretation
memory
projection
divergence
```

## Rule

Store persists records. Record semantics are owned by their primitive family.
