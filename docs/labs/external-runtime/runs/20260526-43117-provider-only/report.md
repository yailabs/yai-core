# External Runtime LAN Provider Report - 2026-05-26 43117 Only

Status: generated local lab report.

## Executive Summary

The curated LAN provider evidence shows that `172.20.10.3:43117` is the live provider port for model discovery with no negative-evidence port included in this curated run. The provider returns `reachable` for `/v1/models` and `auth_blocked` for `/v1/chat/completions`; the current blocker is provider authentication, not LAN reachability.

Headline evidence:

- `172.20.10.3:43117 /v1/models -> reachable`
- `172.20.10.3:43117 /v1/chat/completions -> auth_blocked`

## Provider Readiness

| Target | Endpoint | Curl status | HTTP code | Time seconds | Status | Error |
|---|---|---|---|---|---|---|
| `172.20.10.3:43117` | `/v1/models` | 0 | 200 | 0.023909 | `reachable` |  |
| `172.20.10.3:43117` | `/v1/chat/completions` | 0 | 401 | 0.008594 | `auth_blocked` | Invalid API Key |

![Endpoint status matrix](assets/endpoint-status-matrix.svg)

Caption: curated endpoint status matrix using the shared chart style contract.

![Endpoint latency](assets/endpoint-latency.svg)

Caption: curl total time for each curated endpoint probe. Failed and blocked probes remain visible evidence.

## Model Inventory

Observed through the reachable `/v1/models` endpoint.

| Field | Value |
|---|---|
| Model id | `Qwen_Qwen3-8B-Q4_K_M.gguf` |
| Owner | `llamacpp` |
| Format | `gguf` |
| Parameters | 8190735360 |
| Size bytes | 5021827072 |
| Train context | 32768 |
| Embedding size | 4096 |

## Prompt Inventory

| Prompt ID | Run | Role | Content | Max tokens | Temperature |
|---|---|---|---|---|---|
| `lan-provider-20260526-43117-run3-escalated.P001` | `lan-provider-20260526-43117-run3-escalated` | `user` | Rispondi in una frase: provider LAN pronto sulla porta 43117. | 96 | 0 |

## Benchmark Panels

| Run | Method | Vectors | Dimensions | Queries | k | Query ms/query |
|---|---|---|---|---|---|---|
| `hnsw-smoke-20260526-run1` | `python_bruteforce` | 1000 | 64 | 20 | 10 | 2.911083 |

![HNSW query latency](assets/hnsw-query-latency.svg)

Caption: vector-search smoke benchmark. This run used the standard-library fallback because NumPy and hnswlib were not installed on the operator host.

## 3D View

[Open provider topology 3D](assets/provider-topology-3d.html)

Caption: 3D boundary view separating the operator host, the live provider port and the chat authentication gate.

## Interpretation

- Proven: the provider host responds on `43117` for `/v1/models`.
- Blocked: `/v1/chat/completions` reaches the server but returns `401 Invalid API Key`, classified as `auth_blocked`.
- Missing: a valid provider API key is required before chat generation, token throughput and VRAM plots become meaningful.

## Reproducibility Appendix

| Run | Kind | Status | Raw folder |
|---|---|---|---|
| `hnsw-smoke-20260526-run1` | `hnsw_benchmark` | `completed` | `benchmarks/nvidia/hnsw-smoke-20260526-run1` |
| `lan-provider-20260526-43117-run3-escalated` | `lan_provider_probe` | `completed` | `benchmarks/nvidia/lan-provider-20260526-43117-run3-escalated` |

Generated at `2026-05-26T07:51:06Z` from `20260526-lan-provider-43117-only`.
