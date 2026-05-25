# Review and Freeze Checklist

Use this checklist before marking a document `frozen`.

## Metadata

- `id` is stable.
- `type` matches the taxonomy.
- `status` is valid.
- `version` is set.
- `date` is set.
- `authors` are listed.
- `project` and `lab` are set.
- `summary` is present.

## Structure

- Required sections for the document type are present.
- Unknowns are marked as `TODO`, `Not measured` or `Not recorded`.
- No required section silently disappears.

## Claims and Evidence

- Claims are separated from results.
- Results are separated from interpretation.
- Evidence links to artifacts or declares missing artifacts.
- Limitations are explicit.

## Reproducibility

- Commit or source state is recorded when relevant.
- Commands are listed.
- Inputs, configs and outputs are linked.
- Known nondeterminism is stated.

## Safety

- No secrets are included.
- No unsupported public claims are present.
- No exploratory note is presented as a scientific result.
