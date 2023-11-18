#include <stdlib.h>
#include <stdio.h>
#include <alds.h>
#include "simple.h"

int main(void) {

    ALDS_LOG_INFO("Demo APP", "Demo App started\n");
    
    stack();
    stack_static();

    ALDS_LOG_INFO("Demo APP", "Demo App finished\n");

    return 0;
}