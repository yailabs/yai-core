# Transcript - LAN Provider Run 2026-05-26 43117 Only

## Execution Record

This run packages the current clean provider-port evidence. The curated run
contains the live `43117` provider probe and HNSW smoke evidence. It does not
include the obsolete `44317` probe in the curated set.

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
| `44317` | Not included in this curated run |

## Prompt

The prompt artifact used for the blocked chat generation attempt is stored at
`assets/P001-provider-readiness.md`.

## Artifacts

- `report.md`
- `metrics.json`
- `manifest.json`
- `assets/normalized-runs.json`
- `assets/P001-provider-readiness.md`
- `assets/endpoint-latency.svg`
- `assets/endpoint-status-matrix.svg`
- `assets/hnsw-query-latency.svg`
- `assets/provider-topology-3d.html`

## Limitations

- Converted from an existing generated report.
- No benchmark or provider probe was executed during this compacting wave.
- Chat generation remains blocked by authentication.
- GPU throughput and VRAM are not measured.
