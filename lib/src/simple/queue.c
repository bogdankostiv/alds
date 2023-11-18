#include "simple/queue.h"
#include "alds_memory.h"
#include "alds_log.h"
#include <string.h>
#include <stdbool.h>

#define LOG_MODULE_NAME "queue.c"

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
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ALDS_DATA_INIT_DYNAMIC(buff, items * item_size);
    if (!is_data_valid(&buff)) {
        ALDS_LOG_ERROR_ALLOC(LOG_MODULE_NAME);
        return e_alds_err_memalloc;
    }

    ctx->buff = buff;
    ctx->item_size = item_size;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = items;
    ctx->items_quantity_curr = 0;

    return e_alds_err_success;
}

alds_err_t alds_queue_init_external(alds_queue_t * ctx, const alds_data_t * buff, size_t item_size) {
    if (NULL == ctx || !is_data_valid(buff) || 0 == item_size) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    ctx->buff = *buff;
    ctx->item_size = item_size;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = ctx->buff.size / item_size;
    ctx->items_quantity_curr = 0;

    return e_alds_err_success;
}

void alds_queue_deinit(alds_queue_t * ctx) {
    if (NULL == ctx) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return;
    }

    ALDS_DATA_FREE(ctx->buff);
    ctx->item_size = 0;
    ctx->item_index_head = 0;
    ctx->item_index_tail = 0;
    ctx->items_quantity = 0;
    ctx->items_quantity_curr = 0;
}

alds_err_t alds_queue_push(alds_queue_t * ctx, const void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (is_full(ctx)) {
        ALDS_LOG_ERROR_FULL(LOG_MODULE_NAME);
        return e_alds_err_full;
    }

    ctx->items_quantity_curr++;
    memcpy(ctx->buff.ptr + ctx->item_index_head * ctx->item_size, data, ctx->item_size);
    ctx->item_index_head = next_item_index(ctx, ctx->item_index_head);

    return e_alds_err_success;
}

alds_err_t alds_queue_pop(alds_queue_t * ctx, void * data) {
    if (NULL == ctx || NULL == data) {
        ALDS_LOG_ERROR_NULL_ARG(LOG_MODULE_NAME);
        return e_alds_err_arg;
    }

    if (is_empty(ctx)) {
        ALDS_LOG_ERROR_EMPTY(LOG_MODULE_NAME);
        return e_alds_err_empty;
    }

    ctx->items_quantity_curr--;
    memcpy(data, ctx->buff.ptr + ctx->item_index_tail * ctx->item_size, ctx->item_size);
    ctx->item_index_tail = next_item_index(ctx, ctx->item_index_tail);

    return e_alds_err_success;
}
