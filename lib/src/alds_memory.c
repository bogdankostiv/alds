#include "alds_memory.h"
#include "alds_log.h"

static void * alds_malloc_default(size_t size);
static void * alds_calloc_default(size_t size);
static void * alds_realloc_default(void *  ptr, size_t new_size);
static void alds_free_default(void ** ptr);

static alds_memory_t mem_cb = {
    .alds_malloc_cb = alds_malloc_default,
    .alds_calloc_cb = alds_calloc_default,
    .alds_realloc_cb = alds_realloc_default,
    .alds_free_cb = alds_free_default
};

alds_err_t alds_memory_init(const alds_memory_t * cb) {
    if (NULL == cb || 
        NULL == cb->alds_malloc_cb || NULL == cb->alds_calloc_cb ||
        NULL == cb->alds_realloc_cb || NULL == cb->alds_free_cb) {
            ALDS_LOG_ERROR("alds_memory", "At least one of init callbacks is NULL");
        return e_alds_err_arg;
    }

    mem_cb.alds_malloc_cb = cb->alds_malloc_cb;
    mem_cb.alds_calloc_cb = cb->alds_calloc_cb;
    mem_cb.alds_realloc_cb = cb->alds_realloc_cb;
    mem_cb.alds_free_cb = cb->alds_free_cb;

    return e_alds_err_success;
}

void alds_memory_reset(void) {
    mem_cb.alds_malloc_cb = alds_malloc_default;
    mem_cb.alds_calloc_cb = alds_calloc_default;
    mem_cb.alds_realloc_cb = alds_realloc_default;
    mem_cb.alds_free_cb = alds_free_default;
}

void * alds_malloc(size_t size) {
    return mem_cb.alds_malloc_cb(size);
}

void * alds_calloc(size_t size) {
    return mem_cb.alds_calloc_cb(size);
}

void * alds_realloc(void *  ptr, size_t new_size) {
    return mem_cb.alds_realloc_cb(ptr, new_size);
}

void alds_free(void ** ptr) {
    mem_cb.alds_free_cb(ptr);
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
