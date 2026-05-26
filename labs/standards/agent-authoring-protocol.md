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
16. Treat `report.md` as the final human artifact; figures support the report,
    not the reverse.
17. Keep lab notebooks human-guided; move orchestration scripts into `run.sh`
    or `labs/shared/bin/`.

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
- keep the canonical notebook at lab/test root as a guided human demo;
- keep `runbook.md` as the complete CLI procedure;
- keep `prompts.json` as the prompt/query text authority;
- do not embed prompt bodies in notebooks except as read-only inspection output
  from the prompt catalog;
- do not put long shell control flow, JSON construction, daemon bootstrap or
  cleanup orchestration in notebooks;
- call `run.sh`, `make`, `yai` or shared helper scripts from notebooks instead
  of reimplementing those scripts in cells;
- do not put `notebook.ipynb` inside run assets;
- keep resolved prompt payloads as run assets when they affect results;
- record which `pack-fixture/` material was used when a lab run uses fixture
  input material;
- distinguish pack fixture material from materialized case state;
- mark unknown materialization behavior as `unknown` or `not implemented`;
- declare visual provenance and source data;
- generate figures only from existing run metrics or declared run assets;
- omit unsupported charts instead of inventing values;
- reference generated figures from both `report.md` and `manifest.json`;
- use markdown tables instead of SVG when a tiny categorical table is clearer;
- keep diagnostic presence/absence figures out of benchmark interpretation;
- add missing-measurement and limitation sections when evidence is absent;
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
prompts.json
run.sh
runs/YYYYMMDD-<run-slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

Do not create separate aggregate folders, manuals surfaces, lab-level outputs
or lab-level tests files for canonical labs.

Before finishing any lab structure change, run:

```bash
make check-labs
make check-lab-runs
```
