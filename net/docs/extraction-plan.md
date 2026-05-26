# NET Extraction Plan

NET.SPINE.0 starts the audit surface only. It does not mutate `system/` or
`engine/`.

## system/ Candidates To Inspect Later

- provider health
- endpoint checks
- carrier transport assumptions
- LAN/discovery assumptions
- service lifecycle assumptions

## engine/ Candidates To Inspect Later

- endpoint state
- external service status
- live transport residue
- provider runtime assumptions

Extraction waves must quarantine or move network assumptions into NET without
changing YAI authority ownership.
