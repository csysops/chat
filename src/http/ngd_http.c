#include "ngd_http.c"
//

void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_pool_t *pool_req;
    ngd_http_t *http;
    void *buf;
    int ret;
    //
    pool = ngd_pool_create();
    if (pool == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    //
    pool_req = ngd_pool_create();
    if (pool_req == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    //
    http = ngd_pool_alloc(pool, sizeof(*http));
    if (http == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    http->pool = pool;
    http->pool_req = pool_req;
    //
    ngd_file_init(http->file_temp);
    //
    buf = ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    ngd_buf_init(
        &http->headbuf,
        ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    )
    //
    buf = ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    ngd_buf_init(
        &http->bodybuf,
        ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    )
    //
    buf = ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(c);
        return;
    }
    ngd_buf_init(
        &http->outbuf,
        ngd_pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    )
    //
    ret = ngd_conn_init(
        c,
        ngd_http_handle_conn,
        (void *)http,
        NGD_HTTP_TIMEOUT_READ
    );
    if (ret == NGD_ERR)
        ngd_http_close_conn(c);
}
void
ngd_http_close_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    //
    http = ngd_conn_get_data(c);
    //
    if (http->pool != NULL)
        ngd_pool_destroy(http->pool);
    if (http->pool_req != NULL)
        ngd_pool_destroy(http->pool_req);
    //
    ngd_conn_close(c);
}
int
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    enum {
        ps_start=NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_body_chunk,
        ps_body_len,
        ps_build_resp,
        ps_send_resp
    } state;
    int ret;
    ngd_buf_t *hb;
    ngd_buf_t *bb;
    ngd_buf_t *ob;
    size_t n;
    ngd_http_header_t *header;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    hb = http->headbuf;
    bb = http->bodybuf;
    ob = http->outbuf;
    //
    if (ngd_conn_on_timeout(c))
        goto error;
    //
    for (;;)
    {

    }
}
//
