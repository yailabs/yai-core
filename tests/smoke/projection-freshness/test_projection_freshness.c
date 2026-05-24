#include "yai/projection/freshness.h"

#include <stdio.h>

static int require_policy(const char *label,
                          const char *consumer,
                          yai_hot_freshness_t freshness,
                          yai_hot_stale_reason_t reason,
                          yai_projection_freshness_policy_t expected) {
    yai_projection_freshness_policy_t actual =
        yai_projection_freshness_policy_for_consumer(consumer, freshness, reason);
    if (actual != expected) {
        fprintf(stderr,
                "%s expected %s got %s\n",
                label,
                yai_projection_freshness_policy_string(expected),
                yai_projection_freshness_policy_string(actual));
        return 1;
    }
    return 0;
}

int main(void) {
    if (require_policy("fresh model",
                       "model",
                       YAI_HOT_FRESHNESS_FRESH,
                       YAI_HOT_STALE_NONE,
                       YAI_PROJECTION_POLICY_USABLE) != 0) {
        return 1;
    }
    printf("projection:fresh policy=usable consumer=model\n");

    if (require_policy("receipt model",
                       "model",
                       YAI_HOT_FRESHNESS_STALE,
                       YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION,
                       YAI_PROJECTION_POLICY_REFRESH_REQUIRED) != 0) {
        return 1;
    }
    printf("receipt:marked\n");
    printf("projection:stale reason=new_receipt_after_projection policy=refresh_required consumer=model\n");

    if (require_policy("receipt operator",
                       "operator",
                       YAI_HOT_FRESHNESS_STALE,
                       YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION,
                       YAI_PROJECTION_POLICY_REFRESH_RECOMMENDED) != 0) {
        return 1;
    }
    printf("projection:stale policy=refresh_recommended consumer=operator\n");

    if (require_policy("authority model",
                       "model",
                       YAI_HOT_FRESHNESS_STALE,
                       YAI_HOT_STALE_NEW_AUTHORITY_SCOPE_AFTER_PROJECTION,
                       YAI_PROJECTION_POLICY_BLOCKED_FOR_MODEL) != 0) {
        return 1;
    }
    printf("projection:stale reason=new_authority_scope_after_projection policy=blocked_for_model consumer=model\n");

    if (require_policy("thread model",
                       "model",
                       YAI_HOT_FRESHNESS_STALE,
                       YAI_HOT_STALE_THREAD_CHANGED,
                       YAI_PROJECTION_POLICY_BLOCKED_FOR_MODEL) != 0) {
        return 1;
    }
    printf("projection:stale reason=thread_changed policy=blocked_for_model consumer=model\n");

    if (require_policy("refresh model",
                       "model",
                       YAI_HOT_FRESHNESS_FRESH,
                       YAI_HOT_STALE_NONE,
                       YAI_PROJECTION_POLICY_USABLE) != 0) {
        return 1;
    }
    printf("projection:refreshed policy=usable\n");
    return 0;
}
