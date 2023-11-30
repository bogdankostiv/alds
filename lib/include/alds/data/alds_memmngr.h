#ifndef ALDS_MEMMNGR_H
#define ALDS_MEMMNGR_H

#include <alds/data/alds_alloc.h>
#include <alds/data/alds_data.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Memory management data type
 */
typedef enum {
    eAldsMemMngr_invalid,
    eAldsMemMngr_buffer,
    eAldsMemMngr_alloc,
} alds_memmngr_type_t;

/**
 * @brief Memory management structure
 */
typedef struct {
    alds_memmngr_type_t type;
    union {
        alds_data_t data;
        alds_alloc_t alloc;
    } container;
} alds_memmngr_t;

#define ALDS_MEMMNGR_FROM_BUFFER(d)                                                                                    \
    (alds_memmngr_t) {                                                                                                 \
        .type = eAldsMemMngr_buffer, .container.data = (d)                                                             \
    }

#define ALDS_MEMMNGR_FROM_ALLOCATOR(cb)                                                                                \
    (alds_memmngr_t) {                                                                                                 \
        .type = eAldsMemMngr_alloc, .container.alloc = (cb)                                                            \
    }

static inline bool alds_memmngr_is_valid(const alds_memmngr_t * const memngr) {
    return eAldsMemMngr_buffer == memngr->type  ? ALDS_DATA_PTR_IS_VALID(&memngr->container.data)
           : eAldsMemMngr_alloc == memngr->type ? ALDS_ALLOC_PTR_IS_VALID(&memngr->container.alloc)
                                                : false;
}

static inline void * alds_memmngr_buffer_get(alds_memmngr_t * mngr, size_t size) {
    void * buff = NULL;

    if (mngr->type == eAldsMemMngr_alloc) {
        buff = alds_malloc_custom(&mngr->container.alloc, size);
    } else if (mngr->type == eAldsMemMngr_buffer) {
        if (mngr->container.data.size >= size) {
            buff = mngr->container.data.ptr;
        }
    }

    return buff;
}

static inline void alds_memmngr_buffer_release(alds_memmngr_t * mngr, void * buff) {
    if (!alds_memmngr_is_valid(mngr) || NULL == buff) {
        return;
    }

    if (mngr->type == eAldsMemMngr_alloc) {
        alds_free_custom(&mngr->container.alloc, buff);
    } else if (mngr->type == eAldsMemMngr_buffer) {
        ALDS_DATA_FREE(mngr->container.data);
    }
}

#ifdef __cplusplus
}
#endif

#endif // ALDS_MEMMNGR_H