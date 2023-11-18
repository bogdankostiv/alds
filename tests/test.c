#include <stdlib.h>
#include <stdio.h>
#include "alds.h"
#include "log_tests.h"
#include "simple/simple_tests.h"
#include "cmocka_incl.h"

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

static void * alds_malloc_default(size_t size) {
    return test_malloc(size);
}

static void * alds_calloc_default(size_t size) {
    return test_calloc(size, 1);
}

static void * alds_realloc_default(void *  ptr, size_t new_size) {
    return test_realloc(ptr, new_size);
}

static void alds_free_default(void ** ptr) {
    test_free(*ptr);
    *ptr = NULL;
}

/* A test case that does nothing and succeeds. */
static void canary_test(void ** state) {
    (void) state; /* unused */
    assert_int_equal(0, 0);
}

static void alds_buff_static_test(void ** state) {
    (void) state; /* unused */

    ALDS_DATA_INIT_STATIC(buffer, 10);

    assert_non_null(buffer.ptr);
    assert_int_equal(buffer.size, 10);
    assert_null(buffer.free_cb);

    ((uint8_t *)buffer.ptr)[9] = 100;
    assert_int_equal(((uint8_t *)buffer.ptr)[9], 100);

    ALDS_DATA_FREE(buffer);
    assert_null(buffer.ptr);
    assert_int_equal(buffer.size, 0);
    assert_null(buffer.free_cb);

    ALDS_DATA_INIT_STATIC(buffer1, 10);
    ALDS_DATA_PTR_FREE(&buffer1);
    assert_null(buffer1.ptr);
    assert_int_equal(buffer1.size, 0);
    assert_null(buffer1.free_cb);
}

static void alds_buff_dynamic_test(void ** state) {
    (void) state; /* unused */

    ALDS_DATA_INIT_DYNAMIC(buffer, 10);

    assert_non_null(buffer.ptr);
    assert_int_equal(buffer.size, 10);
    assert_non_null(buffer.free_cb);

    ((uint8_t *)buffer.ptr)[9] = 100;
    assert_int_equal(((uint8_t *)buffer.ptr)[9], 100);

    ALDS_DATA_FREE(buffer);
    assert_null(buffer.ptr);
    assert_int_equal(buffer.size, 0);
    assert_null(buffer.free_cb);

    ALDS_DATA_INIT_DYNAMIC(buffer1, 10);
    ALDS_DATA_PTR_FREE(&buffer1);
    assert_null(buffer1.ptr);
    assert_int_equal(buffer1.size, 0);
    assert_null(buffer1.free_cb);
}

static void alds_dummy_log_cb(__attribute__((__unused__)) const char * const msg, 
                                __attribute__((__unused__)) va_list va){
}

int main(int argc, char * argv[]) {
    int result = 0;

    result = alds_memory_init(&mem_cb);
    if (result != e_alds_err_success) {
        printf("Custom memory allocation init failed:\n");
        return -1;
    }

    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(canary_test),
        cmocka_unit_test(alds_buff_static_test),
        cmocka_unit_test(alds_buff_dynamic_test)
    };

    printf("Unit tests:\n");
    result |= cmocka_run_group_tests(unit_tests, NULL, NULL);

    result |= log_tests();

    // suppress error log messages in unit tests
    alds_set_log_cb(alds_dummy_log_cb);

    result |= stack_tests();
    result |= queue_tests();

    alds_clear_log_cb();


    if (0 != result) {
        printf("\n\nERROR!!! One or more tests have failed\n");
    }

    return result;
}