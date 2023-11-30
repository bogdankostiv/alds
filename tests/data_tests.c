#include "alds/alds.h"
#include "cmocka_incl.h"
#include <stdarg.h>
#include <stdio.h>

static void * alds_malloc_default_local(size_t size);
static void * alds_calloc_default_local(size_t size);
static void * alds_realloc_default_local(void * ptr, size_t new_size);
static void alds_free_default_local(void * ptr);

static alds_alloc_t alloc_local = {.alds_malloc_cb = alds_malloc_default_local,
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

static void test_alloc_default(void ** state) {
    (void)state; /* unused */

    alds_alloc_default_reset();

    const alds_alloc_t * const alloc = alds_alloc_default_get();
    assert_non_null(alloc);
    assert_non_null(alloc->alds_malloc_cb);
    assert_non_null(alloc->alds_calloc_cb);
    assert_non_null(alloc->alds_realloc_cb);
    assert_non_null(alloc->alds_free_cb);

    void * ptr1 = alds_malloc(10);
    assert_non_null(ptr1);

    void * ptr2 = alds_calloc(10);
    assert_non_null(ptr2);
    uint8_t * ptr2_empty[10] = {0};
    assert_memory_equal(ptr2, ptr2_empty, 10);

    void * ptr3 = alds_realloc(ptr1, 15);
    assert_non_null(ptr3);

    alds_free(ptr2);
    alds_free(ptr3);
}

static void test_alloc_default_custom(void ** state) {
    (void)state; /* unused */

    assert_int_equal(alds_alloc_default_set(&alloc_local), e_alds_err_success);

    const alds_alloc_t * const alloc = alds_alloc_default_get();
    assert_non_null(alloc);
    assert_non_null(alloc->alds_malloc_cb);
    assert_non_null(alloc->alds_calloc_cb);
    assert_non_null(alloc->alds_realloc_cb);
    assert_non_null(alloc->alds_free_cb);
    assert_ptr_equal(alloc_local.alds_malloc_cb, alloc->alds_malloc_cb);
    assert_ptr_equal(alloc_local.alds_calloc_cb, alloc->alds_calloc_cb);
    assert_ptr_equal(alloc_local.alds_realloc_cb, alloc->alds_realloc_cb);
    assert_ptr_equal(alloc_local.alds_free_cb, alloc->alds_free_cb);

    void * ptr1 = alds_malloc(10);
    assert_non_null(ptr1);

    void * ptr2 = alds_calloc(10);
    assert_non_null(ptr2);
    uint8_t * ptr2_empty[10] = {0};
    assert_memory_equal(ptr2, ptr2_empty, 10);

    void * ptr3 = alds_realloc(ptr1, 15);
    assert_non_null(ptr3);

    alds_free(ptr2);
    alds_free(ptr3);
}

static void test_alloc_custom(void ** state) {
    (void)state; /* unused */

    alds_alloc_default_reset();

    const alds_alloc_t * const alloc = alds_alloc_default_get();
    assert_non_null(alloc);
    assert_non_null(alloc->alds_malloc_cb);
    assert_non_null(alloc->alds_calloc_cb);
    assert_non_null(alloc->alds_realloc_cb);
    assert_non_null(alloc->alds_free_cb);
    assert_ptr_not_equal(alloc_local.alds_malloc_cb, alloc->alds_malloc_cb);
    assert_ptr_not_equal(alloc_local.alds_calloc_cb, alloc->alds_calloc_cb);
    assert_ptr_not_equal(alloc_local.alds_realloc_cb, alloc->alds_realloc_cb);
    assert_ptr_not_equal(alloc_local.alds_free_cb, alloc->alds_free_cb);

    void * ptr1 = alds_malloc_custom(&alloc_local, 10);
    assert_non_null(ptr1);

    void * ptr2 = alds_calloc_custom(&alloc_local, 10);
    assert_non_null(ptr1);
    uint8_t * ptr2_empty[10] = {0};
    assert_memory_equal(ptr2, ptr2_empty, 10);

    void * ptr3 = alds_realloc_custom(&alloc_local, ptr1, 15);
    assert_non_null(ptr3);

    alds_free_custom(&alloc_local, ptr2);
    alds_free_custom(&alloc_local, ptr3);
}

static void alds_buff_local_test(void ** state) {
    (void)state; /* unused */

    ALDS_DATA_INIT_LOCAL(buffer, 10);

    assert_non_null(buffer.ptr);
    assert_int_equal(buffer.size, 10);
    assert_null(buffer.free_cb);
    assert_true(ALDS_DATA_PTR_IS_VALID(&buffer));

    ((uint8_t *)buffer.ptr)[9] = 100;
    assert_int_equal(((uint8_t *)buffer.ptr)[9], 100);

    ALDS_DATA_FREE(buffer);
    assert_null(buffer.ptr);
    assert_int_equal(buffer.size, 0);
    assert_null(buffer.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer));

    ALDS_DATA_INIT_LOCAL(buffer1, 10);
    ALDS_DATA_PTR_FREE(&buffer1);
    assert_null(buffer1.ptr);
    assert_int_equal(buffer1.size, 0);
    assert_null(buffer1.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer1));
}

