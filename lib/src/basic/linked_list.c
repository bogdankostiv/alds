#include "alds/basic/linked_list.h"
#include "alds/alds_helpers.h"
#include "alds/alds_log.h"
#include <string.h>

#define MODULE_NAME "linked_list.c"

alds_err_t alds_ll_init(alds_ll_ctx_t * ctx, const alds_alloc_t * alloc, size_t data_size, alds_free_cb_t data_free_cb,
                        bool is_circular) {
    if (NULL == ctx || NULL == alloc || !ALDS_ALLOC_PTR_IS_VALID(alloc) || 0 == data_size) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return e_alds_err_arg;
    }

    ctx->alloc = alloc;
    ctx->is_circular = is_circular;
    ctx->data_size = data_size;
    ctx->data_free_cb = data_free_cb;
    ctx->head = NULL;

    return e_alds_err_success;
}

void alds_ll_deinit(alds_ll_ctx_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return;
    }

    while (NULL != ctx->head) {
        alds_err_t err = alds_ll_delete(ctx, ctx->head);
        if (e_alds_err_success != err) {
            ALDS_LOG_ERROR_GENERAL(MODULE_NAME, "alds_ll_delete failed");
            break;
        }
    }

    ctx->alloc = NULL;
    ctx->data_size = 0;
}

alds_err_t alds_ll_delete(alds_ll_ctx_t * ctx, alds_ll_t * item) {
    if (NULL == ctx || NULL == item) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return e_alds_err_arg;
    }

    alds_ll_t * del = item;

    // check if it's the last item of circular LL, clear head
    if (ctx->head == del && ctx->is_circular && ctx->head->next == ctx->head) {
        ctx->head = NULL;
    }

    if (NULL != alds_ll_prev(del)) {
        alds_ll_prev(del)->next = del->next;
    }
    if (NULL != alds_ll_next(del)) {
        alds_ll_next(del)->prev = del->prev;
    }

    if (ctx->head == item) {
        ctx->head = item->next;
    }

    if (NULL != ctx->data_free_cb) {
        void * user_data = ALDS_LL_DATA_PTR(void *, del);
        (ctx->data_free_cb)(user_data);
    }

    ctx->alloc->alds_free_cb((void *)del);

    return e_alds_err_success;
}

alds_err_t alds_ll_prepend(alds_ll_ctx_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return e_alds_err_arg;
    }

    alds_ll_t * insert_item = ctx->alloc->alds_calloc_cb(ctx->data_size + OFFSETOF(alds_ll_t, data));
    if (NULL == insert_item) {
        ALDS_LOG_ERROR_ALLOC(MODULE_NAME);
        return e_alds_err_memalloc;
    }
    memcpy(insert_item->data, data, ctx->data_size);

    if (NULL == ctx->head) {
        ctx->head = insert_item;
        if (ctx->is_circular) {
            ctx->head->next = ctx->head;
            ctx->head->prev = ctx->head;
        }
    } else {
        if (NULL != alds_ll_prev(ctx->head)) {
            alds_ll_prev(ctx->head)->next = insert_item;
        }
        insert_item->next = ctx->head;
        insert_item->prev = ctx->head->prev;
        ctx->head->prev = insert_item;

        ctx->head = insert_item;
    }

    return e_alds_err_success;
}

alds_err_t alds_ll_insert(alds_ll_ctx_t * ctx, alds_ll_t * item, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return e_alds_err_arg;
    }

    if (NULL == item) {
        return alds_ll_prepend(ctx, data);
    }

    alds_ll_t * insert_item = ctx->alloc->alds_calloc_cb(ctx->data_size + OFFSETOF(alds_ll_t, data));
    if (NULL == insert_item) {
        ALDS_LOG_ERROR_ALLOC(MODULE_NAME);
        return e_alds_err_memalloc;
    }
    memcpy(insert_item->data, data, ctx->data_size);

    if (NULL != alds_ll_next(item)) {
        alds_ll_next(item)->prev = insert_item;
    }
    insert_item->next = item->next;
    insert_item->prev = item;
    item->next = insert_item;

    return e_alds_err_success;
}

alds_ll_t * alds_ll_head(alds_ll_ctx_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_INVALID_ARG(MODULE_NAME);
        return NULL;
    }

    return ctx->head;
}

alds_ll_t * alds_ll_next(alds_ll_t * item) {
    return NULL == item ? NULL : item->next;
}

alds_ll_t * alds_ll_prev(alds_ll_t * item) {
    return NULL == item ? NULL : item->prev;
}