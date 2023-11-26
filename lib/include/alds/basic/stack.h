#ifndef ALDS_SIMPLE_STACK_H
#define ALDS_SIMPLE_STACK_H

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
} alds_stack_t;

alds_err_t alds_stack_init(alds_stack_t * ctx, alds_memmngr_t * memmngr,
                           size_t item_quantity, size_t item_size);
void alds_stack_deinit(alds_stack_t * ctx);

alds_err_t alds_stack_push(alds_stack_t * ctx, const void * data);
alds_err_t alds_stack_pop(alds_stack_t * ctx, void * data);

#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_STACK_H
