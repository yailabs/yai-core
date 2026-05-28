#!/usr/bin/env bash
# YAI - context residency benchmark matrix
#
# Purpose:
#   Build LAB.TEST.2S matrix runs across naked, raw-context, mini-RAG,
#   YAI case-bound and logical residency conditions.
#
# Scope:
#   Case setup and case-bound model calls use YAI CLI. Direct HTTP provider
#   calls are allowed only for naked/direct baselines C0-C3.
#
# Non-goals:
#   No Context Compiler, HNSW, external vector DB, LLM judge, provider
#   supervisor, KV cache, CLORI or core runtime behavior change.
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: ./run.sh [--mode no-ai|case-native|naked|raw-context|mini-rag|compare|synthetic|all] [--provider-url URL] [--model MODEL] [--output DIR] [--help]

Default mode is --mode no-ai.

Canonical comparison requires --mode all with provider configured.

Environment defaults are loaded from .yai/env when present:
  YAI_PROVIDER_BASE_URL / YAI_LAB_PROVIDER_URL
  YAI_PROVIDER_MODEL / YAI_LAB_PROVIDER_MODEL
  OPENCODE_LLM_API_KEY / YAI_PROVIDER_API_KEY / YAI_LAB_PROVIDER_API_KEY
EOF
}

LAB_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "${LAB_DIR}/../.." && pwd)"

if [ -f "${REPO_ROOT}/.yai/env" ]; then
  set -a
  # shellcheck disable=SC1091
  . "${REPO_ROOT}/.yai/env"
  set +a
fi

mode="no-ai"
provider_url="${YAI_LAB_PROVIDER_URL:-${YAI_PROVIDER_BASE_URL:-}}"
provider_model="${YAI_LAB_PROVIDER_MODEL:-${YAI_PROVIDER_MODEL:-}}"
provider_api_key="${YAI_LAB_PROVIDER_API_KEY:-${YAI_PROVIDER_API_KEY:-${OPENCODE_LLM_API_KEY:-}}}"
output_dir=""

while [ "$#" -gt 0 ]; do
  case "$1" in
    --mode)
      [ "$#" -ge 2 ] || { echo "missing value for --mode" >&2; exit 2; }
      mode="$2"
      shift 2
      ;;
    --provider-url)
      [ "$#" -ge 2 ] || { echo "missing value for --provider-url" >&2; exit 2; }
      provider_url="$2"
      shift 2
      ;;
    --model)
      [ "$#" -ge 2 ] || { echo "missing value for --model" >&2; exit 2; }
      provider_model="$2"
      shift 2
      ;;
    --output)
      [ "$#" -ge 2 ] || { echo "missing value for --output" >&2; exit 2; }
      output_dir="$2"
      shift 2
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

case "$mode" in
  no-ai|case-native|naked|raw-context|mini-rag|compare|synthetic|all) ;;
  *) echo "unsupported mode: $mode" >&2; exit 2 ;;
esac

cd "$LAB_DIR"

if [ -z "$output_dir" ]; then
  output_dir="runs/$(date -u +"%Y%m%dT%H%M%S%NZ")-$$"
fi
case "$output_dir" in
  /*) ;;
  *) output_dir="${LAB_DIR}/${output_dir}" ;;
esac

python3 "$LAB_DIR/bin/run-matrix.py" \
  --mode "$mode" \
  --provider-url "$provider_url" \
  --model "$provider_model" \
  --api-key "$provider_api_key" \
  --output "$output_dir" \
  --lab-dir "$LAB_DIR" \
  --repo-root "$REPO_ROOT"
