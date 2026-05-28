# SPINE.52A - Model-Native Actor / Adapter Roadmap Rebase

Status: implemented

Macro:

```text
MODEL
VIEW
CONTROL
MEMORY
OBSERVABILITY
```

Type: roadmap/doctrine update only.

## Purpose

Align the internal spine after completed SPINE.52 so that YAI-native model
behavior, actor adapters, specialist model candidates and DGX/yai-dev lab
evidence are represented before future Projection Delta, Context Compiler,
Context Residency and Model Runtime work proceeds.

## Scope

- Add Model-Native Actor / Adapter Doctrine.
- Reserve SPINE.58S-SPINE.58T for model-visible YAI grammar and lab evidence /
  training corpus boundaries.
- Reserve SPINE.93G-SPINE.93J for model participation profiles, actor adapter
  contracts, specialist model lab boundaries and model-native actor boundary
  freeze.
- Clarify Level 3 through Level 6 posture.
- Clarify runtime-owned vs model-adaptable responsibilities.
- Clarify that DGX/yai-dev lab evidence may inform future contracts but is not
  core runtime behavior.

## Command Surface

No command surface changes.

## Non-Goals

- No model training.
- No fine-tuning.
- No LoRA or adapter loading.
- No tokenizer or special-token implementation.
- No model architecture implementation.
- No runner implementation.
- No provider implementation.
- No ContextFrame lifecycle implementation.
- No KV/cache implementation.
- No behavior fact extraction changes.
- No carrier dispatch changes.
- No policy authority changes.
- No memory implementation.

## Validation

- Roadmap marks SPINE.52 as done.
- Roadmap includes SPINE.52A immediately after SPINE.52.
- Roadmap includes SPINE.58S and SPINE.58T after SPINE.58R.
- Roadmap includes SPINE.93G, SPINE.93H, SPINE.93I and SPINE.93J after
  SPINE.93F and before SPINE.94.
- Doctrine includes runtime-owned vs model-adaptable split.
- Doctrine states model cannot approve, execute, mutate truth or replace
  records, graph, facts, memory, policy or audit.
- Doctrine states KV/cache is runner-local optimization, not memory, truth,
  authority or provenance.
- Command Surface explicitly says no command surface changes.
