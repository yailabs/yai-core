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
11. Use test IDs, run IDs and visual artifact IDs when writing numbered run
    material.
12. Do not overwrite visuals from earlier runs.
13. Link figure manifests when reports use figures, charts, diagrams or tables.

## Required Agent Checks

Before writing:

- identify the target lab;
- identify the document type;
- identify the test ID and run ID when writing run material;
- inspect existing artifacts if they exist;
- decide whether the document is draft, active or frozen.

While writing:

- keep unsupported claims out of the `claims` metadata field;
- separate observations from interpretation;
- preserve missing data as missing;
- keep raw, normalized, figures, diagrams, tables and exports separated;
- declare visual provenance and source data;
- avoid broad validity language unless supported.

Before completion:

- confirm required sections exist;
- confirm artifacts are linked or explicitly absent;
- confirm run and figure manifests exist when required;
- confirm generated visuals do not overwrite older run visuals;
- confirm limitations are present;
- confirm no secrets are included.
