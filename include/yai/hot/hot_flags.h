#ifndef YAI_HOT_HOT_FLAGS_H
#define YAI_HOT_HOT_FLAGS_H

typedef enum yai_hot_dirty_flag {
    YAI_HOT_DIRTY_RECORD = 1u << 0,
    YAI_HOT_DIRTY_RECEIPT = 1u << 1,
    YAI_HOT_DIRTY_GRAPH = 1u << 2,
    YAI_HOT_DIRTY_MEMORY = 1u << 3,
    YAI_HOT_DIRTY_PROJECTION = 1u << 4,
    YAI_HOT_DIRTY_AUTHORITY = 1u << 5,
    YAI_HOT_DIRTY_THREAD = 1u << 6,
    YAI_HOT_DIRTY_OBLIGATION = 1u << 7,
    YAI_HOT_DIRTY_CARRIER = 1u << 8
} yai_hot_dirty_flag_t;

#endif
