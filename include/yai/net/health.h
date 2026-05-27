/*
 * YAI - NET health vocabulary
 *
 * Purpose:
 *   Names health, readiness and liveness report vocabulary.
 *
 * Ownership:
 *   Public NET health state, check kind, subject kind and report v1
 *   vocabulary.
 *
 * Boundary:
 *   Health vocabulary describes observations and declarations. It does not
 *   implement probes, call HTTP, open sockets, route or authorize use.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_HEALTH_H
#define YAI_NET_HEALTH_H

#include <stdint.h>

typedef enum yai_net_health_state {
    YAI_NET_HEALTH_STATE_UNKNOWN = 0,
    YAI_NET_HEALTH_STATE_ALIVE = 1,
    YAI_NET_HEALTH_STATE_READY = 2,
    YAI_NET_HEALTH_STATE_DEGRADED = 3,
    YAI_NET_HEALTH_STATE_UNAVAILABLE = 4,
    YAI_NET_HEALTH_STATE_FAILED = 5
} yai_net_health_state_t;

typedef enum yai_net_health_check_kind {
    YAI_NET_HEALTH_CHECK_KIND_HEALTH = 0,
    YAI_NET_HEALTH_CHECK_KIND_LIVENESS = 1,
    YAI_NET_HEALTH_CHECK_KIND_READINESS = 2,
    YAI_NET_HEALTH_CHECK_KIND_CAPABILITY_READINESS = 3,
    YAI_NET_HEALTH_CHECK_KIND_ENDPOINT_REACHABILITY = 4
} yai_net_health_check_kind_t;

typedef enum yai_net_health_subject_kind {
    YAI_NET_HEALTH_SUBJECT_KIND_NODE = 0,
    YAI_NET_HEALTH_SUBJECT_KIND_ENDPOINT = 1,
    YAI_NET_HEALTH_SUBJECT_KIND_CAPABILITY = 2,
    YAI_NET_HEALTH_SUBJECT_KIND_TRANSPORT = 3,
    YAI_NET_HEALTH_SUBJECT_KIND_EXTERNAL = 4
} yai_net_health_subject_kind_t;

typedef struct yai_net_health_report_v1 {
    const char *health_report_id;
    const char *subject_id;
    yai_net_health_subject_kind_t subject_kind;
    yai_net_health_check_kind_t check_kind;
    yai_net_health_state_t health_state;
    const char *observed_by_node_id;
    const char *reason;
    const char *metadata_json;
    uint64_t observed_at_unix_ms;
} yai_net_health_report_v1_t;

#endif
