#include <stdlib.h>
//
#include "ngd_pool.h"
//
typedef struct ngd_pool_block_t ngd_pool_block_t;
typedef struct ngd_pool_large_t ngd_pool_large_t;
//
struct ngd_pool_block_t {
    int fail;
    u_char *last;
    u_char *end;
    ngd_pool_block_t *next;
};
//
struct ngd_pool_large_t {
    ngd_pool_large_t *next;
};
//
struct ngd_pool_t {
    ngd_pool_block_t *blocks;
    ngd_pool_block_t *busy_blocks;
    ngd_pool_large_t *large_blocks;
    size_t block_range;
};
// helpers
static ngd_pool_block_t *
create_block(void)
{
    ngd_pool_block_t *blk;
    //
    blk = (ngd_pool_block_t *)malloc(NGD_POOL_BLOCKSIZE);
    if (blk == NULL)
        return NULL;
    //
    blk->fail = 0;
    blk->last = (u_char *)blk + sizeof(ngd_pool_block_t);
    blk->end = (u_char *)blk + NGD_POOL_BLOCKSIZE;
    blk->next = NULL;
    //
    return blk;
}
//
static void *
alloc_block(ngd_pool_t *pool, size_t size)
{
    ngd_pool_block_t *prev_blk, *cur_blk;
    void *p;
    //
    prev_blk = pool->blocks;
    cur_blk = pool->blocks;
    //
    while (cur_blk != NULL)
    {
        if (size <= cur_blk->end - cur_blk->last) {
            p = cur_blk->last;
            cur_blk->last += size;
            return p;
        }

        if (++cur_blk->fail > NGD_POOL_FAIL) {
            prev_blk->next = cur_blk->next;
            if (pool->busy_blocks == NULL)
                cur_blk->next = NULL;
            else
                cur_blk->next = pool->busy_blocks;
            pool->busy_blocks = cur_blk;
            cur_blk = prev_blk->next;
            continue;
        }

        prev_blk = cur_blk;
        cur_blk = cur_blk->next;
    }
    //
    cur_blk = create_block();
    if (pool->blocks == NULL)
        pool->blocks = cur_blk;
    else
        prev_blk->next = cur_blk;
    //
    p = cur_blk->last;
    cur_blk->last += size;
    //
    return p;
}
//
static void *
alloc_large(ngd_pool_t *pool, size_t size)
{
    ngd_pool_large_t *large_blk;
    void *p;
    //
    large_blk = (ngd_pool_large_t *)malloc(sizeof(ngd_pool_large_t) + size);
    if (pool->large_blocks == NULL)
        large_blk->next = NULL;
    else
        large_blk->next = pool->large_blocks;
    pool->large_blocks = large_blk;
    //
    p = (u_char *)large_blk + sizeof(ngd_pool_large_t);
    //
    return p;
}
//
ngd_pool_t *
ngd_pool_create()
{
    ngd_pool_t *pool;
    //
    pool = (ngd_pool_t *)malloc(sizeof(ngd_pool_t));
    pool->blocks = NULL;
    pool->large_blocks = NULL;
    pool->block_range = NGD_POOL_BLOCKSIZE - sizeof(ngd_pool_block_t);
    //
    return pool;
}
//
void *
ngd_pool_alloc(ngd_pool_t *pool, size_t size)
{
    void *p;
    //
    if (size <= pool->block_range)
        p = alloc_block(pool, size);
    else
        p = alloc_large(pool, size);
    //
    return p;
}
//
void
ngd_pool_destroy(ngd_pool_t *pool)
{
    ngd_pool_block_t *blk, *next_blk;
    ngd_pool_large_t *large_blk, *next_large_blk;
    //
    blk = pool->blocks;
    while(blk)
    {
        next_blk = blk->next;
        free(blk);
        blk = next_blk;
    }
    //
    blk = pool->busy_blocks;
    while(blk)
    {
        next_blk = blk->next;
        free(blk);
        blk = next_blk;
    }
    //
    large_blk = pool->large_blocks;
    while (large_blk)
    {
        next_large_blk = large_blk->next;
        free(large_blk);
        large_blk = next_large_blk;
    }
    //
    free(pool);
}
