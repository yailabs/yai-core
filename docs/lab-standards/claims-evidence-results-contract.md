# Claims, Evidence and Results Contract

This contract separates propositions from observations and interpretations.

## Terms

claim: a proposition the document supports.

evidence: material that supports or contests a claim.

result: an observation produced by a defined method or experiment.

interpretation: meaning assigned to a result.

limitation: boundary of validity.

## Rules

A result is not automatically a claim.

A retrieval hit is not evidence by itself.

A model output is not a result unless the method defines how it is evaluated.

Evidence must point to an artifact, a reference, or an explicit statement that
no artifact exists yet.

Interpretation must not be written as if it were the result.

Limitations must be stated before a document is frozen.

## Minimum Trace

Every supported claim should have this trace:

```text
claim -> evidence/artifact -> result or source -> limitation
```

If the trace is missing, mark the claim as draft, unsupported or TODO.
