# REPO.4 -- Backend / Provider Boundary Reference + Experiment Matrix

Status: completed locally

## What Changed

Created a public provider-boundary reference and experiment matrix:

- provider boundary rules;
- minimal provider probe shape;
- candidate provider matrix;
- protocol/client reference note;
- current YAI evidence;
- explicit non-claims.

The document is not DS4-centered. DS4 is listed only as a hardware-gated sketch.

## Files Read

- `README.md`
- `docs/index.md`
- `docs/quickstart.md`
- `docs/test-cases.md`
- `docs/labs/model-behavior/README.md`
- `docs/labs/model-behavior/filesystem-loop-qwen3-8b-q4.md`
- `docs/engineering/command-surface.md`
- `docs/engineering/testing.md`

## Provider Sources Checked

- llama.cpp server docs
- Ollama OpenAI compatibility docs
- LM Studio developer docs
- llama-cpp-python server docs
- LocalAI quickstart
- vLLM OpenAI-compatible server docs
- SGLang OpenAI-compatible API docs
- Hugging Face TGI HTTP API docs
- TabbyAPI docs
- MLX LM server docs
- Open WebUI OpenAI-compatible provider guide
- DS4 OpenClaw provider note

## Files Created

- `docs/providers.md`
- `docs/internal/repo-readiness/repo-4-backend-provider-boundary-reference.md`

## Files Updated

- `README.md`
- `docs/index.md`

## Providers Included

- `llama.cpp / llama-server`
- `Ollama`
- `LM Studio`
- `llama-cpp-python server`
- `LocalAI`
- `vLLM`
- `SGLang`
- `Hugging Face TGI`
- `TabbyAPI / ExLlama`
- `MLX LM server`
- `DS4`

## Status Posture

- `llama-server`: tested/manual
- `Ollama`: candidate
- `LM Studio`: candidate
- `llama-cpp-python`: candidate
- `LocalAI`: candidate
- `vLLM`: candidate/server
- `SGLang`: candidate/server
- `TGI`: candidate/server
- `TabbyAPI`: candidate/manual
- `MLX LM`: hardware-gated
- `DS4`: hardware-gated/sketch

## Material Intentionally Not Claimed

- no broad provider support claim;
- no DS4 integration claim;
- no deterministic model behavior claim;
- no provider requirement for baseline tests;
- no invented YAI provider commands;
- no JSON output or exit-code contract.

## Validation Results

Passed:

```sh
git diff --check
make info
```

Link target checks for the new provider doc and updated public entrypoints
passed.

Current branch caveat:

```sh
make check
```

failed in unrelated smoke validation after source/check work from the
SPINE.33A branch state changed the command status line. The failing smoke
expected:

```text
status: SPINE.32 LMDB Case / Subject / Receipt Indexes
```

but current `yai info` reports:

```text
status: SPINE.33A Control / Carrier Substrate Primitives
```

This REPO.4 wave did not alter source code, Makefile targets, or smoke tests.

## Remaining Risks

- Provider feature support changes quickly; this matrix should be treated as a
  point-in-time experiment guide.
- Many fields are candidate-level until a YAI provider probe records evidence.
- Current branch contains unrelated source/check work that may affect full
  repository validation.

## Next Wave

REPO.5 Provider Probe / Minimal Adapter Experiment, or the next repository
readiness wave selected by maintainers.
