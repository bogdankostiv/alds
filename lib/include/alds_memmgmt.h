#ifndef ALDS_MEMMGMNT_H
#define ALDS_MEMMGMNT_H

#include <stdlib.h>
#include <alds_error.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void * (*alds_malloc_cb_t)(size_t size);
typedef void * (*alds_calloc_cb_t)(size_t size);
typedef void * (*alds_realloc_cb_t)(void *  ptr, size_t new_size);
typedef void (*alds_free_cb_t)(void ** ptr);

typedef struct {
    alds_malloc_cb_t alds_malloc_cb;
    alds_calloc_cb_t alds_calloc_cb;
    alds_realloc_cb_t alds_realloc_cb;
    alds_free_cb_t alds_free_cb;
} alds_memgmnt_t;

/**
 * @brief Custom memory management initialisation. The library uses malloc/calloc/calloc/free from C standard library by default.
*/
alds_err_t alds_init_memmgmnt(const alds_memgmnt_t * cb);

/**
 * @brief Reset custom memory management into initial state (malloc/calloc/calloc/free usage).
*/
void alds_reset_memmgmnt(void);

void * alds_malloc(size_t size);
void * alds_calloc(size_t size);
void * alds_realloc(void *  ptr, size_t new_size);
void alds_free(void ** ptr);

#ifdef __cplusplus
}
#endif

#endif // ALDS_MEMMGMNT_H
