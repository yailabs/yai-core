/*
 * YAI - NET receipt transport vocabulary
 *
 * Purpose:
 *   Names receipt transport events carried by NET streams.
 *
 * Ownership:
 *   Public NET receipt transport kind enum.
 *
 * Boundary:
 *   NET transports receipts; YAI decides whether transported receipts become
 *   authoritative record material.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_RECEIPT_H
#define YAI_NET_RECEIPT_H

typedef enum yai_net_receipt_transport_kind {
    YAI_NET_RECEIPT_TRANSPORT_CHUNK = 0,
    YAI_NET_RECEIPT_TRANSPORT_FINAL = 1,
    YAI_NET_RECEIPT_TRANSPORT_ERROR = 2
} yai_net_receipt_transport_kind_t;

#endif
