# LAB.TEST.2S / Case-Native Benchmark Matrix

Status: canonical Test 2 lab benchmark.

Purpose: compare the same questions across naked no-context, naked raw-context,
naked mini-RAG, YAI case-bound and logical YAI residency conditions.

This lab depends on the filesystem-loop case grammar and uses
`case:new12-filesystem` style fixtures. It does not duplicate the
filesystem-loop lab itself.

Test 2 is a benchmark family, not one prompt comparison. The canonical
comparison is:

```text
naked_no_context
naked_raw_context
naked_mini_rag
yai_case_bound
yai_case_residency
```

The canonical YAI path opens/materializes a YAI case and asks the model through
YAI CLI. Direct provider calls are allowed only for naked baselines. The
fixture/synthetic renderer is retained only as a non-canonical microbenchmark.

Related benchmark:

```text
Test 1 - labs/filesystem-loop
Test 2 - labs/context-residency
```

## Canonical Files

| Concern | Path |
|---|---|
| Entry point and protocol | `labs/context-residency/README.md` |
| Runbook | `labs/context-residency/runbook.md` |
| Tests | `labs/context-residency/tests.md` |
| Outputs | `labs/context-residency/outputs.md` |
| Question catalog | `labs/context-residency/prompts.json` |
| Run script | `labs/context-residency/run.sh` |
| Matrix helpers | `labs/context-residency/bin/` |
| Synthetic fixtures | `labs/context-residency/fixtures/` |
| Runs | `labs/context-residency/runs/` |

Runs follow `labs/standards/compact-run-layout.md`: `transcript.md` is the
complete chronological execution record, `report.md` is the analytical human
artifact, and detailed logs/JSON live under run-local `assets/`.

## Modes

```text
no-ai       - real case setup, corpus build, mini-RAG selections and score shells
case-native - real YAI case setup and case-bound prompt path through YAI CLI
naked       - C0 naked_no_context direct provider baseline
raw-context - C1 naked_raw_context prompt stuffing baseline
mini-rag    - C2 naked_mini_rag lexical retrieval baseline
compare     - score available outputs and render comparison artifacts
synthetic   - non-canonical fixture/rendering compatibility mode
all         - no-ai + case-native + naked + raw-context + mini-rag + compare
```

This benchmark is invalid as a case-native benchmark if case-bound model calls
do not go through YAI CLI.

## Run

Default no-provider execution:

```bash
labs/context-residency/run.sh --mode no-ai
```

Full local matrix with an OpenAI-compatible provider:

```bash
export YAI_LAB_PROVIDER_URL="http://127.0.0.1:PORT/v1/chat/completions"
export YAI_LAB_PROVIDER_MODEL="Qwen_Qwen3-8B-Q4_K_M.gguf"
labs/context-residency/run.sh --mode all
```

Provider defaults may also be loaded from `.yai/env`:
`YAI_PROVIDER_BASE_URL`, `YAI_PROVIDER_MODEL` and `OPENCODE_LLM_API_KEY`.
Missing provider configuration skips provider-backed conditions cleanly while
still validating case setup, corpus, mini-RAG selection, scorecard shells and
report generation.

## Non-Goals

- No Context Compiler implementation.
- No CaseModelSession core implementation.
- No KV cache implementation.
- No runner/provider change.
- No model invocation required by default.
- No HNSW, DuckDB facts, graph changes, CLORI or filesystem-loop behavior
  change.
