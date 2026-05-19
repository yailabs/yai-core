# Ingest Protocol

Purpose: normalize material entering YAI Core.

## Inputs

```text
source_ref
source_kind
locator
material_kind
raw_observation_ref
received_at
trust_class
case_hint
subject_hint
```

## Outputs

```text
material_ref
observation_ref
claim_ref optional
external_receipt_ref optional
subject_binding_candidate optional
```

## Rule

Ingest does not decide, believe or execute. It registers material and prepares it for subject binding, control or reconstruction.
