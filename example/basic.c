#include <alds/alds.h>
#include <alds/basic/stack.h>
#include <alds/basic/queue.h>

void stack(void) {
    // create memory manager for dynamic allocation
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    // create a stack for 3 elements uint32_t
    alds_stack_t ctx;
    alds_stack_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the stack
    uint32_t data = 1;
    alds_stack_push(&ctx, &data);

    // pop it back
    alds_stack_pop(&ctx, &data);

    // deinit the stack
    alds_stack_deinit(&ctx);

    ALDS_LOG_INFO("stack dynamic", "... done\n");
}

void stack_static(void) {
    // create memory manager module with a local buffer
    ALDS_DATA_INIT_LOCAL(buffer, (3 * sizeof(uint32_t)));
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_BUFFER(buffer);

    // init a stack for 3 elements uint32_t
    alds_stack_t ctx;
    alds_stack_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the stack
    uint32_t data = 1;
    alds_stack_push(&ctx, &data);

    // pop it back
    alds_stack_pop(&ctx, &data);

    // deinit the stack
    alds_stack_deinit(&ctx);

    ALDS_LOG_INFO("stack static", "... done\n");
}

void queue(void) {
    // create memory manager for dynamic allocation
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    // create a queue for 3 elements uint32_t
    alds_queue_t ctx;
    alds_queue_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the queue
    uint32_t data = 1;
    alds_queue_push(&ctx, &data);

    // pop it back
    alds_queue_pop(&ctx, &data);

    // deinit the queue
    alds_queue_deinit(&ctx);

    ALDS_LOG_INFO("queue dynamic", "... done\n");
}

void queue_static(void) {
    // create memory manager module with a local buffer
    ALDS_DATA_INIT_LOCAL(buffer, (3 * sizeof(uint32_t)));
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_BUFFER(buffer);

    // init a queue for 3 elements uint32_t
    alds_queue_t ctx;
    alds_queue_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the queue
    uint32_t data = 1;
    alds_queue_push(&ctx, &data);

    // pop it back
    alds_queue_pop(&ctx, &data);

    // deinit the queue
    alds_queue_deinit(&ctx);

    ALDS_LOG_INFO("queue static", "... done\n");
}