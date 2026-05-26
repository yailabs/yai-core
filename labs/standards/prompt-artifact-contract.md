# Prompt Artifact Contract

Prompted runs must preserve the prompt surface as a first-class artifact.

A prompt artifact is required when a run sends text, messages or instructions
to a model, provider, agent or notebook cell that affects the measured output.

## Required Fields

Each lab prompt catalog entry must declare:

```text
id
kind
description
role
content
optional max_tokens
optional temperature
tags
expected evidence files
```

## Catalog and Run Assets

Prompt text authority lives beside the lab:

```text
labs/<lab>/prompts.json
```

Resolved prompt artifacts live under each prompted run:

```text
assets/prompt.json
assets/prompt.txt
```

## Rules

- Do not benchmark a model interaction without recording the prompt.
- Use one catalog ID per distinct prompt or message set.
- Record `Not applicable` only for runs that do not call a model/provider.
- Record `Not available` for historical runs where the prompt cannot be
  reconstructed.
- Do not include secrets, authorization headers or private environment values.
- Notebooks and runbooks may show prompt text for human readability, but they
  must not become the hidden source of prompt text.
- Different workloads should use different prompt IDs.
- `run.sh --prompt-id <id>` executes exactly one prompt/query by default.
