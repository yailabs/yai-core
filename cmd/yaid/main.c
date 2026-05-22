#include <stdio.h>
#include <string.h>

#include "yai/yai.h"

static const char *arg_value(int argc, char **argv, const char *name) {
    int index = 1;
    while (index + 1 < argc) {
        if (strcmp(argv[index], name) == 0) {
            return argv[index + 1];
        }
        index += 1;
    }
    return 0;
}

static int has_arg(int argc, char **argv, const char *name) {
    int index = 1;
    while (index < argc) {
        if (strcmp(argv[index], name) == 0) {
            return 1;
        }
        index += 1;
    }
    return 0;
}

static int print_status_json(const char *state, const char *message) {
    yai_daemon_status_t status;
    char buffer[512];

    if (yai_daemon_status_init(&status, state, message) != YAI_OK ||
        yai_daemon_status_json(&status, buffer, sizeof(buffer)) != YAI_OK) {
        return 2;
    }
    fputs(buffer, stdout);
    return 0;
}

static void print_usage(void) {
    puts("usage: yaid [--version|--once status|--socket <path> --foreground]");
}

int main(int argc, char **argv) {
    const char *socket_path = 0;
    yai_daemon_ipc_server_t server;
    yai_status_t status;

    if (argc == 1) {
        print_usage();
        return 0;
    }
    if (has_arg(argc, argv, "--version")) {
        printf("yaid %s\n", YAI_CORE_VERSION);
        return 0;
    }
    if (has_arg(argc, argv, "--once")) {
        const char *request = arg_value(argc, argv, "--once");
        if (request != 0 && strcmp(request, "status") == 0) {
            return print_status_json("ok", "yaid one-shot status ok");
        }
        if (request != 0 && strcmp(request, "info") == 0) {
            return print_status_json("ok", "yaid NEW.12 daemon-backed core loop v0");
        }
        fprintf(stderr, "unsupported --once request\n");
        return 2;
    }
    socket_path = arg_value(argc, argv, "--socket");
    if (socket_path == 0) {
        fprintf(stderr, "--socket <path> is required for daemon mode\n");
        return 2;
    }
    status = yai_daemon_ipc_server_init(&server, socket_path);
    if (status != YAI_OK) {
        fprintf(stderr, "failed to start ipc server: %s\n", yai_status_string(status));
        return 2;
    }
    status = yai_daemon_ipc_server_run(&server);
    yai_daemon_ipc_server_close(&server);
    return status == YAI_OK ? 0 : 2;
}
