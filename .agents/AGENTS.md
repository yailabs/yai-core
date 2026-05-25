# Agent Instructions

## README Boundary

Do not modify the root `README.md` for SPINE implementation, roadmap, carrier,
retrieval, model-runtime, command-surface or doctrine updates unless the user
explicitly asks for a root README edit in that turn.

Put operational detail in the canonical engineering and architecture docs
instead:

```text
docs/engineering/command-surface.md
docs/engineering/current-status.md
docs/engineering/four-repo-roadmap.md
docs/engineering/data-plane-roadmap.md
docs/architecture/
```

The root README is a principal public entry document. Do not re-add detailed
carrier command blocks, skeleton carrier details, retrieval roadmap doctrine or
model-runtime roadmap doctrine there as incidental wave work.
