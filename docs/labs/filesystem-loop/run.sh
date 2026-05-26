#!/usr/bin/env bash
set -euo pipefail

LAB_NAME="filesystem-loop"

usage() {
  cat <<'EOF'
Usage: ./run.sh [--slug <slug>] [--command "<command>"] [--dry-run] [--help]

Creates a compact run folder for this lab:
  runs/YYYYMMDD-<slug>/
    transcript.md
    report.md
    manifest.json
    metrics.json
    assets/

If --command is provided, the command is executed from the lab directory and
stdout/stderr are captured under assets/.
EOF
}

slug="manual"
run_command=""
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
cd "$LAB_DIR"

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
  [ -n "$run_command" ] && echo "would execute command from ${LAB_DIR}"
  exit 0
fi

if [ -e "$run_dir" ]; then
  echo "run already exists: ${run_dir}" >&2
  exit 1
fi

mkdir -p "${run_dir}/assets"

cat > "${run_dir}/transcript.md" <<EOF
# Transcript - ${LAB_NAME} ${run_slug}

Created at: ${created_at}

## Run Identity

- Lab: ${LAB_NAME}
- Run: ${run_slug}
- Status: fill after execution

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

Fill after execution.

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

if [ -n "$run_command" ]; then
  stdout_path="${run_dir}/assets/command.stdout.txt"
  stderr_path="${run_dir}/assets/command.stderr.txt"
  {
    echo
    echo "## Command Execution"
    echo
    echo '```sh'
    printf '%s\n' "$run_command"
    echo '```'
  } >> "${run_dir}/transcript.md"

  set +e
  bash -lc "$run_command" > "$stdout_path" 2> "$stderr_path"
  exit_code=$?
  set -e

  {
    echo
    echo "Exit code: ${exit_code}"
    echo
    echo "Stdout: assets/command.stdout.txt"
    echo
    echo "Stderr: assets/command.stderr.txt"
  } >> "${run_dir}/transcript.md"

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
    "assets/command.stdout.txt",
    "assets/command.stderr.txt"
  ],
  "limitations": [
    "Report and metrics require human review after command execution."
  ]
}
EOF

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
