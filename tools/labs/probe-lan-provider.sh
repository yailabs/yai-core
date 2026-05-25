#!/usr/bin/env bash
set -u

if [ -f .yai/env ]; then
  set -a
  . ./.yai/env
  set +a
fi

host="${YAI_PROVIDER_HOST:-172.20.10.3}"
port="${YAI_PROVIDER_PORT:-43117}"
model="${YAI_PROVIDER_MODEL:-qwen-local}"
out_dir=""
prompt="Rispondi in una frase: provider LAN pronto sulla porta 43117."
api_key="${YAI_PROVIDER_API_KEY:-}"
api_key_env="${YAI_PROVIDER_API_KEY_ENV:-}"

if [ -z "$api_key" ] && [ -n "$api_key_env" ]; then
  api_key="$(printenv "$api_key_env" 2>/dev/null || true)"
fi

while [ "$#" -gt 0 ]; do
  case "$1" in
    --host)
      host="${2:-}"
      shift 2
      ;;
    --port)
      port="${2:-}"
      shift 2
      ;;
    --model)
      model="${2:-}"
      shift 2
      ;;
    --out)
      out_dir="${2:-}"
      shift 2
      ;;
    --prompt)
      prompt="${2:-}"
      shift 2
      ;;
    --api-key-env)
      api_key_env="${2:-}"
      api_key="$(printenv "$api_key_env" 2>/dev/null || true)"
      shift 2
      ;;
    *)
      printf 'unknown argument: %s\n' "$1" >&2
      exit 2
      ;;
  esac
done

if [ -z "$out_dir" ]; then
  out_dir="benchmarks/nvidia/lan-provider-$(date +%Y%m%d-%H%M%S)"
fi

api_base="http://${host}:${port}/v1"
models_url="${api_base}/models"
chat_url="${api_base}/chat/completions"

mkdir -p "$out_dir"

cat >"$out_dir/chat-request.json" <<EOF
{
  "model": "$model",
  "stream": false,
  "messages": [
    {
      "role": "user",
      "content": "$prompt"
    }
  ],
  "max_tokens": 96,
  "temperature": 0
}
EOF

cat >"$out_dir/meta.json" <<EOF
{
  "schema": "yai.lan_provider_probe.v1",
  "created_at_utc": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "host": "$host",
  "port": "$port",
  "api_base": "$api_base",
  "model": "$model",
  "models_url": "$models_url",
  "chat_url": "$chat_url"
}
EOF

curl -sS --max-time 10 \
  -w 'http_code=%{http_code}\ntime_total=%{time_total}\nremote_ip=%{remote_ip}\nremote_port=%{remote_port}\n' \
  -o "$out_dir/models.json" \
  "$models_url" >"$out_dir/models.curl-meta.txt" 2>"$out_dir/models.stderr.txt"
models_status="$?"

chat_headers=(-H 'Content-Type: application/json')
if [ -n "$api_key" ]; then
  chat_headers+=(-H "Authorization: Bearer $api_key")
fi

curl -sS --max-time 90 \
  "${chat_headers[@]}" \
  -w 'http_code=%{http_code}\ntime_total=%{time_total}\nremote_ip=%{remote_ip}\nremote_port=%{remote_port}\n' \
  -d "@$out_dir/chat-request.json" \
  -o "$out_dir/chat.json" \
  "$chat_url" >"$out_dir/chat.curl-meta.txt" 2>"$out_dir/chat.stderr.txt"
chat_status="$?"

models_http_code="$(sed -n 's/^http_code=//p' "$out_dir/models.curl-meta.txt" | head -n 1)"
chat_http_code="$(sed -n 's/^http_code=//p' "$out_dir/chat.curl-meta.txt" | head -n 1)"
models_time="$(sed -n 's/^time_total=//p' "$out_dir/models.curl-meta.txt" | head -n 1)"
chat_time="$(sed -n 's/^time_total=//p' "$out_dir/chat.curl-meta.txt" | head -n 1)"

chat_preview=""
if command -v python3 >/dev/null 2>&1; then
  chat_preview="$(python3 -c 'import json,sys; p=sys.argv[1];
try:
 data=json.load(open(p,encoding="utf-8"))
 text=data.get("choices",[{}])[0].get("message",{}).get("content","")
 if not text and isinstance(data.get("error"), dict):
  text="error: " + str(data["error"].get("message", data["error"]))
 print(text[:1000])
except Exception as exc:
 print("unparsed chat response: " + str(exc))
' "$out_dir/chat.json")"
fi

cat >"$out_dir/summary.md" <<EOF
# LAN Provider Probe

Status: local lab observation.

| Field | Value |
|---|---|
| Host | \`$host\` |
| Port | \`$port\` |
| API base | \`$api_base\` |
| Model | \`$model\` |
| Created UTC | \`$(date -u +%Y-%m-%dT%H:%M:%SZ)\` |

## Endpoint Results

| Probe | Curl status | HTTP code | Time seconds |
|---|---:|---:|---:|
| \`GET /v1/models\` | $models_status | ${models_http_code:-n/a} | ${models_time:-n/a} |
| \`POST /v1/chat/completions\` | $chat_status | ${chat_http_code:-n/a} | ${chat_time:-n/a} |

## Chat Preview

\`\`\`text
$chat_preview
\`\`\`

## Files

- \`meta.json\`
- \`models.json\`
- \`models.curl-meta.txt\`
- \`models.stderr.txt\`
- \`chat-request.json\`
- \`chat.json\`
- \`chat.curl-meta.txt\`
- \`chat.stderr.txt\`
EOF

printf 'lan provider probe written: %s\n' "$out_dir"

if [ "$models_status" -ne 0 ] || [ "$chat_status" -ne 0 ]; then
  exit 1
fi
