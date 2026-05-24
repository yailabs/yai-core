# Carrier Coverage Matrix

Status: active
Owner: system/effect and command surface
Purpose: Track SPINE.33F carrier coverage matrix implementation.
Not source of truth for: real network/database/git/service/socket/model execution

Implemented files:

```text
include/yai/effect/carrier_mode.h
include/yai/effect/carrier_coverage.h
system/effect/carrier_mode.c
system/effect/carrier_coverage.c
tests/smoke/carrier-coverage-matrix/test_carrier_coverage_matrix.c
```

Commands:

```bash
yai carrier coverage
yai carrier coverage --family filesystem
yai carrier coverage --family process
yai carrier coverage --family database
yai carrier coverage --family unknown
yai carrier coverage --mode controlled
```

This surface is visibility only. It does not execute non-filesystem/non-process
carriers and does not claim support beyond the matrix.
