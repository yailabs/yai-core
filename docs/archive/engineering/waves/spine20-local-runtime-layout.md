Historical/superseded engineering record. Not an active source of truth.

# SPINE.20 Local Runtime Layout

SPINE.20 introduces the first local runtime home for `yai-core`.

Runtime rule:

```text
build/tmp = test/lab
YAI_HOME = real local runtime
```

Implemented surfaces:

```text
make install-local
make uninstall-local
make doctor-local
make print-install-paths
yai doctor
```

Default layout:

```text
PREFIX=$HOME/.local
YAI_HOME=$HOME/.yai
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
$(YAI_HOME)/run
$(YAI_HOME)/store
$(YAI_HOME)/log
$(YAI_HOME)/tmp
$(YAI_HOME)/cases
$(YAI_HOME)/sockets
$(YAI_HOME)/config
$(YAI_HOME)/run/yaid.sock
```

Old-yai audit was read-only. The old `yai` worktree had an existing dirty
architecture file, so no old-yai files were modified. Classification lives in
`docs/internal/extraction-inventory.tsv`.