static void alds_buff_static_test(void ** state) {
    (void)state; /* unused */

    ALDS_DATA_INIT_STATIC(buffer, 10);

    assert_non_null(buffer.ptr);
    assert_int_equal(buffer.size, 10);
    assert_null(buffer.free_cb);
    assert_true(ALDS_DATA_PTR_IS_VALID(&buffer));

    ((uint8_t *)buffer.ptr)[9] = 100;
    assert_int_equal(((uint8_t *)buffer.ptr)[9], 100);

    ALDS_DATA_FREE(buffer);
    assert_null(buffer.ptr);
    assert_int_equal(buffer.size, 0);
    assert_null(buffer.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer));

    ALDS_DATA_INIT_STATIC(buffer1, 10);
    ALDS_DATA_PTR_FREE(&buffer1);
    assert_null(buffer1.ptr);
    assert_int_equal(buffer1.size, 0);
    assert_null(buffer1.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer1));
}

static void alds_buff_dynamic_test(void ** state) {
    (void)state; /* unused */

    void * data_ptr = alds_malloc_default_local(10);
    ALDS_DATA_INIT_CUSTOM(buffer, data_ptr, 10, alds_free_default_local);
    assert_non_null(buffer.ptr);
    assert_int_equal(buffer.size, 10);
    assert_non_null(buffer.free_cb);
    assert_true(ALDS_DATA_PTR_IS_VALID(&buffer));

    ((uint8_t *)buffer.ptr)[9] = 100;
    assert_int_equal(((uint8_t *)buffer.ptr)[9], 100);

    ALDS_DATA_FREE(buffer);
    assert_null(buffer.ptr);
    assert_int_equal(buffer.size, 0);
    assert_null(buffer.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer));

    data_ptr = alds_malloc_default_local(10);
    ALDS_DATA_INIT_CUSTOM(buffer1, data_ptr, 10, alds_free_default_local);
    ALDS_DATA_PTR_FREE(&buffer1);
    assert_null(buffer1.ptr);
    assert_int_equal(buffer1.size, 0);
    assert_null(buffer1.free_cb);
    assert_false(ALDS_DATA_PTR_IS_VALID(&buffer1));
}

int data_tests(void) {
    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(test_alloc_default),    cmocka_unit_test(test_alloc_default_custom),
        cmocka_unit_test(test_alloc_custom),     cmocka_unit_test(alds_buff_local_test),
        cmocka_unit_test(alds_buff_static_test), cmocka_unit_test(alds_buff_dynamic_test)};

    printf("\nData testing:\n");
    return cmocka_run_group_tests(unit_tests, NULL, NULL);
}