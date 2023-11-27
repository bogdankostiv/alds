#include "alds/basic/stack.h"
#include "alds/alds.h"
#include "alds/alds_log.h"
#include <string.h>

#define LOG_MODULE_NAME "stack.c"

alds_err_t alds_stack_init(alds_stack_t * ctx, alds_memmngr_t * memmngr, size_t item_quantity, size_t item_size) {
    if (NULL == ctx || NULL == memmngr || !alds_memmngr_is_valid(memmngr) || 0 == item_quantity || 0 == item_size) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ctx->mem_mngr = *memmngr;

    ctx->buff = alds_memmngr_buffer_get(&ctx->mem_mngr, item_quantity * item_size);
    if (NULL == ctx->buff) {
        ALDS_LOG_ERROR_ALLOC(LOG_MODULE_NAME);
        return e_alds_err_memalloc;
    }
    ctx->buff_size = item_quantity * item_size;

    ctx->item_size = item_size;
    ctx->items_quantity = 0;
    return e_alds_err_success;
}

void alds_stack_deinit(alds_stack_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return;
    }

    alds_memmngr_buffer_release(&ctx->mem_mngr, &ctx->buff);

    ctx->item_size = 0;
    ctx->items_quantity = 0;
}

alds_err_t alds_stack_push(alds_stack_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if ((ctx->items_quantity + 1) * ctx->item_size > ctx->buff_size) {
        ALDS_LOG_ERROR_FULL(LOG_MODULE_NAME);
        return e_alds_err_full;
    }

    memcpy(ctx->buff + ctx->items_quantity * ctx->item_size, data, ctx->item_size);
    ctx->items_quantity++;

    return e_alds_err_success;
}

alds_err_t alds_stack_pop(alds_stack_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (0 == ctx->items_quantity) {
        ALDS_LOG_ERROR_EMPTY(LOG_MODULE_NAME);
        return e_alds_err_empty;
    }

    ctx->items_quantity--;
    memcpy(data, ctx->buff + ctx->items_quantity * ctx->item_size, ctx->item_size);

    return e_alds_err_success;
}
