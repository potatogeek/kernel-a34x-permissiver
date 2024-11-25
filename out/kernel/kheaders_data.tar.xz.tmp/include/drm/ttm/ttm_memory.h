

#ifndef TTM_MEMORY_H
#define TTM_MEMORY_H

#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/bug.h>
#include <linux/wait.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/mm.h>
#include "ttm_bo_api.h"



#define TTM_MEM_MAX_ZONES 2
struct ttm_mem_zone;
struct ttm_mem_global {
	struct kobject kobj;
	struct ttm_bo_global *bo_glob;
	struct workqueue_struct *swap_queue;
	struct work_struct work;
	spinlock_t lock;
	uint64_t lower_mem_limit;
	struct ttm_mem_zone *zones[TTM_MEM_MAX_ZONES];
	unsigned int num_zones;
	struct ttm_mem_zone *zone_kernel;
#ifdef CONFIG_HIGHMEM
	struct ttm_mem_zone *zone_highmem;
#else
	struct ttm_mem_zone *zone_dma32;
#endif
};

extern int ttm_mem_global_init(struct ttm_mem_global *glob);
extern void ttm_mem_global_release(struct ttm_mem_global *glob);
extern int ttm_mem_global_alloc(struct ttm_mem_global *glob, uint64_t memory,
				struct ttm_operation_ctx *ctx);
extern void ttm_mem_global_free(struct ttm_mem_global *glob,
				uint64_t amount);
extern int ttm_mem_global_alloc_page(struct ttm_mem_global *glob,
				     struct page *page, uint64_t size,
				     struct ttm_operation_ctx *ctx);
extern void ttm_mem_global_free_page(struct ttm_mem_global *glob,
				     struct page *page, uint64_t size);
extern size_t ttm_round_pot(size_t size);
extern uint64_t ttm_get_kernel_zone_memory_size(struct ttm_mem_global *glob);
extern bool ttm_check_under_lowerlimit(struct ttm_mem_global *glob,
			uint64_t num_pages, struct ttm_operation_ctx *ctx);
#endif
