#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_dir() {
  if [ ! -d "$ROOT/$1" ]; then
    printf 'required directory missing: %s\n' "$1" >&2
    exit 1
  fi
}

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'required file missing: %s\n' "$1" >&2
    exit 1
  fi
}

for dir in docs include/yai lib daemon ctl cmd/yai proto/schemas proto/fixtures tests tools/checks packaging examples vendor; do
  require_dir "$dir"
done

for dir in tests/unit tests/integration tests/adversarial tests/smoke; do
  require_dir "$dir"
done

for root in base ingest subject case op control effect store index graph memory projection reconcile; do
  require_dir "include/yai/$root"
  require_dir "lib/$root"
done

require_dir "include/yai/daemon"
require_dir "lib/internal"
require_file "include/yai/yai.h"
require_file "README.md"
require_file "VERSION"
require_file "Makefile"
require_file "cmd/yai/Cargo.toml"
require_file "cmd/yai/src/main.rs"
require_file "engine/Cargo.toml"
require_file "engine/README.md"
require_file "engine/yai-engine/Cargo.toml"
require_file "engine/yai-engine-ffi/Cargo.toml"
require_file "tests/smoke/minimum-loop/test_minimum_loop.c"
require_file "tests/smoke/persistent-journal/test_persistent_journal.c"
require_file "tests/smoke/control-gate/test_control_gate.c"
require_file "tests/smoke/filesystem-carrier/test_filesystem_carrier.c"
require_file "tests/smoke/graph-reconstruction/test_graph_reconstruction.c"

printf 'check-required-layout: ok\n'
