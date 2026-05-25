# Benchmark Visual Language

Status: publication/reporting convention for local lab benchmark reports.

Purpose: keep YAI benchmark reports readable, comparable and publishable across
runs without turning raw lab folders into the report itself.

## Report Language

Public benchmark reports are written in English. Local operator notes may remain
in Italian when they are raw run context, but headings, chart labels, captions
and interpretation sections use English.

## Canonical Statuses

| Status | Meaning | Color |
|---|---|---|
| `reachable` | Endpoint or benchmark completed successfully enough to inspect. | `#1f9d55` |
| `auth_blocked` | Server was reached but rejected authentication. | `#d97706` |
| `connection_refused` | Network path reached the host/port but no service accepted it. | `#dc2626` |
| `no_gpu` | GPU telemetry was requested on a host without NVIDIA telemetry. | `#64748b` |
| `completed` | Non-endpoint benchmark completed. | `#2563eb` |
| `incomplete` | Run did not provide enough evidence for a stronger status. | `#6b7280` |

Failed probes are evidence. Do not delete or hide them. If they are not part of
the headline, keep them in the reproducibility appendix.

## Chart Names

Generated report assets use stable names:

```text
endpoint-latency.svg
endpoint-status-matrix.svg
hnsw-query-latency.svg
provider-topology-3d.html
```

The chart title describes the claim being visualized. Captions describe the
boundary of the claim, not just the visible marks.

## Caption Rules

- State what was measured and where.
- Name the status vocabulary when a color encodes status.
- Separate observed evidence from interpretation.
- Never imply model quality from endpoint availability.
- Never include API keys, authorization headers or secrets.

## Current Baseline Headline

```text
172.20.10.3:43117 /v1/models -> reachable
172.20.10.3:43117 /v1/chat/completions -> auth_blocked
172.20.10.3:44317 -> connection_refused
```
