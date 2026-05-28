# Case-Native Benchmark Matrix Tests

Status: canonical LAB.TEST.2S test matrix.

Test 2 is a benchmark family, not one prompt comparison. The canonical
comparison is naked_no_context, naked_raw_context, naked_mini_rag,
yai_case_bound and yai_case_residency. The naked_no_context condition is a
negative control and cannot be treated as a useful case-answering baseline.

## Modes

| Mode | Expected |
|---|---|
| `no-ai` | Build the real case via CLI, build corpus, run mini-RAG selection and produce scorecard shells without provider calls. |
| `case-native` | Use YAI CLI to install/open/materialize the real filesystem-loop case, replay journal, build graph/facts, enter case and optionally prompt through YAI CLI. |
| `naked` | Run C0 naked_no_context directly against the provider outside YAI. |
| `raw-context` | Run C1 naked_raw_context with direct provider calls and raw corpus stuffing. |
| `mini-rag` | Run C2 naked_mini_rag with lexical top-k chunks and direct provider calls. |
| `compare` | Score available outputs and write per-question comparison JSON. |
| `synthetic` | Non-canonical fixture rendering compatibility mode. |
| `all` | Run the full matrix. |

## Required Checks

```text
case setup via CLI works
corpus is generated from command outputs
mini-RAG selected chunks are generated
scorecard schema is generated
provider configuration uses yai provider start when provider is configured
direct provider calls are limited to naked baselines
case-bound AI is skipped if provider attach/prompt CLI is absent
report marks skipped portions honestly
no fixture/synthetic benchmark is reported as canonical
```

## Smoke Contract

`make smoke-lab-context-residency-matrix` runs without requiring provider and
must print:

```text
context_residency_matrix:case_setup ok
context_residency_matrix:corpus ok
context_residency_matrix:mini_rag ok
context_residency_matrix:scorecard ok
context_residency_matrix:no_fake_case_bound_ai ok
context_residency_matrix:synthetic_noncanonical ok
```
