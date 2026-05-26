.PHONY: info check

REQUIRED_DOCS := \
	README.md \
	docs/architecture.md \
	docs/boundary.md \
	docs/terminology.md \
	docs/component-map.md \
	docs/yai-integration.md \
	docs/interfaces-transport-boundary.md \
	docs/external-node-boundary.md \
	docs/clori-node-boundary.md \
	docs/extraction-plan.md

REQUIRED_HEADERS := \
	include/yai/net/net.h \
	include/yai/net/version.h \
	include/yai/net/error.h \
	include/yai/net/ids.h \
	include/yai/net/stream.h \
	include/yai/net/node.h \
	include/yai/net/capability.h \
	include/yai/net/endpoint.h \
	include/yai/net/health.h \
	include/yai/net/invocation.h \
	include/yai/net/trace.h \
	include/yai/net/receipt.h \
	include/yai/net/metrics.h

REQUIRED_DIRS := \
	src/core \
	src/registry \
	src/stream \
	src/node \
	src/capability \
	src/endpoint \
	src/health \
	src/transport \
	src/route \
	src/observe \
	src/compat \
	tests \
	fixtures \
	benches \
	examples

info:
	@printf "net: yai runtime communication substrate\n"
	@printf "status: scaffold\n"
	@printf "discovery: not implemented\n"
	@printf "transport: not implemented\n"
	@printf "routing: not implemented\n"
	@printf "external-nodes: planned\n"

check:
	@for path in $(REQUIRED_DOCS); do test -f "$$path" || { printf "missing NET doc: %s\n" "$$path" >&2; exit 1; }; done
	@for path in $(REQUIRED_HEADERS); do test -f "$$path" || { printf "missing NET header: %s\n" "$$path" >&2; exit 1; }; done
	@for path in $(REQUIRED_DIRS); do test -d "$$path" || { printf "missing NET dir: %s\n" "$$path" >&2; exit 1; }; done
	@grep -F "YAI controls authority." README.md >/dev/null
	@grep -F "NET moves streams." README.md >/dev/null
	@grep -F "External nodes execute capabilities." README.md >/dev/null
	@grep -F "NET does not own case authority" docs/boundary.md >/dev/null
	@grep -F "NET does not own policy resolution" docs/boundary.md >/dev/null
	@grep -F "NET does not own neural execution" docs/boundary.md >/dev/null
	@grep -F "NET does not own graph truth" docs/boundary.md >/dev/null
	@grep -F "NET does not own memory truth" docs/boundary.md >/dev/null
	@grep -F "NET does not own journal truth" docs/boundary.md >/dev/null
	@grep -F "CLORI is external" docs/clori-node-boundary.md >/dev/null
	@grep -F "CLORI is not vendored into YAI" docs/clori-node-boundary.md >/dev/null
	@grep -F "CLORI executes neural computation" docs/clori-node-boundary.md >/dev/null
	@printf "net check: ok\n"
