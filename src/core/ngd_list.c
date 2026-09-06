#include "ngd_list.h"
//
void
ngd_list_init(ngd_list_t *l, ngd_pool_t *pool)
{
    l->pool = pool;
}
int
ngd_list_append(ngd_list_t *l, void *data)
{
    ngd_list_node_t *node;
    //
    node = (ngd_list_node_t *)ngd_pool_alloc(l->pool, sizeof(*node));
    if (node == NULL)
        return NGD_ERR;
    node->data = data;
    node->next = l->head;
    l->head = node;
    //
    return NGD_OK;
}
