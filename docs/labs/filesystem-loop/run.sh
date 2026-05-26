#!/usr/bin/env bash
set -euo pipefail

LAB_NAME="filesystem-loop"

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

If --prompt-id is provided, exactly one prompt or query is resolved from
prompts.json, written under assets/ and executed.
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

## Pack Fixture Used

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

## Pack Material Used

Fill after execution.

## Case Materialization Posture

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
  prompt_command="$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1], encoding="utf-8")).get("command",""))' "$prompt_json")"

  if [ "$prompt_kind" = "yai-prompt" ] || [ "$prompt_kind" = "yai-prompt-dry-run" ]; then
    prompt_flag=""
    [ "$prompt_kind" = "yai-prompt-dry-run" ] && prompt_flag="--dry-run"
    display_command="${YAI:-yai} prompt ${prompt_flag}< assets/prompt.txt"
    set +e
    (
      cd "$REPO_ROOT"
      export YAI_LAB_RUN_DIR="${LAB_DIR}/${run_dir}"
      export YAI_LAB_RUN_ASSETS="${LAB_DIR}/${run_dir}/assets"
      export YAI_LAB_RUN_SLUG="${run_slug}"
      if [ -f "${YAI_RUN:-build/tmp/manual-case-001}/journal.path" ]; then
        export YAI_JOURNAL="$(cat "${YAI_RUN:-build/tmp/manual-case-001}/journal.path")"
      fi
      "${YAI:-yai}" prompt ${prompt_flag} < "${LAB_DIR}/${prompt_txt}"
    ) > "$stdout_path" 2> "$stderr_path"
    exit_code=$?
    set -e
  elif [ -n "$prompt_command" ]; then
    display_command="$prompt_command"
    set +e
    (
      cd "$REPO_ROOT"
      export YAI_LAB_RUN_DIR="${LAB_DIR}/${run_dir}"
      export YAI_LAB_RUN_ASSETS="${LAB_DIR}/${run_dir}/assets"
      export YAI_LAB_RUN_SLUG="${run_slug}"
      bash -lc "$prompt_command"
    ) > "$stdout_path" 2> "$stderr_path"
    exit_code=$?
    set -e
  else
    echo "unsupported prompt kind: ${prompt_kind}" > "$stderr_path"
    : > "$stdout_path"
    display_command="unsupported prompt kind: ${prompt_kind}"
    exit_code=2
  fi

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
  } > "${run_dir}/transcript.md"

  {
    echo "# ${LAB_NAME} ${run_slug} Report"
    echo
    echo "## Summary"
    echo
    echo "This is a one-prompt lab run created by \`run.sh --prompt-id ${prompt_id}\`."
    echo
    echo "## Prompt"
    echo
    echo "- Prompt id: ${prompt_id}"
    echo "- Prompt kind: ${prompt_kind}"
    echo "- Prompt payload: \`assets/prompt.json\`"
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
    echo "\`metrics.json\` records the command exit code and prompt id."
    echo
    echo "## Evidence / Artifacts"
    echo
    find "${run_dir}/assets" -type f | sort | while IFS= read -r asset_path; do
      printf -- '- `%s`\n' "${asset_path#${run_dir}/}"
    done
    echo
    echo "## Limitations"
    echo
    echo "This run makes no model quality claim, provider readiness claim, hardware benchmark claim or benchmark superiority claim."
  } > "${run_dir}/report.md"

  write_manifest '"One-prompt filesystem-loop run.", "No model quality, provider readiness, hardware benchmark or benchmark superiority claim."'
  cat > "${run_dir}/metrics.json" <<EOF
{
  "schema": "yai.run.metrics.v0",
  "lab": "${LAB_NAME}",
  "run_slug": "${run_slug}",
  "status": "command_recorded",
  "created_at": "${created_at}",
  "command_exit_code": ${exit_code},
  "measurements": {
    "prompt_id": "${prompt_id}",
    "prompt_kind": "${prompt_kind}"
  }
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
    echo "## Pack Fixture Used"
    echo
    echo "No pack runtime was executed by this command run. The lab pack fixture remains at \`pack-fixture/\`."
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
    echo "This is a canonical structural smoke run created by \`run.sh\` after lab-owned tooling and prompt catalog cleanup."
    echo
    echo "## Pack Material Used"
    echo
    echo "No pack runtime was executed. The lab pack fixture remains unchanged."
    echo
    echo "## Case Materialization Posture"
    echo
    echo "No case materialization was performed by this command run."
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
    echo "This run makes no model quality claim, provider readiness claim, hardware benchmark claim or benchmark superiority claim."
  } > "${run_dir}/report.md"

  write_manifest '"Structural/canonical smoke run.", "No model quality, provider readiness, hardware benchmark or benchmark superiority claim."'
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
