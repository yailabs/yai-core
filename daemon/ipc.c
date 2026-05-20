#include "yai/daemon/ipc.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "yai/daemon/daemon_loop.h"
#include "yai/daemon/daemon_status.h"
#include "yai/daemon/ipc_message.h"

static void copy_socket_path(char *dst, size_t dst_size, const char *src) {
    if (dst == 0 || dst_size == 0) {
        return;
    }
    (void)snprintf(dst, dst_size, "%s", src == 0 ? "" : src);
}

const char *yai_daemon_ipc_request_name(yai_daemon_ipc_request_type_t type) {
    switch (type) {
    case YAI_DAEMON_IPC_STATUS:
        return "status";
    case YAI_DAEMON_IPC_INFO:
        return "info";
    case YAI_DAEMON_IPC_SHUTDOWN:
        return "shutdown";
    case YAI_DAEMON_IPC_RUN_MINIMUM_LOOP:
        return "run_minimum_loop";
    case YAI_DAEMON_IPC_RUN_FILESYSTEM_LOOP:
        return "run_filesystem_loop";
    case YAI_DAEMON_IPC_JOURNAL_SUMMARY:
        return "journal_summary";
    case YAI_DAEMON_IPC_PROJECTION_SUMMARY:
        return "projection_summary";
    default:
        return "unknown";
    }
}

static void copy_request_value(char *dst, size_t dst_size, const char *src, size_t len) {
    size_t copy_len = 0;

    if (dst == 0 || dst_size == 0 || src == 0) {
        return;
    }
    copy_len = len;
    if (copy_len >= dst_size) {
        copy_len = dst_size - 1;
    }
    memcpy(dst, src, copy_len);
    dst[copy_len] = '\0';
}

static void parse_request_field(yai_daemon_ipc_request_t *request,
                                const char *token,
                                size_t token_len) {
    const char *equal = 0;
    size_t key_len = 0;
    const char *value = 0;
    size_t value_len = 0;

    if (request == 0 || token == 0 || token_len == 0) {
        return;
    }
    equal = memchr(token, '=', token_len);
    if (equal == 0) {
        return;
    }
    key_len = (size_t)(equal - token);
    value = equal + 1;
    value_len = token_len - key_len - 1;
    if (key_len == strlen("request_id") &&
        strncmp(token, "request_id", key_len) == 0) {
        copy_request_value(request->request_id, sizeof(request->request_id), value, value_len);
    } else if (key_len == strlen("case_ref") &&
               strncmp(token, "case_ref", key_len) == 0) {
        copy_request_value(request->case_ref, sizeof(request->case_ref), value, value_len);
    } else if (key_len == strlen("subject_ref") &&
               strncmp(token, "subject_ref", key_len) == 0) {
        copy_request_value(request->subject_ref, sizeof(request->subject_ref), value, value_len);
    } else if (key_len == strlen("payload") &&
               strncmp(token, "payload", key_len) == 0) {
        copy_request_value(request->payload, sizeof(request->payload), value, value_len);
    }
}

yai_status_t yai_daemon_ipc_request_parse(const char *line,
                                          yai_daemon_ipc_request_t *request) {
    char token[32];
    size_t len = 0;
    const char *cursor = 0;

    if (line == 0 || request == 0) {
        return YAI_ERR_INVALID;
    }
    memset(request, 0, sizeof(*request));
    request->type = YAI_DAEMON_IPC_UNKNOWN;
    while (line[len] != '\0' && line[len] != '\n' && line[len] != '\r' &&
           line[len] != ' ' && line[len] != '\t' && len + 1 < sizeof(token)) {
        token[len] = line[len];
        len += 1;
    }
    token[len] = '\0';
    if (strcmp(token, "status") == 0) {
        request->type = YAI_DAEMON_IPC_STATUS;
    } else if (strcmp(token, "info") == 0) {
        request->type = YAI_DAEMON_IPC_INFO;
    } else if (strcmp(token, "shutdown") == 0) {
        request->type = YAI_DAEMON_IPC_SHUTDOWN;
    } else if (strcmp(token, "run_minimum_loop") == 0) {
        request->type = YAI_DAEMON_IPC_RUN_MINIMUM_LOOP;
    } else if (strcmp(token, "run_filesystem_loop") == 0) {
        request->type = YAI_DAEMON_IPC_RUN_FILESYSTEM_LOOP;
    } else if (strcmp(token, "journal_summary") == 0) {
        request->type = YAI_DAEMON_IPC_JOURNAL_SUMMARY;
    } else if (strcmp(token, "projection_summary") == 0) {
        request->type = YAI_DAEMON_IPC_PROJECTION_SUMMARY;
    } else {
        return YAI_ERR_UNSUPPORTED;
    }
    cursor = line + len;
    while (*cursor != '\0' && *cursor != '\n' && *cursor != '\r') {
        const char *field_start = 0;
        size_t field_len = 0;
        while (*cursor == ' ' || *cursor == '\t') {
            cursor += 1;
        }
        field_start = cursor;
        while (cursor[field_len] != '\0' && cursor[field_len] != '\n' &&
               cursor[field_len] != '\r' && cursor[field_len] != ' ' &&
               cursor[field_len] != '\t') {
            field_len += 1;
        }
        parse_request_field(request, field_start, field_len);
        cursor += field_len;
    }
    if (request->request_id[0] == '\0') {
        (void)snprintf(request->request_id,
                       sizeof(request->request_id),
                       "request:%s",
                       yai_daemon_ipc_request_name(request->type));
    }
    return YAI_OK;
}

