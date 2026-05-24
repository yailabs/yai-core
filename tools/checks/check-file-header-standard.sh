#!/bin/sh
set -eu

# YAI - file header standard guard
#
# Purpose:
#   Ensure the repository has an active ownership/header standard before broad
#   header application starts.
#
# Scope:
#   Checks the doctrine docs and wave/extraction references required by
#   REPO.HYGIENE.0.
#
# Non-goals:
#   This guard does not yet enforce headers across every source file.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing file header standard file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$ROOT/$file"; then
    printf 'missing file header standard phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/engineering/file-header-standard.md"
require_file "docs/engineering/agent-operating-appendix.md"
require_file "docs/engineering/wave-template.md"
require_file "docs/engineering/operational-extraction-contract.md"

require_phrase "docs/engineering/file-header-standard.md" "Purpose"
require_phrase "docs/engineering/file-header-standard.md" "Ownership"
require_phrase "docs/engineering/file-header-standard.md" "Boundary"
require_phrase "docs/engineering/file-header-standard.md" "Transitional C shim"
require_phrase "docs/engineering/agent-operating-appendix.md" "Before Editing"
require_phrase "docs/engineering/agent-operating-appendix.md" "A file is not ready for agent modification"
require_phrase "docs/engineering/wave-template.md" "File/header impact"
require_phrase "docs/engineering/operational-extraction-contract.md" "agent-operating-appendix.md"

printf 'file-header-standard: ok\n'
