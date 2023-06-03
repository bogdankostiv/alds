#include "alds_memmgmt.h"

static void * alds_malloc_default(size_t size);
static void * alds_calloc_default(size_t size);
static void * alds_realloc_default(void *  ptr, size_t new_size);
static void alds_free_default(void ** ptr);

static alds_memgmnt_t memgmnt_cb = {
    .alds_malloc_cb = alds_malloc_default,
    .alds_calloc_cb = alds_calloc_default,
    .alds_realloc_cb = alds_realloc_default,
    .alds_free_cb = alds_free_default
};

alds_err_t alds_init_memmgmnt(const alds_memgmnt_t * cb) {
    if (NULL == cb || 
        NULL == cb->alds_malloc_cb || NULL == cb->alds_calloc_cb ||
        NULL == cb->alds_realloc_cb || NULL == cb->alds_free_cb) {
        return e_alds_err_arg;
    }

    memgmnt_cb.alds_malloc_cb = cb->alds_malloc_cb;
    memgmnt_cb.alds_calloc_cb = cb->alds_calloc_cb;
    memgmnt_cb.alds_realloc_cb = cb->alds_realloc_cb;
    memgmnt_cb.alds_free_cb = cb->alds_free_cb;

    return e_alds_err_success;
}

void alds_reset_memmgmnt(void) {
    memgmnt_cb.alds_malloc_cb = alds_malloc_default;
    memgmnt_cb.alds_calloc_cb = alds_calloc_default;
    memgmnt_cb.alds_realloc_cb = alds_realloc_default;
    memgmnt_cb.alds_free_cb = alds_free_default;
}

void * alds_malloc(size_t size) {
    return memgmnt_cb.alds_malloc_cb(size);
}

void * alds_calloc(size_t size) {
    return memgmnt_cb.alds_calloc_cb(size);
}

void * alds_realloc(void *  ptr, size_t new_size) {
    return memgmnt_cb.alds_realloc_cb(ptr, new_size);
}

void alds_free(void ** ptr) {
    memgmnt_cb.alds_free_cb(ptr);
}

static void * alds_malloc_default(size_t size) {
    return malloc(size);
}

static void * alds_calloc_default(size_t size) {
    return calloc(size, 1);
}

static void * alds_realloc_default(void *  ptr, size_t new_size) {
    return realloc(ptr, new_size);
}

static void alds_free_default(void ** ptr) {
    free(*ptr);
    *ptr = NULL;
}
