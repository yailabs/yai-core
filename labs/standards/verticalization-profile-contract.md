# Verticalization Profile Contract

Future labs may define a vertical profile at:

```text
labs/<lab>/document-profile.md
```

This wave does not create vertical profiles.

## What A Profile May Restrict

A vertical profile may restrict:

```text
allowed document types
required metrics
artifact layout
domain vocabulary
forbidden claims
required validation commands
```

## Abstract Examples

A retrieval lab may require `recall@k` and query latency.

A model behavior lab may require `model_ref`, `prompt_ref` and `output_ref`.

A hardware baseline lab may require device, driver and runtime environment.

## Rule

A vertical profile may add stricter requirements but must not weaken the common
claim, evidence, result, limitation or reproducibility rules.
