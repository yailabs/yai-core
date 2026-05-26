#ifndef YAI_NET_RECEIPT_H
#define YAI_NET_RECEIPT_H

/*
 * NET transports receipts. YAI decides whether transported receipts become
 * authoritative record material.
 */
typedef enum yai_net_receipt_transport_kind {
    YAI_NET_RECEIPT_TRANSPORT_CHUNK = 0,
    YAI_NET_RECEIPT_TRANSPORT_FINAL = 1,
    YAI_NET_RECEIPT_TRANSPORT_ERROR = 2
} yai_net_receipt_transport_kind_t;

#endif
