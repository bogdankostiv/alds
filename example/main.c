#include "basic.h"
#include <alds/alds.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    ALDS_LOG_INFO("Demo APP", "Demo App started\n\n");

    stack_dynamic();
    stack_fixed();

    queue_dynamic();
    queue_fixed();

    ll_simple();
    ll_custom_alloc_data();
    ll_custom_alds_data();

    ALDS_LOG_INFO("Demo APP", "Demo App finished\n\n");

    return 0;
}