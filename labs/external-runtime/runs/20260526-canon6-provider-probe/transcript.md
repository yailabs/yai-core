# Transcript - external-runtime 20260526-canon6-provider-probe

Created at: 2026-05-26T16:10:19Z

## Run Identity

- Lab: external-runtime
- Run: 20260526-canon6-provider-probe
- Status: command_recorded

## Environment

- Repository root: /home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai
- Lab directory: /home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai/labs/external-runtime

## Prompt

- Prompt id: local-provider-ready
- Prompt kind: provider-chat
- Prompt catalog: prompts.json
- Prompt payload: assets/prompt.json

```text
Rispondi in una frase: provider locale pronto sulla porta 43117.
```

## Provider / Endpoint Attachment

# Local Provider Probe

Status: local lab observation.

| Field | Value |
|---|---|
| Host | `127.0.0.1` |
| Port | `43117` |
| API base | `http://127.0.0.1:43117/v1` |
| Model | `qwen-local` |
| Prompt | `Rispondi in una frase: provider locale pronto sulla porta 43117.` |
| Max tokens | `96` |
| Temperature | `0` |
| Created UTC | `2026-05-26T16:10:20Z` |

## Endpoint Results

| Probe | Curl status | HTTP code | Time seconds |
|---|---:|---:|---:|
| `GET /v1/models` | 0 | 200 | 0.000267 |
| `POST /v1/chat/completions` | 0 | 200 | 1.285040 |

## Chat Preview

```text

```

## Files

- `meta.json`
- `models.json`
- `models.curl-meta.txt`
- `models.stderr.txt`
- `chat-request.json`
- `chat.json`
- `chat.curl-meta.txt`
- `chat.stderr.txt`

## Command Execution

```sh
labs/shared/bin/probe-local-provider.sh --host ${YAI_PROVIDER_HOST:-127.0.0.1} --port ${YAI_PROVIDER_PORT:-43117} --prompt-id local-provider-ready
```

Exit code: 0

Stdout: assets/command.stdout.txt
Stderr: assets/command.stderr.txt

## Captured Stdout

```text
local provider probe written: /home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai/labs/external-runtime/runs/20260526-canon6-provider-probe/assets/local-provider-probe
```

## Captured Stderr

```text
```

## Provider Request Payload

Stored at: assets/local-provider-probe/chat-request.json

```json
{
  "model": "qwen-local",
  "stream": false,
  "messages": [
    {
      "role": "user",
      "content": "Rispondi in una frase: provider locale pronto sulla porta 43117."
    }
  ],
  "max_tokens": 96,
  "temperature": 0.0
}
```

## Endpoint Curl Metadata

GET /v1/models:

```text
http_code=200
time_total=0.000267
remote_ip=127.0.0.1
remote_port=43117
```

POST /v1/chat/completions:

```text
http_code=200
time_total=1.285040
remote_ip=127.0.0.1
remote_port=43117
```
