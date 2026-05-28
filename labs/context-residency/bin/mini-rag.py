#!/usr/bin/env python3
"""Deterministic lexical mini-RAG selector."""
from __future__ import annotations

import argparse
import json
import re
import time
from pathlib import Path

STOP = {
    "a", "an", "and", "are", "as", "be", "by", "for", "from", "if", "in",
    "is", "it", "of", "or", "that", "the", "this", "to", "what", "which",
    "with", "should", "me", "you", "i", "was", "were",
}


def terms(text: str) -> list[str]:
    return [t for t in re.findall(r"[a-z0-9:_-]+", text.lower()) if t not in STOP and len(t) > 1]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--run-dir", required=True)
    parser.add_argument("--question-id", required=True)
    parser.add_argument("--query", required=True)
    parser.add_argument("--top-k", type=int, default=4)
    args = parser.parse_args()

    run_dir = Path(args.run_dir)
    chunks_path = run_dir / "assets" / "corpus" / "chunks.jsonl"
    query_terms = terms(args.query)
    query_set = set(query_terms)
    start = time.perf_counter()
    scored = []
    with chunks_path.open(encoding="utf-8") as fh:
        for line in fh:
            chunk = json.loads(line)
            chunk_terms = terms(chunk["text"])
            overlap = query_set.intersection(chunk_terms)
            marker_hits = [term for term in query_terms if ":" in term and term in chunk["text"].lower()]
            score = len(overlap) + 3 * len(marker_hits)
            if score:
                item = dict(chunk)
                item["score"] = score
                item["overlap_terms"] = sorted(overlap)
                item["marker_hits"] = marker_hits
                scored.append(item)
    scored.sort(key=lambda item: (-item["score"], item["chunk_id"]))
    selected = scored[: args.top_k]
    elapsed = int(round((time.perf_counter() - start) * 1000))
    output = {
        "schema": "yai.context_residency.mini_rag.selection.v1",
        "question_id": args.question_id,
        "top_k": args.top_k,
        "retrieval_ms": elapsed,
        "selected_sources": [item["source"] for item in selected],
        "selected_tokens_estimate": sum(max(1, (len(item["text"]) + 3) // 4) for item in selected),
        "score_sum": sum(item["score"] for item in selected),
        "chunks": selected,
    }
    out_path = run_dir / "assets" / "corpus" / "selected-chunks.jsonl"
    with out_path.open("a", encoding="utf-8") as fh:
        fh.write(json.dumps(output, sort_keys=True) + "\n")
    print(json.dumps(output, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
