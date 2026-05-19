# Protocols

This folder defines first protocol shapes for the future YAI Core. These are not final public API contracts. Public API, SDK and conformance truth will belong to `interfaces`.

Each protocol must preserve the machine spine:

```text
ingest -> subject -> case -> op -> control -> effect -> receipt -> store -> graph/index -> memory -> projection -> reconcile
```
