# Provider Boundary

This page documents how model providers and inference engines should be
understood around YAI.

YAI does not run models. A provider remains an external boundary around a case:
it may produce candidate model output, but that output is not authority over the
case. YAI records which case the provider activity belongs to, which subject it
touches, what control material applies, what was observed, and what receipt,
record, projection, or memory may be derived.

This is an experiment matrix, not a support matrix. Do not read `candidate`,
`manual`, or `hardware-gated` as tested YAI support.

## Status Legend

- `tested/manual`: exercised in a local YAI lab or manual run.
- `candidate`: plausible provider boundary for future experiments.
- `candidate/server`: plausible server-oriented provider experiment.
- `candidate/manual`: plausible manual experiment with local setup.
- `hardware-gated`: requires hardware or environment not generally available.
- `hardware-gated/sketch`: documented as a possible specialized experiment,
  not tested in YAI.
- `planned`: documented direction, not current YAI behavior.
- `unsupported`: not a current YAI provider experiment.
- `unknown`: not enough verified information for a useful claim.

## Boundary Rules

- OpenAI-compatible does not mean identical behavior.
- A working `/v1/chat/completions` endpoint is only the first probe.
- `/v1/models` is useful for discovery but is not implemented consistently by
  every provider.
- Streaming, tools, embeddings, reasoning fields, usage accounting, and error
  shapes must be probed separately.
- Provider output becomes candidate material inside a YAI case; it does not
  mutate decisions, receipts, records, or durable case state by itself.
- No provider is required for the baseline repository tests.

## Minimal Provider Probe Shape

A future YAI provider probe should prefer small, explicit checks:

1. `GET /v1/models`, if the provider implements it.
2. `POST /v1/chat/completions` with a short non-streaming prompt.
3. `POST /v1/chat/completions` with `stream: true`, if streaming is claimed.
4. Tool-call probe only if the provider documents tool support.
5. Embedding probe only if the provider documents embeddings.
6. Case-bound prompt dry run before sending model traffic.

Probe results should be recorded as provider observations, not as proof that the
provider is generally supported.

## Provider Experiment Matrix

| provider_id | Engine | Protocol | Default base URL | Model format | Small model candidate | Requires GPU | Requires model download | Streaming | Tools | Embeddings | Tested in YAI | Lab status | Notes |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| `llama-server` | `llama.cpp / llama-server` | OpenAI-compatible | `http://127.0.0.1:<PORT>/v1` | GGUF | Qwen3 8B Q4, Phi, Gemma small | no, but useful | yes | provider-documented | not verified in YAI | provider-documented | yes | tested/manual | Current model-behavior lab baseline. |
| `ollama` | Ollama | OpenAI-compatible subset | `http://localhost:11434/v1` | Ollama-managed models | `qwen3:8b`, `llama3.2` | no, but useful | yes | candidate | candidate | provider-documented | no | candidate | Good first alternate local server; compatibility is partial. |
| `lm-studio` | LM Studio | OpenAI-compatible, Anthropic-compatible | verify local server config, commonly local `/v1` | app-managed GGUF and local models | any loaded small instruct model | no, but useful | yes | provider-documented | provider-documented | provider-documented | no | candidate | Useful manual GUI or headless local test. |
| `llama-cpp-python` | llama-cpp-python server | OpenAI-compatible | verify server launch | GGUF | small GGUF instruct model | no, but useful | yes | candidate | provider-documented | candidate | no | candidate | Python wrapper path for quick prototypes. |
| `localai` | LocalAI | OpenAI-compatible | `http://localhost:8080/v1` | multi-backend | small GGUF or bundled example model | depends on backend | usually | candidate | candidate | candidate | no | candidate | Useful compatibility layer, but backend behavior varies. |
| `vllm` | vLLM | OpenAI-compatible | `http://localhost:8000/v1` | Hugging Face models | small instruct model with chat template | usually yes | yes | provider-documented | provider-documented | provider-documented | no | candidate/server | More serious GPU/server path; chat template matters. |
| `sglang` | SGLang | OpenAI-compatible and native | verify launch | Hugging Face models | Qwen, Gemma, Llama small | usually yes | yes | provider-documented | candidate | candidate | no | candidate/server | Advanced serving path; probe concrete endpoints before YAI claims. |
| `tgi` | Hugging Face TGI | OpenAI Messages API compatible | `http://localhost:3000/v1` in docs examples | Hugging Face models | small instruct model | usually yes | yes | provider-documented | unknown | unknown | no | candidate/server | Deployment-oriented server path; Messages API requires compatible TGI version. |
| `tabbyapi` | TabbyAPI / ExLlama | OAI-compatible | verify config | EXL2, EXL3, GPTQ | small EXL model | NVIDIA path | yes | provider-documented | unknown | unknown | no | candidate/manual | Interesting single-GPU NVIDIA/ExLlama path. |
| `mlx-lm` | MLX LM server | OpenAI-like HTTP | `http://localhost:8080/v1` | MLX / Hugging Face MLX repos | `mlx-community/Mistral-7B-Instruct-v0.3-4bit` or smaller | Apple Silicon / Metal | yes | provider-documented | unknown | unknown | no | hardware-gated | Apple Silicon path; docs say the server is not recommended for production. |
| `ds4` | DS4 DeepSeek V4 Flash server | OpenAI-compatible sketch | source example: `http://127.0.0.1:18000/v1` | DS4 DeepSeek V4 Flash GGUF | none small | Metal / specialized memory profile | yes | source-claimed | source-claimed | unknown | no | hardware-gated/sketch | Specialized provider sketch only. No YAI integration is claimed. |

