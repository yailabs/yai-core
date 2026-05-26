# NET.SPINE.0R - Integrated Module Topology Realignment

Status: delivered when the integrated topology guard passes.

## Reason

NET.SPINE.0 correctly introduced NET as a root YAI communication substrate, but
the initial scaffold made `net/` look like a repository inside the repository:
it had local docs, source, tests, fixtures, benches, examples and Makefile
islands.

NET is an integrated YAI module. CLORI is the standalone external repository.

## Old Wrong Shape

- `net/docs/`
- `net/src/`
- `net/tests/`
- `net/fixtures/`
- `net/benches/`
- `net/examples/`
- `net/Makefile`
- `net/include/yai/net/`

## New Integrated Shape

- `net/<area>/` owns integrated runtime module areas.
- `include/yai/net/` owns public NET headers.
- `work/spines/net-spine.md` owns the authoritative NET spine.
- `work/archive/*` holds snapshots.
- `labs/` remains the future experiment/run/benchmark space.

## Moved Paths

- `net/include/yai/net/*.h` to `include/yai/net/*.h`
- `net/src/<area>/` to `net/<area>/`
- NET spine and doctrine references to `work/`

## Deleted Placeholder Islands

- removed `net/docs/`
- removed `net/src/`
- removed `net/tests/`
- removed `net/fixtures/`
- removed `net/benches/`
- removed `net/examples/`
- removed `net/Makefile`

## Guard Updates

`tools/checks/check-net-boundary.sh` now enforces integrated topology and fails
if repository-shaped NET islands return.

## Validation

```sh
git diff --check
make info
make net-info
tools/checks/check-net-boundary.sh
make check-net-boundary
make check-layout
make check-docs
```

## Non-Goals

- no discovery implementation
- no transport implementation
- no server implementation
- no CLI behavior
- no CLORI code
- no system/ extraction yet
- no engine/ extraction yet
- no labs benchmark work yet
