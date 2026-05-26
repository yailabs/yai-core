#!/usr/bin/env bash
set -euo pipefail

LAB_NAME="external-runtime"

usage() {
  cat <<'EOF'
Usage: ./run.sh [--slug <slug>] [--command "<command>"] [--prompt-id <id>] [--list-prompts] [--dry-run] [--help]

Creates a compact run folder for this lab:
  runs/YYYYMMDD-<slug>/
    transcript.md
    report.md
    manifest.json
    metrics.json
    assets/

If --command is provided, the command is executed from the repository root and
stdout/stderr are captured under assets/.

If --prompt-id is provided, exactly one provider prompt is resolved from
prompts.json, written under assets/ and executed against the local provider.
EOF
}

slug="manual"
run_command=""
prompt_id=""
list_prompts=0
dry_run=0

while [ "$#" -gt 0 ]; do
  case "$1" in
    --slug)
      [ "$#" -ge 2 ] || { echo "missing value for --slug" >&2; exit 2; }
      slug="$2"
      shift 2
      ;;
    --command)
      [ "$#" -ge 2 ] || { echo "missing value for --command" >&2; exit 2; }
      run_command="$2"
      shift 2
      ;;
    --prompt-id)
      [ "$#" -ge 2 ] || { echo "missing value for --prompt-id" >&2; exit 2; }
      prompt_id="$2"
      shift 2
      ;;
    --list-prompts)
      list_prompts=1
      shift
      ;;
    --dry-run)
      dry_run=1
      shift
      ;;
    --help|-h)
      usage
      exit 0
      ;;
    *)
      echo "unknown argument: $1" >&2
      usage >&2
      exit 2
      ;;
  esac
done

case "$slug" in
  *[!A-Za-z0-9._-]*|"")
    echo "slug must contain only A-Z, a-z, 0-9, dot, underscore or dash" >&2
    exit 2
    ;;
esac

LAB_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "${LAB_DIR}/../../.." && pwd)"
PROMPTS_FILE="${LAB_DIR}/prompts.json"
SHARED_BIN="${REPO_ROOT}/labs/shared/bin"
cd "$LAB_DIR"

if [ "$list_prompts" -eq 1 ]; then
  python3 - "$PROMPTS_FILE" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as fh:
    data = json.load(fh)
for item in data.get("prompts", []):
    print(f"{item['id']}\t{item.get('kind', '')}\t{item.get('description', '')}")
PY
  exit 0
fi

if [ -n "$run_command" ] && [ -n "$prompt_id" ]; then
  echo "--command and --prompt-id are mutually exclusive" >&2
  exit 2
fi

today="$(date +%Y%m%d)"
created_at="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
run_slug="${today}-${slug}"
run_dir="runs/${run_slug}"

