#include <stdlib.h>
#include <stdio.h>
#include "cmoka_incl.h"

/* A test case that does nothing and succeeds. */
static void canary_test(void ** state) {
    (void) state; /* unused */
    assert_int_equal(0, 0);
}

int main(int argc, char * argv[]) {
    int result = 0;

    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(canary_test)
    };

    printf("Unit tests:\n");
    result = cmocka_run_group_tests(unit_tests, NULL, NULL);
    if (0 != result) {
        return result;
    }

    return 0;
}