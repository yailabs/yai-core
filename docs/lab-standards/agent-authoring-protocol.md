# Agent Authoring Protocol

Agents writing lab documents must follow this protocol.

1. Determine document type before writing.
2. Load the matching contract.
3. Use the matching template.
4. Do not mix claim, result and interpretation.
5. Do not invent data.
6. Mark unknowns as `TODO` or `Not measured`.
7. Declare limitations.
8. Link artifacts if present.
9. Do not convert exploratory notes into scientific results.
10. Do not produce public claims from draft lab notes.
11. Use test IDs, run slugs and visual artifact IDs when writing run material.
12. Do not overwrite visuals from earlier runs.
13. Link manifest entries when reports use figures, charts, diagrams or tables.
14. Do not create or publish a prompted benchmark without a prompt artifact.
15. Do not infer production pack behavior from `pack-fixture/` files.

## Required Agent Checks

Before writing:

- identify the target lab;
- identify the document type;
- identify the test ID and run ID when writing run material;
- inspect existing artifacts if they exist;
- identify whether the run is prompted or non-prompted;
- decide whether the document is draft, active or frozen.

While writing:

- keep unsupported claims out of the `claims` metadata field;
- separate observations from interpretation;
- preserve missing data as missing;
- keep run artifacts in `assets/` for compact runs;
- keep the canonical notebook at lab/test root;
- keep `runbook.md` as the static Markdown copy of the canonical notebook;
- do not put `notebook.ipynb` inside run assets;
- keep prompts as explicit assets or metric fields when they affect results;
- record which `pack-fixture/` material was used when a lab run uses fixture
  input material;
- distinguish pack fixture material from materialized case state;
- mark unknown materialization behavior as `unknown` or `not implemented`;
- declare visual provenance and source data;
- avoid broad validity language unless supported.

Before completion:

- confirm required sections exist;
- confirm artifacts are linked or explicitly absent;
- confirm prompted runs include prompt artifacts or a `Not available` boundary;
- confirm run manifests and metrics files exist when required;
- confirm generated visuals do not overwrite older run visuals;
- confirm limitations are present;
- confirm no secrets are included.

## Compact Layout Rule

For new canonical lab work, create:

```text
README.md
runbook.md
notebook.ipynb
run.sh
runs/YYYYMMDD-<run-slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

Do not create separate aggregate folders.

Before finishing any lab structure change, run:

```bash
make check-labs
make check-lab-runs
```
