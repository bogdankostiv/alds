#ifndef ALDS_HELPERS_H
#define ALDS_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#define OFFSETOF(struct_type, field_name) ((size_t) & (((struct_type *)0)->field_name))

#ifdef __cplusplus
}
#endif

#endif // ALDS_HELPERS_H
