#ifndef ALDS_SIMPLE_QUEUE_H
#define ALDS_SIMPLE_QUEUE_H

#include <alds_error.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    bool is_static;
    uint8_t * buffer;
    size_t buffer_size;
    size_t item_size;
    size_t items_quantity;
    size_t items_quantity_curr;
    size_t item_index_head;
    size_t item_index_tail;
} alds_queue_t;

alds_err_t alds_queue_init(alds_queue_t * ctx, size_t items, size_t item_size);
alds_err_t alds_queue_init_static(alds_queue_t * ctx, uint8_t * buffer, size_t buffer_size, size_t item_size);
void alds_queue_deinit(alds_queue_t * ctx);

alds_err_t alds_queue_push(alds_queue_t * ctx, const void * data);
alds_err_t alds_queue_pop(alds_queue_t * ctx, void * data);


#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_QUEUE_H