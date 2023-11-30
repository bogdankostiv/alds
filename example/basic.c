#include <alds/alds.h>
#include <alds/basic/linked_list.h>
#include <alds/basic/queue.h>
#include <alds/basic/stack.h>
#include <string.h>

void stack_dynamic(void) {
    const char * module_name = "stack dynamic";

    // create memory manager for dynamic allocation
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    // create a stack for 3 elements uint32_t
    alds_stack_t ctx;
    alds_stack_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the stack
    uint32_t data_push = 1;
    alds_stack_push(&ctx, &data_push);

    // pop it back
    uint32_t data_pop = 0;
    alds_stack_pop(&ctx, &data_pop);

    if (data_pop == data_push) {
        ALDS_LOG_INFO(module_name, "values verification - done\n");
    }

    alds_stack_deinit(&ctx);

    ALDS_LOG_INFO(module_name, "... done\n\n");
}

void stack_fixed(void) {
    const char * module_name = "stack fixed";

    // create memory manager module with a local buffer
    ALDS_DATA_INIT_LOCAL(buffer, (3 * sizeof(uint32_t)));
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_BUFFER(buffer);

    // init a stack for 3 elements uint32_t
    alds_stack_t ctx;
    alds_stack_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the stack
    uint32_t data_push = 1;
    alds_stack_push(&ctx, &data_push);

    // pop it back
    uint32_t data_pop = 0;
    alds_stack_pop(&ctx, &data_pop);

    if (data_pop == data_push) {
        ALDS_LOG_INFO(module_name, "values verification - done\n");
    }

    alds_stack_deinit(&ctx);

    ALDS_LOG_INFO(module_name, "... done\n\n");
}

void queue_dynamic(void) {
    const char * module_name = "queue dynamic";

    // create memory manager for dynamic allocation
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_ALLOCATOR(*alds_alloc_default_get());

    // create a queue for 3 elements uint32_t
    alds_queue_t ctx;
    alds_queue_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the queue
    uint32_t data_push = 1;
    alds_queue_push(&ctx, &data_push);

    // pop it back
    uint32_t data_pop = 0;
    alds_queue_pop(&ctx, &data_pop);

    if (data_pop == data_push) {
        ALDS_LOG_INFO(module_name, "values verification - done\n");
    }

    alds_queue_deinit(&ctx);

    ALDS_LOG_INFO(module_name, "... done\n\n");
}

void queue_fixed(void) {
    const char * module_name = "queue fixed";

    // create memory manager module with a local buffer
    ALDS_DATA_INIT_LOCAL(buffer, (3 * sizeof(uint32_t)));
    alds_memmngr_t mm = ALDS_MEMMNGR_FROM_BUFFER(buffer);

    // init a queue for 3 elements uint32_t
    alds_queue_t ctx;
    alds_queue_init(&ctx, &mm, 3, sizeof(uint32_t));

    // push one item to the queue
    uint32_t data_push = 1;
    alds_queue_push(&ctx, &data_push);

    // pop it back
    uint32_t data_pop = 0;
    alds_queue_pop(&ctx, &data_pop);

    if (data_pop == data_push) {
        ALDS_LOG_INFO(module_name, "values verification - done\n");
    }

    alds_queue_deinit(&ctx);

    ALDS_LOG_INFO(module_name, "... done\n\n");
}

void ll_simple(void) {
    uint32_t data;
    size_t data_size = sizeof(data);
    alds_ll_ctx_t ctx;
    const char * module_name = "linked list";

    ALDS_LOG_INFO(module_name, "simple example\n");

    // init non-circular ll without data de-allocator
    alds_ll_init(&ctx, alds_alloc_default_get(), data_size, NULL, false);

    // put 1 2 3
    data = 3;
    alds_ll_prepend(&ctx, &data);
    data = 2;
    alds_ll_prepend(&ctx, &data);
    data = 1;
    alds_ll_prepend(&ctx, &data);

    // get the head of the linked list and print it's content
    alds_ll_t * ll_item = alds_ll_head(&ctx);
    ALDS_LOG_INFO(module_name, "the first element's value: %d\n", *ALDS_LL_DATA_PTR(uint32_t *, ll_item));

    // the third element
    ll_item = alds_ll_next(alds_ll_next(ll_item));
    ALDS_LOG_INFO(module_name, "the third element's value: %d\n", *ALDS_LL_DATA_PTR(uint32_t *, ll_item));

    // let's delete it
    alds_ll_delete(&ctx, ll_item);

    alds_ll_deinit(&ctx);

    ALDS_LOG_INFO(module_name, "... done\n\n");
}

