#ifndef YAI_DAEMON_DAEMON_LOOP_H
#define YAI_DAEMON_DAEMON_LOOP_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/daemon/ipc_message.h"

typedef struct yai_daemon_loop_response {
    char request_id[64];
    int ok;
    char status[32];
    char message[128];
    char journal_path[256];
    size_t record_count;
    size_t receipt_count;
    size_t projection_count;
    char decision_outcome[32];
    char receipt_status[32];
    char fs_read[32];
    char fs_write_blocked[32];
    char fs_write_allowed[32];
    char case_session[32];
    char case_world[32];
    char case_context[32];
    char authority_scope[64];
} yai_daemon_loop_response_t;

void yai_daemon_loop_response_init(yai_daemon_loop_response_t *response,
                                   const yai_daemon_ipc_request_t *request);
yai_status_t yai_daemon_loop_response_json(const yai_daemon_loop_response_t *response,
                                           char *buffer,
                                           size_t buffer_size);
yai_status_t yai_daemon_run_minimum_loop(const yai_daemon_ipc_request_t *request,
                                         yai_daemon_loop_response_t *response);
yai_status_t yai_daemon_run_filesystem_loop(const yai_daemon_ipc_request_t *request,
                                            yai_daemon_loop_response_t *response);
yai_status_t yai_daemon_journal_summary(const yai_daemon_ipc_request_t *request,
                                        yai_daemon_loop_response_t *response);
yai_status_t yai_daemon_projection_summary(const yai_daemon_ipc_request_t *request,
                                           yai_daemon_loop_response_t *response);

#endif
