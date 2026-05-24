/*
 * YAI - Receipt guarantee mode
 *
 * Implements:
 *   String conversion for receipt guarantee posture.
 *
 * This file owns:
 *   Receipt guarantee vocabulary conversion.
 *
 * This file does not own:
 *   Receipt creation, persistence or logging behavior.
 */

#include "yai/effect/receipt_guarantee.h"

#include <string.h>

const char *yai_receipt_guarantee_mode_to_string(yai_receipt_guarantee_mode_t mode) {
    switch (mode) {
    case YAI_RECEIPT_GUARANTEE_OBSERVED:
        return "observed";
    case YAI_RECEIPT_GUARANTEE_INTERPOSED:
        return "interposed";
    case YAI_RECEIPT_GUARANTEE_CARRIER_OWNED:
        return "carrier_owned";
    case YAI_RECEIPT_GUARANTEE_EMBEDDED:
        return "embedded";
    case YAI_RECEIPT_GUARANTEE_EXTERNAL_IMPORT:
        return "external_import";
    case YAI_RECEIPT_GUARANTEE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_receipt_guarantee_mode_t yai_receipt_guarantee_mode_from_string(const char *value) {
    if (value == 0) {
        return YAI_RECEIPT_GUARANTEE_UNKNOWN;
    }
    if (strcmp(value, "observed") == 0) return YAI_RECEIPT_GUARANTEE_OBSERVED;
    if (strcmp(value, "interposed") == 0) return YAI_RECEIPT_GUARANTEE_INTERPOSED;
    if (strcmp(value, "carrier_owned") == 0) return YAI_RECEIPT_GUARANTEE_CARRIER_OWNED;
    if (strcmp(value, "embedded") == 0) return YAI_RECEIPT_GUARANTEE_EMBEDDED;
    if (strcmp(value, "external_import") == 0) return YAI_RECEIPT_GUARANTEE_EXTERNAL_IMPORT;
    return YAI_RECEIPT_GUARANTEE_UNKNOWN;
}
