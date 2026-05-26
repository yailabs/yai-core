# Prompt Artifact Contract

Prompted runs must preserve the prompt surface as a first-class artifact.

A prompt artifact is required when a run sends text, messages or instructions
to a model, provider, agent or notebook cell that affects the measured output.

## Required Fields

Each prompt artifact must declare:

```text
id
source_run
role
content
source_file
purpose
expected_output
limitations
```

## Directory

Prompt artifacts live under each run:

```text
assets/
  P001-<prompt-slug>.md
```

## Rules

- Do not benchmark a model interaction without recording the prompt.
- Use one prompt artifact per distinct prompt or message set.
- Record `Not applicable` only for runs that do not call a model/provider.
- Record `Not available` for historical runs where the prompt cannot be
  reconstructed.
- Do not include secrets, authorization headers or private environment values.
- If a notebook cell defines the prompt, either link that cell or copy the
  prompt into a numbered prompt artifact.
- Different workloads should use different prompt IDs.
