#ifndef ALDS_ALLOC_H
#define ALDS_ALLOC_H

#include <alds/alds_error.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void * (*alds_malloc_cb_t)(size_t size);
typedef void * (*alds_calloc_cb_t)(size_t size);
typedef void * (*alds_realloc_cb_t)(void * ptr, size_t new_size);
typedef void (*alds_free_cb_t)(void ** ptr);

typedef struct {
    alds_malloc_cb_t alds_malloc_cb;
    alds_calloc_cb_t alds_calloc_cb;
    alds_realloc_cb_t alds_realloc_cb;
    alds_free_cb_t alds_free_cb;
} alds_alloc_t;

#define ALDS_ALLOC_PTR_IS_VALID(p)                                             \
    (NULL != (p)->alds_malloc_cb && NULL != (p)->alds_calloc_cb &&             \
     NULL != (p)->alds_realloc_cb && NULL != (p)->alds_free_cb)

/**
 * @brief Default memory allocators initialisation. The library uses
 * malloc/calloc/calloc/free from C standard library by default.
 */
alds_err_t alds_alloc_default_set(const alds_alloc_t * cb);

/**
 * @brief Get default memory allocators.
 */
const alds_alloc_t * const alds_alloc_default_get(void);

/**
 * @brief Reset default memory management into the initial state
 * (malloc/calloc/calloc/free usage).
 */
void alds_alloc_default_reset(void);

/**
 * @brief Custom malloc/calloc/calloc/free
 */
void * alds_malloc_custom(const alds_alloc_t * cb, size_t size);
void * alds_calloc_custom(const alds_alloc_t * cb, size_t size);
void * alds_realloc_custom(const alds_alloc_t * cb, void * ptr,
                           size_t new_size);
void alds_free_custom(const alds_alloc_t * cb, void ** ptr);

/**
 * @brief Default malloc/calloc/calloc/free
 */
void * alds_malloc(size_t size);
void * alds_calloc(size_t size);
void * alds_realloc(void * ptr, size_t new_size);
void alds_free(void ** ptr);

#ifdef __cplusplus
}
#endif

#endif // ALDS_ALLOC_H
