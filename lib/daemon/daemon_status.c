#include "yai/daemon/daemon_status.h"

#include <stdio.h>
#include <string.h>

#include "yai/yai.h"

static void copy_text(char *dst, size_t dst_size, const char *src) {
    if (dst == 0 || dst_size == 0) {
        return;
    }
    (void)snprintf(dst, dst_size, "%s", src == 0 ? "" : src);
}

yai_status_t yai_daemon_status_init(yai_daemon_status_t *status,
                                    const char *state,
                                    const char *message) {
    if (status == 0) {
        return YAI_ERR_INVALID;
    }
    memset(status, 0, sizeof(*status));
    status->ok = 1;
    copy_text(status->version, sizeof(status->version), YAI_CORE_VERSION);
    copy_text(status->status, sizeof(status->status), state == 0 ? "ok" : state);
    copy_text(status->message, sizeof(status->message), message == 0 ? "yaid local ipc ready" : message);
    return YAI_OK;
}

yai_status_t yai_daemon_status_json(const yai_daemon_status_t *status,
                                    char *buffer,
                                    size_t buffer_size) {
    int written = 0;

    if (status == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }
    written = snprintf(buffer,
                       buffer_size,
                       "{\"ok\":%s,\"version\":\"%s\",\"status\":\"%s\",\"message\":\"%s\"}\n",
                       status->ok ? "true" : "false",
                       status->version,
                       status->status,
                       status->message);
    if (written < 0 || (size_t)written >= buffer_size) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}
