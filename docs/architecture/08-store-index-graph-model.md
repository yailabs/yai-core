# Store, Index And Graph Model

The data plane is split into separate roots because persistence, retrieval and reconstruction are different responsibilities.

## Store

Store owns physical persistence posture:

```text
journal
record store
receipt store
source store
blob store
retention
integrity
backend abstraction
```

Store does not own semantic truth.

## Index

Index owns technical retrieval structures:

```text
lexical index
structured index
semantic index
vector index
hybrid retrieval
freshness
query safety
```

Index does not own memory or projection truth.

## Graph

Graph owns causal reconstruction and relationship edges:

```text
subject graph
operation graph
decision graph
receipt graph
policy graph
memory graph
causal reconstruction
```

Graph is the replacement for final `lineage` semantics.

## Split Rule

Records are facts, store is persistence, graph is relation, index is retrieval, memory is continuity, projection is read model.
