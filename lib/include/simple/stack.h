#ifndef ALDS_SIMPLE_STACK_H
#define ALDS_SIMPLE_STACK_H

#include <alds_error.h>
#include <alds_data.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    alds_data_t buff;
    size_t item_size;
    size_t items_quantity;
} alds_stack_t;

alds_err_t alds_stack_init(alds_stack_t * ctx, size_t items, size_t item_size);
alds_err_t alds_stack_init_external(alds_stack_t * ctx, const alds_data_t * buff, size_t item_size);
void alds_stack_deinit(alds_stack_t * ctx);

alds_err_t alds_stack_push(alds_stack_t * ctx, const void * data);
alds_err_t alds_stack_pop(alds_stack_t * ctx, void * data);


#ifdef __cplusplus
}
#endif

#endif // ALDS_SIMPLE_STACK_H
