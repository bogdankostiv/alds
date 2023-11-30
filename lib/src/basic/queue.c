#include "alds/basic/queue.h"
#include "alds/alds_log.h"
#include "alds/data/alds_memmngr.h"
#include <stdbool.h>
#include <string.h>

#define LOG_MODULE_NAME "queue_dynamic.c"

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

alds_err_t alds_queue_init(alds_queue_t * ctx, alds_memmngr_t * memmngr, size_t items_quantity, size_t item_size) {
    if (NULL == ctx || NULL == memmngr || !alds_memmngr_is_valid(memmngr) || 0 == items_quantity || 0 == item_size) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ctx->mem_mngr = *memmngr;
    ctx->buff = alds_memmngr_buffer_get(&ctx->mem_mngr, items_quantity * item_size);
    if (NULL == ctx->buff) {
        ALDS_LOG_ERROR_ALLOC(LOG_MODULE_NAME);
        return e_alds_err_memalloc;
    }
    ctx->buff_size = items_quantity * item_size;

    ctx->item_size = item_size;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = items_quantity;
    ctx->items_quantity_curr = 0;

    return e_alds_err_success;
}

void alds_queue_deinit(alds_queue_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return;
    }

    alds_memmngr_buffer_release(&ctx->mem_mngr, ctx->buff);

    ctx->item_size = 0;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = 0;
    ctx->items_quantity_curr = 0;
}

alds_err_t alds_queue_push(alds_queue_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (is_full(ctx)) {
        ALDS_LOG_ERROR_FULL(LOG_MODULE_NAME);
        return e_alds_err_full;
    }

    ctx->items_quantity_curr++;
    memcpy(ctx->buff + ctx->item_index_head * ctx->item_size, data, ctx->item_size);
    ctx->item_index_head = next_item_index(ctx, ctx->item_index_head);

    return e_alds_err_success;
}

alds_err_t alds_queue_pop(alds_queue_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_INVALID_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (is_empty(ctx)) {
        ALDS_LOG_ERROR_EMPTY(LOG_MODULE_NAME);
        return e_alds_err_empty;
    }

    ctx->items_quantity_curr--;
    memcpy(data, ctx->buff + ctx->item_index_tail * ctx->item_size, ctx->item_size);
    ctx->item_index_tail = next_item_index(ctx, ctx->item_index_tail);

    return e_alds_err_success;
}
