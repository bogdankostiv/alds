#ifndef ALDS_SIMPLE_LINKED_LIST_H
#define ALDS_SIMPLE_LINKED_LIST_H

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

#define LL_GET_DATA(data_type, data)

typedef struct {
    alds_ll_t * head;
    bool is_circular; //**< Tail's next points on the head and vice versa
    bool is_sentinel; //**< The head data is NULL, it is used as a marker
} alds_ll_ctx_t;

alds_err_t alds_ll_init(alds_ll_ctx_t * ctx, bool is_circular,
                        bool is_sentinel);
void alds_ll_deinit(alds_ll_ctx_t * ctx);

// prepend (head, data)
// insert (item, data)
// delete (item)

// next
// prev

#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_LINKED_LIST_H