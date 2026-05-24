/*
 * YAI - Receipt guarantee mode
 *
 * Purpose:
 *   Names how strongly YAI can attest a receipt or evidence source.
 *
 * Ownership:
 *   Receipt guarantee enum and string conversion ABI.
 *
 * Boundary:
 *   This file does not create receipts and receipt is not logging.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_RECEIPT_GUARANTEE_H
#define YAI_EFFECT_RECEIPT_GUARANTEE_H

typedef enum yai_receipt_guarantee_mode {
    YAI_RECEIPT_GUARANTEE_OBSERVED = 0,
    YAI_RECEIPT_GUARANTEE_INTERPOSED = 1,
    YAI_RECEIPT_GUARANTEE_CARRIER_OWNED = 2,
    YAI_RECEIPT_GUARANTEE_EMBEDDED = 3,
    YAI_RECEIPT_GUARANTEE_EXTERNAL_IMPORT = 4,
    YAI_RECEIPT_GUARANTEE_UNKNOWN = 5
} yai_receipt_guarantee_mode_t;

const char *yai_receipt_guarantee_mode_to_string(yai_receipt_guarantee_mode_t mode);
yai_receipt_guarantee_mode_t yai_receipt_guarantee_mode_from_string(const char *value);

#endif
