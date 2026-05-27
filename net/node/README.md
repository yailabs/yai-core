# node

Owns NET node identity vocabulary and future node projection material.

Node identity v1 is represented in `include/yai/net/node.h`. Node fixtures live
under `work/protocols/fixtures/net/node/`.

This area does not implement discovery yet and does not probe the local machine
yet.

A node advertises communication identity; it does not grant authority.

Current status: scaffold.

Next expected wave: NET.SPINE.5 local endpoint registry skeleton.
