#ifndef YAI_EFFECT_FILESYSTEM_CARRIER_H
#define YAI_EFFECT_FILESYSTEM_CARRIER_H

#include "yai/base/error.h"
#include "yai/control/decision.h"
#include "yai/effect/receipt.h"
#include "yai/op/attempt.h"

yai_status_t yai_filesystem_carrier_read(const char *receipt_id,
                                         const yai_op_attempt_t *attempt,
                                         const yai_control_decision_t *decision,
                                         const char *sandbox_root,
                                         const char *path,
                                         yai_effect_receipt_t *receipt);
yai_status_t yai_filesystem_carrier_write(const char *receipt_id,
                                          const yai_op_attempt_t *attempt,
                                          const yai_control_decision_t *decision,
                                          const char *sandbox_root,
                                          const char *path,
                                          const char *content,
                                          yai_effect_receipt_t *receipt);
int yai_filesystem_path_is_inside_sandbox(const char *sandbox_root,
                                          const char *path);

#endif