static yai_status_t write_response(int fd, const char *state, const char *message) {
    yai_daemon_status_t status;
    char response[512];
    size_t remaining = 0;
    const char *cursor = response;

    if (yai_daemon_status_init(&status, state, message) != YAI_OK ||
        yai_daemon_status_json(&status, response, sizeof(response)) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    remaining = strlen(response);
    while (remaining > 0) {
        ssize_t written = write(fd, cursor, remaining);
        if (written <= 0) {
            return YAI_ERR_INVALID;
        }
        cursor += written;
        remaining -= (size_t)written;
    }
    return YAI_OK;
}

static yai_status_t write_loop_response(int fd,
                                        const yai_daemon_loop_response_t *loop_response) {
    char response[1024];
    size_t remaining = 0;
    const char *cursor = response;

    if (yai_daemon_loop_response_json(loop_response, response, sizeof(response)) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    remaining = strlen(response);
    while (remaining > 0) {
        ssize_t written = write(fd, cursor, remaining);
        if (written <= 0) {
            return YAI_ERR_INVALID;
        }
        cursor += written;
        remaining -= (size_t)written;
    }
    return YAI_OK;
}

static void handle_loop_request(int client_fd, const yai_daemon_ipc_request_t *request) {
    yai_daemon_loop_response_t response;
    yai_status_t status = YAI_ERR_UNSUPPORTED;

    yai_daemon_loop_response_init(&response, request);
    if (request->type == YAI_DAEMON_IPC_RUN_MINIMUM_LOOP) {
        status = yai_daemon_run_minimum_loop(request, &response);
    } else if (request->type == YAI_DAEMON_IPC_RUN_FILESYSTEM_LOOP) {
        status = yai_daemon_run_filesystem_loop(request, &response);
    } else if (request->type == YAI_DAEMON_IPC_JOURNAL_SUMMARY) {
        status = yai_daemon_journal_summary(request, &response);
    } else if (request->type == YAI_DAEMON_IPC_PROJECTION_SUMMARY) {
        status = yai_daemon_projection_summary(request, &response);
    }
    if (status != YAI_OK) {
        response.ok = 0;
        (void)snprintf(response.status, sizeof(response.status), "error");
        (void)snprintf(response.message,
                       sizeof(response.message),
                       "daemon request failed: %s",
                       yai_status_string(status));
    }
    (void)write_loop_response(client_fd, &response);
}

static void handle_client(yai_daemon_ipc_server_t *server, int client_fd) {
    char request_line[512];
    ssize_t read_len = 0;
    yai_daemon_ipc_request_t request;

    read_len = read(client_fd, request_line, sizeof(request_line) - 1);
    if (read_len <= 0) {
        (void)write_response(client_fd, "error", "empty request");
        return;
    }
    request_line[read_len] = '\0';
    if (yai_daemon_ipc_request_parse(request_line, &request) != YAI_OK) {
        (void)write_response(client_fd, "error", "unsupported request");
        return;
    }
    if (request.type == YAI_DAEMON_IPC_STATUS) {
        (void)write_response(client_fd, "ok", "yaid local ipc ready");
    } else if (request.type == YAI_DAEMON_IPC_INFO) {
        (void)write_response(client_fd, "ok", "yaid NEW.12 daemon-backed core loop v0");
    } else if (request.type == YAI_DAEMON_IPC_SHUTDOWN) {
        (void)write_response(client_fd, "ok", "yaid shutdown accepted");
        server->should_stop = 1;
    } else {
        handle_loop_request(client_fd, &request);
    }
}

yai_status_t yai_daemon_ipc_server_init(yai_daemon_ipc_server_t *server,
                                        const char *socket_path) {
    struct sockaddr_un addr;

    if (server == 0 || socket_path == 0 || socket_path[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    memset(server, 0, sizeof(*server));
    server->listen_fd = -1;
    copy_socket_path(server->socket_path, sizeof(server->socket_path), socket_path);
    if (strlen(socket_path) >= sizeof(addr.sun_path)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    server->listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server->listen_fd < 0) {
        return YAI_ERR_INVALID;
    }
    (void)unlink(socket_path);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    copy_socket_path(addr.sun_path, sizeof(addr.sun_path), socket_path);
    if (bind(server->listen_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0 ||
        listen(server->listen_fd, 8) != 0) {
        yai_daemon_ipc_server_close(server);
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

yai_status_t yai_daemon_ipc_server_run(yai_daemon_ipc_server_t *server) {
    if (server == 0 || server->listen_fd < 0) {
        return YAI_ERR_INVALID;
    }
    while (!server->should_stop) {
        int client_fd = accept(server->listen_fd, 0, 0);
        if (client_fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            return YAI_ERR_INVALID;
        }
        handle_client(server, client_fd);
        (void)close(client_fd);
    }
    return YAI_OK;
}

void yai_daemon_ipc_server_close(yai_daemon_ipc_server_t *server) {
    if (server == 0) {
        return;
    }
    if (server->listen_fd >= 0) {
        (void)close(server->listen_fd);
        server->listen_fd = -1;
    }
    if (server->socket_path[0] != '\0') {
        (void)unlink(server->socket_path);
    }
}
