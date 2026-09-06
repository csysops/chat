#ifndef NGD_CONN_H
#define NGD_CONN_H
//
#include "ngd_core.h"
#include "ngd_event.h"
#include "ngd_timer.h"
//
typedef struct ngd_conn_t ngd_conn_t;
//
void ngd_conn_module_init(
    int port,
    int backlog,
    bool on_tls,
    void (*init_conn)(ngd_conn_t *)
);
void ngd_conn_module_start(void);
//
int ngd_conn_init(
    ngd_conn_t *c,
    void (*handler)(ngd_conn_t *),
    void *data,
    uint64_t timeout_ms
);
void ngd_conn_close(ngd_conn_t *c);
int ngd_conn_enable_write(ngd_conn_t *c);
int ngd_conn_disable_write(ngd_conn_t *c);
int ngd_conn_reset_timeout(ngd_conn_t *c, uint64_t timer_ms);
//
int ngd_conn_recv(ngd_conn_t *c, u_char *buf, size_t len, size_t *bytes_recved);
int ngd_conn_send(ngd_conn_t *c, u_char *buf, size_t len, size_t *bytes_sent);
//
void *ngd_conn_get_data(ngd_conn_t *c);
//
bool ngd_conn_on_read(ngd_conn_t *c);
bool ngd_conn_on_write(ngd_conn_t *c);
bool ngd_conn_on_timeout(ngd_conn_t *c);
//
#endif
