# Experiment Report Contract

Document type: `experiment_report`.

Required sections:

```text
Research Question
Hypothesis
Setup
Method
Metrics
Results
Failure Modes
Interpretation
Limitations
Reproducibility
Artifacts
```

## Section Requirements

Research Question: state what the experiment asks.

Hypothesis: state the expected outcome before results are interpreted.

Setup: identify hardware, software, configuration and environment when
relevant.

Method: describe commands, procedure and evaluation path.

Metrics: define measured quantities and units.

Results: report observed outputs without overclaiming.

Failure Modes: list observed or expected ways the method can fail.

Interpretation: explain what the results suggest.

Limitations: define validity boundaries.

Reproducibility: list commit, commands, inputs, configs, outputs and
nondeterminism.

Artifacts: link raw and normalized artifacts.

## Rule

If a section is unknown, write `Not measured` or `TODO`, not invented content.
