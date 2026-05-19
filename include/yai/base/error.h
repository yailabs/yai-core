#ifndef YAI_BASE_ERROR_H
#define YAI_BASE_ERROR_H

typedef enum yai_status {
    YAI_OK = 0,
    YAI_ERR_INVALID = 1,
    YAI_ERR_NOT_FOUND = 2,
    YAI_ERR_BUFFER_TOO_SMALL = 3,
    YAI_ERR_FORBIDDEN = 4,
    YAI_ERR_UNSUPPORTED = 5
} yai_status_t;

const char *yai_status_string(yai_status_t status);

#endif
