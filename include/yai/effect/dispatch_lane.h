/*
 * YAI - Dispatch lane vocabulary
 *
 * Purpose:
 *   Names carrier dispatch lanes and their scheduling policy vocabulary.
 *
 * Ownership:
 *   Dispatch lane, ordering, capacity, lock, timeout and retry enums.
 *
 * Boundary:
 *   This file does not own queues, workers, carrier execution or receipts.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_DISPATCH_LANE_H
#define YAI_EFFECT_DISPATCH_LANE_H

#include "yai/effect/carrier_family.h"

typedef enum yai_dispatch_lane {
    YAI_DISPATCH_LANE_FILESYSTEM = 0,
    YAI_DISPATCH_LANE_PROCESS = 1,
    YAI_DISPATCH_LANE_NETWORK_HTTP = 2,
    YAI_DISPATCH_LANE_DATABASE = 3,
    YAI_DISPATCH_LANE_REPOSITORY_GIT = 4,
    YAI_DISPATCH_LANE_MODEL_PROVIDER = 5,
    YAI_DISPATCH_LANE_OBSERVATION = 6,
    YAI_DISPATCH_LANE_REVIEW = 7,
    YAI_DISPATCH_LANE_UNKNOWN = 8
} yai_dispatch_lane_t;

typedef enum yai_ordering_policy {
    YAI_ORDERING_SERIAL_PER_CASE = 0,
    YAI_ORDERING_PARALLEL_BY_SUBJECT = 1,
    YAI_ORDERING_EXCLUSIVE_GLOBAL = 2,
    YAI_ORDERING_UNORDERED = 3,
    YAI_ORDERING_UNKNOWN = 4
} yai_ordering_policy_t;

typedef enum yai_capacity_policy {
    YAI_CAPACITY_SINGLE_INFLIGHT = 0,
    YAI_CAPACITY_BOUNDED_PARALLEL = 1,
    YAI_CAPACITY_UNBOUNDED_PLANNED = 2,
    YAI_CAPACITY_DISABLED = 3,
    YAI_CAPACITY_UNKNOWN = 4
} yai_capacity_policy_t;

typedef enum yai_lock_policy {
    YAI_LOCK_CASE = 0,
    YAI_LOCK_SUBJECT = 1,
    YAI_LOCK_TARGET = 2,
    YAI_LOCK_CARRIER = 3,
    YAI_LOCK_NONE = 4,
    YAI_LOCK_UNKNOWN = 5
} yai_lock_policy_t;

typedef enum yai_timeout_policy {
    YAI_TIMEOUT_SHORT = 0,
    YAI_TIMEOUT_NORMAL = 1,
    YAI_TIMEOUT_LONG = 2,
    YAI_TIMEOUT_MANUAL = 3,
    YAI_TIMEOUT_UNKNOWN = 4
} yai_timeout_policy_t;

typedef enum yai_retry_policy {
    YAI_RETRY_NONE = 0,
    YAI_RETRY_SAFE = 1,
    YAI_RETRY_MANUAL = 2,
    YAI_RETRY_REQUIRES_REVIEW = 3,
    YAI_RETRY_UNKNOWN = 4
} yai_retry_policy_t;

const char *yai_dispatch_lane_to_string(yai_dispatch_lane_t lane);
yai_dispatch_lane_t yai_dispatch_lane_from_string(const char *value);

const char *yai_ordering_policy_to_string(yai_ordering_policy_t policy);
const char *yai_capacity_policy_to_string(yai_capacity_policy_t policy);
const char *yai_lock_policy_to_string(yai_lock_policy_t policy);
const char *yai_timeout_policy_to_string(yai_timeout_policy_t policy);
const char *yai_retry_policy_to_string(yai_retry_policy_t policy);

yai_carrier_family_t yai_dispatch_lane_carrier_family(yai_dispatch_lane_t lane);

#endif
