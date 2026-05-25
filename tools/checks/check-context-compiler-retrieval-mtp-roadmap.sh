#!/usr/bin/env bash
# YAI - context compiler retrieval MTP roadmap guard
#
# Purpose:
#   Ensure SPINE.33K doctrine keeps projection, retrieval and MTP boundaries
#   correctly placed.
#
# Scope:
#   Checks docs only. This guard does not validate implementation.
#
# Non-goals:
#   Does not implement HNSW, embeddings, context compiler, MTP or runners.

set -euo pipefail

require_file() {
  test -f "$1" || {
    printf 'missing required file: %s\n' "$1" >&2
    exit 1
  }
}

require_text() {
  local file="$1"
  local text="$2"
  grep -F "$text" "$file" >/dev/null || {
    printf 'missing required text in %s: %s\n' "$file" "$text" >&2
    exit 1
  }
}

roadmap="docs/engineering/four-repo-roadmap.md"
data_roadmap="docs/engineering/data-plane-roadmap.md"
spine="docs/architecture/00-spine.md"
projection="docs/architecture/10-projection-model.md"
context_doc="docs/architecture/26-context-compiler-retrieval-boundary.md"
mtp_doc="docs/architecture/27-model-runtime-mtp-boundary.md"
commands="docs/engineering/command-surface.md"
archive="docs/archive/engineering/waves/spine33k-context-compiler-retrieval-mtp-roadmap-correction.md"

for file in "$roadmap" "$data_roadmap" "$spine" "$projection" "$context_doc" "$mtp_doc" "$commands" "$archive"; do
  require_file "$file"
done

for text in \
  "Context Compiler" \
  "Projection does not disappear" \
  "compiled model context is not truth" \
  "HNSW is candidate retrieval" \
  "HNSW is not graph" \
  "HNSW is not memory" \
  "HNSW is not decision authority" \
  "retrieval unit" \
  "Embedding Manifest" \
  "token budget" \
  "MTP belongs to model runtime" \
  "MTP is not retrieval" \
  "MTP is not policy authority" \
  "supports_mtp" \
  "SPINE.58A" \
  "SPINE.93D"
do
  require_text "$roadmap" "$text"
done

for text in \
  "Context Compiler" \
  "Projection does not disappear" \
  "compiled model context" \
  "HNSW is candidate retrieval" \
  "retrieval_unit_id" \
  "embedding_model_ref" \
  "token budget"
do
  require_text "$context_doc" "$text"
done

for text in \
  "MTP belongs to model runtime" \
  "MTP is not retrieval" \
  "MTP is not policy authority" \
  "supports_mtp" \
  "acceptance_rate" \
  "latency_delta" \
  "tokens_per_second_delta"
do
  require_text "$mtp_doc" "$text"
done

for text in \
  "yai context compile --dry-run" \
  "yai context inspect <frame/ref>" \
  "yai model-runtime capabilities" \
  "yai model-runtime benchmark --decoding normal|mtp" \
  "not implemented by SPINE.33J or SPINE.33K"
do
  require_text "$commands" "$text"
done

require_text "$projection" "Projection does not disappear"
require_text "$spine" "Context Compiler"
require_text "$data_roadmap" "SPINE.58E Context Assembly / Rerank / Token Budget Packing"
require_text "$archive" "No MTP implementation."

printf 'context-compiler-retrieval-mtp-roadmap: ok\n'
