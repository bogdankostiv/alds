#include "simple/stack.h"
#include "alds.h"
#include "alds_log.h"
#include <string.h>

#define LOG_MODULE_NAME "stack.c"

alds_err_t alds_stack_init(alds_stack_t * ctx, size_t items, size_t item_size) {
    if (NULL == ctx || 0 == items || 0 == item_size) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ALDS_DATA_INIT_DYNAMIC(buff, items * item_size);
    if (!is_data_valid(&buff)) {
        ALDS_LOG_ERROR_ALLOC(LOG_MODULE_NAME);
        return e_alds_err_memalloc;
    }
    
    ctx->item_size = item_size;
    ctx->items_quantity = 0;
    ctx->buff = buff;

    return e_alds_err_success;
}

alds_err_t alds_stack_init_external(alds_stack_t * ctx, const alds_data_t * buff, size_t item_size) {
    if (NULL == ctx || !is_data_valid(buff) || 0 == item_size) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ctx->buff = *buff;
    ctx->item_size = item_size;
    ctx->items_quantity = 0;

    return e_alds_err_success;
}

void alds_stack_deinit(alds_stack_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return;
    }

    ALDS_DATA_FREE(ctx->buff);
    ctx->item_size = 0;
    ctx->items_quantity = 0;
}

alds_err_t alds_stack_push(alds_stack_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if ((ctx->items_quantity + 1) * ctx->item_size > ctx->buff.size) {
        ALDS_LOG_ERROR_FULL(LOG_MODULE_NAME);
        return e_alds_err_full;
    }

    memcpy(ctx->buff.ptr + ctx->items_quantity * ctx->item_size, data, ctx->item_size);
    ctx->items_quantity++;

    return e_alds_err_success;
}

alds_err_t alds_stack_pop(alds_stack_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (0 == ctx->items_quantity) {
        ALDS_LOG_ERROR_EMPTY(LOG_MODULE_NAME);
        return e_alds_err_empty;
    }

    ctx->items_quantity--;
    memcpy(data, ctx->buff.ptr + ctx->items_quantity * ctx->item_size, ctx->item_size);

    return e_alds_err_success;
}
