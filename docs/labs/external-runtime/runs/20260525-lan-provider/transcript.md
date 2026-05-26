# Transcript - LAN Provider Run 2026-05-25

## Execution Record

This run packages the 2026-05-25 LAN provider report as a compact run. It
includes a MacBook operator baseline, an HNSW smoke run, `43117` live provider
evidence and `44317` negative endpoint evidence.

## Commands

Commands are recorded in the normalized evidence under
`assets/normalized-runs.json`. This compacting wave did not re-execute the
benchmarks or provider probes.

## Headline Observations

| Observation | Value |
|---|---|
| Live provider host | `172.20.10.3` |
| Live provider port | `43117` |
| `/v1/models` on `43117` | `reachable` |
| `/v1/chat/completions` on `43117` | `auth_blocked` |
| `44317` | `connection_refused` negative evidence |

## Artifacts

- `report.md`
- `metrics.json`
- `manifest.json`
- `assets/normalized-runs.json`
- `assets/endpoint-latency.svg`
- `assets/endpoint-status-matrix.svg`
- `assets/hnsw-query-latency.svg`
- `assets/provider-topology-3d.html`

## Limitations

- Converted from an existing generated report.
- No benchmark or provider probe was executed during this compacting wave.
- Chat generation remains blocked by authentication.
- GPU throughput and VRAM are not measured.
