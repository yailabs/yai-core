/*
 * YAI - NET endpoint vocabulary
 *
 * Purpose:
 *   Names endpoint kinds for future NET communication paths.
 *
 * Ownership:
 *   Public NET endpoint kind enum.
 *
 * Boundary:
 *   Endpoint vocabulary does not connect, listen, bind, discover or approve
 *   operations.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_ENDPOINT_H
#define YAI_NET_ENDPOINT_H

typedef enum yai_net_endpoint_kind {
    YAI_NET_ENDPOINT_LOCAL_PROCESS = 0,
    YAI_NET_ENDPOINT_LOCALHOST_HTTP = 1,
    YAI_NET_ENDPOINT_LOCAL_IPC = 2,
    YAI_NET_ENDPOINT_LAN_HTTP = 3,
    YAI_NET_ENDPOINT_REMOTE_HTTP = 4,
    YAI_NET_ENDPOINT_FUTURE_TRANSPORT = 5
} yai_net_endpoint_kind_t;

#endif
