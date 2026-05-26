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
