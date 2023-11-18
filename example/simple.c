#include <alds.h>
#include <simple/stack.h>
#include <simple/queue.h>

void stack(void) {
    alds_stack_t ctx;
    // create stack for 3 elements uint32_t
    alds_stack_init(&ctx, 3, sizeof(uint32_t));

    uint32_t data = 1;
    // push one item to the stack
    alds_stack_push(&ctx, &data);

    // pop it back
    alds_stack_pop(&ctx, &data);

    // deinit the stack
    alds_stack_deinit(&ctx);
}

void stack_static(void) {
    // create buffer for the stack
    ALDS_DATA_INIT_STATIC(buffer, 3 * sizeof(uint32_t));

    alds_stack_t ctx;
    // init stack for 3 elements uint32_t
    alds_stack_init_external(&ctx, &buffer, sizeof(uint32_t));

    uint32_t data = 1;
    // push one item to the stack
    alds_stack_push(&ctx, &data);

    // pop it back
    alds_stack_pop(&ctx, &data);

    // deinit the stack
    alds_stack_deinit(&ctx);
}