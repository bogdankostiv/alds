#include "alds/basic//linked_list.h"
#include "alds/alds_log.h"

#define MODULE_NAME "linked_list.c"

static alds_ll_t data;

// alds_err_t alds_ll_init(alds_ll_ctx_t * ctx, bool is_circular, bool
// is_sentinel) {
//     if (NULL == ctx || (is_sentinel && !is_circular)) {
//         ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
//         return e_alds_err_arg;
//     }
//
//     ctx->is_sentinel = is_sentinel;
//     ctx->is_circular = is_circular;
//
//     if (is_sentinel) {
//         ALDS_DATA_INIT_DYNAMIC(head_item, sizeof(alds_ll_t));
//         ctx->head = head_item;
//         alds_ll_t * head = (alds_ll_t *) (ctx->head.ptr);
//         head->data.ptr = NULL;
//         head->data.size = 0;
//         head->data.size = 0;
//     }
// }

// void alds_ll_deinit(alds_ll_ctx_t * ctx) {
//
// }