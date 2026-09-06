#ifndef NGD_LIST_H
#define NGD_LIST_H
//
#include "ngd_core.h"
#include "ngd_pool.h"
//
typedef struct ngd_list_node_t ngd_list_node_t;
typedef struct ngd_list_t ngd_list_t;
//
struct ngd_list_node_t {
    void *data;
    ngd_list_node_t *next;
};
//
struct ngd_list_t {
    ngd_list_node_t *head;
    ngd_pool_t *pool;
};
//
void ngd_list_init(ngd_list_t *l, ngd_pool_t *pool);
int ngd_list_append(ngd_list_t *l, void *data);
//
/*
 loop:
    ngd_list_node_t *p;
    for (p = l->head; p != NULL; p->next)
    {
        do something;
    }
 */
//
#endif
