#include <sys/epoll.h>
//
#include "ngd_event.h"
//
static int epfd;
static struct epoll_event events[NGD_EVENT_MAX_GET];
//
static uint8_t
ngd_event_evflags_to_retflags(uint32_t evflags)
{
    uint8_t retflags;
    //
    retflags = 0x00;
    //
    if (evflags & EPOLLIN)
        retflags |= NGD_EVENT_READ;
    if (evflags & EPOLLOUT)
        retflags |= NGD_EVENT_WRITE;
    if (evflags & EPOLLHUP)
        retflags |= NGD_EVENT_HUP;
    if (evflags & EPOLLERR)
        retflags |= NGD_EVENT_ERR;
    //
    return retflags;
}
static int
ngd_event_set(ngd_event_t *ev, int op, int evflags)
{
    struct epoll_event ee;
    //
    ee.events = evflags;
    ee.data.ptr = ev;
    if (epoll_ctl(epfd, op, ev->fd, &ee) == -1)
        return NGD_ERR;
    //
    return NGD_OK;
}
//
void
ngd_event_module_init(void)
{
    epfd = epoll_create1(0);
    if (epfd == -1)
        exit(EXIT_FAILURE);
}
void
ngd_event_module_loop(int timeout)
{
    int n;
    ngd_event_t *ev;
    uint32_t evflags;
    //
    for (;;) {
        n = epoll_wait(epfd, events, sizeof(events), timeout);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            exit(EXIT_FAILURE);
        }
        //
        for (int i=0; i<n; i++)
        {
            ev = events[i].data.ptr;
            evflags = events[i].events;
            //
            ev->retflags = ngd_event_evflags_to_retflags(evflags);
            ev->handler(ev);
        }
        break;
    }
}
//
int ngd_event_regis(ngd_event_t *ev,
                    int fd,
                    void (*handler)(ngd_event_t *ev),
                    void *data)
{
    ev->fd = fd;
    ev->handler = handler;
    ev->data = data;
    return ngd_event_set(ev, EPOLL_CTL_ADD, EPOLLIN);
}
int
ngd_event_unregis(ngd_event_t *ev)
{
    return ngd_event_set(ev, EPOLL_CTL_DEL, 0);
}
int
ngd_event_enable_write(ngd_event_t *ev)
{
    return ngd_event_set(ev, EPOLL_CTL_ADD, EPOLLIN | EPOLLOUT);
}
int
ngd_event_disable_write(ngd_event_t *ev)
{
    return ngd_event_set(ev, EPOLL_CTL_ADD, EPOLLIN);
}
