#ifndef ALDS_LOG_H
#define ALDS_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdarg.h>

#define ALDS_LOG_LEVEL_INFO       0
#define ALDS_LOG_LEVEL_WARNING    1
#define ALDS_LOG_LEVEL_ERROR      2

// Set default logging level. 
#if !defined(ALDS_LOG_LEVEL)
    #define ALDS_LOG_LEVEL ALDS_LOG_LEVEL_WARNING
#endif

/**
 * @brief Info level logging
 */
#if defined(VERBOSE) && ALDS_LOG_LEVEL <= ALDS_LOG_LEVEL_INFO
    #define ALDS_LOG_INFO(module, ...)      alds_log(ALDS_LOG_LEVEL_INFO, module, __VA_ARGS__)
#else 
    #define ALDS_LOG_INFO(module, ...) 
#endif

/**
 * @brief Warning level logging
 */
#if defined(VERBOSE) && ALDS_LOG_LEVEL <= ALDS_LOG_LEVEL_WARNING
    #define ALDS_LOG_WARNING(module, ...)  alds_log(ALDS_LOG_LEVEL_WARNING, module, __VA_ARGS__)
#else 
    #define ALDS_LOG_WARNING(module, ...) 
#endif

/**
 * @brief Error level logging
 */
#if defined(VERBOSE) && ALDS_LOG_LEVEL <= ALDS_LOG_LEVEL_ERROR
    #define ALDS_LOG_ERROR(module, ...)    alds_log(ALDS_LOG_LEVEL_ERROR, module, __VA_ARGS__)
#else 
    #define ALDS_LOG_ERROR(module, ...) 
#endif


#define ALDS_LOG_ERROR_NULL_ARG(module)     ALDS_LOG_ERROR(module, "Fn %s, line %d: NULL or invalid arg(s)\n", __func__, __LINE__)
#define ALDS_LOG_ERROR_ALLOC(module)        ALDS_LOG_ERROR(module, "Fn %s, line %d: allocation failed\n", __func__, __LINE__)
#define ALDS_LOG_ERROR_FULL(module)         ALDS_LOG_ERROR(module, "Fn %s, line %d: full\n", __func__, __LINE__)
#define ALDS_LOG_ERROR_EMPTY(module)        ALDS_LOG_ERROR(module, "Fn %s, line %d: empty\n", __func__, __LINE__)


/**
 * @brief Callback data type for custom logging implementation
*/
typedef void (*alds_log_cb_t)(const char * const, va_list);

/**
 * @brief Clear custom logging, stdio logging will be used by default
 */
void alds_clear_log_cb(void);

/**
 * @brief Set custom logging call back
 * @param cb - custom callback
 */
void alds_set_log_cb(alds_log_cb_t cb);

/**
 * @brief Log function. It is recommended to use macros instead of direct call to exclude unused log strings
 * @param log_level - Log level
 * @param module - Pointer on a string with the name of module
 * @param fmt - Format string with following variadic parameters
 */
void alds_log(uint32_t log_level, const char * const module, const char * const fmt, ... );

#ifdef __cplusplus
}
#endif

#endif // ALDS_LOG_H