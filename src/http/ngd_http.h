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
    int state;
    int state_req;
    //
    ngd_pool_t *pool;
    ngd_pool_t *pool_req;
    //
    ngd_buf_t headbuf;
    ngd_buf_t bodybuf;
    ngd_buf_t outbuf;
    //reqline
    str_t smethod;
    str_t suri;
    str_t sver;
    u_char *method_start;
    u_char *method_end;
    u_char *uri_start;
    u_char *uri_end;
    u_char *ver_start;
    u_char *ver_end;
    //headers
    ngd_list_t headers;
    u_char *key_start;
    u_char *key_end;
    u_char *value_start;
    u_char *value_end;
    //
    size_t content_length;

}
//
#endif
