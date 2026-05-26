# Document Taxonomy

Supported document types:

```text
note
experiment_report
technical_report
position_paper
systems_paper
benchmark_report
```

## note

Purpose: capture bounded observations, open questions or planning residue.

Use when:

- the work is exploratory;
- evidence is incomplete;
- no method has produced a result yet.

Do not use when:

- making a supported claim;
- reporting measured results;
- freezing a technical finding.

Required sections:

- Summary
- Context
- Notes
- Unknowns
- Artifacts

Expected artifacts:

- optional links to source notes, logs, configs or references.

## experiment_report

Purpose: report a method-driven experiment with question, hypothesis, setup,
metrics and results.

Use when:

- a method was executed;
- inputs, commands and outputs can be identified;
- metrics or observations are produced by the method.

Do not use when:

- the work is only a design note;
- the method is not defined;
- results are unavailable.

Required sections:

- Research Question
- Hypothesis
- Setup
- Method
- Metrics
- Results
- Failure Modes
- Interpretation
- Limitations
- Reproducibility
- Artifacts

Expected artifacts:

- commands, configs, inputs, outputs, logs, normalized data or figures.

## technical_report

Purpose: summarize a technical system state, investigation or implementation
finding.

Use when:

- documenting system state;
- collecting findings from source inspection or run evidence;
- preparing next engineering work.

Do not use when:

- arguing a broad position;
- reporting a controlled experiment;
- publishing benchmark claims.

Required sections:

- Scope
- System State
- Method / Sources
- Findings
- Discussion
- Limitations
- Next Work
- Artifacts

Expected artifacts:

- source references, commands, logs, reports, snapshots or diagrams.

## position_paper

Purpose: state and defend a technical position.

Use when:

- the main output is an argument;
- evidence supports a position but does not constitute a full systems paper.

Do not use when:

- reporting measured benchmark results;
- presenting implementation details as the primary contribution.

Required sections:

- Problem
- Position
- Argument
- Implications
- Counterarguments
- Limitations
- References

Expected artifacts:

- references, supporting notes and evidence links when available.

## systems_paper

Purpose: present a system model, architecture, implementation and evaluation.

Use when:

- there is a system to describe;
- design goals and implementation are stable enough to evaluate.

Do not use when:

- the system is only planned;
- the document is a short engineering note.

Required sections:

- Abstract
- Introduction
- Problem
- Design Goals
- System Model
- Architecture
- Implementation
- Evaluation
- Discussion
- Limitations
- Related Work
- Conclusion
- References
- Appendix

Expected artifacts:

- implementation references, diagrams, evaluation data and reproducibility
  package.

## benchmark_report

Purpose: report benchmark motivation, systems under test, workloads, metrics,
setup, results and validity threats.

Use when:

- a benchmark was run or is being frozen as an evidence package;
- metrics and systems under test are explicit.

Do not use when:

- only reachability was checked and no benchmark claim is made;
- raw output has not been curated.

Required sections:

- Benchmark Motivation
- Systems Under Test
- Workloads
- Metrics
- Experimental Setup
- Results
- Analysis
- Threats to Validity
- Reproducibility Package

Expected artifacts:

- raw data, normalized data, scripts, configs, figures and report outputs.
