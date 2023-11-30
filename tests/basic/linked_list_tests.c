#include "../cmocka_incl.h"
#include "alds/alds.h"
#include "alds/basic/linked_list.h"
#include "basic_tests.h"
#include <stdio.h>

static void ll_init_deinit(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint32_t);
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(NULL, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_arg);
    assert_int_equal(alds_ll_init(&ctx, NULL, data_size, NULL, false), e_alds_err_arg);
    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), 0, NULL, false), e_alds_err_arg);

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_success);

    alds_ll_deinit(&ctx);
}

static void ll_prev_next_and_deinit(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint16_t);
    alds_ll_t * ll_item = NULL;
    uint16_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_success);
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    data = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_null(alds_ll_prev(ll_item));
    assert_int_equal(*ALDS_LL_DATA_PTR(uint16_t *, ll_item), 1);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint16_t *, ll_item), 2);
    assert_null(alds_ll_next(ll_item));
    ll_item = alds_ll_prev(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint16_t *, ll_item), 1);

    // deinit should delete all allocated memory, sanity check will fall otherwise
    alds_ll_deinit(&ctx);
}

static void ll_empty_insert(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint16_t);
    alds_ll_t * ll_item = NULL;
    uint16_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_success);
    data = 1;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_null(alds_ll_prev(ll_item));
    assert_null(alds_ll_next(ll_item));
    assert_int_equal(*ALDS_LL_DATA_PTR(uint16_t *, ll_item), 1);

    alds_ll_deinit(&ctx);
}

static void ll_non_circular(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint32_t);
    alds_ll_t * ll_item = NULL;
    uint32_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_success);

    assert_null(alds_ll_head(NULL));
    assert_null(alds_ll_prev(NULL));
    assert_null(alds_ll_next(NULL));

    assert_null(alds_ll_head(&ctx));

    // 1
    data = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_non_null(ll_item);
    assert_null(ll_item->next);
    assert_null(ll_item->prev);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 1);

    // 2, 1
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_null(ll_item->prev);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->next), 1);
    ll_item = alds_ll_next(ll_item);
    assert_null(ll_item->next);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->prev), 2);

    // 2, 3, 1
    data = 3;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_null(ll_item->prev);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->next), 3);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->prev), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->next), 1);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->prev), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_null(ll_item->next);

    // get the last item in the ll
    ll_item = alds_ll_head(&ctx);
    while (NULL != alds_ll_next(ll_item)) {
        ll_item = alds_ll_next(ll_item);
    }
    // 2, 3, 1, 4
    data = 4;
    assert_int_equal(alds_ll_insert(&ctx, ll_item, &data), e_alds_err_success);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->prev), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->next), 4);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item->prev), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 4);
    assert_null(ll_item->next);

    alds_ll_deinit(&ctx);
}

static void ll_circular(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint64_t);
    alds_ll_t * ll_item = NULL;
    uint64_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, true), e_alds_err_success);

    assert_null(alds_ll_head(NULL));
    assert_null(alds_ll_prev(NULL));
    assert_null(alds_ll_next(NULL));
    assert_null(alds_ll_head(&ctx));

    // 2
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 2);

    // 1 2 3
    data = 3;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);
    data = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, alds_ll_next(ll_item)), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, alds_ll_prev(ll_item)), 3);

    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 2);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 1);

    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 2);

    alds_ll_deinit(&ctx);
}

static void ll_non_circular_delete(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint32_t);
    alds_ll_t * ll_item = NULL;
    uint32_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false), e_alds_err_success);

    // 1 2 3
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    data = 3;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);
    data = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    // 1 3
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)->next), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, alds_ll_next(ll_item)), 3);
    assert_null(alds_ll_prev(ll_item));

    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 3);
    assert_null(alds_ll_next(ll_item));
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, alds_ll_prev(ll_item)), 1);

    // 3
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint32_t *, ll_item), 3);
    assert_null(alds_ll_next(ll_item));
    assert_null(alds_ll_prev(ll_item));

    // empty
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)), e_alds_err_success);
    assert_null(alds_ll_head(&ctx));

    alds_ll_deinit(&ctx);
}

