#ifndef ALDS_ERROR_H
#define ALDS_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    e_alds_err_success = 0,
    e_alds_err_general,
    e_alds_err_arg,
    e_alds_err_memalloc,
    e_alds_err_memmngr,
    e_alds_err_empty,
    e_alds_err_full,
} alds_err_t;

#ifdef __cplusplus
}
#endif

#endif // ALDS_ERROR_H