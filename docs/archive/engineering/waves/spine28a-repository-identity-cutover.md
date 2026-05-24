Historical/superseded engineering record. Not an active source of truth.

# SPINE.28A Repository Identity Cutover

Status: done.

## Decision

The canonical local AI operational control system is now `yai`.

The old concept-mine repository is now `yai-dev`.

```text
YAI/yai     = canonical local AI operational control system
YAI/yai-dev = development lab, concept mine, harness and scenario workspace
```

`interfaces`, `console` and Studio were not modified by this wave.

## Local Directory Cutover

Completed locally:

```text
YAI/yai-core -> YAI/yai
YAI/yai      -> YAI/yai-dev
```

The local worktrees were clean before the physical rename.

## Remote Caveat

Remote URLs were inspected but intentionally left unchanged because GitHub
repository rename order must be confirmed first.

Current local remote posture at the time of this record:

```text
YAI/yai     origin -> yailabs/yai-core.git
YAI/yai-dev origin -> yailabs/yai.git
```

Do not push the renamed repos until the GitHub repository names are confirmed.

## Active Updates

Updated active `yai` docs, command output, guards and extraction inventory to
use:

```text
yai
yai-dev
```

instead of:

```text
yai-core
old yai
ai-environment
```

Added `check-repository-identity` so active docs and tests do not regress to
the old repository names.

Updated `yai-dev/README.md` with the development-lab role note.
