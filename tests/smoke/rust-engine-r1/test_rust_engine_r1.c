#include <assert.h>
#include <stdio.h>

#include "yai/yai.h"

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static void append_json(yai_rust_engine_t *engine, const char *line) {
    require_ok(yai_rust_engine_append_record_json(engine, line));
}

int main(void) {
    char summary[256];
    yai_rust_engine_t *engine = yai_rust_engine_create();

    assert(engine != 0);
    printf("rust-engine:r1 handle created\n");

    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:case\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"case\",\"subject_ref\":\"subject:none\",\"attempt_id\":\"\",\"decision_id\":\"\",\"receipt_id\":\"\",\"summary\":\"case:new10-rust opened\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:binding\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"subject_binding\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"\",\"decision_id\":\"\",\"receipt_id\":\"\",\"summary\":\"subject:repo-test bound\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:attempt\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"attempt\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"op:rust-engine\",\"decision_id\":\"\",\"receipt_id\":\"\",\"summary\":\"op:fs.write mutative\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:decision\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"decision\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"op:rust-engine\",\"decision_id\":\"decision:require-review\",\"receipt_id\":\"\",\"summary\":\"decision:require_review\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:receipt\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"receipt\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"op:rust-engine\",\"decision_id\":\"decision:require-review\",\"receipt_id\":\"receipt:blocked-rust\",\"summary\":\"receipt:blocked\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:edge\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"graph_edge\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"op:rust-engine\",\"decision_id\":\"decision:require-review\",\"receipt_id\":\"receipt:blocked-rust\",\"summary\":\"edge:receipt_records_effect\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"record:memory\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"memory_candidate\",\"subject_ref\":\"subject:repo-test\",\"attempt_id\":\"op:rust-engine\",\"decision_id\":\"decision:require-review\",\"receipt_id\":\"receipt:blocked-rust\",\"summary\":\"memory:decision\"}");
    append_json(engine, "{\"schema\":\"yai.store.record.v0\",\"record_id\":\"projection-result:operator\",\"case_ref\":\"case:new10-rust\",\"record_kind\":\"projection_result\",\"subject_ref\":\"subject:none\",\"attempt_id\":\"\",\"decision_id\":\"\",\"receipt_id\":\"\",\"summary\":\"projection_result:operator consumer:operator\"}");

    assert(yai_rust_engine_record_count(engine) == 8);
    assert(yai_rust_engine_count_kind(engine, "receipt") == 1);
    assert(yai_rust_engine_query_kind(engine, "memory_candidate") == 1);
    require_ok(yai_rust_engine_projection_summary_json(engine, summary, sizeof(summary)));
    assert(summary[0] == '{');

    printf("rust-engine:records appended:%zu\n", yai_rust_engine_record_count(engine));
    printf("rust-engine:kind receipt count:%zu\n", yai_rust_engine_count_kind(engine, "receipt"));
    printf("rust-engine:query memory_candidate matched:%zu\n", yai_rust_engine_query_kind(engine, "memory_candidate"));
    printf("rust-engine:projection summary built\n");

    yai_rust_engine_destroy(engine);
    return 0;
}
