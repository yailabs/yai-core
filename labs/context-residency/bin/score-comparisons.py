#!/usr/bin/env python3
"""Render scorecard summaries from matrix metrics."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--run-dir", required=True)
    args = parser.parse_args()
    run_dir = Path(args.run_dir)
    metrics = json.loads((run_dir / "metrics.json").read_text(encoding="utf-8"))
    reports = run_dir / "assets" / "derived"
    reports.mkdir(exist_ok=True)
    scorecard = {
        "schema": "yai.context_residency.matrix.scorecard.v1",
        "run_id": metrics.get("run_id"),
        "conditions": metrics.get("conditions", {}),
        "score_summary": metrics.get("score_summary", {}),
    }
    (reports / "scorecard.json").write_text(json.dumps(scorecard, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
