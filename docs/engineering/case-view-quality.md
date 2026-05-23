# Case View Quality

Status: SPINE.4 quality vector.

Case View Quality, `CVQ`, is the canonical vector for measuring whether a live
case view is good enough for controlled behavior.

CVQ does not say the case is true. It says whether the current view over the
case is fresh, causal, provenanced, coherent, complete, replayable and useful
enough for a specific consumer.

## Fields

| Field | Meaning |
|---|---|
| `freshness` | How current the view is relative to case residue and latest delta. |
| `causal_completeness` | Whether relevant actions, decisions, effects and receipts form reconstructable chains. |
| `provenance_sufficiency` | Whether claims in the view have enough source refs. |
| `projection_consistency` | Whether the projection matches its declared case version, redaction and source refs. |
| `authority_alignment` | Whether the view matches the consumer authority scope. |
| `memory_basis_quality` | Whether memory included in the view has record, receipt, graph or divergence basis. |
| `divergence_exposure` | Whether known mismatch is visible to the right consumer. |
| `delta_accuracy` | Whether the next view correctly represents the changes since the prior view. |
| `cost` | Latency, storage, compute and model-visible overhead of producing the view. |

## Consumer Rule

Different consumers can have different acceptable CVQ thresholds. A debug view
may accept broad provenance and raw refs. A model view must remain authority
aligned, redacted and freshness-aware. An operator view may expose divergence
more directly.

CVQ belongs to YAI Core because YAI owns the quality of the controlled case
view. Model inference behavior and inference latency remain model/provider
behavior.
