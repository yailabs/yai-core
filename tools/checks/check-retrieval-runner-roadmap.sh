#!/usr/bin/env bash
# YAI - retrieval and runner roadmap guard
#
# Purpose:
#   Ensure roadmap doctrine reserves retrieval and model-runtime boundaries.
#
# Scope:
#   Checks docs only. This guard does not validate implementation.
#
# Non-goals:
#   Does not implement HNSW, embeddings, DS4, model runners or commands.

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

roadmap="work/spines/yai-spine.md"
data_roadmap="work/spines/data-plane.md"
spine="work/archive/architecture-snapshots/00-spine.md"
projection="work/archive/architecture-snapshots/10-projection-model.md"
commands="work/spines/command-surface.md"
archive="work/archive/legacy-docs/engineering/waves/spine33j-retrieval-model-runtime-roadmap-rebase.md"

for file in "$roadmap" "$data_roadmap" "$spine" "$projection" "$commands" "$archive"; do
  require_file "$file"
done

for text in \
  "RETRIEVAL" \
  "Context Compiler" \
  "HNSW is candidate retrieval" \
  "candidate retrieval" \
  "not graph" \
  "not memory" \
  "not decision authority" \
  "Embedding Manifest" \
  "case / subject / policy scoped retrieval" \
  "Model Runtime / Runner Boundary" \
  "not universal inference engine" \
  "runner output is not authority" \
  "SPINE.58A" \
  "SPINE.58G" \
  "SPINE.93A" \
  "SPINE.93F"
do
  require_text "$roadmap" "$text"
done

for text in \
  "yai retrieval status" \
  "yai retrieval build" \
  "yai retrieval query" \
  "yai retrieval candidates" \
  "yai context compile --dry-run" \
  "yai model-runtime inspect" \
  "yai model-runtime capabilities" \
  "yai model-runtime invoke --dry-run" \
  "not implemented by SPINE.33J or SPINE.33K"
do
  require_text "$commands" "$text"
done

require_text "$spine" "HNSW is candidate retrieval"
require_text "$spine" "YAI must not immediately reinvent"
require_text "$projection" "retrieval candidates are not projection truth"
require_text "$data_roadmap" "SPINE.58A Context Compiler / Retrieval Boundary Doctrine"
require_text "$archive" "No HNSW implementation."

printf 'retrieval-runner-roadmap: ok\n'
