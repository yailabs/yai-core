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
subject
attempt
decision
receipt
policy
memory
projection
divergence
```

## Rule

Store persists records. Record semantics are owned by their primitive family.
