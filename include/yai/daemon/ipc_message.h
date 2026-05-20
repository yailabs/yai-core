#ifndef YAI_DAEMON_IPC_MESSAGE_H
#define YAI_DAEMON_IPC_MESSAGE_H

#include "yai/base/error.h"

typedef enum yai_daemon_ipc_request_type {
    YAI_DAEMON_IPC_STATUS = 0,
    YAI_DAEMON_IPC_INFO = 1,
    YAI_DAEMON_IPC_SHUTDOWN = 2,
    YAI_DAEMON_IPC_UNKNOWN = 3
} yai_daemon_ipc_request_type_t;

typedef struct yai_daemon_ipc_request {
    yai_daemon_ipc_request_type_t type;
} yai_daemon_ipc_request_t;

yai_status_t yai_daemon_ipc_request_parse(const char *line,
                                          yai_daemon_ipc_request_t *request);
const char *yai_daemon_ipc_request_name(yai_daemon_ipc_request_type_t type);

#endif
