#include <assert.h>
#include <stdio.h>

#include "yai/yai.h"

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static void append_record(yai_journal_t *journal,
                          const yai_case_ref_t *case_ref,
                          const yai_subject_ref_t *subject_ref,
                          yai_record_kind_t kind,
                          const char *record_id,
                          const char *summary) {
    yai_store_record_t record;
    require_ok(yai_store_record_init(&record,
                                     record_id,
                                     case_ref,
                                     kind,
                                     subject_ref,
                                     0,
                                     0,
                                     0,
                                     summary));
    require_ok(yai_journal_append(journal, &record));
}

int main(void) {
    yai_case_ref_t case_ref;
    yai_subject_ref_t filesystem_ref;
    yai_subject_ref_t model_ref;
    yai_subject_ref_t terminal_ref;
    yai_case_domain_t domain;
    yai_case_attachment_t attachment;
    yai_case_binding_t binding;
    yai_store_record_t record;
    yai_journal_t journal;
    yai_case_session_t session;

    require_ok(yai_case_ref_init(&case_ref,
                                 "case:new18b-context",
                                 "case_context",
                                 "open"));
    require_ok(yai_subject_ref_init(&filesystem_ref,
                                    "subject:filesystem-sandbox",
                                    "filesystem",
                                    "sandbox://new18b"));
    require_ok(yai_subject_ref_init(&model_ref,
                                    "subject:llm-provider",
                                    "model_provider",
                                    "provider:local-openai-compatible"));
    require_ok(yai_subject_ref_init(&terminal_ref,
                                    "subject:linenoise-terminal",
                                    "terminal_interface",
                                    "linenoise:local"));
    require_ok(yai_case_domain_init(&domain,
                                    &case_ref,
                                    YAI_CASE_DOMAIN_OPERATIONAL,
                                    "filesystem_sandbox",
                                    "case://root",
                                    "manual-validation",
                                    "filesystem-loop"));
    require_ok(yai_case_attachment_init(&attachment,
                                        &case_ref,
                                        &filesystem_ref,
                                        YAI_CASE_ATTACHMENT_RUNTIME_SUBJECT,
                                        YAI_CASE_POSTURE_BOUND,
                                        "attachment:filesystem-sandbox",
                                        "asset:filesystem-sandbox",
                                        "trace:new18b"));
    require_ok(yai_case_binding_init(&binding,
                                     &case_ref,
                                     YAI_CASE_BINDING_MODEL,
                                     YAI_CASE_POSTURE_BOUND,
                                     "binding:llm-provider-model-context",
                                     "subject:llm-provider",
                                     "trace:new18b"));

    require_ok(yai_journal_init(&journal));
    require_ok(yai_case_domain_record_init(&record, "record:domain", &domain));
    require_ok(yai_journal_append(&journal, &record));
    require_ok(yai_case_attachment_record_init(&record, "record:attachment", &attachment));
    require_ok(yai_journal_append(&journal, &record));
    require_ok(yai_case_binding_record_init(&record, "record:binding", &binding));
    require_ok(yai_journal_append(&journal, &record));

    append_record(&journal,
                  &case_ref,
                  &filesystem_ref,
                  YAI_RECORD_SUBJECT_BINDING,
                  "record:filesystem-subject",
                  "subject:filesystem-sandbox bound");
    append_record(&journal,
                  &case_ref,
                  &model_ref,
                  YAI_RECORD_SUBJECT_BINDING,
                  "record:model-subject",
                  "subject:llm-provider bound role:model_provider");
    append_record(&journal,
                  &case_ref,
                  &terminal_ref,
                  YAI_RECORD_SUBJECT_BINDING,
                  "record:terminal-subject",
                  "subject:linenoise-terminal bound role:terminal_interface");
    append_record(&journal,
                  &case_ref,
                  &model_ref,
                  YAI_RECORD_PROJECTION_RULE,
                  "record:model-projection-rule",
                  "projection_rule:model-context-only subject:llm-provider");
    append_record(&journal,
                  &case_ref,
                  &model_ref,
                  YAI_RECORD_AUTHORITY_SCOPE,
                  "record:model-authority",
                  "authority_scope:model subject:llm-provider may:claim_proposal no:carrier_effects");
    append_record(&journal,
                  &case_ref,
                  &terminal_ref,
                  YAI_RECORD_AUTHORITY_SCOPE,
                  "record:terminal-authority",
                  "authority_scope:terminal subject:linenoise-terminal may:display_submit_input no:execution_authority");
    append_record(&journal,
                  &case_ref,
                  &filesystem_ref,
                  YAI_RECORD_FILESYSTEM_RECEIPT,
                  "record:fs-receipt",
                  "fs:write status:executed sandbox:inside");
    append_record(&journal,
                  &case_ref,
                  &model_ref,
                  YAI_RECORD_PROJECTION_RESULT,
                  "record:model-projection",
                  "consumer:model kind:model_context redaction:summary_only freshness:fresh");

    require_ok(yai_case_session_open(&session,
                                     "session:new18b-context",
                                     "journal:in-memory",
                                     &case_ref,
                                     &model_ref,
                                     &journal));
    assert(yai_case_session_is_active(&session));
    assert(session.context.case_domain_count == 1);
    assert(session.context.case_attachment_count == 1);
    assert(session.context.case_binding_count == 1);
    assert(session.context.authority_scope_count == 2);
    assert(session.context.projection_result_count == 1);
    assert(session.context.receipt_count == 1);

    printf("case_session:active\n");
    printf("case_world:loaded\n");
    printf("case_context:%s\n", yai_case_context_state_string(session.context.state));
    printf("domain:%s\n", domain.scope_id);
    printf("attachment:filesystem_sandbox bound\n");
    printf("authority:model interpretation_only\n");
    printf("authority:terminal display_only\n");
    printf("filesystem loop completed\n");
    printf("records persisted with case_ref\n");

    yai_journal_free(&journal);
    return 0;
}
