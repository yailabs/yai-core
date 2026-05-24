/*
 * YAI - Carrier contract
 *
 * Purpose:
 *   Defines carrier.v1 phases and filesystem carrier contract metadata.
 *
 * Ownership:
 *   Carrier contract version and phase vocabulary.
 *
 * Boundary:
 *   This file does not execute carriers or validate receipts by itself.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_CONTRACT_H
#define YAI_EFFECT_CARRIER_CONTRACT_H

#define YAI_CARRIER_CONTRACT_V1 "carrier.v1"

typedef enum yai_carrier_contract_phase {
    YAI_CARRIER_PHASE_PRE_STATE_OBSERVATION = 0,
    YAI_CARRIER_PHASE_CONTROLLED_EXECUTION_OR_OBSERVATION = 1,
    YAI_CARRIER_PHASE_POST_STATE_OBSERVATION = 2,
    YAI_CARRIER_PHASE_EVIDENCE_CAPTURE = 3,
    YAI_CARRIER_PHASE_RECEIPT_ASSEMBLY = 4,
    YAI_CARRIER_PHASE_RECEIPT_VALIDATION = 5,
    YAI_CARRIER_PHASE_RESIDUE_RECORDING = 6,
    YAI_CARRIER_PHASE_UNKNOWN = 7
} yai_carrier_contract_phase_t;

const char *yai_carrier_contract_version(void);
const char *yai_carrier_contract_phase_to_string(yai_carrier_contract_phase_t phase);

#endif