static void ll_circular_delete(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(uint64_t);
    alds_ll_t * ll_item = NULL;
    uint64_t data = 0;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, true), e_alds_err_success);

    // 1 2 3
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    data = 3;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);
    data = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    // 1 3
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)->next), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 3);

    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 3);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 1);

    // 1
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)->next), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, ll_item), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_next(ll_item)), 1);
    assert_int_equal(*ALDS_LL_DATA_PTR(uint64_t *, alds_ll_prev(ll_item)), 1);

    // empty
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)), e_alds_err_success);
    assert_null(alds_ll_head(&ctx));

    alds_ll_deinit(&ctx);
}

static void ll_data_free_cb(void * ptr) {
    uint64_t ** data = (uint64_t **)ptr;
    alds_free(*data);
}

static void ll_custom_deallocator(void ** state) {
    (void)state; /* unused */

    uint64_t * data_ptr = NULL;
    size_t data_size = sizeof(data_ptr);
    alds_ll_t * ll_item = NULL;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, ll_data_free_cb, true),
                     e_alds_err_success);

    // 1 2
    data_ptr = alds_malloc(sizeof(*data_ptr));
    *data_ptr = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data_ptr), e_alds_err_success);

    data_ptr = alds_malloc(sizeof(*data_ptr));
    *data_ptr = 1;
    assert_int_equal(alds_ll_prepend(&ctx, &data_ptr), e_alds_err_success);

    // 1
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)->next), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, ll_item), 1);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, alds_ll_next(ll_item)), 1);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, alds_ll_prev(ll_item)), 1);

    // empty
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)), e_alds_err_success);
    assert_null(alds_ll_head(&ctx));

    alds_ll_deinit(&ctx);
}

static void ll_data_free_alds_data_cb(void * ptr) {
    ALDS_DATA_PTR_FREE((alds_data_t *)ptr);
}

static void ll_custom_deallocator_for_alds_data(void ** state) {
    (void)state; /* unused */

    size_t data_size = sizeof(alds_data_t);
    alds_ll_t * ll_item = NULL;
    alds_data_t data;
    uint64_t value;
    alds_ll_ctx_t ctx;

    assert_int_equal(alds_ll_init(&ctx, alds_alloc_default_get(), data_size, ll_data_free_alds_data_cb, true),
                     e_alds_err_success);

    // 1 2
    value = 2;
    data.size = sizeof(value);
    data.ptr = alds_malloc(data.size);
    memcpy(data.ptr, &value, data.size);
    data.free_cb = alds_free;

    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    value = 1;
    data.size = sizeof(value);
    data.ptr = alds_malloc(data.size);
    memcpy(data.ptr, &value, data.size);
    data.free_cb = alds_free;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);

    // 1
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)->next), e_alds_err_success);

    ll_item = alds_ll_head(&ctx);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, ll_item), 1);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, alds_ll_next(ll_item)), 1);
    assert_int_equal(**ALDS_LL_DATA_PTR(uint64_t **, alds_ll_prev(ll_item)), 1);

    // empty
    assert_int_equal(alds_ll_delete(&ctx, alds_ll_head(&ctx)), e_alds_err_success);
    assert_null(alds_ll_head(&ctx));

    alds_ll_deinit(&ctx);
}

int linked_list_tests(void) {
    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(ll_init_deinit),
        cmocka_unit_test(ll_prev_next_and_deinit),
        cmocka_unit_test(ll_empty_insert),
        cmocka_unit_test(ll_non_circular),
        cmocka_unit_test(ll_circular),
        cmocka_unit_test(ll_non_circular_delete),
        cmocka_unit_test(ll_circular_delete),
        cmocka_unit_test(ll_custom_deallocator),
        cmocka_unit_test(ll_custom_deallocator_for_alds_data),
    };

    printf("\nLinked list testing:\n");
    return cmocka_run_group_tests(unit_tests, NULL, NULL);
}