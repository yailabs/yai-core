#!/usr/bin/env python3
"""Print one prompt field from a yai.lab.prompts.v1 catalog."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("catalog", type=Path)
    parser.add_argument("prompt_id")
    parser.add_argument("--field", default="content")
    args = parser.parse_args()

    with args.catalog.open(encoding="utf-8") as fh:
        catalog = json.load(fh)

    for item in catalog.get("prompts", []):
        if item.get("id") != args.prompt_id:
            continue
        value = item.get(args.field, "")
        if isinstance(value, (dict, list)):
            print(json.dumps(value, ensure_ascii=False))
        else:
            print(value)
        return 0

    raise SystemExit(f"prompt id not found: {args.prompt_id}")


if __name__ == "__main__":
    raise SystemExit(main())
