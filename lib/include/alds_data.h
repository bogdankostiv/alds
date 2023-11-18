#ifndef ALDS_DATA_H
#define ALDS_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <alds_memory.h>
#include <stdbool.h>

/**
 * @brief Universal structure for data.
 */
typedef struct {
    void * ptr;                 /**< Buffer pointer */
    size_t size;                /**< Buffer size in bytes */
    alds_free_cb_t free_cb;     /**< Optional free callback, should be NULL if de-alocation is not needed */
} alds_data_t;

/**
 * @brief Macro to declare statically allocated buffer
 */
#define ALDS_DATA_INIT_STATIC(data_name, data_size)    \
    uint8_t data_name##buff[data_size];         \
    alds_data_t data_name = {                   \
        .ptr = (void *) data_name##buff,        \
        .size = data_size,                      \
        .free_cb = NULL                         \
    }

/**
 * @brief Macro to declare dynamically allocated buffer
 */
#define ALDS_DATA_INIT_DYNAMIC(data_name, data_size) \
    alds_data_t data_name = {                   \
        .ptr = alds_malloc(data_size),          \
        .size = data_size,                      \
        .free_cb = alds_free                    \
    }

/**
 * @brief Macro for custom alloction 
 */
#define ALDS_DATA_INIT_CUSTOM(data_name, buff_ptr, data_size, free_cb) \
    alds_data_t data_name = {                   \
        .ptr = buff_ptr,                        \
        .size = data_size,                      \
        .free_cb = free_cb                      \
    }

/**
 * @brief Free buffer via pointer 
 */
#define ALDS_DATA_PTR_FREE(data_name_ptr)                           \
    if (NULL != (data_name_ptr)) {                                  \
        if (NULL != (data_name_ptr)->free_cb) {                     \
            ((data_name_ptr)->free_cb)(&((data_name_ptr)->ptr));    \
        }                                                           \
        (data_name_ptr)->ptr = NULL;                                \
        (data_name_ptr)->size = 0;                                  \
        (data_name_ptr)->free_cb = NULL;                            \
    }

/**
 * @brief Free buffer
 */
#define ALDS_DATA_FREE(data_name)  ALDS_DATA_PTR_FREE(&(data_name))

static inline bool is_data_valid(const alds_data_t * const data) {
    if (NULL == data || NULL == data->ptr || 0 == data->size) {
        return false;
    }

    return true;
}

#ifdef __cplusplus
}
#endif

#endif // ALDS_DATA_H