# Carrier Contract v1

Status: active
Owner: system/effect and command surface
Purpose: Track SPINE.33C carrier.v1 filesystem adapter work.
Not source of truth for: process carrier, network carrier or receipt replay

SPINE.33C defines carrier.v1 and adapts the existing filesystem carrier as the
first minimal contract example.

Implemented files:

```text
include/yai/effect/carrier_contract.h
include/yai/effect/carrier_outcome.h
include/yai/effect/carrier_receipt.h
system/effect/carrier_contract.c
system/effect/carrier_outcome.c
system/effect/carrier_receipt.c
system/effect/carriers/filesystem_carrier.c
```

filesystem is the first carrier.v1 adapter.

Manual inspection:

```bash
yai carrier inspect filesystem
yai carrier route --family filesystem
yai store record list --kind filesystem_receipt --limit 10
```

The filesystem adapter maps:

```text
fs.read -> observed
blocked fs.write -> blocked
allowed fs.write -> executed
guarantee_mode -> interposed
receipt_required -> yes
```

Boundary:

```text
carrier outcome is not gate outcome
carrier outcome is not dispatch status
carrier outcome is not receipt guarantee mode
```

No process, network, database, git, model or observation carrier execution is
added in this wave.
