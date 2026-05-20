#ifndef YAI_DAEMON_IPC_H
#define YAI_DAEMON_IPC_H

#include "yai/base/error.h"

typedef struct yai_daemon_ipc_server {
    int listen_fd;
    int should_stop;
    char socket_path[256];
} yai_daemon_ipc_server_t;

yai_status_t yai_daemon_ipc_server_init(yai_daemon_ipc_server_t *server,
                                        const char *socket_path);
yai_status_t yai_daemon_ipc_server_run(yai_daemon_ipc_server_t *server);
void yai_daemon_ipc_server_close(yai_daemon_ipc_server_t *server);

#endif
