# Command Surface

This is the active operator command index for `yai-core`.

Rule:

```text
A primitive that cannot be inspected is not operational yet.
```

This file maps SPINE.20-SPINE.27 primitives to their current view, command,
manual test and documentation surface. It does not define new core semantics.

## Runtime Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| `YAI_HOME` | resolved runtime home | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md`, `testing.md` |
| runtime dirs | resolved `run/store/log/tmp/cases/sockets/config` | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md` |
| install paths | Makefile install map | `make print-install-paths` | `make print-install-paths PREFIX=/tmp/yai-core-install-test YAI_HOME=/tmp/yai-core-home-test` | `filesystem-target.md` |
| local install | binaries plus runtime dirs | `make install-local` | `make install-local PREFIX=/tmp/yai-core-install-test YAI_HOME=/tmp/yai-core-home-test` | `testing.md` |
| local uninstall | remove installed binaries only | `make uninstall-local` | `make uninstall-local PREFIX=/tmp/yai-core-install-test` | `testing.md` |
| local doctor | installed binary/path diagnostic | `make doctor-local` | `make doctor-local PREFIX=/tmp/yai-core-install-test YAI_HOME=/tmp/yai-core-home-test` | `testing.md` |

`yai doctor` is the canonical runtime path view. There is no separate
`yai runtime paths` command in SPINE.24A.

## Daemon Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| `yaid` binary | daemon version | `yaid --version` | `/tmp/yai-core-install-test/bin/yaid --version` | `testing.md` |
| local socket | daemon foreground endpoint | `yaid --socket <path> --foreground` | `yaid --socket /tmp/yai-core-home-test/run/yaid.sock --foreground` | `filesystem-target.md` |
| daemon liveness | status response | `yai daemon status --socket <path>` | `yai daemon status --socket /tmp/yai-core-home-test/run/yaid.sock` | `testing.md` |
| daemon metadata | daemon info response | `yai daemon info --socket <path>` | `yai daemon info --socket /tmp/yai-core-home-test/run/yaid.sock` | `testing.md` |
| minimum loop | daemon-backed control loop | `yai daemon run-minimum-loop --socket <path>` | `yai daemon run-minimum-loop --socket /tmp/yai-core-home-test/run/yaid.sock` | `testing.md` |
| filesystem loop | daemon-backed filesystem loop | `yai daemon run-filesystem-loop --socket <path>` | `yai daemon run-filesystem-loop --socket /tmp/yai-core-home-test/run/yaid.sock` | `testing.md` |
| shutdown | daemon stop request | `yai daemon shutdown --socket <path>` | `yai daemon shutdown --socket /tmp/yai-core-home-test/run/yaid.sock` | `testing.md` |

## Hot State Commands

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| hot state | live cache status | `yai hot status` | `target/debug/yai hot status` | `hot-state-plane.md`, `data-plane-roadmap.md` |
| hot snapshot path | snapshot source | `yai hot status` | `YAI_HOME=/tmp/yai-core-home-test yai hot status` | `filesystem-target.md` |
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
| active docs | canonical engineering doc set | `make check-docs` | `make check-docs` | `README.md` |

Pack runtime commands are not implemented yet. SPINE.21 only provides pack
doctrine and guards.

## Foundation / Layout Checks

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| source layout | no old active roots | `make check-layout` | `make check-layout` | `filesystem-target.md` |
| foundation freeze | freeze guard | `make check-foundation-freeze` | `make check-foundation-freeze` | `current-status.md` |
| full docs guard | active doc requirements | `make check-docs` | `make check-docs` | `README.md` |
| full check | layout, docs, build, smoke | `make check` | `make check` | `testing.md` |

`make check-foundation-freeze` is integrated through `make check-docs`, and
therefore through `make check`.

## Install Commands

Manual install matrix:

```sh
rm -rf /tmp/yai-core-install-test /tmp/yai-core-home-test
make install-local PREFIX=/tmp/yai-core-install-test YAI_HOME=/tmp/yai-core-home-test
PATH=/tmp/yai-core-install-test/bin:$PATH yai --version
PATH=/tmp/yai-core-install-test/bin:$PATH yai info
PATH=/tmp/yai-core-install-test/bin:$PATH yai doctor
PATH=/tmp/yai-core-install-test/bin:$PATH yai hot status
/tmp/yai-core-install-test/bin/yaid --version
make uninstall-local PREFIX=/tmp/yai-core-install-test
test ! -e /tmp/yai-core-install-test/bin/yai
test ! -e /tmp/yai-core-install-test/bin/yaid
```

`uninstall-local` removes installed binaries. It does not delete `YAI_HOME`.

## Manual Validation Recipes

Daemon and hot-state recipe:

```sh
mkdir -p /tmp/yai-core-home-test/run
/tmp/yai-core-install-test/bin/yaid \
  --socket /tmp/yai-core-home-test/run/yaid.sock \
  --foreground &
DAEMON_PID=$!

PATH=/tmp/yai-core-install-test/bin:$PATH \
  yai daemon status --socket /tmp/yai-core-home-test/run/yaid.sock

PATH=/tmp/yai-core-install-test/bin:$PATH \
  yai daemon run-minimum-loop --socket /tmp/yai-core-home-test/run/yaid.sock

PATH=/tmp/yai-core-install-test/bin:$PATH yai hot status

PATH=/tmp/yai-core-install-test/bin:$PATH \
  yai daemon shutdown --socket /tmp/yai-core-home-test/run/yaid.sock

wait $DAEMON_PID
```

Expected key output:

```text
hot_state: active
snapshot: /tmp/yai-core-home-test/run/hot-state.json
schema: yai.hot_state.v1
case_session: active
case_context: active
projection_policy: usable
freshness_policy: usable
```

Snapshot edge cases:

```sh
rm -f /tmp/yai-core-home-test/run/hot-state.json
PATH=/tmp/yai-core-install-test/bin:$PATH yai hot status

printf '{broken\n' > /tmp/yai-core-home-test/run/hot-state.json
PATH=/tmp/yai-core-install-test/bin:$PATH yai hot status
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
```
