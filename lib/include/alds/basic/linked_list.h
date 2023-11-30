#ifndef ALDS_SIMPLE_LINKED_LIST_H
#define ALDS_SIMPLE_LINKED_LIST_H

#include <alds/data/alds_alloc.h>
#include <alds/data/alds_data.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct alds_ll {
    struct alds_ll * next;
    struct alds_ll * prev;
    uint8_t data[];
} alds_ll_t;

#define ALDS_LL_DATA_PTR(data_type, ll_item) ((data_type)(&(ll_item)->data[0]))

typedef struct {
    alds_ll_t * head;
    const alds_alloc_t * alloc;
    size_t data_size;
    alds_free_cb_t data_free_cb;
    bool is_circular;
} alds_ll_ctx_t;

alds_err_t alds_ll_init(alds_ll_ctx_t * ctx, const alds_alloc_t * alloc, size_t data_size,
                        alds_free_cb_t data_free_cb_____, bool is_circular);
void alds_ll_deinit(alds_ll_ctx_t * ctx);

alds_err_t alds_ll_prepend(alds_ll_ctx_t * ctx, void * data);
alds_err_t alds_ll_insert(alds_ll_ctx_t * ctx, alds_ll_t * item, void * data);
alds_err_t alds_ll_delete(alds_ll_ctx_t * ctx, alds_ll_t * item);

alds_ll_t * alds_ll_head(alds_ll_ctx_t * ctx);
alds_ll_t * alds_ll_next(alds_ll_t * item);
alds_ll_t * alds_ll_prev(alds_ll_t * item);

#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_LINKED_LIST_H