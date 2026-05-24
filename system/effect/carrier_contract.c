/*
 * YAI - Carrier contract
 *
 * Implements:
 *   carrier.v1 contract version and phase vocabulary.
 *
 * This file owns:
 *   Static contract vocabulary for controlled carrier realization.
 *
 * This file does not own:
 *   Carrier execution, receipt persistence or runtime queues.
 */

#include "yai/effect/carrier_contract.h"

const char *yai_carrier_contract_version(void) {
    return YAI_CARRIER_CONTRACT_V1;
}

const char *yai_carrier_contract_phase_to_string(yai_carrier_contract_phase_t phase) {
    switch (phase) {
    case YAI_CARRIER_PHASE_PRE_STATE_OBSERVATION:
        return "pre_state_observation";
    case YAI_CARRIER_PHASE_CONTROLLED_EXECUTION_OR_OBSERVATION:
        return "controlled_execution_or_observation";
    case YAI_CARRIER_PHASE_POST_STATE_OBSERVATION:
        return "post_state_observation";
    case YAI_CARRIER_PHASE_EVIDENCE_CAPTURE:
        return "evidence_capture";
    case YAI_CARRIER_PHASE_RECEIPT_ASSEMBLY:
        return "receipt_assembly";
    case YAI_CARRIER_PHASE_RECEIPT_VALIDATION:
        return "receipt_validation";
    case YAI_CARRIER_PHASE_RESIDUE_RECORDING:
        return "residue_recording";
    case YAI_CARRIER_PHASE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}
