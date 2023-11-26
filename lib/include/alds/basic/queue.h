#ifndef ALDS_SIMPLE_QUEUE_H
#define ALDS_SIMPLE_QUEUE_H

#include <alds/alds_error.h>
#include <alds/data/alds_memmngr.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    alds_memmngr_t mem_mngr;
    void * buff;
    size_t buff_size;
    size_t item_size;
    size_t items_quantity;
    size_t items_quantity_curr;
    size_t item_index_head;
    size_t item_index_tail;
} alds_queue_t;

alds_err_t alds_queue_init(alds_queue_t * ctx, alds_memmngr_t * memmngr,
                           size_t items_quantity, size_t item_size);
void alds_queue_deinit(alds_queue_t * ctx);

alds_err_t alds_queue_push(alds_queue_t * ctx, const void * data);
alds_err_t alds_queue_pop(alds_queue_t * ctx, void * data);

#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_QUEUE_H