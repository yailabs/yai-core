#!/usr/bin/env bash
# YAI - lab notebook quality guard
#
# Purpose:
#   Keep lab notebooks valid and aligned with their lab role. The filesystem
#   loop notebook is intentionally a long manual operator notebook with explicit
#   shell cells and hand-written prompts; smaller labs should remain compact.
set -euo pipefail

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$repo_root"

python3 - "$repo_root" <<'PY'
import json
import re
import sys
from pathlib import Path

root = Path(sys.argv[1])
notebooks = sorted((root / "labs").glob("*/notebook.ipynb"))
if not notebooks:
    raise SystemExit("check-lab-notebooks: no lab notebooks found")

forbidden_text = [
    "echo ",
    "mkdir ",
    "cp ",
    "rm -rf",
    "cat >",
    "python - <<",
    "python3 - <<",
    "json.dump",
    "subprocess",
    ("docs/" "manuals"),
    ("tools/" "labs"),
    ("policy-" "packs"),
    ("docs/" "labs/scientific-" "docs"),
]

for notebook in notebooks:
    data = json.loads(notebook.read_text(encoding="utf-8"))
    cells = data.get("cells", [])
    rel = notebook.relative_to(root).as_posix()

    if rel == "labs/filesystem-loop/notebook.ipynb":
        text = "\n".join(
            "".join(cell.get("source", []))
            if isinstance(cell.get("source", []), list)
            else str(cell.get("source", ""))
            for cell in cells
        )
        required = [
            "Notebook Di Validazione Del Filesystem Loop",
            "Command Surface Matrix SPINE.48",
            "yai prompt <<'EOF'",
            "Lab A: Orientamento",
            "Lab K: Controllo Fuori Caso",
            "Lab L: Prompt Manuale Sul Review Loop",
            "yai daemon run-filesystem-loop",
            "yai control pending",
            "yai graph fanout",
            "yai facts extract",
            "facts summary",
        ]
        for phrase in required:
            if phrase not in text:
                raise SystemExit(
                    f"check-lab-notebooks: filesystem-loop notebook missing {phrase!r}"
                )
        for token in [("docs/" "manuals"), ("docs/" "labs/"), ("policy-" "packs")]:
            if token in text:
                raise SystemExit(
                    f"check-lab-notebooks: forbidden filesystem-loop notebook token {token!r}"
                )
        if len(cells) < 80:
            raise SystemExit(
                f"check-lab-notebooks: filesystem-loop notebook is too small: {len(cells)} cells"
            )
        continue

    if len(cells) > 40:
        raise SystemExit(
            f"check-lab-notebooks: too many cells in {notebook.relative_to(root)}: {len(cells)} > 40"
        )

    for index, cell in enumerate(cells, start=1):
        source = cell.get("source", [])
        text = "".join(source) if isinstance(source, list) else str(source)
        for token in forbidden_text:
            if token in text:
                raise SystemExit(
                    f"check-lab-notebooks: forbidden notebook token {token!r} "
                    f"in {notebook.relative_to(root)} cell {index}"
                )
        for line_number, line in enumerate(text.splitlines(), start=1):
            stripped = line.lstrip()
            if re.match(r"^(for|while)\s+", stripped):
                raise SystemExit(
                    f"check-lab-notebooks: shell-style control flow in "
                    f"{notebook.relative_to(root)} cell {index} line {line_number}: {stripped}"
                )

print("check-lab-notebooks: ok")
PY
