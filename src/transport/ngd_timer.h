#ifndef NGD_TIMER_H
#define NGD_TIMER_H
//DON'T USE MEMBER DIRECTLY
#include "ngd_core.h"
//
#define NGD_TIMER_GET_DATA(tmr) ((tmr)->data)
//
typedef struct ngd_timer_t ngd_timer_t;
//
struct ngd_timer_t {
    uint64_t expire;
    void (*handler)(ngd_timer_t *);
    void *data;

    ngd_timer_t *prev;
    ngd_timer_t *next;
};
void ngd_timer_module_init(void);
int ngd_timer_module_get_next_timeout(void);
uint64_t ngd_timer_module_get_now(void);
void ngd_timer_module_expire(uint64_t now);
//
int ngd_timer_regis(ngd_timer_t *tmr,
                    void (*handler)(ngd_timer_t *),
                    void *data,
                    uint64_t timeout_ms);
int ngd_timer_unregis(ngd_timer_t *tmr);
int ngd_timer_reset(ngd_timer_t *tmr, uint64_t timeout_ms);
//
#endif
