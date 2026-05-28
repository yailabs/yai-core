/*
 * YAI - NET version vocabulary
 *
 * Purpose:
 *   Names the NET component and current scaffold boundary version.
 *
 * Ownership:
 *   Public NET component macros.
 *
 * Boundary:
 *   Version macros do not claim production transport readiness.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_VERSION_H
#define YAI_NET_VERSION_H

#define YAI_NET_COMPONENT_NAME "net"
#define YAI_NET_COMPONENT_STATUS "c-translation-unit-scaffold"
#define YAI_NET_COMPONENT_VERSION "NET.SPINE.9C.0"

const char *yai_net_component_name(void);
const char *yai_net_component_status(void);
const char *yai_net_component_version(void);

#endif
