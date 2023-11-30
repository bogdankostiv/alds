#include "alds/data/alds_alloc.h"
#include "alds/alds_log.h"

#define LOG_MODULE_NAME "alds_alloc"

static void * alds_malloc_default(size_t size);
static void * alds_calloc_default(size_t size);
static void * alds_realloc_default(void * ptr, size_t new_size);
static void alds_free_default(void * ptr);

static alds_alloc_t default_alloc = {.alds_malloc_cb = alds_malloc_default,
                                     .alds_calloc_cb = alds_calloc_default,
                                     .alds_realloc_cb = alds_realloc_default,
                                     .alds_free_cb = alds_free_default};

alds_err_t alds_alloc_default_set(const alds_alloc_t * alloc) {
    if (NULL == alloc || NULL == alloc->alds_malloc_cb || NULL == alloc->alds_calloc_cb ||
        NULL == alloc->alds_realloc_cb || NULL == alloc->alds_free_cb) {
        ALDS_LOG_ERROR(LOG_MODULE_NAME, "At least one of init callbacks is NULL");
        return e_alds_err_arg;
    }

    default_alloc.alds_malloc_cb = alloc->alds_malloc_cb;
    default_alloc.alds_calloc_cb = alloc->alds_calloc_cb;
    default_alloc.alds_realloc_cb = alloc->alds_realloc_cb;
    default_alloc.alds_free_cb = alloc->alds_free_cb;

    return e_alds_err_success;
}

const alds_alloc_t * alds_alloc_default_get(void) {
    return &default_alloc;
}

void alds_alloc_default_reset(void) {
    default_alloc.alds_malloc_cb = alds_malloc_default;
    default_alloc.alds_calloc_cb = alds_calloc_default;
    default_alloc.alds_realloc_cb = alds_realloc_default;
    default_alloc.alds_free_cb = alds_free_default;
}

void * alds_malloc_custom(const alds_alloc_t * alloc, size_t size) {
    if (NULL != alloc->alds_malloc_cb) {
        return alloc->alds_malloc_cb(size);
    } else {
        ALDS_LOG_ERROR(LOG_MODULE_NAME, "alds_malloc failed");
        return NULL;
    }
}

void * alds_calloc_custom(const alds_alloc_t * alloc, size_t size) {
    if (NULL != alloc->alds_malloc_cb) {
        return alloc->alds_calloc_cb(size);
    } else {
        ALDS_LOG_ERROR(LOG_MODULE_NAME, "alds_calloc failed");
        return NULL;
    }
}

void * alds_realloc_custom(const alds_alloc_t * alloc, void * ptr, size_t new_size) {
    if (NULL != alloc->alds_malloc_cb) {
        return alloc->alds_realloc_cb(ptr, new_size);
    } else {
        ALDS_LOG_ERROR(LOG_MODULE_NAME, "alds_realloc failed");
        return NULL;
    }
}

void alds_free_custom(const alds_alloc_t * alloc, void * ptr) {
    if (NULL != alloc->alds_malloc_cb) {
        alloc->alds_free_cb(ptr);
    } else {
        ALDS_LOG_ERROR(LOG_MODULE_NAME, "alds_free_cb failed");
    }
}

void * alds_malloc(size_t size) {
    return alds_malloc_custom(alds_alloc_default_get(), size);
}

void * alds_calloc(size_t size) {
    return alds_calloc_custom(alds_alloc_default_get(), size);
}

void * alds_realloc(void * ptr, size_t new_size) {
    return alds_realloc_custom(alds_alloc_default_get(), ptr, new_size);
}

void alds_free(void * ptr) {
    alds_free_custom(alds_alloc_default_get(), ptr);
}

static void * alds_malloc_default(size_t size) {
    return malloc(size);
}

static void * alds_calloc_default(size_t size) {
    return calloc(size, 1);
}

static void * alds_realloc_default(void * ptr, size_t new_size) {
    return realloc(ptr, new_size);
}

static void alds_free_default(void * ptr) {
    free(ptr);
}
