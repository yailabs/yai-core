# Local Runtime Benchmark Matrix

Status: local lab planning.

| workload_id | Kind | Target | Primary metric | Secondary metric | Notes |
|---|---|---|---|---|---|
| `w1-smoke` | chat/completion | Linux LAN provider | endpoint works | latency | Short prompt for wiring only. |
| `w2-technical-summary` | chat/completion | Linux LAN provider | wall time | estimated tokens/s | Medium prompt for repeatability. |
| `w3-long-report` | chat/completion | Linux LAN provider | wall time | peak VRAM | Longer generation for stable sampling. |
| `hnsw-synthetic-20k` | vector search | MacBook or Linux | query latency | build time | Uses hnswlib if installed, NumPy fallback otherwise. |

## Repetition

Run each model workload at least three times after one warm-up run. Keep raw
stdout and stderr. Prefer median wall time over best time.

## Boundary

These benchmarks compare local runtime posture. They do not validate model
quality, policy behavior or YAI case authority.
