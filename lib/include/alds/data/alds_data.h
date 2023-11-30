#ifndef ALDS_DATA_H
#define ALDS_DATA_H

#include <alds/data/alds_alloc.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Universal structure for data.
 */
typedef struct {
    void * ptr;             /**< Buffer pointer */
    size_t size;            /**< Buffer size in bytes */
    alds_free_cb_t free_cb; /**< Optional free callback, should be NULL if
                               de-allocation is not needed */
} alds_data_t;

/**
 * @brief Macro to declare local buffer
 */
#define ALDS_DATA_INIT_LOCAL(data_name, data_size)                                                                     \
    uint8_t data_name##buff[data_size];                                                                                \
    alds_data_t data_name = {.ptr = (void *)data_name##buff, .size = data_size, .free_cb = NULL}

/**
 * @brief Macro to declare static buffer
 */
#define ALDS_DATA_INIT_STATIC(data_name, data_size)                                                                    \
    static uint8_t data_name##buff[data_size];                                                                         \
    static alds_data_t data_name = {.ptr = (void *)data_name##buff, .size = data_size, .free_cb = NULL}

/**
 * @brief Macro to declare custom allocated buffer
 */
#define ALDS_DATA_INIT_CUSTOM(data_name, data_ptr, data_size, free_cb_in)                                              \
    alds_data_t data_name = {.ptr = data_ptr, .size = data_size, .free_cb = free_cb_in}

/**
 * @brief Free buffer via pointer
 */
#define ALDS_DATA_PTR_FREE(data_name_ptr)                                                                              \
    if (NULL != (data_name_ptr)) {                                                                                     \
        if (NULL != (data_name_ptr)->free_cb) {                                                                        \
            ((data_name_ptr)->free_cb)((data_name_ptr)->ptr);                                                          \
        }                                                                                                              \
        (data_name_ptr)->ptr = NULL;                                                                                   \
        (data_name_ptr)->size = 0;                                                                                     \
        (data_name_ptr)->free_cb = NULL;                                                                               \
    }

/**
 * @brief Free buffer
 */
#define ALDS_DATA_FREE(data_name)    ALDS_DATA_PTR_FREE(&(data_name))

#define ALDS_DATA_PTR_IS_VALID(data) ((NULL == (data) || NULL == (data)->ptr || 0 == (data)->size) ? false : true)

#ifdef __cplusplus
}
#endif

#endif // ALDS_DATA_H