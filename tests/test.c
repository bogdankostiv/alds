#include <stdlib.h>
#include <stdio.h>
#include "simple/simple_tests.h"
#include "alds_memmgmt.h"
#include "cmocka_incl.h"

static void * alds_malloc_default(size_t size);
static void * alds_calloc_default(size_t size);
static void * alds_realloc_default(void *  ptr, size_t new_size);
static void alds_free_default(void ** ptr);

static alds_memgmnt_t memgmnt_cb = {
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

int main(int argc, char * argv[]) {
    int result = 0;

    result = alds_init_memmgmnt(&memgmnt_cb);
    if (result != e_alds_err_success) {
        printf("Custom memory allocation init failed:\n");
        return -1;
    }

    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(canary_test)
    };

    printf("Unit tests:\n");
    result |= cmocka_run_group_tests(unit_tests, NULL, NULL);

    result |= stack_tests();
    result |= queue_tests();

    return result;
}