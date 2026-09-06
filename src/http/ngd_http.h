#ifndef NGD_HTTP_H
#define NGD_HTTP_H
//
#include "ngd_core.h"
#include "ngd_pool.h"
#include "ngd_buf.h"
#include "ngd_str.h"
#include "ngd_list.h"
#include "ngd_file.h"
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
    bool on_keep_alive;
    bool on_chunk;
    bool on_content_length;
    //body
    bool on_body_file;
    ngd_file_t file_temp;
    size_t body_recved;
    //send
    ngd_file_t file_send;
};
// connection
void ngd_http_init_conn(ngd_conn_t *c);
void ngd_http_close_conn(ngd_conn_t *c);
int ngd_http_handle_conn(ngd_conn_t *c);
// http
int ngd_http_parse_reqline(ngd_http_t *http); //(until pos == last) -> undone -> again
int ngd_http_parse_headers(ngd_http_t *http); //(pos <= last) -> done -> ok
int ngd_http_parse_body(ngd_http_t *http);
int ngd_http_build_resp(ngd_http_t *http);
int ngd_http_send_resp(ngd_http_t *http);
#endif
