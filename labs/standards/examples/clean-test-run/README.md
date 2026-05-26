---
id: EXAMPLE.T001
type: test_definition
title: Example Compact Test
status: draft
version: 0.2.0
date: 2026-05-26
authors:
  - YAI lab author
project: YAI
lab: example
tags: []
summary: Minimal compact test example.
latest_run: 20260525-baseline
report: runs/20260525-baseline/report.md
notebook: notebook.ipynb
artifacts:
  - notebook.ipynb
  - runs/20260525-baseline/manifest.json
  - runs/20260525-baseline/metrics.json
related: []
supersedes:
superseded_by:
---

# Example Compact Test

## Purpose

Show the compact test-local run layout.

## Scope

This example is structural only.

## Expected Outputs

```text
notebook.ipynb
runs/YYYYMMDD-<slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

## Limitations

This example does not prove a system property.