case "$run_dir" in
  runs/*) ;;
  *) echo "internal path guard failed" >&2; exit 2 ;;
esac

if [ "$dry_run" -eq 1 ]; then
  echo "would create ${run_dir}"
  [ -n "$run_command" ] && echo "would execute command from ${REPO_ROOT}"
  [ -n "$prompt_id" ] && echo "would execute prompt id ${prompt_id} from ${PROMPTS_FILE}"
  exit 0
fi

if [ -e "$run_dir" ]; then
  echo "run already exists: ${run_dir}" >&2
  exit 1
fi

mkdir -p "${run_dir}/assets"

write_skeleton() {
  cat > "${run_dir}/transcript.md" <<EOF
# Transcript - ${LAB_NAME} ${run_slug}

Created at: ${created_at}

## Run Identity

- Lab: ${LAB_NAME}
- Run: ${run_slug}
- Status: skeleton

## Environment

Fill after execution.

## Provider / Endpoint Attachment

Fill after execution.

## Commands

Fill after execution.

## Raw Output

Fill after execution.

## Errors

Fill after execution.
EOF

  cat > "${run_dir}/report.md" <<EOF
# ${LAB_NAME} ${run_slug} Report

## Summary

Generated skeleton. Fill after execution.

## Runtime Target

Fill after execution.

## Execution Result

Fill after execution.

## Metrics

Fill after execution.

## Evidence / Artifacts

Fill after execution.

## Limitations

Fill after execution.

## Next Run

Fill after execution.
EOF

  cat > "${run_dir}/manifest.json" <<EOF
{
  "schema": "yai.compact-run.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "skeleton",
  "created_at": "${created_at}",
  "protocol": "../../README.md",
  "transcript": "transcript.md",
  "report": "report.md",
  "metrics": "metrics.json",
  "assets_dir": "assets",
  "command_provided": false,
  "limitations": [
    "Generated skeleton; fill after execution."
  ]
}
EOF

  cat > "${run_dir}/metrics.json" <<EOF
{
  "schema": "yai.run.metrics.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "skeleton",
  "created_at": "${created_at}",
  "command_exit_code": null,
  "measurements": {}
}
EOF
}

write_manifest() {
  local limitations="$1"
  cat > "${run_dir}/manifest.json" <<EOF
{
  "schema": "yai.compact-run.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "command_recorded",
  "created_at": "${created_at}",
  "protocol": "../../README.md",
  "transcript": "transcript.md",
  "report": "report.md",
  "metrics": "metrics.json",
  "assets_dir": "assets",
  "command_provided": true,
  "assets": [
EOF
  first_asset=1
  while IFS= read -r asset_path; do
    asset="${asset_path#${run_dir}/}"
    if [ "$first_asset" -eq 1 ]; then
      first_asset=0
    else
      printf ',\n' >> "${run_dir}/manifest.json"
    fi
    printf '    "%s"' "$asset" >> "${run_dir}/manifest.json"
  done < <(find "${run_dir}/assets" -type f | sort)
  cat >> "${run_dir}/manifest.json" <<EOF

  ],
  "limitations": [
    ${limitations}
  ]
}
EOF
}

resolve_prompt() {
  python3 - "$PROMPTS_FILE" "$prompt_id" "${run_dir}/assets/prompt.json" "${run_dir}/assets/prompt.txt" <<'PY'
import json
import sys

catalog_path, prompt_id, json_out, text_out = sys.argv[1:5]
with open(catalog_path, encoding="utf-8") as fh:
    catalog = json.load(fh)
for item in catalog.get("prompts", []):
    if item.get("id") == prompt_id:
        with open(json_out, "w", encoding="utf-8") as out:
            json.dump(item, out, indent=2, ensure_ascii=False)
            out.write("\n")
        with open(text_out, "w", encoding="utf-8") as out:
            out.write(item.get("content", ""))
            out.write("\n")
        raise SystemExit(0)
raise SystemExit(f"prompt id not found: {prompt_id}")
PY
}

write_skeleton

if [ -n "$prompt_id" ]; then
  resolve_prompt
  stdout_path="${run_dir}/assets/command.stdout.txt"
  stderr_path="${run_dir}/assets/command.stderr.txt"
  prompt_json="${run_dir}/assets/prompt.json"
  prompt_txt="${run_dir}/assets/prompt.txt"
  prompt_kind="$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1], encoding="utf-8")).get("kind",""))' "$prompt_json")"
  max_tokens="$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1], encoding="utf-8")).get("max_tokens",96))' "$prompt_json")"
  temperature="$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1], encoding="utf-8")).get("temperature",0))' "$prompt_json")"

  if [ "$prompt_kind" != "provider-chat" ]; then
    echo "unsupported prompt kind for external-runtime: ${prompt_kind}" > "$stderr_path"
    : > "$stdout_path"
    exit_code=2
  else
    display_command="labs/shared/bin/probe-local-provider.sh --host \${YAI_PROVIDER_HOST:-127.0.0.1} --port \${YAI_PROVIDER_PORT:-43117} --prompt-id ${prompt_id}"
    set +e
    (
      cd "$REPO_ROOT"
      export YAI_LAB_RUN_DIR="${LAB_DIR}/${run_dir}"
      export YAI_LAB_RUN_ASSETS="${LAB_DIR}/${run_dir}/assets"
      export YAI_LAB_RUN_SLUG="${run_slug}"
      "${SHARED_BIN}/probe-local-provider.sh" \
        --host "${YAI_PROVIDER_HOST:-127.0.0.1}" \
        --port "${YAI_PROVIDER_PORT:-43117}" \
        --model "${YAI_PROVIDER_MODEL:-qwen-local}" \
        --prompt "$(cat "${LAB_DIR}/${prompt_txt}")" \
        --max-tokens "$max_tokens" \
        --temperature "$temperature" \
        --api-key-env OPENCODE_LLM_API_KEY \
        --out "${LAB_DIR}/${run_dir}/assets/local-provider-probe"
    ) > "$stdout_path" 2> "$stderr_path"
    exit_code=$?
    set -e
  fi

  prompt_payload="${run_dir}/assets/local-provider-probe/chat-request.json"
  probe_summary="${run_dir}/assets/local-provider-probe/summary.md"
  models_meta="${run_dir}/assets/local-provider-probe/models.curl-meta.txt"
  chat_meta="${run_dir}/assets/local-provider-probe/chat.curl-meta.txt"

  {
    echo "# Transcript - ${LAB_NAME} ${run_slug}"
    echo
    echo "Created at: ${created_at}"
    echo
    echo "## Run Identity"
    echo
    echo "- Lab: ${LAB_NAME}"
    echo "- Run: ${run_slug}"
    echo "- Status: command_recorded"
    echo
    echo "## Environment"
    echo
    echo "- Repository root: ${REPO_ROOT}"
    echo "- Lab directory: ${LAB_DIR}"
    echo "- Run assets: assets/"
    echo
    echo "## Prompt"
    echo
    echo "- Prompt id: ${prompt_id}"
    echo "- Prompt kind: ${prompt_kind}"
    echo "- Prompt catalog: prompts.json"
    echo "- Prompt payload: assets/prompt.json"
    echo
    echo '```text'
    sed -n '1,200p' "$prompt_txt" || true
    echo '```'
    echo
    echo "## Provider / Endpoint Attachment"
    echo
    if [ -f "$probe_summary" ]; then
      sed -n '1,80p' "$probe_summary"
    else
      echo "No provider probe summary was generated by this command."
    fi
    echo
    echo "## Command Execution"
    echo
    echo '```sh'
    printf '%s\n' "$display_command"
    echo '```'
    echo
    echo "Exit code: ${exit_code}"
    echo
    echo "Stdout: assets/command.stdout.txt"
    echo "Stderr: assets/command.stderr.txt"
    echo
    echo "## Captured Stdout"
    echo
    echo '```text'
    cat "$stdout_path" || true
    echo '```'
    echo
    echo "## Captured Stderr"
    echo
    echo '```text'
    cat "$stderr_path" || true
    echo '```'
    if [ -f "$prompt_payload" ]; then
      echo
      echo "## Provider Request Payload"
      echo
      echo "Stored at: assets/local-provider-probe/chat-request.json"
      echo
      echo '```json'
      sed -n '1,160p' "$prompt_payload" || true
      echo '```'
    fi
    if [ -f "$models_meta" ] || [ -f "$chat_meta" ]; then
      echo
      echo "## Endpoint Curl Metadata"
      echo
      if [ -f "$models_meta" ]; then
        echo "GET /v1/models:"
        echo
        echo '```text'
        sed -n '1,40p' "$models_meta" || true
        echo '```'
      fi
      if [ -f "$chat_meta" ]; then
        echo
        echo "POST /v1/chat/completions:"
        echo
        echo '```text'
        sed -n '1,40p' "$chat_meta" || true
        echo '```'
      fi
    fi
  } > "${run_dir}/transcript.md"

  {
    echo "# ${LAB_NAME} ${run_slug} Report"
    echo
    echo "## Summary"
    echo
    echo "This is a canonical provider probe run created by \`run.sh --prompt-id ${prompt_id}\` after lab-owned tooling and prompt catalog cleanup."
    echo
    echo "## Prompt"
    echo
    echo "- Prompt id: ${prompt_id}"
    echo "- Prompt kind: ${prompt_kind}"
    echo "- Prompt payload: \`assets/prompt.json\`"
    echo "- Provider request payload: \`assets/local-provider-probe/chat-request.json\`"
    echo
    echo "## Runtime Target"
    echo
    if [ -f "$probe_summary" ]; then
      sed -n '1,80p' "$probe_summary"
    else
      echo "No provider endpoint was attached by this command."
    fi
    echo
    echo "## Execution Result"
    echo
    echo '```sh'
    printf '%s\n' "$display_command"
    echo '```'
    echo
    echo "Exit code: ${exit_code}"
    echo
    echo "## Metrics"
    echo
    echo "\`metrics.json\` records the command exit code and endpoint metadata when available."
    echo
    echo "## Evidence / Artifacts"
    echo
    find "${run_dir}/assets" -type f | sort | while IFS= read -r asset_path; do
      printf -- '- `%s`\n' "${asset_path#${run_dir}/}"
    done
    echo
    echo "## Limitations"
    echo
    echo "This run makes no provider quality claim, model quality claim, hardware benchmark claim or benchmark superiority claim."
  } > "${run_dir}/report.md"

  measurements_json='{}'
  if [ -f "$models_meta" ] || [ -f "$chat_meta" ]; then
    models_http_code="$(sed -n 's/^http_code=//p' "$models_meta" 2>/dev/null | head -n 1 || true)"
    chat_http_code="$(sed -n 's/^http_code=//p' "$chat_meta" 2>/dev/null | head -n 1 || true)"
    models_remote_ip="$(sed -n 's/^remote_ip=//p' "$models_meta" 2>/dev/null | head -n 1 || true)"
    chat_remote_ip="$(sed -n 's/^remote_ip=//p' "$chat_meta" 2>/dev/null | head -n 1 || true)"
    measurements_json=$(cat <<EOF_MEASUREMENTS
{
    "prompt_id": "${prompt_id}",
    "prompt_kind": "${prompt_kind}",
    "models_http_code": "${models_http_code:-unknown}",
    "chat_http_code": "${chat_http_code:-unknown}",
    "models_remote_ip": "${models_remote_ip:-unknown}",
    "chat_remote_ip": "${chat_remote_ip:-unknown}"
  }
EOF_MEASUREMENTS
)
  fi

  write_manifest '"One-prompt external-runtime run.", "No provider quality, model quality, hardware benchmark or benchmark superiority claim."'
  cat > "${run_dir}/metrics.json" <<EOF
{
  "schema": "yai.run.metrics.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "command_recorded",
  "created_at": "${created_at}",
  "command_exit_code": ${exit_code},
  "measurements": ${measurements_json}
}
EOF
  echo "created ${run_dir}"
  exit "$exit_code"
fi

if [ -n "$run_command" ]; then
  stdout_path="${run_dir}/assets/command.stdout.txt"
  stderr_path="${run_dir}/assets/command.stderr.txt"

  set +e
  (
    cd "$REPO_ROOT"
    export YAI_LAB_RUN_DIR="${LAB_DIR}/${run_dir}"
    export YAI_LAB_RUN_ASSETS="${LAB_DIR}/${run_dir}/assets"
    export YAI_LAB_RUN_SLUG="${run_slug}"
    bash -lc "$run_command"
  ) > "$stdout_path" 2> "$stderr_path"
  exit_code=$?
  set -e

  {
    echo "# Transcript - ${LAB_NAME} ${run_slug}"
    echo
    echo "Created at: ${created_at}"
    echo
    echo "## Run Identity"
    echo
    echo "- Lab: ${LAB_NAME}"
    echo "- Run: ${run_slug}"
    echo "- Status: command_recorded"
    echo
    echo "## Environment"
    echo
    echo "- Repository root: ${REPO_ROOT}"
    echo "- Lab directory: ${LAB_DIR}"
    echo "- Run assets: assets/"
    echo
    echo "## Provider / Endpoint Attachment"
    echo
    echo "No provider prompt catalog was used by this command run."
    echo
    echo "## Command Execution"
    echo
    echo '```sh'
    printf '%s\n' "$run_command"
    echo '```'
    echo
    echo "Exit code: ${exit_code}"
    echo
    echo "Stdout: assets/command.stdout.txt"
    echo "Stderr: assets/command.stderr.txt"
    echo
    echo "## Captured Stdout"
    echo
    echo '```text'
    cat "$stdout_path" || true
    echo '```'
    echo
    echo "## Captured Stderr"
    echo
    echo '```text'
    cat "$stderr_path" || true
    echo '```'
  } > "${run_dir}/transcript.md"

  {
    echo "# ${LAB_NAME} ${run_slug} Report"
    echo
    echo "## Summary"
    echo
    echo "This is a structural/canonical smoke run created by \`run.sh\`."
    echo
    echo "## Runtime Target"
    echo
    echo "No provider endpoint was attached by this command."
    echo
    echo "## Execution Result"
    echo
    echo '```sh'
    printf '%s\n' "$run_command"
    echo '```'
    echo
    echo "Exit code: ${exit_code}"
    echo
    echo "## Metrics"
    echo
    echo "\`metrics.json\` records the command exit code."
    echo
    echo "## Evidence / Artifacts"
    echo
    find "${run_dir}/assets" -type f | sort | while IFS= read -r asset_path; do
      printf -- '- `%s`\n' "${asset_path#${run_dir}/}"
    done
    echo
    echo "## Limitations"
    echo
    echo "This run makes no provider quality claim, model quality claim, hardware benchmark claim or benchmark superiority claim."
  } > "${run_dir}/report.md"

  write_manifest '"Structural/canonical smoke run.", "No provider quality, model quality, hardware benchmark or benchmark superiority claim."'
  cat > "${run_dir}/metrics.json" <<EOF
{
  "schema": "yai.run.metrics.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "command_recorded",
  "created_at": "${created_at}",
  "command_exit_code": ${exit_code},
  "measurements": {}
}
EOF
  echo "created ${run_dir}"
  exit "$exit_code"
fi

echo "created ${run_dir}"
