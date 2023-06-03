#include "simple/stack.h"
#include "alds_memmgmt.h"
#include <string.h>

alds_err_t alds_stack_init(alds_stack_t * ctx, size_t items, size_t item_size) {
    if (NULL == ctx || 0 == items || 0 == item_size) {
        return e_alds_err_arg;
    }

    ctx->is_static = false;
    ctx->item_size = item_size;
    ctx->items_quantity = 0;
    ctx->buffer_size = items * item_size;
    ctx->buffer = alds_malloc(ctx->buffer_size);
    if (NULL == ctx->buffer) {
        return e_alds_err_memalloc;
    }

    return e_alds_err_success;
}

alds_err_t alds_stack_init_static(alds_stack_t * ctx, uint8_t * buffer, size_t buffer_size, size_t item_size) {
    if (NULL == ctx || NULL == buffer || 0 == buffer_size || 0 == item_size) {
        return e_alds_err_arg;
    }

    ctx->buffer_size = buffer_size;
    ctx->buffer = buffer;
    ctx->is_static = true;
    ctx->item_size = item_size;
    ctx->items_quantity = 0;

    return e_alds_err_success;
}

void alds_stack_deinit(alds_stack_t * ctx) {
    if (NULL == ctx) {
        return;
    }

    if (ctx->is_static) {
        ctx->buffer = NULL;
    } else {
        alds_free((void**) &ctx->buffer);
    }
    
    ctx->buffer_size = 0;
    ctx->item_size = 0;
    ctx->items_quantity = 0;
}

alds_err_t alds_stack_push(alds_stack_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        return e_alds_err_arg;
    }

    if ((ctx->items_quantity + 1) * ctx->item_size > ctx->buffer_size) {
        return e_alds_err_full;
    }

    memcpy(ctx->buffer + ctx->items_quantity * ctx->item_size, data, ctx->item_size);
    ctx->items_quantity++;

    return e_alds_err_success;
}

alds_err_t alds_stack_pop(alds_stack_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        return e_alds_err_arg;
    }

    if (0 == ctx->items_quantity) {
        return e_alds_err_empty;
    }

    ctx->items_quantity--;
    memcpy(data, ctx->buffer + ctx->items_quantity * ctx->item_size, ctx->item_size);

    return e_alds_err_success;
}
