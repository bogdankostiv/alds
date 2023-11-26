#include <stdio.h>
#include <stdarg.h>
#include "alds/alds.h"
#include "cmocka_incl.h"

static uint32_t call_index = 0; 

static void alds_log_info(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS INFO [Module 1]: general_log_test 1");
    call_index = 1; 
}

static void alds_log_warning(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS WARNING [Module 2]: general_log_test 2");
    call_index = 2; 
}

static void alds_log_error(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS ERROR [Module 3]: general_log_test 3");
    call_index = 3; 
}

static void alds_log_module_len_limit(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS ERROR [VeryLongModuleLimit123456]: general_log_test 4");
    call_index = 4; 
}

static void alds_log_module_len_cut(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS ERROR [VeryLongModuleLimit123...]: general_log_test 5");
    call_index = 5; 
}

static void alds_log_module_va_arg(const char * const msg, va_list va){
    assert_string_equal(msg, "ALDS INFO [Module]: Some module");
    uint32_t arg = va_arg(va, uint32_t);
    assert_int_equal(arg, 5);
    call_index = 6; 
}

static void general_log_test(void ** state) {
    (void) state; /* unused */

    alds_set_log_cb(alds_log_info);
    ALDS_LOG_INFO("Module 1", "general_log_test 1");
    assert_int_equal(call_index, 1);

    alds_set_log_cb(alds_log_warning);
    ALDS_LOG_WARNING("Module 2", "general_log_test 2");
    assert_int_equal(call_index, 2);

    alds_set_log_cb(alds_log_error);
    ALDS_LOG_ERROR("Module 3", "general_log_test 3");
    assert_int_equal(call_index, 3);

    alds_set_log_cb(alds_log_module_len_limit);
    ALDS_LOG_ERROR("VeryLongModuleLimit123456", "general_log_test 4");
    assert_int_equal(call_index, 4);

    alds_set_log_cb(alds_log_module_len_cut);
    ALDS_LOG_ERROR("VeryLongModuleLimit123456789", "general_log_test 5");
    assert_int_equal(call_index, 5);

    alds_set_log_cb(alds_log_module_va_arg);
    ALDS_LOG_INFO("Module", "Some module", (uint32_t) 5);
    assert_int_equal(call_index, 6);

    alds_clear_log_cb();
}

int log_tests(void) {
    const struct CMUnitTest unit_tests[] = {
        cmocka_unit_test(general_log_test),

    };

    printf("\nLog testing:\n");
    return cmocka_run_group_tests(unit_tests, NULL, NULL);
}