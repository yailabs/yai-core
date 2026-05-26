#!/usr/bin/env python3
import argparse
import json
import math
import os
import random
import time


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--n", type=int, required=True)
    parser.add_argument("--dim", type=int, required=True)
    parser.add_argument("--queries", type=int, required=True)
    parser.add_argument("--k", type=int, required=True)
    parser.add_argument("--m", type=int, default=16)
    parser.add_argument("--ef-construction", type=int, default=100)
    parser.add_argument("--ef-search", type=int, default=64)
    parser.add_argument("--out", required=True)
    parser.add_argument("--seed", type=int, default=7)
    return parser.parse_args()


def l2_normalize_np(x, np):
    norms = np.linalg.norm(x, axis=1, keepdims=True)
    norms[norms == 0] = 1.0
    return x / norms


def brute_force_np(data, queries, k, np):
    start = time.perf_counter()
    labels = []
    for query in queries:
        scores = data @ query
        idx = np.argpartition(-scores, k)[:k]
        idx = idx[np.argsort(-scores[idx])]
        labels.append(idx.tolist())
    elapsed = time.perf_counter() - start
    return labels, elapsed


def l2_normalize_py(vec):
    norm = math.sqrt(sum(v * v for v in vec)) or 1.0
    return [v / norm for v in vec]


def brute_force_py(data, queries, k):
    start = time.perf_counter()
    labels = []
    for query in queries:
        scored = []
        for idx, row in enumerate(data):
            scored.append((sum(a * b for a, b in zip(row, query)), idx))
        scored.sort(reverse=True)
        labels.append([idx for _, idx in scored[:k]])
    return labels, time.perf_counter() - start


def main():
    args = parse_args()
    os.makedirs(args.out, exist_ok=True)

    method = "python_bruteforce"
    build_seconds = 0.0
    query_seconds = 0.0
    labels = []
    hnsw_available = False
    numpy_available = False

    try:
        import numpy as np

        numpy_available = True
        rng = np.random.default_rng(args.seed)
        data = l2_normalize_np(rng.normal(size=(args.n, args.dim)).astype(np.float32), np)
        queries = l2_normalize_np(rng.normal(size=(args.queries, args.dim)).astype(np.float32), np)
        method = "numpy_bruteforce"
        import hnswlib

        hnsw_available = True
        method = "hnswlib_cosine"
        index = hnswlib.Index(space="cosine", dim=args.dim)
        start = time.perf_counter()
        index.init_index(max_elements=args.n, ef_construction=args.ef_construction, M=args.m)
        index.add_items(data, np.arange(args.n))
        index.set_ef(args.ef_search)
        build_seconds = time.perf_counter() - start
        start = time.perf_counter()
        labels, _ = index.knn_query(queries, k=args.k)
        query_seconds = time.perf_counter() - start
        labels = labels.tolist()
    except ImportError:
        if numpy_available:
            labels, query_seconds = brute_force_np(data, queries, args.k, np)
        else:
            rng = random.Random(args.seed)
            data = [l2_normalize_py([rng.gauss(0.0, 1.0) for _ in range(args.dim)]) for _ in range(args.n)]
            queries = [l2_normalize_py([rng.gauss(0.0, 1.0) for _ in range(args.dim)]) for _ in range(args.queries)]
            labels, query_seconds = brute_force_py(data, queries, args.k)

    per_query_ms = (query_seconds / args.queries) * 1000.0 if args.queries else math.nan
    summary = {
        "schema": "yai.hnsw_benchmark.v1",
        "created_at_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        "method": method,
        "numpy_available": numpy_available,
        "hnswlib_available": hnsw_available,
        "n": args.n,
        "dim": args.dim,
        "queries": args.queries,
        "k": args.k,
        "m": args.m,
        "ef_construction": args.ef_construction,
        "ef_search": args.ef_search,
        "build_seconds": build_seconds,
        "query_seconds": query_seconds,
        "query_ms_per_query": per_query_ms,
        "seed": args.seed,
    }

    with open(os.path.join(args.out, "summary.json"), "w", encoding="utf-8") as f:
        json.dump(summary, f, indent=2, sort_keys=True)
        f.write("\n")
    with open(os.path.join(args.out, "labels-preview.json"), "w", encoding="utf-8") as f:
        json.dump({"first_queries": labels[:5]}, f, indent=2)
        f.write("\n")

    print(f"hnsw benchmark written: {args.out}")


if __name__ == "__main__":
    main()
