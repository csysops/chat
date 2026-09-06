#ifndef NGD_POOL_H
#define NGD_POOL_H
//
#include "ngd_core.h"
//
#define NGD_POOL_BLOCKSIZE 1024
#define NGD_POOL_FAIL 4
//
typedef struct ngd_pool_t ngd_pool_t;
//
ngd_pool_t *ngd_pool_create();
void *ngd_pool_alloc(ngd_pool_t *pool, size_t size);
void ngd_pool_destroy(ngd_pool_t *pool);
//
#endif
