/*
 * YAI - Carrier receipt posture
 *
 * Purpose:
 *   Summarizes a carrier.v1 receipt posture for a controlled effect.
 *
 * Ownership:
 *   Carrier receipt posture struct, initialization and effect receipt mapping.
 *
 * Boundary:
 *   This file does not own LMDB records, journal replay or carrier execution.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_RECEIPT_H
#define YAI_EFFECT_CARRIER_RECEIPT_H

#include "yai/base/error.h"
#include "yai/effect/carrier_family.h"
#include "yai/effect/carrier_outcome.h"
#include "yai/effect/receipt.h"
#include "yai/effect/receipt_guarantee.h"

typedef struct yai_carrier_receipt {
    yai_carrier_family_t carrier_family;
    char carrier_name[32];
    char contract_version[16];
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    yai_carrier_outcome_t outcome;
    yai_receipt_guarantee_mode_t guarantee_mode;
    int pre_state_observed;
    int post_state_observed;
    int evidence_captured;
    int receipt_validated;
    int residue_recorded;
} yai_carrier_receipt_t;

yai_status_t yai_carrier_receipt_init(yai_carrier_receipt_t *receipt);
yai_status_t yai_carrier_receipt_from_effect_receipt(
    const yai_effect_receipt_t *effect_receipt,
    yai_carrier_receipt_t *out
);

#endif
