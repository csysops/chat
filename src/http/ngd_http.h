#ifndef NGD_HTTP_H
#define NGD_HTTP_H
//
#include "ngd_core.h"
//
#define NGD_HTTP_ROOT_PATH "/WWW"
#define NGD_HTTP_BUFLEN (1024 * 16) //16kb
#define NGD_HTTP_LIMIT_BODY (1024 * 1024) //1mb
#define NGD_HTTP_FULL_HEADER_DONE 1
//
typedef struct ngd_http_t ngd_http_t;
typedef struct ngd_http_header_t ngd_http_header_t;
//
struct ngd_http_header_t {
    str_t key;
    str_t value;
};
//
struct ngd_http_t {

}
//
#endif
