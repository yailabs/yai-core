# node

Owns NET node identity vocabulary and future node projection material.

Node identity v1 is represented in `include/yai/net/node.h`. Node fixtures live
under `proto/fixtures/net/node/`.

Node fixture `capability_refs` now point to capability advertisement fixtures
under `proto/fixtures/net/capability/`.

Node fixture ids may be referenced by endpoint descriptors.

Node fixture ids may be referenced by health reports.

This area does not implement discovery yet and does not probe the local machine
yet.

A node advertises communication identity; it does not grant authority.

`net/node/node.c` provides pure node-kind name and validity helpers only.

Current status: C translation unit scaffold started.

Next expected wave: NET.SPINE.5 local endpoint registry skeleton.
