#!/usr/bin/env python3
"""Build a deterministic case corpus from real run artifacts."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def read(path: Path) -> str:
    if not path.is_file():
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def chunk_text(source: str, text: str, size: int, overlap: int) -> list[dict]:
    chunks = []
    start = 0
    while start < len(text):
        end = min(len(text), start + size)
        chunk_id = f"chunk:{len(chunks) + 1:04d}"
        chunks.append(
            {
                "chunk_id": chunk_id,
                "source": source,
                "text": text[start:end],
                "char_start": start,
                "char_end": end,
            }
        )
        if end == len(text):
            break
        start = max(end - overlap, start + 1)
    return chunks


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--run-dir", required=True)
    parser.add_argument("--lab-dir", required=True)
    parser.add_argument("--chunk-size", type=int, default=800)
    parser.add_argument("--chunk-overlap", type=int, default=120)
    args = parser.parse_args()

    run_dir = Path(args.run_dir)
    lab_dir = Path(args.lab_dir)
    corpus_dir = run_dir / "assets" / "corpus"
    corpus_dir.mkdir(parents=True, exist_ok=True)
    raw_case = run_dir / "assets" / "raw" / "case-setup"

    sources: list[tuple[str, str]] = []
    for path in sorted(raw_case.glob("*.txt")):
        sources.append((path.name, read(path)))

    pack_dir = lab_dir.parent / "filesystem-loop" / "pack-fixture"
    for path in sorted(pack_dir.rglob("*")):
        if path.is_file() and path.suffix in {".md", ".json", ".yaml", ".yml"}:
            rel = path.relative_to(pack_dir)
            sources.append((f"pack-fixture/{rel}", read(path)))

    sections = []
    all_chunks = []
    for source, text in sources:
        if not text.strip():
            continue
        sections.append(f"\n\n## Source: {source}\n\n{text.strip()}\n")
        all_chunks.extend(chunk_text(source, text, args.chunk_size, args.chunk_overlap))

    raw_text = "# Raw Case Corpus\n" + "".join(sections)
    (corpus_dir / "raw-case-corpus.md").write_text(raw_text, encoding="utf-8")
    with (corpus_dir / "chunks.jsonl").open("w", encoding="utf-8") as fh:
        for chunk in all_chunks:
            fh.write(json.dumps(chunk, sort_keys=True) + "\n")
    index = {
        "schema": "yai.context_residency.corpus.index.v1",
        "chunk_size_chars": args.chunk_size,
        "chunk_overlap_chars": args.chunk_overlap,
        "sources": [source for source, text in sources if text.strip()],
        "chunks_total": len(all_chunks),
        "raw_case_corpus_bytes": len(raw_text.encode("utf-8")),
    }
    (corpus_dir / "index.json").write_text(json.dumps(index, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(json.dumps(index, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
