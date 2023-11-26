#include <alds/alds_log.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static alds_log_cb_t custom_print_cb = NULL;

static void alds_log_default(const char * const fmt, va_list argp);
static void alds_log_formatted(uint32_t log_level, const char * const module, const char * const fmt, va_list argp);

void alds_clear_log_cb(void) {
    custom_print_cb = NULL;
}

void alds_set_log_cb(alds_log_cb_t cb) {
    if (NULL != cb) {
        custom_print_cb = cb;
    }
}

void alds_log(uint32_t log_level, const char * const module, const char * const fmt, ... ) {
    if (NULL == module || NULL == fmt) {
        // we can't do anything here
        return;
    }

    va_list argp;
	va_start(argp, fmt);
	alds_log_formatted(log_level, module, fmt, argp);
	va_end(argp);
}

static void alds_log_default(const char * const fmt, va_list argp) {
    vfprintf(stdout, fmt, argp);
}

static void alds_log_formatted(uint32_t log_level, const char * const module, const char * const fmt, va_list argp) {
    uint8_t ext_len = 40;
    char new_fmt[strlen(fmt) + ext_len + 1];
    
    char * level_str = log_level == ALDS_LOG_LEVEL_INFO ? "ALDS INFO [" :
                            log_level == ALDS_LOG_LEVEL_WARNING ? "ALDS WARNING [" :
                            log_level == ALDS_LOG_LEVEL_ERROR ? "ALDS ERROR [" :
                            "ALDS UNKNOWN [";
    strcpy(new_fmt, level_str);
    ext_len -= strlen(level_str);

    if (strlen(module) <= ext_len - 3) { // 3 extra symbols for "]: "
        strcpy(new_fmt + strlen(new_fmt), module);
        strcpy(new_fmt + strlen(new_fmt), "]: ");
    } else {
        size_t new_len = strlen(new_fmt) + ext_len - 6;
        strncpy(new_fmt + strlen(new_fmt), module, ext_len - 6); 
        new_fmt[new_len] = '\0';
        strcpy(new_fmt + strlen(new_fmt), "...]: ");
    }

    strcpy(new_fmt + strlen(new_fmt), fmt);

    if (NULL == custom_print_cb) {
        alds_log_default(new_fmt, argp);
    } else {
        (custom_print_cb)(new_fmt, argp);
    }
}