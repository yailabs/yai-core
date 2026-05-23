Historical/superseded engineering record. Not an active source of truth.

# NEW.13 Superseded: Local Command And Install Layout

SPINE.1 supersedes this wave assignment. Local command and install layout is no
longer NEW.13; it is delayed to NEW.20. NEW.13 is now target filesystem
doctrine/refactor planning for `system/engine/cmd/include`.

This file remains as historical planning material until the NEW.20 docs are
written. Do not implement install layout from this file before the filesystem /
data-spine refoundation waves.

## Commands

```text
yai  = Rust technical control command
yaid = C local daemon
```

`yai` is the command used by docs and future delivery boxes. `yaid` is installed
beside it as the local daemon binary.

## Planned Local Install

```text
make install-local PREFIX=$HOME/.local YAI_HOME=$HOME/.yai
make doctor-local PREFIX=$HOME/.local YAI_HOME=$HOME/.yai
make uninstall-local PREFIX=$HOME/.local
```

Installed binaries:

```text
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
```

Local host layout:

```text
$(YAI_HOME)/run/
$(YAI_HOME)/store/
$(YAI_HOME)/log/
$(YAI_HOME)/tmp/
```

Default daemon socket:

```text
$(YAI_HOME)/run/yaid.sock
```

`doctor-local` warns when `$(PREFIX)/bin` is not on `PATH`. `yai doctor`
reports the current binary path, discovered `yaid` path, version, `YAI_HOME`,
runtime directories, `PATH` posture and default daemon socket.

## Boundaries

NEW.20 should not add service manager integration, package signing, Homebrew,
systemd, launchd, auth, cloud, console ownership or interfaces ownership.

Console remains the future operator UX. `interfaces` remains the downstream
projection/API/SDK/conformance layer. The local `yai` command is a technical
core command, not the Console product surface.
