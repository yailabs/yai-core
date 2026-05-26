#!/usr/bin/env python3
import argparse
import json
import os
from pathlib import Path


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", required=True)
    parser.add_argument("--out", required=True)
    return parser.parse_args()


def load_json(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError) as exc:
        return {"schema": "unreadable", "error": str(exc)}


def main():
    args = parse_args()
    root = Path(args.root)
    rows = []
    for summary_path in sorted(root.glob("**/summary.json")):
        data = load_json(summary_path)
        rows.append((summary_path, data))

    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("# Generated Benchmark Summary\n\n")
        f.write("Status: generated local lab material.\n\n")
        if not rows:
            f.write("No `summary.json` files found.\n")
            return
        f.write("| Run | Schema | Model/Method | Workload | Wall seconds | Tokens/s | Peak VRAM MiB | Query ms |\n")
        f.write("|---|---|---|---|---:|---:|---:|---:|\n")
        for path, data in rows:
            rel = os.path.relpath(path.parent, root)
            schema = data.get("schema", "")
            model_or_method = data.get("model") or data.get("method") or ""
            workload = data.get("workload") or ""
            wall = data.get("wall_time_seconds", "")
            tok_s = data.get("estimated_tokens_per_second", "")
            vram = data.get("peak_vram_mib", "")
            query_ms = data.get("query_ms_per_query", "")
            f.write(f"| `{rel}` | `{schema}` | `{model_or_method}` | `{workload}` | {wall} | {tok_s} | {vram} | {query_ms} |\n")
    print(f"summary written: {out_path}")


if __name__ == "__main__":
    main()
