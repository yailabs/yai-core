# Command Surface

This is the active operator command index for `yai`.

Rule:

```text
A primitive that cannot be inspected is not operational yet.
```

This file maps SPINE.20-SPINE.29 primitives to their current view, command,
manual test and documentation surface. It does not define new core semantics.
The operator-facing runbook is `docs/manuals/manual-filesystem-loop-validation.md`
with notebook companions `docs/manuals/manual-filesystem-loop-validation.ipynb`
and `docs/manuals/manual-filesystem-loop-validation.it.ipynb`.

## Runtime Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| `YAI_HOME` | resolved runtime home | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md`, `testing.md` |
| runtime dirs | resolved `run/store/log/tmp/cases/sockets/config` | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md` |
| install paths | Makefile install map | `make print-install-paths` | `make print-install-paths PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `filesystem-target.md` |
| local install | binaries plus runtime dirs | `make install-local` | `make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `testing.md` |
| local uninstall | remove installed binaries only | `make uninstall-local` | `make uninstall-local PREFIX=/tmp/yai-install-test` | `testing.md` |
| local doctor | installed binary/path diagnostic | `make doctor-local` | `make doctor-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `testing.md` |

`yai doctor` is the canonical runtime path view. There is no separate
`yai runtime paths` command in SPINE.24A.

## Daemon Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| `yaid` binary | daemon version | `yaid --version` | `/tmp/yai-install-test/bin/yaid --version` | `testing.md` |
| local socket | daemon foreground endpoint | `yaid --socket <path> --foreground` | `yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground` | `filesystem-target.md` |
| daemon liveness | status response | `yai daemon status --socket <path>` | `yai daemon status --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| daemon metadata | daemon info response | `yai daemon info --socket <path>` | `yai daemon info --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| minimum loop | daemon-backed control loop | `yai daemon run-minimum-loop --socket <path>` | `yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| filesystem loop | daemon-backed filesystem loop | `yai daemon run-filesystem-loop --socket <path>` | `yai daemon run-filesystem-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| shutdown | daemon stop request | `yai daemon shutdown --socket <path>` | `yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |

## Hot State Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| hot state | live cache status | `yai hot status` | `target/debug/yai hot status` | `hot-state-plane.md`, `data-plane-roadmap.md` |
| hot snapshot path | snapshot source | `yai hot status` | `YAI_HOME=/tmp/yai-home-test yai hot status` | `filesystem-target.md` |
| snapshot schema | `yai.hot_state.v1` validation | `yai hot status` | valid daemon snapshot after `run-minimum-loop` | `hot-state-plane.md` |
| missing snapshot | unavailable reason | `yai hot status` | `rm -f $YAI_HOME/run/hot-state.json; yai hot status` | `testing.md` |
| invalid snapshot | unavailable reason | `yai hot status` | `printf '{broken\n' > $YAI_HOME/run/hot-state.json; yai hot status` | `testing.md` |
| doctor hot state | path/readability/schema | `yai doctor` | `target/debug/yai doctor` | `testing.md` |

Required `yai hot status` fields:

```text
hot_state: active|unavailable
snapshot: <path>
schema: yai.hot_state.v1|unknown
case_session: active|inactive|unknown
case_world: loaded|not_loaded|unknown
case_context: active|inactive|unknown
active_thread: <id>|none|unknown
participant_view: <id>|none|unknown
projection: fresh|stale|unknown|rebuilding
freshness_policy: usable|refresh_required|refresh_recommended|blocked_for_model|unknown
stale_reason: ...
updated_at: ...
```

`projection_policy` may also appear as a compatibility alias for the same
value. `freshness_policy` is the canonical field.

There is no `yai hot status --json` convention in SPINE.27.

SPINE.28 freezes this command surface before LMDB work. The stable hot-state
operator fields are `hot_state`, `snapshot`, `schema`, `case_session`,
`case_world`, `case_context`, `projection`, `stale_reason`,
`freshness_policy`, `updated_at`, plus latest decision/receipt refs when
available. LMDB will add durable record lookup later; it will not replace
`yai hot status`.

## Record Store Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| record store | LMDB record-plane readiness | `yai store status` | `target/debug/yai store status` | `lmdb-record-plane.md`, `data-plane-roadmap.md` |
| doctor record store | path/backend/status | `yai doctor` | `target/debug/yai doctor` | `lmdb-record-plane.md`, `testing.md` |

`yai store status` is the SPINE.29 command because `store` already names the
durable data root and LMDB is the record-plane backend under it. Future
subcommands can extend the same surface with `yai store record get`,
`yai store record list --case` and `yai store rebuild status`.

Required fields:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|unavailable
record_store_path: <YAI_HOME>/store/lmdb
record_env: record_env
schema: yai.record.v1
```

`missing` means `YAI_HOME/store/lmdb` is absent. `not_initialized` means the
directory exists but no LMDB schema metadata has been initialized. `ready` must
not be reported until a later wave implements the write path and schema guard.

