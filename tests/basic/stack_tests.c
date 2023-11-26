#include "../cmocka_incl.h"
#include "alds/alds.h"
#include "alds/basic/stack.h"
#include "basic_tests.h"
#include <stdio.h>

static void stack_dynamic_positive(void ** state) {
    (void)state; /* unused */

    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    alds_stack_t ctx;
    assert_int_equal(alds_stack_init(&ctx, &mm, 3, sizeof(uint32_t)),
                     e_alds_err_success);

    uint32_t data = 1;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 2;
    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 1);

    data = 2;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 3;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 4;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);

    data = 1;
    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 4);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 3);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 2);

    alds_stack_deinit(&ctx);
}

static void stack_dynamic_negative_range(void ** state) {
    (void)state; /* unused */

    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    alds_stack_t ctx;
    assert_int_equal(alds_stack_init(&ctx, &mm, 1, sizeof(uint8_t)),
                     e_alds_err_success);

    uint8_t data = 1;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 2;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_full);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 1);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_empty);

    alds_stack_deinit(&ctx);
}

static void stack_dynamic_negative_arguments(void ** state) {
    (void)state; /* unused */

    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    alds_stack_t ctx;
    assert_int_equal(alds_stack_init(NULL, &mm, 1, sizeof(uint8_t)),
                     e_alds_err_arg);
    assert_int_equal(alds_stack_init(&ctx, NULL, 0, sizeof(uint8_t)),
                     e_alds_err_arg);
    assert_int_equal(alds_stack_init(&ctx, &mm, 1, 0), e_alds_err_arg);

    assert_int_equal(alds_stack_init(&ctx, &mm, 1, sizeof(uint8_t)),
                     e_alds_err_success);

    uint8_t data = 1;
    assert_int_equal(alds_stack_push(NULL, &data), e_alds_err_arg);
    assert_int_equal(alds_stack_push(&ctx, NULL), e_alds_err_arg);
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);

    data = 2;
    assert_int_equal(alds_stack_pop(NULL, &data), e_alds_err_arg);
    assert_int_equal(alds_stack_pop(&ctx, NULL), e_alds_err_arg);
    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 1);

    alds_stack_deinit(&ctx);
}

static void stack_static_full(void ** state) {
    (void)state; /* unused */

    ALDS_DATA_INIT_LOCAL(buffer, (2 * sizeof(uint16_t)));
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_BUFFER(buffer);

    alds_stack_t ctx;

    assert_int_equal(alds_stack_init(NULL, &mm, 2, sizeof(uint16_t)),
                     e_alds_err_arg);
    assert_int_equal(alds_stack_init(&ctx, NULL, 0, sizeof(uint16_t)),
                     e_alds_err_arg);
    assert_int_equal(alds_stack_init(&ctx, &mm, 2, 0), e_alds_err_arg);

    assert_int_equal(alds_stack_init(&ctx, &mm, 3, sizeof(uint16_t)),
                     e_alds_err_memalloc);

    assert_int_equal(alds_stack_init(&ctx, &mm, 2, sizeof(uint16_t)),
                     e_alds_err_success);

    uint16_t data = 2;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 3;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_success);
    data = 4;
    assert_int_equal(alds_stack_push(&ctx, &data), e_alds_err_full);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 3);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_success);
    assert_int_equal(data, 2);

    assert_int_equal(alds_stack_pop(&ctx, &data), e_alds_err_empty);

    alds_stack_deinit(&ctx);
}

int stack_tests(void) {
    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(stack_dynamic_positive),
        cmocka_unit_test(stack_dynamic_negative_range),
        cmocka_unit_test(stack_dynamic_negative_arguments),
        cmocka_unit_test(stack_static_full),
    };

    printf("\nStack testing:\n");
    return cmocka_run_group_tests(unit_tests, NULL, NULL);
}