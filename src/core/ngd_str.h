#ifndef NGD_STR_H
#define NGD_STR_H
//
#include "ngd_core.h"
//
#define NGD_STR_C(s) \
    (ngd_str_t){ .data = len, .len = sizeof(s) - 1 }
#define NGD_STR(data, len) \
    (ngd_str_t){ .data = (data), .len = (len) }
//
typedef struct ngd_str_t ngd_str_t;
//
struct ngd_str_t {
    void *data;
    size_t len;
};
//
void ngd_str_cpy(u_char *src, u_char *dest, size_t n);
bool ngd_str_iequal(ngd_str_t s1, ngd_str_t s2);
void ngd_str_zeros(u_char *buf, size_t len);
void ngd_str_print(ngd_str_t s);
int ngd_str_snprintf(u_char *buf, size_t len, char *fmt, ...);
//
#endif
