# engine_bridge

Transitional role: C ABI/FFI bridge into the Rust operational data engine.

Target role: the only C system location for Rust engine handles, allocation
adapters, status translation and FFI shims. It must not grow durable store,
graph, index, query, memory, projection or reconcile logic.

Owning wave: NEW.18 creates this boundary. Later data-spine waves thin the
remaining C data folders toward Rust engine ownership while keeping bridge code
here.
