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

The root README is a principal public entry document. Treat its current content
as frozen. Do not add, remove, rewrite, compress, expand, modernize, or
roadmap-update README sections as incidental wave work.

In particular, do not alter the current root README sections such as `What YAI
Is`, `Design Constraints`, `What YAI Is Not`, `Operational Model`, `Current
Validation`, `Current Implementation Surface`, or `Documentation` unless
Francesco explicitly asks for that exact README edit in the current turn.