## Protocol / Client References

Open WebUI is not listed as a provider engine here. It is useful as a
protocol-oriented client reference because it emphasizes a small OpenAI-compatible
surface:

- `GET /v1/models` for model discovery, recommended when available;
- `POST /v1/chat/completions` as the required chat endpoint;
- `POST /v1/embeddings` only when retrieval workflows need embeddings.

The same probe shape can be exercised with `curl` or an OpenAI-compatible SDK
against a local provider. YAI should treat those calls as provider-boundary
activity scoped to a case, not as provider ownership of the case.

## Current YAI Evidence

Current repository evidence is limited:

- `llama-server` has been used by local provider and filesystem-loop lab evidence under
  `docs/labs/external-runtime/` and `docs/labs/filesystem-loop/`.
- The lab shows case enter, provider attach, prompt dry run, model prompt,
  model output, model interpretation residue, and post-run records.
- The lab is representative and non-deterministic. It is not a benchmark,
  conformance fixture, or security guarantee.

No other provider in this matrix has current YAI-tested support.

## Experiment Classes

`tested/manual` means a local YAI manual or lab path exists.

`candidate` means the provider appears technically plausible because it exposes
an OpenAI-compatible or OpenAI-like endpoint, but YAI has not tested it.

`candidate/server` means the provider is more deployment- or throughput-oriented
and may require GPU/server setup before a useful YAI probe.

`hardware-gated` means the experiment requires hardware that should not be part
of baseline repository validation.

`hardware-gated/sketch` means the provider is included only to preserve an
interesting future experiment boundary. It is not current YAI support.

## What This Does Not Claim

- It does not claim broad provider support.
- It does not claim DS4 integration.
- It does not claim deterministic model behavior.
- It does not require provider setup for baseline YAI tests.
- It does not define JSON output or exit-code contracts.
- It does not make provider-specific commands part of the YAI command surface.

## References

- [llama.cpp server](https://github.com/ggml-org/llama.cpp/blob/master/tools/server/README.md)
- [Ollama OpenAI compatibility](https://docs.ollama.com/api/openai-compatibility)
- [LM Studio developer docs](https://lmstudio.ai/docs/developer)
- [llama-cpp-python server](https://llama-cpp-python.readthedocs.io/en/stable/server/)
- [LocalAI quickstart](https://localai.io/basics/getting_started/index.html)
- [vLLM OpenAI-compatible server](https://docs.vllm.ai/en/stable/serving/openai_compatible_server/)
- [SGLang OpenAI-compatible APIs](https://docs.sglang.io/docs/basic_usage/openai_api_completions)
- [Hugging Face TGI HTTP API](https://huggingface.co/docs/text-generation-inference/reference/api_reference)
- [TabbyAPI docs](https://theroyallab.github.io/tabbyAPI/)
- [MLX LM server](https://github.com/ml-explore/mlx-lm/blob/main/mlx_lm/SERVER.md)
- [Open WebUI OpenAI-compatible provider guide](https://docs.openwebui.com/getting-started/quick-start/connect-a-provider/starting-with-openai-compatible/)
- [DS4 OpenClaw provider note](https://docs.openclaw.ai/providers/ds4)
