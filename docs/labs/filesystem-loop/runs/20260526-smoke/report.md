# Filesystem Loop Smoke Report - 2026-05-26

Status: completed single-run report.

## Summary

The filesystem-loop smoke run passed the documented repository and runtime smoke
checks:

- `make check-docs`
- `make smoke-spine24a`
- `make smoke-spine27`

## Metrics

| Metric | Value |
|---|---:|
| Commands executed | 3 |
| Commands passed | 3 |
| Commands failed | 0 |
| Prompted model/provider calls | 0 |

## Pack Material Used

The run uses the filesystem-loop `../../pack-fixture/` as lab input posture.
The useful fixture material is the three JSON files under
`../../pack-fixture/policies/` plus the minimal fixture identity in
`../../pack-fixture/pack.yaml`.

## Case Materialization Posture

The current implementation represents the fixture posture through the
filesystem-loop smoke path and journal records. This report distinguishes the
fixture files from materialized case state and does not claim that a generic
pack runtime exists.

## Artifacts

| Artifact | Path |
|---|---|
| Manifest | `manifest.json` |
| Metrics | `metrics.json` |
| Check docs log | `assets/make-check-docs.log` |
| Smoke spine24a log | `assets/make-smoke-spine24a.log` |
| Smoke spine27 log | `assets/make-smoke-spine27.log` |
| Flow diagram | `assets/filesystem-loop-flow.md` |

## Interpretation

The run provides operational smoke evidence for the filesystem-loop lab. It
does not provide model behavior evidence, provider readiness evidence or
throughput metrics.

## Limitations

- No model/provider prompt was executed.
- No latency, token throughput or VRAM metric is claimed.
- The evidence is command-output based.
