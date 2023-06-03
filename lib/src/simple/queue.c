#include "simple/queue.h"
#include "alds_memmgmt.h"
#include <string.h>
#include <stdbool.h>

static bool is_empty(alds_queue_t * ctx);
static bool is_full(alds_queue_t * ctx);
static size_t next_item_index(alds_queue_t * ctx, size_t item_index);

static inline bool is_empty(alds_queue_t * ctx) {
    return 0 == ctx->items_quantity_curr;
}

static inline bool is_full(alds_queue_t * ctx) {
    return (ctx->items_quantity_curr + 1) > ctx->items_quantity;
}

static inline size_t next_item_index(alds_queue_t * ctx, size_t item_index) {
    return (item_index + 1) >= ctx->items_quantity ? 0 : item_index + 1;
}

alds_err_t alds_queue_init(alds_queue_t * ctx, size_t items, size_t item_size) {
    if (NULL == ctx || 0 == items || 0 == item_size) {
        return e_alds_err_arg;
    }

    ctx->is_static = false;
    ctx->item_size = item_size;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = items;
    ctx->items_quantity_curr = 0;
    ctx->buffer_size = items * item_size;
    ctx->buffer = alds_malloc(ctx->buffer_size);
    if (NULL == ctx->buffer) {
        return e_alds_err_memalloc;
    }

    return e_alds_err_success;
}

alds_err_t alds_queue_init_static(alds_queue_t * ctx, uint8_t * buffer, size_t buffer_size, size_t item_size) {
    if (NULL == ctx || NULL == buffer || 0 == buffer_size || 0 == item_size) {
        return e_alds_err_arg;
    }

    ctx->is_static = true;
    ctx->buffer_size = buffer_size;
    ctx->buffer = buffer;
    ctx->item_size = item_size;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = buffer_size / item_size;
    ctx->items_quantity_curr = 0;

    return e_alds_err_success;
}

void alds_queue_deinit(alds_queue_t * ctx) {
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
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = 0;
    ctx->items_quantity_curr = 0;
}

alds_err_t alds_queue_push(alds_queue_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        return e_alds_err_arg;
    }

    if (is_full(ctx)) {
        return e_alds_err_full;
    }

    ctx->items_quantity_curr++;
    memcpy(ctx->buffer + ctx->item_index_head * ctx->item_size, data, ctx->item_size);
    ctx->item_index_head = next_item_index(ctx, ctx->item_index_head);

    return e_alds_err_success;
}

alds_err_t alds_queue_pop(alds_queue_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        return e_alds_err_arg;
    }

    if (is_empty(ctx)) {
        return e_alds_err_empty;
    }

    ctx->items_quantity_curr--;
    memcpy(data, ctx->buffer + ctx->item_index_tail * ctx->item_size, ctx->item_size);
    ctx->item_index_tail = next_item_index(ctx, ctx->item_index_tail);

    return e_alds_err_success;
}
