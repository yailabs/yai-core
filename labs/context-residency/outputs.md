# Case-Native Benchmark Matrix Outputs

Status: canonical output anchors for LAB.TEST.2S.

Test 2 is a benchmark family. The canonical outputs distinguish quality,
safety, latency, context efficiency and skipped conditions across naked
no-context, naked raw-context, naked mini-RAG, YAI case-bound and logical YAI
residency.

## Metrics

Every run writes `metrics.json` with:

```text
schema: yai.context_residency.matrix.metrics.v1
test: LAB.TEST.2
run_id
case_ref
provider_model
provider_url
conditions.C0_naked_no_context
conditions.C1_naked_raw_context
conditions.C2_naked_mini_rag
conditions.C3_naked_yai_projection_dump
conditions.C4_yai_case_bound
conditions.C5_yai_case_residency
setup_metrics
score_summary
artifact_paths
```

Token counts are estimates unless a model tokenizer/provider tokenizer is
available. The v0 tokenizer is `approximate_chars_div_4`.

## Corpus

Each case-backed run writes:

```text
corpus/raw-case-corpus.md
assets/corpus/chunks.jsonl
assets/corpus/index.json
assets/corpus/selected-chunks.jsonl
```

The corpus is generated from actual command outputs and filesystem-loop pack
material. Hand-written fixtures are synthetic-only.

## Reports

```text
report.md
transcript.md
assets/commands.log
assets/derived/comparison.md
assets/derived/scorecard.json
assets/derived/condition-summary.md
assets/derived/answers.md
assets/raw/naked/provider-requests.jsonl
assets/raw/naked/provider-responses.jsonl
assets/raw/yai-case/answers.md
assets/raw/comparisons/comparisons.json
```

`scorecard.json` uses deterministic marker scoring. No LLM judge is used in
this wave.

Primary human and machine outputs are aggregated to avoid per-question file
sprawl. Per-command setup logs may remain granular for debugging.

`transcript.md` is the one-file chronological execution transcript: command
log, case setup outputs, naked provider request/response JSONL, YAI case-bound
prompt commands, full answers and summaries.

`report.md` must state that a naked no-context answer is a negative control and
cannot be treated as a useful case-answering baseline.
