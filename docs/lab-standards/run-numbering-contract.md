# Run Numbering Contract

Status: LAB.DOCS.1.

This contract defines stable identifiers for lab tests, runs and visual
artifacts. Compact folders use readable date slugs, while manifests may retain
formal IDs when a lab profile requires them.

## Test IDs

Test IDs use:

```text
<LABCODE>.T###
```

Examples:

```text
FSLOOP.T001
MODEL.T001
HWB.T001
RETR.T001
GRAPH.T001
```

`LABCODE` is assigned by the lab profile or registry. `T###` is a
zero-padded sequence number within the lab.

## Run IDs And Slugs

Run IDs use:

```text
<TEST-ID>.R###
```

Examples:

```text
MODEL.T001.R001
HWB.T001.R001
RETR.T001.R001
```

Each run belongs to exactly one test.

Compact run folders use:

```text
YYYYMMDD-<run-slug>
```

The run manifest may include both a formal run ID and the compact `run_slug`.

## Visual Artifact IDs

Visual artifact IDs use:

```text
<RUN-ID>.F###
<RUN-ID>.C###
<RUN-ID>.D###
<RUN-ID>.TBL###
```

Where:

```text
F   figure
C   chart
D   diagram
TBL table
```

File names inside a run may use the short local prefix under `assets/`:

```text
assets/C001-endpoint-latency.svg
assets/D001-provider-topology.svg
assets/TBL001-model-output-summary.md
```

The manifest must contain the full ID:

```json
{
  "id": "HWB.T001.R001.C001",
  "path": "assets/C001-endpoint-latency.svg"
}
```

## Non-Overwrite Rule

Generated visuals must not overwrite visuals from previous runs. A new run
requires a new run directory.
