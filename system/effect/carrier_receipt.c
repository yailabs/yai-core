/*
 * YAI - Carrier receipt posture
 *
 * Implements:
 *   carrier.v1 receipt posture initialization and filesystem effect mapping.
 *
 * This file owns:
 *   Minimal carrier receipt posture derivation from existing effect receipts.
 *
 * This file does not own:
 *   Journal records, LMDB writes, carrier execution or receipt replay.
 */

#include "yai/effect/carrier_receipt.h"

#include "yai/effect/carrier_contract.h"

#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static yai_carrier_outcome_t outcome_from_receipt_status(yai_receipt_status_t status) {
    switch (status) {
    case YAI_RECEIPT_EXECUTED:
        return YAI_CARRIER_OUTCOME_EXECUTED;
    case YAI_RECEIPT_OBSERVED:
        return YAI_CARRIER_OUTCOME_OBSERVED;
    case YAI_RECEIPT_BLOCKED:
        return YAI_CARRIER_OUTCOME_BLOCKED;
    case YAI_RECEIPT_FAILED:
        return YAI_CARRIER_OUTCOME_FAILED;
    case YAI_RECEIPT_IMPORTED:
        return YAI_CARRIER_OUTCOME_OBSERVED;
    case YAI_RECEIPT_UNKNOWN:
        return YAI_CARRIER_OUTCOME_UNKNOWN;
    default:
        return YAI_CARRIER_OUTCOME_UNKNOWN;
    }
}

yai_status_t yai_carrier_receipt_init(yai_carrier_receipt_t *receipt) {
    if (receipt == 0) {
        return YAI_ERR_INVALID;
    }
    memset(receipt, 0, sizeof(*receipt));
    receipt->carrier_family = YAI_CARRIER_FAMILY_UNKNOWN;
    yai_copy_string(receipt->carrier_name, sizeof(receipt->carrier_name), "unknown");
    yai_copy_string(receipt->contract_version, sizeof(receipt->contract_version), YAI_CARRIER_CONTRACT_V1);
    receipt->outcome = YAI_CARRIER_OUTCOME_UNKNOWN;
    receipt->guarantee_mode = YAI_RECEIPT_GUARANTEE_UNKNOWN;
    return YAI_OK;
}

yai_status_t yai_carrier_receipt_from_effect_receipt(
    const yai_effect_receipt_t *effect_receipt,
    yai_carrier_receipt_t *out
) {
    if (effect_receipt == 0 || out == 0) {
        return YAI_ERR_INVALID;
    }
    if (yai_carrier_receipt_init(out) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (effect_receipt->carrier_kind == YAI_CARRIER_FILESYSTEM) {
        out->carrier_family = YAI_CARRIER_FAMILY_FILESYSTEM;
        yai_copy_string(out->carrier_name, sizeof(out->carrier_name), "filesystem");
        out->guarantee_mode = YAI_RECEIPT_GUARANTEE_INTERPOSED;
    }
    out->attempt_id = effect_receipt->attempt_id;
    out->decision_id = effect_receipt->decision_id;
    out->receipt_id = effect_receipt->receipt_id;
    out->outcome = outcome_from_receipt_status(effect_receipt->status);
    out->pre_state_observed = 1;
    out->post_state_observed = effect_receipt->status == YAI_RECEIPT_EXECUTED ||
                               effect_receipt->status == YAI_RECEIPT_OBSERVED ||
                               effect_receipt->status == YAI_RECEIPT_BLOCKED;
    out->evidence_captured = effect_receipt->summary[0] != '\0';
    out->receipt_validated = effect_receipt->receipt_id.value[0] != '\0' &&
                             out->outcome != YAI_CARRIER_OUTCOME_UNKNOWN;
    out->residue_recorded = 1;
    return YAI_OK;
}
