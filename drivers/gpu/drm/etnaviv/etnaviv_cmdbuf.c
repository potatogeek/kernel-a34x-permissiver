// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2017-2018 Etnaviv Project
 */

<<<<<<< HEAD
#include <drm/drm_mm.h>

#include "etnaviv_cmdbuf.h"
=======
#include <linux/dma-mapping.h>

#include <drm/drm_mm.h>

#include "etnaviv_cmdbuf.h"
#include "etnaviv_gem.h"
>>>>>>> upstream/android-13
#include "etnaviv_gpu.h"
#include "etnaviv_mmu.h"
#include "etnaviv_perfmon.h"

<<<<<<< HEAD
#define SUBALLOC_SIZE		SZ_256K
=======
#define SUBALLOC_SIZE		SZ_512K
>>>>>>> upstream/android-13
#define SUBALLOC_GRANULE	SZ_4K
#define SUBALLOC_GRANULES	(SUBALLOC_SIZE / SUBALLOC_GRANULE)

struct etnaviv_cmdbuf_suballoc {
	/* suballocated dma buffer properties */
<<<<<<< HEAD
	struct etnaviv_gpu *gpu;
	void *vaddr;
	dma_addr_t paddr;

	/* GPU mapping */
	u32 iova;
	struct drm_mm_node vram_node; /* only used on MMUv2 */

=======
	struct device *dev;
	void *vaddr;
	dma_addr_t paddr;

>>>>>>> upstream/android-13
	/* allocation management */
	struct mutex lock;
	DECLARE_BITMAP(granule_map, SUBALLOC_GRANULES);
	int free_space;
	wait_queue_head_t free_event;
};

struct etnaviv_cmdbuf_suballoc *
<<<<<<< HEAD
etnaviv_cmdbuf_suballoc_new(struct etnaviv_gpu * gpu)
=======
etnaviv_cmdbuf_suballoc_new(struct device *dev)
>>>>>>> upstream/android-13
{
	struct etnaviv_cmdbuf_suballoc *suballoc;
	int ret;

	suballoc = kzalloc(sizeof(*suballoc), GFP_KERNEL);
	if (!suballoc)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	suballoc->gpu = gpu;
	mutex_init(&suballoc->lock);
	init_waitqueue_head(&suballoc->free_event);

	suballoc->vaddr = dma_alloc_wc(gpu->dev, SUBALLOC_SIZE,
				       &suballoc->paddr, GFP_KERNEL);
	if (!suballoc->vaddr)
		goto free_suballoc;

	ret = etnaviv_iommu_get_suballoc_va(gpu, suballoc->paddr,
					    &suballoc->vram_node, SUBALLOC_SIZE,
					    &suballoc->iova);
	if (ret)
		goto free_dma;

	return suballoc;

free_dma:
	dma_free_wc(gpu->dev, SUBALLOC_SIZE, suballoc->vaddr, suballoc->paddr);
free_suballoc:
	kfree(suballoc);

	return NULL;
=======
	suballoc->dev = dev;
	mutex_init(&suballoc->lock);
	init_waitqueue_head(&suballoc->free_event);

	BUILD_BUG_ON(ETNAVIV_SOFTPIN_START_ADDRESS < SUBALLOC_SIZE);
	suballoc->vaddr = dma_alloc_wc(dev, SUBALLOC_SIZE,
				       &suballoc->paddr, GFP_KERNEL);
	if (!suballoc->vaddr) {
		ret = -ENOMEM;
		goto free_suballoc;
	}

	return suballoc;

free_suballoc:
	kfree(suballoc);

	return ERR_PTR(ret);
}

int etnaviv_cmdbuf_suballoc_map(struct etnaviv_cmdbuf_suballoc *suballoc,
				struct etnaviv_iommu_context *context,
				struct etnaviv_vram_mapping *mapping,
				u32 memory_base)
{
	return etnaviv_iommu_get_suballoc_va(context, mapping, memory_base,
					     suballoc->paddr, SUBALLOC_SIZE);
}

void etnaviv_cmdbuf_suballoc_unmap(struct etnaviv_iommu_context *context,
				   struct etnaviv_vram_mapping *mapping)
{
	etnaviv_iommu_put_suballoc_va(context, mapping);
>>>>>>> upstream/android-13
}

void etnaviv_cmdbuf_suballoc_destroy(struct etnaviv_cmdbuf_suballoc *suballoc)
{
<<<<<<< HEAD
	etnaviv_iommu_put_suballoc_va(suballoc->gpu, &suballoc->vram_node,
				      SUBALLOC_SIZE, suballoc->iova);
	dma_free_wc(suballoc->gpu->dev, SUBALLOC_SIZE, suballoc->vaddr,
=======
	dma_free_wc(suballoc->dev, SUBALLOC_SIZE, suballoc->vaddr,
>>>>>>> upstream/android-13
		    suballoc->paddr);
	kfree(suballoc);
}

int etnaviv_cmdbuf_init(struct etnaviv_cmdbuf_suballoc *suballoc,
			struct etnaviv_cmdbuf *cmdbuf, u32 size)
{
	int granule_offs, order, ret;

	cmdbuf->suballoc = suballoc;
	cmdbuf->size = size;

	order = order_base_2(ALIGN(size, SUBALLOC_GRANULE) / SUBALLOC_GRANULE);
retry:
	mutex_lock(&suballoc->lock);
	granule_offs = bitmap_find_free_region(suballoc->granule_map,
					SUBALLOC_GRANULES, order);
	if (granule_offs < 0) {
		suballoc->free_space = 0;
		mutex_unlock(&suballoc->lock);
		ret = wait_event_interruptible_timeout(suballoc->free_event,
						       suballoc->free_space,
						       msecs_to_jiffies(10 * 1000));
		if (!ret) {
<<<<<<< HEAD
			dev_err(suballoc->gpu->dev,
=======
			dev_err(suballoc->dev,
>>>>>>> upstream/android-13
				"Timeout waiting for cmdbuf space\n");
			return -ETIMEDOUT;
		}
		goto retry;
	}
	mutex_unlock(&suballoc->lock);
	cmdbuf->suballoc_offset = granule_offs * SUBALLOC_GRANULE;
	cmdbuf->vaddr = suballoc->vaddr + cmdbuf->suballoc_offset;

	return 0;
}

void etnaviv_cmdbuf_free(struct etnaviv_cmdbuf *cmdbuf)
{
	struct etnaviv_cmdbuf_suballoc *suballoc = cmdbuf->suballoc;
	int order = order_base_2(ALIGN(cmdbuf->size, SUBALLOC_GRANULE) /
				 SUBALLOC_GRANULE);

	mutex_lock(&suballoc->lock);
	bitmap_release_region(suballoc->granule_map,
			      cmdbuf->suballoc_offset / SUBALLOC_GRANULE,
			      order);
	suballoc->free_space = 1;
	mutex_unlock(&suballoc->lock);
	wake_up_all(&suballoc->free_event);
}

<<<<<<< HEAD
u32 etnaviv_cmdbuf_get_va(struct etnaviv_cmdbuf *buf)
{
	return buf->suballoc->iova + buf->suballoc_offset;
=======
u32 etnaviv_cmdbuf_get_va(struct etnaviv_cmdbuf *buf,
			  struct etnaviv_vram_mapping *mapping)
{
	return mapping->iova + buf->suballoc_offset;
>>>>>>> upstream/android-13
}

dma_addr_t etnaviv_cmdbuf_get_pa(struct etnaviv_cmdbuf *buf)
{
	return buf->suballoc->paddr + buf->suballoc_offset;
}
