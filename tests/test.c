#include "basic/basic_tests.h"
#include "cmocka_incl.h"
#include "data_tests.h"
#include "log_tests.h"
#include <alds/alds.h>
#include <stdio.h>
#include <stdlib.h>

static void * alds_malloc_default_local(size_t size);
static void * alds_calloc_default_local(size_t size);
static void * alds_realloc_default_local(void * ptr, size_t new_size);
static void alds_free_default_local(void * ptr);

static alds_alloc_t alloc = {.alds_malloc_cb = alds_malloc_default_local,
                             .alds_calloc_cb = alds_calloc_default_local,
                             .alds_realloc_cb = alds_realloc_default_local,
                             .alds_free_cb = alds_free_default_local};

static void * alds_malloc_default_local(size_t size) {
    return test_malloc(size);
}

static void * alds_calloc_default_local(size_t size) {
    return test_calloc(size, 1);
}

static void * alds_realloc_default_local(void * ptr, size_t new_size) {
    return test_realloc(ptr, new_size);
}

static void alds_free_default_local(void * ptr) {
    test_free(ptr);
}

static void alds_dummy_log_cb(__attribute__((__unused__)) const char * const msg,
                              __attribute__((__unused__)) va_list va) {}

int main(int argc, char * argv[]) {
    uint32_t result = 0;

    // set custom allocators to be able to detect memory leaks
    result |= alds_alloc_default_set(&alloc);
    if (result != e_alds_err_success) {
        printf("Custom memory allocation init failed:\n");
        return -1;
    }

    printf("\nUnit tests:\n");
    result |= log_tests();
    // suppress error log messages in unit tests
    alds_set_log_cb(alds_dummy_log_cb);

    result |= data_tests();

    // Restore custom allocators after data_tests
    result |= alds_alloc_default_set(&alloc);
    if (result != e_alds_err_success) {
        printf("Custom memory allocation init failed:\n");
        return -1;
    }

    result |= stack_tests();
    result |= queue_tests();
    result |= linked_list_tests();

    if (0 != result) {
        printf("\n\nERROR!!! One or more tests have failed\n");
    }

    return result;
}
