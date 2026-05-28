# Case-Native Benchmark Matrix Runbook

Status: executable operator runbook for LAB.TEST.2S.

## Purpose

Run one command and inspect logs, metrics and reports for:

```text
real YAI case setup
journal replay into LMDB
graph/runtime/facts materialization
raw case corpus generation
mini-RAG chunk selection
naked no-context provider baseline
naked raw-context provider baseline
naked mini-RAG provider baseline
case-bound prompts through YAI CLI
logical YAI residency/delta posture
matrix scoring and comparison
```

Test 2 is a benchmark family. The canonical comparison is naked no-context,
naked raw-context, naked mini-RAG, YAI case-bound and YAI logical residency.
Direct provider calls are allowed only for naked baselines; case-bound calls
must go through YAI CLI.

## Default No-AI Run

```bash
labs/context-residency/run.sh --mode no-ai
```

Expected terminal summary:

```text
context_residency_matrix:case_setup ok
context_residency_matrix:corpus ok
context_residency_matrix:mini_rag ok
context_residency_matrix:scorecard ok
context_residency_matrix:no_fake_case_bound_ai ok
context_residency_matrix:synthetic_noncanonical ok
```

## All Modes With Optional Provider

```bash
export YAI_LAB_PROVIDER_URL="http://127.0.0.1:PORT/v1/chat/completions"
export YAI_LAB_PROVIDER_MODEL="Qwen_Qwen3-8B-Q4_K_M.gguf"
labs/context-residency/run.sh --mode all
```

Optional:

```bash
export YAI_LAB_PROVIDER_API_KEY="<local-provider-key-if-required>"
```

If the provider URL or model is missing, provider-backed conditions are skipped
with `provider_not_configured`; this is not a failure for the default no-AI
smoke path.

## Case-Native Command Chain

The run records the full chronological execution in `transcript.md` and keeps a
machine command list in `assets/commands.log`. The canonical YAI chain is:

```bash
make install-local PREFIX=/tmp/yai-cr-i-* YAI_HOME=/tmp/yai-cr-h-*
yaid --socket /tmp/yai-cr-h-*/run/yaid.sock --foreground
yai daemon run-filesystem-loop --socket /tmp/yai-cr-h-*/run/yaid.sock
yai journal replay --path "$JOURNAL"
yai graph materialize --case case:new12-filesystem
yai graph rebuild --case case:new12-filesystem --from journal --path "$JOURNAL"
yai facts init
yai facts extract --case case:new12-filesystem --kind all
yai facts report --case case:new12-filesystem
export YAI_JOURNAL="$JOURNAL"
yai case enter --case case:new12-filesystem --subject subject:llm-provider
yai provider start --dry-run --target external --endpoint "$YAI_LAB_PROVIDER_URL" --kind openai_compatible
yai case attach-provider --case case:new12-filesystem --subject subject:llm-provider --base-url "$YAI_LAB_PROVIDER_URL" --model "$YAI_LAB_PROVIDER_MODEL"
yai prompt --case case:new12-filesystem --subject subject:llm-provider --once "$QUESTION"
```

The pack materialization source is the filesystem-loop fixture pack:

```text
pack_materialization: filesystem-loop fixture pack
source: labs/filesystem-loop/pack-fixture
```

## Output Folder

Each run creates:

```text
labs/context-residency/runs/<timestamp>/
  manifest.json
  metrics.json
  report.md
  transcript.md
  assets/
    commands.log
    corpus/
      raw-case-corpus.md
      chunks.jsonl
      index.json
      selected-chunks.jsonl
    raw/
      case-setup/
      naked/provider-requests.jsonl
      naked/provider-responses.jsonl
      yai-case/answers.md
      comparisons/comparisons.json
    logs/
      yai-command-*.stdout.txt
      yai-command-*.stderr.txt
      provider-direct.log
      mini-rag.log
      yai-prompt-commands.jsonl
    derived/
      comparison.md
      scorecard.json
      condition-summary.md
      answers.md
```

## Validation

```bash
make check-context-residency-lab
make smoke-lab-context-residency-matrix
make lab-context-residency
git diff --check
```

Global checks may also include:

```bash
make check-docs
make smoke
make check
```
