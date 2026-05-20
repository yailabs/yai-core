#ifndef YAI_EFFECT_CARRIER_H
#define YAI_EFFECT_CARRIER_H

typedef enum yai_carrier_kind {
    YAI_CARRIER_FILESYSTEM = 0,
    YAI_CARRIER_UNKNOWN = 1
} yai_carrier_kind_t;

typedef enum yai_effect_kind {
    YAI_EFFECT_FS_READ = 0,
    YAI_EFFECT_FS_WRITE = 1,
    YAI_EFFECT_UNKNOWN = 2
} yai_effect_kind_t;

const char *yai_carrier_kind_string(yai_carrier_kind_t kind);
const char *yai_effect_kind_string(yai_effect_kind_t kind);

#endif
