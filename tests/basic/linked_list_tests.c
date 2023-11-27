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
    (void) state; /* unused */

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
    assert_int_equal(*LL_DATA_PTR(uint16_t *, ll_item), 1);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*LL_DATA_PTR(uint16_t *, ll_item), 2);
    assert_null(alds_ll_next(ll_item));
    ll_item = alds_ll_prev(ll_item);
    assert_int_equal(*LL_DATA_PTR(uint16_t *, ll_item), 1);

    // deinit should delete all allocated memory, sanity check will fall otherwise
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
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 1);

    // 2, 1
    data = 2;
    assert_int_equal(alds_ll_prepend(&ctx, &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_null(ll_item->prev);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 2);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->next), 1);
    ll_item = alds_ll_next(ll_item);
    assert_null(ll_item->next);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->prev), 2);

    // 2, 3, 1
    data = 3;
    assert_int_equal(alds_ll_insert(&ctx, alds_ll_head(&ctx), &data), e_alds_err_success);
    ll_item = alds_ll_head(&ctx);
    assert_null(ll_item->prev);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 2);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->next), 3);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->prev), 2);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 3);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->next), 1);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->prev), 3);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_null(ll_item->next);

    //get the last item in the ll
    ll_item = alds_ll_head(&ctx);
    while (NULL != alds_ll_next(ll_item)) {
        ll_item = alds_ll_next(ll_item);
    }
    // 2, 3, 1, 4
    data = 4;
    assert_int_equal(alds_ll_insert(&ctx, ll_item, &data), e_alds_err_success);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->prev), 3);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 1);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->next), 4);
    ll_item = alds_ll_next(ll_item);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item->prev), 1);
    assert_int_equal(*LL_DATA_PTR(uint32_t *, ll_item), 4);
    assert_null(ll_item->next);

    alds_ll_deinit(&ctx);
}

int linked_list_tests(void) {
    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(ll_init_deinit),
        cmocka_unit_test(ll_non_circular),
        cmocka_unit_test(ll_prev_next_and_deinit),
        //            cmocka_unit_test(queue_dynamic_negative_arguments),
        //            cmocka_unit_test(queue_static_full),
    };

    printf("\nLinked list testing:\n");
    return cmocka_run_group_tests(unit_tests, NULL, NULL);
}