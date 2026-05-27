/*
 * YAI - NET lifecycle vocabulary
 *
 * Purpose:
 *   Names local service lifecycle request and report vocabulary.
 *
 * Ownership:
 *   Public NET lifecycle action, state, subject kind, request v1 and report v1
 *   vocabulary.
 *
 * Boundary:
 *   Lifecycle vocabulary describes intent and state material. It does not
 *   spawn processes, supervise services, call OS managers, probe, route or
 *   authorize use.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_LIFECYCLE_H
#define YAI_NET_LIFECYCLE_H

#include <stdint.h>

typedef enum yai_net_lifecycle_action {
    YAI_NET_LIFECYCLE_ACTION_DECLARE = 0,
    YAI_NET_LIFECYCLE_ACTION_PREPARE = 1,
    YAI_NET_LIFECYCLE_ACTION_START = 2,
    YAI_NET_LIFECYCLE_ACTION_STOP = 3,
    YAI_NET_LIFECYCLE_ACTION_RESTART = 4,
    YAI_NET_LIFECYCLE_ACTION_STATUS = 5,
    YAI_NET_LIFECYCLE_ACTION_RETIRE = 6
} yai_net_lifecycle_action_t;

typedef enum yai_net_lifecycle_state {
    YAI_NET_LIFECYCLE_STATE_UNKNOWN = 0,
    YAI_NET_LIFECYCLE_STATE_DECLARED = 1,
    YAI_NET_LIFECYCLE_STATE_PREPARING = 2,
    YAI_NET_LIFECYCLE_STATE_STARTING = 3,
    YAI_NET_LIFECYCLE_STATE_RUNNING = 4,
    YAI_NET_LIFECYCLE_STATE_STOPPING = 5,
    YAI_NET_LIFECYCLE_STATE_STOPPED = 6,
    YAI_NET_LIFECYCLE_STATE_FAILED = 7,
    YAI_NET_LIFECYCLE_STATE_RETIRED = 8
} yai_net_lifecycle_state_t;

typedef enum yai_net_lifecycle_subject_kind {
    YAI_NET_LIFECYCLE_SUBJECT_KIND_NODE = 0,
    YAI_NET_LIFECYCLE_SUBJECT_KIND_ENDPOINT = 1,
    YAI_NET_LIFECYCLE_SUBJECT_KIND_CAPABILITY = 2,
    YAI_NET_LIFECYCLE_SUBJECT_KIND_SERVICE = 3,
    YAI_NET_LIFECYCLE_SUBJECT_KIND_EXTERNAL = 4
} yai_net_lifecycle_subject_kind_t;

typedef struct yai_net_lifecycle_request_v1 {
    const char *lifecycle_request_id;
    const char *subject_id;
    yai_net_lifecycle_subject_kind_t subject_kind;
    yai_net_lifecycle_action_t requested_action;
    const char *requested_by_node_id;
    const char *reason;
    const char *metadata_json;
    uint64_t requested_at_unix_ms;
} yai_net_lifecycle_request_v1_t;

typedef struct yai_net_lifecycle_report_v1 {
    const char *lifecycle_report_id;
    const char *subject_id;
    yai_net_lifecycle_subject_kind_t subject_kind;
    yai_net_lifecycle_state_t lifecycle_state;
    const char *observed_by_node_id;
    const char *reason;
    const char *metadata_json;
    uint64_t observed_at_unix_ms;
} yai_net_lifecycle_report_v1_t;

#endif
