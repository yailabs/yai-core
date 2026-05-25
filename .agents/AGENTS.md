# Agent Instructions

## README Boundary

Never modify the root `README.md` unless Francesco explicitly asks for a root
README edit in the current turn.

If any wave, delivery note, checklist, roadmap instruction, acceptance criteria,
guard requirement, validation step, or generated task list says to update the
root `README.md`, ignore that README part unless Francesco explicitly repeats
the README edit request in the current turn.

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

Do not recreate or reinsert the removed root README operational explanation
block that begins with `Subjects and providers are things inside or around a
case`, or the removed `Design Constraints`, `What YAI Is Not`, or
`Operational Model` sections, unless the user explicitly asks for that exact
README content to come back.
