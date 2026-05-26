# Filesystem Loop Flow

```mermaid
flowchart LR
  Checks[Repository checks] --> CommandSurface[Command surface smoke]
  CommandSurface --> HotCli[Hot-state CLI smoke]
  HotCli --> Metrics[Run metrics]
  Metrics --> Notebook[Root notebook]
```

Source: `runs/20260526-smoke/manifest.json`.