## Projection Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| projection summary | journal-derived projection summary | `yai projection summary --journal <path>` | smoke journal path | `testing.md` |
| projection inspect | projection details and freshness policy | `yai projection inspect --journal <path> --consumer <consumer>` | `yai projection inspect --journal <journal> --consumer model` | `data-plane-roadmap.md` |
| prompt dry run | model-visible projection warning surface | `yai prompt --dry-run --once <text> --journal <path>` | daemon core-loop smoke journal | `testing.md` |

Projection commands are still journal-backed in this phase. Hot state provides
freshness context, not durable projection truth.

Required `yai projection inspect` freshness fields:

```text
projection_freshness: fresh|stale|unknown|rebuilding
stale_reason: ...
freshness_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
consumer: model|operator|audit|debug|agent
freshness_source: hot_state|projection_record|journal|unknown
```

If `projection inspect` cannot correlate the journal case with active hot
state, it reports journal/projection-derived freshness. It must not fake a
hot-state source.

## Pack / Doctrine Checks

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| pack doctrine | required pack doctrine doc/phrases | `make check-pack-doctrine` | `make check-pack-doctrine` | `pack-format.md`, `pack-roadmap.md` |
| manual policy pack fixtures | staged pack material before provider attach | shell copy/JSON validation in manual | `cp docs/manuals/examples/filesystem-loop/policy-packs/*.json "$YAI_RUN/policy-packs"/` | `manual-filesystem-loop-validation.md` |
| pack-derived case residue | materialized policy/projection/authority records | `yai daemon run-filesystem-loop --socket <path>` | `grep subject:policy-pack "$JOURNAL"` | `manual-filesystem-loop-validation.md` |
| active docs | canonical engineering doc set | `make check-docs` | `make check-docs` | `README.md` |

Pack runtime commands are not implemented yet. SPINE.21 only provides pack
doctrine and guards. The filesystem loop manual stages policy pack fixtures and
the daemon loop materializes their current posture into journal records for the
manual case. Do not document a `yai pack` command until the pack runtime exists.

## Foundation / Layout Checks

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| source layout | no old active roots | `make check-layout` | `make check-layout` | `filesystem-target.md` |
| source surface | no venv roots, no README-only source roots, transitional C shims marked | `make check-source-surface-clean` | `make check-source-surface-clean` | `source-surface.md` |
| foundation freeze | freeze guard | `make check-foundation-freeze` | `make check-foundation-freeze` | `current-status.md` |
| full docs guard | active doc requirements | `make check-docs` | `make check-docs` | `README.md` |
| archive hygiene | historical banner on archive records | `make check-archive-historical-records` | `make check-archive-historical-records` | `docs/archive/README.md` |
| full check | layout, docs, build, smoke | `make check` | `make check` | `testing.md` |

`make check-source-surface-clean` is integrated through `make check-layout`.
`make check-foundation-freeze` is integrated through `make check-docs`, and
therefore through `make check`.

## Install Commands

Manual install matrix:

```sh
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai --version
PATH=/tmp/yai-install-test/bin:$PATH yai info
PATH=/tmp/yai-install-test/bin:$PATH yai doctor
PATH=/tmp/yai-install-test/bin:$PATH yai store status
PATH=/tmp/yai-install-test/bin:$PATH yai hot status
/tmp/yai-install-test/bin/yaid --version
make uninstall-local PREFIX=/tmp/yai-install-test
test ! -e /tmp/yai-install-test/bin/yai
test ! -e /tmp/yai-install-test/bin/yaid
```

`uninstall-local` removes installed binaries. It does not delete `YAI_HOME`.

## Manual Validation Recipes

Daemon and hot-state recipe:

```sh
mkdir -p /tmp/yai-home-test/run
/tmp/yai-install-test/bin/yaid \
  --socket /tmp/yai-home-test/run/yaid.sock \
  --foreground &
DAEMON_PID=$!

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon status --socket /tmp/yai-home-test/run/yaid.sock

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock

PATH=/tmp/yai-install-test/bin:$PATH yai hot status

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock

wait $DAEMON_PID
```

Expected key output:

```text
hot_state: active
snapshot: /tmp/yai-home-test/run/hot-state.json
schema: yai.hot_state.v1
case_session: active
case_context: active
projection_policy: usable
freshness_policy: usable
```

Snapshot edge cases:

```sh
rm -f /tmp/yai-home-test/run/hot-state.json
PATH=/tmp/yai-install-test/bin:$PATH yai hot status

printf '{broken\n' > /tmp/yai-home-test/run/hot-state.json
PATH=/tmp/yai-install-test/bin:$PATH yai hot status
```

Expected key output:

```text
hot_state: unavailable
reason: missing_snapshot
hot_state: unavailable
reason: invalid_snapshot
```

Automated retroactive coverage:

```sh
make smoke-spine24a
make smoke-spine27
make smoke-spine29
make check-hot-state-freeze
make check-lmdb-record-plane-doctrine
```
