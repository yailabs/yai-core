# SPINE.45A Documentation Root Canon Collapse

Status: done.

SPINE.45A collapses the active documentation root:

```text
docs/ = curated architecture documentation
labs/ = experiments, runbooks, notebooks, outputs and reports
work/ = spines, waves, inventories, repo-readiness, agent notes and archives
```

CLORI did not execute in this slot and remains out of scope for SPINE.45A.
SPINE.46 DuckDB Fact Plane Doctrine + Schema remains next.

Guard:

```bash
make check-doc-root-canon
```
