#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
//
#include "ngd_conn.h"
#include "ngd_str.h"
//
static void listener_init(int port, int backlog, void (*init_conn)(ngd_conn_t *));
//
/*
 * connection
 */
struct ngd_conn_t {
    int fd;
    ngd_timer_t timer;
    ngd_event_t event;
    void (*handler)(ngd_conn_t *);
    void *data;
    bool on_read;
    bool on_write;
    bool on_timeout;
};
//
static ngd_conn_t *
ngd_conn_get()
{
    return malloc(sizeof(ngd_conn_t));
}
//
static void
ngd_conn_release(ngd_conn_t *c)
{
    free(c);
}
//
static void
ngd_conn_handle_event(ngd_event_t *ev)
{
    uint8_t retflags;
    ngd_conn_t *c;
    //
    c = NGD_EVENT_GET_DATA(ev);
    //
    c->on_read = false;
    c->on_write = false;
    c->on_timeout = false;
    //
    if (NGD_EVENT_IS(ev, NGD_EVENT_READ)) c->on_read = true;
    if (NGD_EVENT_IS(ev, NGD_EVENT_WRITE)) c->on_write = true;
    //
    c->handler(c);
}
//
static void
ngd_conn_handle_timeout(ngd_timer_t *tmr)
{
    ngd_conn_t *c;
    //
    c = NGD_TIMER_GET_DATA(tmr);
    //
    c->on_read = false;
    c->on_write = false;
    c->on_timeout = false;
    //
    c->on_timeout = true;
    //
    c->handler(c);
}
//
void ngd_conn_module_init(int port, int backlog, bool on_tls, void (*init_conn)(ngd_conn_t *))
{
    ngd_event_module_init();
    ngd_timer_module_init();
    //
    listener_init(port, backlog, init_conn);
}
//
void
ngd_conn_module_start(void)
{
    int timeout;
    uint64_t now;
    //
    for (;;)
    {
        timeout = ngd_timer_module_get_next_timeout();
        ngd_event_module_loop(timeout);
        now = ngd_timer_module_get_now();
        ngd_timer_module_expire(now);
    }
}
//
int
ngd_conn_init(ngd_conn_t *c,
              void (*handler)(ngd_conn_t *),
              void *data,
              uint64_t timeout_ms)
{
    c->handler = handler;
    c->data = data;
    //
    if (ngd_event_regis(&c->event, c->fd, ngd_conn_handle_event, c) == NGD_ERR)
        return NGD_ERR;
    if (ngd_timer_regis(&c->timer, ngd_conn_handle_timeout, c, timeout_ms) == NGD_ERR)
        return NGD_ERR;
    //
    return NGD_OK;
}
//
void
ngd_conn_close(ngd_conn_t *c)
{
    ngd_timer_unregis(&c->timer);
    ngd_event_unregis(&c->event);
    close(c->fd);
    ngd_conn_release(c);
}
//
int
ngd_conn_enable_write(ngd_conn_t *c)
{
    return ngd_event_enable_write(&c->event);
}
//
int
ngd_conn_disable_write(ngd_conn_t *c)
{
    return ngd_event_disable_write(&c->event);
}
//
int
ngd_conn_reset_timeout(ngd_conn_t *c, uint64_t timer_ms)
{
    return ngd_timer_reset(&c->timer, timer_ms);
}
//
/*
 * ok-> consume what recved, might st read later
 again-> wait
 err->close
 */
int
ngd_conn_recv(ngd_conn_t *c, u_char *buf, size_t len, size_t *bytes_recved)
{
    ssize_t n;
    //
    for (;;)
    {
        n = recv(c->fd, (void *)buf, len, 0);
        //
        if (n > 0) {
            *(bytes_recved) = n;
            return NGD_OK;
        } else if (n == 0) {
            return NGD_ERR;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return NGD_AGAIN;
            if (errno == EINTR)
                continue;
        }
        //
        break;
    }
    //
    return NGD_ERR;
}
/*
 * ok-> consume what recved, might st write later
 again-> wait
 err->close
 */
int
ngd_conn_send(ngd_conn_t *c, u_char *buf, size_t len, size_t *bytes_sent)
{
    ssize_t n;
    //
    for (;;)
    {
        n = send(c->fd, buf, len, 0);
        //
        if (n > 0) {
            *(bytes_sent) = n;
            return NGD_OK;
        } else if (n == 0) {
            return NGD_ERR;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return NGD_AGAIN;
            if (errno == EINTR)
                continue;
        }
        //
        break;
    }
    //
    return NGD_ERR;
}
//
void *
ngd_conn_get_data(ngd_conn_t *c)
{
    return c->data;
}
//
bool
ngd_conn_on_read(ngd_conn_t *c)
{
    return c->on_read;
}
bool
ngd_conn_on_write(ngd_conn_t *c)
{
    return c->on_write;
}
bool
ngd_conn_on_timeout(ngd_conn_t *c)
{
    return c->on_timeout;
}
/*
 * listener
 */
static struct {
    int fd;
    ngd_event_t event;
    void (*init_conn)(ngd_conn_t *);
} listener;
//
static ngd_conn_t *
listener_accept(void)
{
    ngd_conn_t *c;
    int fd;
    int flag;
    //
    fd = accept(listener.fd, NULL, NULL);
    if (fd == -1)
        return NULL;
    //
    flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    //
    c = ngd_conn_get();
    if (c == NULL) {
        close(fd);
        return NULL;
    }
    c->fd = fd;
    //
    return c;
}
//
static void
listener_handle_event(ngd_event_t *ev)
{
    ngd_conn_t *c;
    //
    c = listener_accept();
    if (c != NULL)
    listener.init_conn(c);
}
//
static void
listener_init(int port, int backlog, void (*init_conn)(ngd_conn_t *))
{
    struct sockaddr_in addr;
    int opt;
    int flag;
    //
    listener.fd = socket(AF_INET, SOCK_STREAM, 0);
    //
    opt = 1;
    setsockopt(listener.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //
    flag = fcntl(listener.fd, F_GETFL, 0);
    fcntl(listener.fd, F_SETFL, flag | O_NONBLOCK);
    //
    ngd_str_zeros((u_char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(listener.fd,
        (struct sockaddr *)&addr,
        sizeof(addr));
    //
    listen(listener.fd, backlog);
    //
    listener.init_conn = init_conn;
    //
    if (ngd_event_regis(&listener.event, listener.fd, listener_handle_event, &listener) == NGD_ERR)
        exit(EXIT_FAILURE);
}
