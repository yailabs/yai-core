#ifndef YAI_DAEMON_DAEMON_STATUS_H
#define YAI_DAEMON_DAEMON_STATUS_H

#include <stddef.h>

#include "yai/base/error.h"

typedef struct yai_daemon_status {
    int ok;
    char version[32];
    char status[32];
    char message[128];
} yai_daemon_status_t;

yai_status_t yai_daemon_status_init(yai_daemon_status_t *status,
                                    const char *state,
                                    const char *message);
yai_status_t yai_daemon_status_json(const yai_daemon_status_t *status,
                                    char *buffer,
                                    size_t buffer_size);

#endif
