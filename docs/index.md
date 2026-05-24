# YAI Documentation

This is the public documentation entrypoint for YAI. Start here if you are
evaluating the repository from the outside and want the short technical path
before entering older architecture or engineering notes.

YAI is a local control runtime for case-bound AI operation. It gives model,
provider, tool, operator, and system activity an operational boundary: case,
subject, attempted operation, control decision, effect or observation, receipt,
record, projection, and operational memory.

## Start Here

- [Repository README](../README.md): project identity, status, boundaries, and
  repository layout.
- [Technical brief](technical-brief.md): five-minute technical explanation.
- [Quickstart](quickstart.md): minimal local orientation and validation entry.
- [Test cases](test-cases.md): reproducible validation paths.
- [Provider boundary](providers.md): provider experiment matrix and boundary
  posture.
- [Architecture summary](architecture.md): public architecture overview.
- [Glossary](glossary.md): short term definitions for first-contact readers.
- [Legal posture](legal.md): source-available repository posture.

## Current Status

YAI is an early source-available technical repository. It is public for
technical evaluation and review, not production use, unless a specific written
permission or component license says otherwise.

The command and test surfaces are still stabilizing. Repository-specific status
and license files are authoritative for current posture.

## Public Docs

- [Technical brief](technical-brief.md)
- [Quickstart](quickstart.md)
- [Test cases](test-cases.md)
- [Provider boundary](providers.md)
- [Architecture summary](architecture.md)
- [Glossary](glossary.md)
- [Legal posture](legal.md)

## Current Engineering References

These are useful today, but they still contain internal planning/history and
should be read as current engineering references rather than polished public
manuals:

- [Command surface](engineering/command-surface.md)
- [Source surface](engineering/source-surface.md)
- [Testing](engineering/testing.md)
- [Test cases](test-cases.md)
- [Filesystem loop manual](manuals/manual-filesystem-loop-validation.md)
- [Architecture notes](architecture/README.md)

## Advanced And Internal Docs

- `docs/adr/` contains architecture decision records.
- `docs/protocols/` contains early protocol shapes.
- `docs/archive/` contains historical engineering material.
- `docs/internal/` contains repository readiness and internal planning records.

## Legal And Contribution

- [License notice](../LICENSE.md)
- [Repository notice](../NOTICE.md)
- [Third-party notices](../THIRD_PARTY_NOTICES.md)
- [Contributing](../CONTRIBUTING.md)
- [Security](../SECURITY.md)
- [Legal summary](legal.md)

YAI is source-available, not open source by default. Technical feedback is
welcome, but broad external contribution is not open yet unless maintainers
explicitly scope the change.

## What Is Still Being Split

The public documentation surface is being split gradually from older
architecture, manual, and engineering material. The current public pages are
short entrypoints. Command and daemon references will be split into focused
public docs in later passes.