static const char * str_hello = "Hello";
static const char * str_world = "World!";

static const char * ll_custom_alloc_module_name = "linked list custom alloc";
static void custom_alloc_free(void * ptr) {
    char ** data = (char **)ptr;
    alds_free(*data);
    ALDS_LOG_INFO(ll_custom_alloc_module_name, "custom_alloc_free call\n");
}

void ll_custom_alloc_data(void) {
    char * data_ptr;
    size_t data_ptr_size = sizeof(data_ptr);
    size_t data_size;
    alds_ll_ctx_t ctx;

    ALDS_LOG_INFO(ll_custom_alloc_module_name, "(char *) data example\n");

    // init non-circular ll without data de-allocator
    alds_ll_init(&ctx, alds_alloc_default_get(), data_ptr_size, custom_alloc_free, false);

    // put Hello world
    data_size = strlen(str_hello) + 1;
    data_ptr = alds_malloc(data_size);
    strcpy(data_ptr, str_hello);
    alds_ll_insert(&ctx, NULL, &data_ptr);

    data_size = strlen(str_world) + 1;
    data_ptr = alds_malloc(data_size);
    strcpy(data_ptr, str_world);
    alds_ll_insert(&ctx, alds_ll_head(&ctx), &data_ptr);

    // print Hello world from the linked list
    alds_ll_t * ll_item_first = alds_ll_head(&ctx);
    alds_ll_t * ll_item_second = alds_ll_next(ll_item_first);
    ALDS_LOG_INFO(ll_custom_alloc_module_name, "Values: %s %s\n", *ALDS_LL_DATA_PTR(char **, ll_item_first),
                  *ALDS_LL_DATA_PTR(char **, ll_item_second));

    alds_ll_deinit(&ctx);

    ALDS_LOG_INFO(ll_custom_alloc_module_name, "... done\n\n");
}

static const char * ll_custom_alds_data_module_name = "linked list alds_data";
static void custom_alds_data_free(void * ptr) {
    ALDS_DATA_PTR_FREE((alds_data_t *)ptr);
    ALDS_LOG_INFO(ll_custom_alds_data_module_name, "custom_alds_data_free call\n");
}

void ll_custom_alds_data(void) {
    alds_data_t data;
    size_t data_size = sizeof(data);
    alds_ll_ctx_t ctx;
    const char * str_hello = "Hello";

    ALDS_LOG_INFO(ll_custom_alds_data_module_name, "alds_data_t data example\n");

    // init non-circular ll without data de-allocator
    alds_ll_init(&ctx, alds_alloc_default_get(), data_size, custom_alds_data_free, false);

    // put Hello world
    data.size = strlen(str_hello) + 1;
    data.ptr = alds_malloc(data.size);
    strcpy(data.ptr, str_hello);
    data.free_cb = alds_free;
    alds_ll_insert(&ctx, NULL, &data);

    data.size = strlen(str_world) + 1;
    data.ptr = alds_malloc(data.size);
    strcpy(data.ptr, str_world);
    data.free_cb = alds_free;
    alds_ll_insert(&ctx, alds_ll_head(&ctx), &data);

    // print Hello world from the linked list
    alds_ll_t * ll_item_first = alds_ll_head(&ctx);
    alds_ll_t * ll_item_second = alds_ll_next(ll_item_first);
    ALDS_LOG_INFO(ll_custom_alds_data_module_name, "Values: %s %s\n",
                  ALDS_LL_DATA_PTR(alds_data_t *, ll_item_first)->ptr,
                  ALDS_LL_DATA_PTR(alds_data_t *, ll_item_second)->ptr);

    alds_ll_deinit(&ctx);

    ALDS_LOG_INFO(ll_custom_alds_data_module_name, "... done\n\n");
}