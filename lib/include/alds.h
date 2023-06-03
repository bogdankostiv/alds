#ifndef ALDS_H
#define ALDS_H

#include <stdlib.h>
#include <alds_error.h>
#include <alds_memmgmt.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Macro to declare static buffer
 */
#define STATIC_BUFFER(buffer_name, items, item_size)     static uint8_t buffer_name[(items) * (item_size)]

/**
 * @brief Macro to declare a buffer. Be careful with on-stack initialsiation. It will be invalidated
 * in case of exiting fro a function where it was initialised.
 */
#define BUFFER(buffer_name, items, item_size)            uint8_t buffer_name[(items) * (item_size)]


#ifdef __cplusplus
}
#endif

#endif // ALDS_H
