#!/usr/bin/env sh
set -eu

# YAI - Model-native actor roadmap guard
#
# Purpose:
#   Keep SPINE.52A as a roadmap/doctrine update after completed SPINE.52.
#
# Non-goals:
#   Does not implement model training, adapters, runners, providers or
#   ContextFrame lifecycle behavior.

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
cd "$ROOT"

fail() {
  printf 'check-model-native-actor-roadmap: %s\n' "$1" >&2
  exit 1
}

for file in \
  work/spines/yai-spine.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/waves/spine52a-model-native-actor-adapter-roadmap-rebase.md
do
  [ -f "$file" ] || fail "missing file: $file"
done

spine="work/spines/yai-spine.md"

for phrase in \
  "SPINE.52  Live Projection Frame Schema                              done" \
  "SPINE.52A Model-Native Actor / Adapter Roadmap Rebase               done" \
  "SPINE.53  Projection Delta Schema                                   planned" \
  "SPINE.58S Model-Visible YAI Grammar / Actor Participation Contract   planned" \
  "SPINE.58T Model-Native Training Corpus / Lab Evidence Boundary       planned" \
  "SPINE.93G Model Participation Profile + Behavior Manifest            planned" \
  "SPINE.93H Actor Adapter Contract + Evaluation Boundary               planned" \
  "SPINE.93I Specialist Model / Architecture Lab Boundary               planned" \
  "SPINE.93J Model-Native Actor Boundary Freeze                         planned" \
  "## Model-Native Actor / Adapter Doctrine" \
  "Runtime-owned:" \
  "Model-adaptable:" \
  "A YAI-native model is not YAI." \
  "A model cannot approve its own operation." \
  "A model cannot mutate durable truth directly." \
  "A model cannot replace records, graph, facts, policy, memory or audit." \
  "KV/cache residency is runner-local optimization, not YAI memory, truth," \
  "Token reduction can be accepted as early evidence." \
  "Latency reduction must not be claimed until measured at runner/provider level." \
  "SPINE.52A is a roadmap/doctrine update only. Command Surface: no command"
do
  grep -F "$phrase" "$spine" >/dev/null || fail "missing yai-spine phrase: $phrase"
done

grep -F "No command surface changes." work/spines/command-surface.md >/dev/null ||
  fail "command-surface must state no command surface changes"

grep -F "No model training." work/waves/spine52a-model-native-actor-adapter-roadmap-rebase.md >/dev/null ||
  fail "wave record must preserve non-goals"

printf 'check-model-native-actor-roadmap: ok\n'
