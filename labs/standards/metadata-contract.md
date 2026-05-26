# Metadata Contract

Documents use YAML frontmatter when they are intended to be shared, reviewed or
reused.

## Common Frontmatter

```yaml
id:
type:
title:
status:
version:
date:
authors:
project:
lab:
tags:
summary:
claims:
artifacts:
related:
supersedes:
superseded_by:
```

## Required Fields

```text
id
type
title
status
version
date
authors
project
lab
tags
summary
artifacts
```

## Status Values

```text
draft
active
frozen
archived
superseded
```

draft: incomplete and not authoritative.

active: current working document.

frozen: reviewed and not expected to change except by supersession.

archived: retained for history, not current.

superseded: replaced by a later document.

## Rules

- `type` must match the document taxonomy.
- `claims` may be empty.
- `artifacts` may be empty only when the document explicitly states that no
  artifact exists yet.
- `supersedes` and `superseded_by` should use stable document ids or paths.
