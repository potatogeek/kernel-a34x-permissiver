// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2015-2018 Etnaviv Project
 */

<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>

>>>>>>> upstream/android-13
#include "common.xml.h"
#include "etnaviv_cmdbuf.h"
#include "etnaviv_drv.h"
#include "etnaviv_gem.h"
#include "etnaviv_gpu.h"
<<<<<<< HEAD
#include "etnaviv_iommu.h"
#include "etnaviv_mmu.h"

static void etnaviv_domain_unmap(struct etnaviv_iommu_domain *domain,
=======
#include "etnaviv_mmu.h"

static void etnaviv_context_unmap(struct etnaviv_iommu_context *context,
>>>>>>> upstream/android-13
				 unsigned long iova, size_t size)
{
	size_t unmapped_page, unmapped = 0;
	size_t pgsize = SZ_4K;

	if (!IS_ALIGNED(iova | size, pgsize)) {
		pr_err("unaligned: iova 0x%lx size 0x%zx min_pagesz 0x%zx\n",
		       iova, size, pgsize);
		return;
	}

	while (unmapped < size) {
<<<<<<< HEAD
		unmapped_page = domain->ops->unmap(domain, iova, pgsize);
=======
		unmapped_page = context->global->ops->unmap(context, iova,
							    pgsize);
>>>>>>> upstream/android-13
		if (!unmapped_page)
			break;

		iova += unmapped_page;
		unmapped += unmapped_page;
	}
}

<<<<<<< HEAD
static int etnaviv_domain_map(struct etnaviv_iommu_domain *domain,
=======
static int etnaviv_context_map(struct etnaviv_iommu_context *context,
>>>>>>> upstream/android-13
			      unsigned long iova, phys_addr_t paddr,
			      size_t size, int prot)
{
	unsigned long orig_iova = iova;
	size_t pgsize = SZ_4K;
	size_t orig_size = size;
	int ret = 0;

	if (!IS_ALIGNED(iova | paddr | size, pgsize)) {
		pr_err("unaligned: iova 0x%lx pa %pa size 0x%zx min_pagesz 0x%zx\n",
		       iova, &paddr, size, pgsize);
		return -EINVAL;
	}

	while (size) {
<<<<<<< HEAD
		ret = domain->ops->map(domain, iova, paddr, pgsize, prot);
=======
		ret = context->global->ops->map(context, iova, paddr, pgsize,
						prot);
>>>>>>> upstream/android-13
		if (ret)
			break;

		iova += pgsize;
		paddr += pgsize;
		size -= pgsize;
	}

	/* unroll mapping in case something went wrong */
	if (ret)
<<<<<<< HEAD
		etnaviv_domain_unmap(domain, orig_iova, orig_size - size);
=======
		etnaviv_context_unmap(context, orig_iova, orig_size - size);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int etnaviv_iommu_map(struct etnaviv_iommu *iommu, u32 iova,
			     struct sg_table *sgt, unsigned len, int prot)
{
	struct etnaviv_iommu_domain *domain = iommu->domain;
	struct scatterlist *sg;
	unsigned int da = iova;
	unsigned int i, j;
	int ret;

	if (!domain || !sgt)
		return -EINVAL;

	for_each_sg(sgt->sgl, sg, sgt->nents, i) {
=======
static int etnaviv_iommu_map(struct etnaviv_iommu_context *context, u32 iova,
			     struct sg_table *sgt, unsigned len, int prot)
{	struct scatterlist *sg;
	unsigned int da = iova;
	unsigned int i;
	int ret;

	if (!context || !sgt)
		return -EINVAL;

	for_each_sgtable_dma_sg(sgt, sg, i) {
>>>>>>> upstream/android-13
		u32 pa = sg_dma_address(sg) - sg->offset;
		size_t bytes = sg_dma_len(sg) + sg->offset;

		VERB("map[%d]: %08x %08x(%zx)", i, iova, pa, bytes);

<<<<<<< HEAD
		ret = etnaviv_domain_map(domain, da, pa, bytes, prot);
=======
		ret = etnaviv_context_map(context, da, pa, bytes, prot);
>>>>>>> upstream/android-13
		if (ret)
			goto fail;

		da += bytes;
	}

	return 0;

fail:
<<<<<<< HEAD
	da = iova;

	for_each_sg(sgt->sgl, sg, i, j) {
		size_t bytes = sg_dma_len(sg) + sg->offset;

		etnaviv_domain_unmap(domain, da, bytes);
		da += bytes;
	}
	return ret;
}

static void etnaviv_iommu_unmap(struct etnaviv_iommu *iommu, u32 iova,
				struct sg_table *sgt, unsigned len)
{
	struct etnaviv_iommu_domain *domain = iommu->domain;
=======
	etnaviv_context_unmap(context, iova, da - iova);
	return ret;
}

static void etnaviv_iommu_unmap(struct etnaviv_iommu_context *context, u32 iova,
				struct sg_table *sgt, unsigned len)
{
>>>>>>> upstream/android-13
	struct scatterlist *sg;
	unsigned int da = iova;
	int i;

<<<<<<< HEAD
	for_each_sg(sgt->sgl, sg, sgt->nents, i) {
		size_t bytes = sg_dma_len(sg) + sg->offset;

		etnaviv_domain_unmap(domain, da, bytes);
=======
	for_each_sgtable_dma_sg(sgt, sg, i) {
		size_t bytes = sg_dma_len(sg) + sg->offset;

		etnaviv_context_unmap(context, da, bytes);
>>>>>>> upstream/android-13

		VERB("unmap[%d]: %08x(%zx)", i, iova, bytes);

		BUG_ON(!PAGE_ALIGNED(bytes));

		da += bytes;
	}
}

<<<<<<< HEAD
static void etnaviv_iommu_remove_mapping(struct etnaviv_iommu *mmu,
=======
static void etnaviv_iommu_remove_mapping(struct etnaviv_iommu_context *context,
>>>>>>> upstream/android-13
	struct etnaviv_vram_mapping *mapping)
{
	struct etnaviv_gem_object *etnaviv_obj = mapping->object;

<<<<<<< HEAD
	etnaviv_iommu_unmap(mmu, mapping->vram_node.start,
=======
	lockdep_assert_held(&context->lock);

	etnaviv_iommu_unmap(context, mapping->vram_node.start,
>>>>>>> upstream/android-13
			    etnaviv_obj->sgt, etnaviv_obj->base.size);
	drm_mm_remove_node(&mapping->vram_node);
}

<<<<<<< HEAD
static int etnaviv_iommu_find_iova(struct etnaviv_iommu *mmu,
=======
static int etnaviv_iommu_find_iova(struct etnaviv_iommu_context *context,
>>>>>>> upstream/android-13
				   struct drm_mm_node *node, size_t size)
{
	struct etnaviv_vram_mapping *free = NULL;
	enum drm_mm_insert_mode mode = DRM_MM_INSERT_LOW;
	int ret;

<<<<<<< HEAD
	lockdep_assert_held(&mmu->lock);
=======
	lockdep_assert_held(&context->lock);
>>>>>>> upstream/android-13

	while (1) {
		struct etnaviv_vram_mapping *m, *n;
		struct drm_mm_scan scan;
		struct list_head list;
		bool found;

<<<<<<< HEAD
		ret = drm_mm_insert_node_in_range(&mmu->mm, node,
=======
		ret = drm_mm_insert_node_in_range(&context->mm, node,
>>>>>>> upstream/android-13
						  size, 0, 0, 0, U64_MAX, mode);
		if (ret != -ENOSPC)
			break;

		/* Try to retire some entries */
<<<<<<< HEAD
		drm_mm_scan_init(&scan, &mmu->mm, size, 0, 0, mode);

		found = 0;
		INIT_LIST_HEAD(&list);
		list_for_each_entry(free, &mmu->mappings, mmu_node) {
=======
		drm_mm_scan_init(&scan, &context->mm, size, 0, 0, mode);

		found = 0;
		INIT_LIST_HEAD(&list);
		list_for_each_entry(free, &context->mappings, mmu_node) {
>>>>>>> upstream/android-13
			/* If this vram node has not been used, skip this. */
			if (!free->vram_node.mm)
				continue;

			/*
			 * If the iova is pinned, then it's in-use,
			 * so we must keep its mapping.
			 */
			if (free->use)
				continue;

			list_add(&free->scan_node, &list);
			if (drm_mm_scan_add_block(&scan, &free->vram_node)) {
				found = true;
				break;
			}
		}

		if (!found) {
			/* Nothing found, clean up and fail */
			list_for_each_entry_safe(m, n, &list, scan_node)
				BUG_ON(drm_mm_scan_remove_block(&scan, &m->vram_node));
			break;
		}

		/*
		 * drm_mm does not allow any other operations while
		 * scanning, so we have to remove all blocks first.
		 * If drm_mm_scan_remove_block() returns false, we
		 * can leave the block pinned.
		 */
		list_for_each_entry_safe(m, n, &list, scan_node)
			if (!drm_mm_scan_remove_block(&scan, &m->vram_node))
				list_del_init(&m->scan_node);

		/*
		 * Unmap the blocks which need to be reaped from the MMU.
		 * Clear the mmu pointer to prevent the mapping_get finding
		 * this mapping.
		 */
		list_for_each_entry_safe(m, n, &list, scan_node) {
<<<<<<< HEAD
			etnaviv_iommu_remove_mapping(mmu, m);
			m->mmu = NULL;
=======
			etnaviv_iommu_remove_mapping(context, m);
			etnaviv_iommu_context_put(m->context);
			m->context = NULL;
>>>>>>> upstream/android-13
			list_del_init(&m->mmu_node);
			list_del_init(&m->scan_node);
		}

		mode = DRM_MM_INSERT_EVICT;

		/*
		 * We removed enough mappings so that the new allocation will
		 * succeed, retry the allocation one more time.
		 */
	}

	return ret;
}

<<<<<<< HEAD
int etnaviv_iommu_map_gem(struct etnaviv_iommu *mmu,
	struct etnaviv_gem_object *etnaviv_obj, u32 memory_base,
	struct etnaviv_vram_mapping *mapping)
=======
static int etnaviv_iommu_insert_exact(struct etnaviv_iommu_context *context,
		   struct drm_mm_node *node, size_t size, u64 va)
{
	lockdep_assert_held(&context->lock);

	return drm_mm_insert_node_in_range(&context->mm, node, size, 0, 0, va,
					   va + size, DRM_MM_INSERT_LOWEST);
}

int etnaviv_iommu_map_gem(struct etnaviv_iommu_context *context,
	struct etnaviv_gem_object *etnaviv_obj, u32 memory_base,
	struct etnaviv_vram_mapping *mapping, u64 va)
>>>>>>> upstream/android-13
{
	struct sg_table *sgt = etnaviv_obj->sgt;
	struct drm_mm_node *node;
	int ret;

	lockdep_assert_held(&etnaviv_obj->lock);

<<<<<<< HEAD
	mutex_lock(&mmu->lock);

	/* v1 MMU can optimize single entry (contiguous) scatterlists */
	if (mmu->version == ETNAVIV_IOMMU_V1 &&
=======
	mutex_lock(&context->lock);

	/* v1 MMU can optimize single entry (contiguous) scatterlists */
	if (context->global->version == ETNAVIV_IOMMU_V1 &&
>>>>>>> upstream/android-13
	    sgt->nents == 1 && !(etnaviv_obj->flags & ETNA_BO_FORCE_MMU)) {
		u32 iova;

		iova = sg_dma_address(sgt->sgl) - memory_base;
		if (iova < 0x80000000 - sg_dma_len(sgt->sgl)) {
			mapping->iova = iova;
<<<<<<< HEAD
			list_add_tail(&mapping->mmu_node, &mmu->mappings);
=======
			list_add_tail(&mapping->mmu_node, &context->mappings);
>>>>>>> upstream/android-13
			ret = 0;
			goto unlock;
		}
	}

	node = &mapping->vram_node;

<<<<<<< HEAD
	ret = etnaviv_iommu_find_iova(mmu, node, etnaviv_obj->base.size);
=======
	if (va)
		ret = etnaviv_iommu_insert_exact(context, node,
						 etnaviv_obj->base.size, va);
	else
		ret = etnaviv_iommu_find_iova(context, node,
					      etnaviv_obj->base.size);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto unlock;

	mapping->iova = node->start;
<<<<<<< HEAD
	ret = etnaviv_iommu_map(mmu, node->start, sgt, etnaviv_obj->base.size,
=======
	ret = etnaviv_iommu_map(context, node->start, sgt, etnaviv_obj->base.size,
>>>>>>> upstream/android-13
				ETNAVIV_PROT_READ | ETNAVIV_PROT_WRITE);

	if (ret < 0) {
		drm_mm_remove_node(node);
		goto unlock;
	}

<<<<<<< HEAD
	list_add_tail(&mapping->mmu_node, &mmu->mappings);
	mmu->flush_seq++;
unlock:
	mutex_unlock(&mmu->lock);
=======
	list_add_tail(&mapping->mmu_node, &context->mappings);
	context->flush_seq++;
unlock:
	mutex_unlock(&context->lock);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
void etnaviv_iommu_unmap_gem(struct etnaviv_iommu *mmu,
=======
void etnaviv_iommu_unmap_gem(struct etnaviv_iommu_context *context,
>>>>>>> upstream/android-13
	struct etnaviv_vram_mapping *mapping)
{
	WARN_ON(mapping->use);

<<<<<<< HEAD
	mutex_lock(&mmu->lock);

	/* If the vram node is on the mm, unmap and remove the node */
	if (mapping->vram_node.mm == &mmu->mm)
		etnaviv_iommu_remove_mapping(mmu, mapping);

	list_del(&mapping->mmu_node);
	mmu->flush_seq++;
	mutex_unlock(&mmu->lock);
}

void etnaviv_iommu_destroy(struct etnaviv_iommu *mmu)
{
	drm_mm_takedown(&mmu->mm);
	mmu->domain->ops->free(mmu->domain);
	kfree(mmu);
}

struct etnaviv_iommu *etnaviv_iommu_new(struct etnaviv_gpu *gpu)
{
	enum etnaviv_iommu_version version;
	struct etnaviv_iommu *mmu;

	mmu = kzalloc(sizeof(*mmu), GFP_KERNEL);
	if (!mmu)
		return ERR_PTR(-ENOMEM);

	if (!(gpu->identity.minor_features1 & chipMinorFeatures1_MMU_VERSION)) {
		mmu->domain = etnaviv_iommuv1_domain_alloc(gpu);
		version = ETNAVIV_IOMMU_V1;
	} else {
		mmu->domain = etnaviv_iommuv2_domain_alloc(gpu);
		version = ETNAVIV_IOMMU_V2;
	}

	if (!mmu->domain) {
		dev_err(gpu->dev, "Failed to allocate GPU IOMMU domain\n");
		kfree(mmu);
		return ERR_PTR(-ENOMEM);
	}

	mmu->gpu = gpu;
	mmu->version = version;
	mutex_init(&mmu->lock);
	INIT_LIST_HEAD(&mmu->mappings);

	drm_mm_init(&mmu->mm, mmu->domain->base, mmu->domain->size);

	return mmu;
}

void etnaviv_iommu_restore(struct etnaviv_gpu *gpu)
{
	if (gpu->mmu->version == ETNAVIV_IOMMU_V1)
		etnaviv_iommuv1_restore(gpu);
	else
		etnaviv_iommuv2_restore(gpu);
}

int etnaviv_iommu_get_suballoc_va(struct etnaviv_gpu *gpu, dma_addr_t paddr,
				  struct drm_mm_node *vram_node, size_t size,
				  u32 *iova)
{
	struct etnaviv_iommu *mmu = gpu->mmu;

	if (mmu->version == ETNAVIV_IOMMU_V1) {
		*iova = paddr - gpu->memory_base;
		return 0;
	} else {
		int ret;

		mutex_lock(&mmu->lock);
		ret = etnaviv_iommu_find_iova(mmu, vram_node, size);
		if (ret < 0) {
			mutex_unlock(&mmu->lock);
			return ret;
		}
		ret = etnaviv_domain_map(mmu->domain, vram_node->start, paddr,
					 size, ETNAVIV_PROT_READ);
		if (ret < 0) {
			drm_mm_remove_node(vram_node);
			mutex_unlock(&mmu->lock);
			return ret;
		}
		mmu->flush_seq++;
		mutex_unlock(&mmu->lock);

		*iova = (u32)vram_node->start;
		return 0;
	}
}

void etnaviv_iommu_put_suballoc_va(struct etnaviv_gpu *gpu,
				   struct drm_mm_node *vram_node, size_t size,
				   u32 iova)
{
	struct etnaviv_iommu *mmu = gpu->mmu;

	if (mmu->version == ETNAVIV_IOMMU_V2) {
		mutex_lock(&mmu->lock);
		etnaviv_domain_unmap(mmu->domain, iova, size);
		drm_mm_remove_node(vram_node);
		mutex_unlock(&mmu->lock);
	}
}
size_t etnaviv_iommu_dump_size(struct etnaviv_iommu *iommu)
{
	return iommu->domain->ops->dump_size(iommu->domain);
}

void etnaviv_iommu_dump(struct etnaviv_iommu *iommu, void *buf)
{
	iommu->domain->ops->dump(iommu->domain, buf);
=======
	mutex_lock(&context->lock);

	/* If the vram node is on the mm, unmap and remove the node */
	if (mapping->vram_node.mm == &context->mm)
		etnaviv_iommu_remove_mapping(context, mapping);

	list_del(&mapping->mmu_node);
	context->flush_seq++;
	mutex_unlock(&context->lock);
}

static void etnaviv_iommu_context_free(struct kref *kref)
{
	struct etnaviv_iommu_context *context =
		container_of(kref, struct etnaviv_iommu_context, refcount);

	etnaviv_cmdbuf_suballoc_unmap(context, &context->cmdbuf_mapping);

	context->global->ops->free(context);
}
void etnaviv_iommu_context_put(struct etnaviv_iommu_context *context)
{
	kref_put(&context->refcount, etnaviv_iommu_context_free);
}

struct etnaviv_iommu_context *
etnaviv_iommu_context_init(struct etnaviv_iommu_global *global,
			   struct etnaviv_cmdbuf_suballoc *suballoc)
{
	struct etnaviv_iommu_context *ctx;
	int ret;

	if (global->version == ETNAVIV_IOMMU_V1)
		ctx = etnaviv_iommuv1_context_alloc(global);
	else
		ctx = etnaviv_iommuv2_context_alloc(global);

	if (!ctx)
		return NULL;

	ret = etnaviv_cmdbuf_suballoc_map(suballoc, ctx, &ctx->cmdbuf_mapping,
					  global->memory_base);
	if (ret)
		goto out_free;

	if (global->version == ETNAVIV_IOMMU_V1 &&
	    ctx->cmdbuf_mapping.iova > 0x80000000) {
		dev_err(global->dev,
		        "command buffer outside valid memory window\n");
		goto out_unmap;
	}

	return ctx;

out_unmap:
	etnaviv_cmdbuf_suballoc_unmap(ctx, &ctx->cmdbuf_mapping);
out_free:
	global->ops->free(ctx);
	return NULL;
}

void etnaviv_iommu_restore(struct etnaviv_gpu *gpu,
			   struct etnaviv_iommu_context *context)
{
	context->global->ops->restore(gpu, context);
}

int etnaviv_iommu_get_suballoc_va(struct etnaviv_iommu_context *context,
				  struct etnaviv_vram_mapping *mapping,
				  u32 memory_base, dma_addr_t paddr,
				  size_t size)
{
	mutex_lock(&context->lock);

	if (mapping->use > 0) {
		mapping->use++;
		mutex_unlock(&context->lock);
		return 0;
	}

	/*
	 * For MMUv1 we don't add the suballoc region to the pagetables, as
	 * those GPUs can only work with cmdbufs accessed through the linear
	 * window. Instead we manufacture a mapping to make it look uniform
	 * to the upper layers.
	 */
	if (context->global->version == ETNAVIV_IOMMU_V1) {
		mapping->iova = paddr - memory_base;
	} else {
		struct drm_mm_node *node = &mapping->vram_node;
		int ret;

		ret = etnaviv_iommu_find_iova(context, node, size);
		if (ret < 0) {
			mutex_unlock(&context->lock);
			return ret;
		}

		mapping->iova = node->start;
		ret = etnaviv_context_map(context, node->start, paddr, size,
					  ETNAVIV_PROT_READ);
		if (ret < 0) {
			drm_mm_remove_node(node);
			mutex_unlock(&context->lock);
			return ret;
		}

		context->flush_seq++;
	}

	list_add_tail(&mapping->mmu_node, &context->mappings);
	mapping->use = 1;

	mutex_unlock(&context->lock);

	return 0;
}

void etnaviv_iommu_put_suballoc_va(struct etnaviv_iommu_context *context,
		  struct etnaviv_vram_mapping *mapping)
{
	struct drm_mm_node *node = &mapping->vram_node;

	mutex_lock(&context->lock);
	mapping->use--;

	if (mapping->use > 0 || context->global->version == ETNAVIV_IOMMU_V1) {
		mutex_unlock(&context->lock);
		return;
	}

	etnaviv_context_unmap(context, node->start, node->size);
	drm_mm_remove_node(node);
	mutex_unlock(&context->lock);
}

size_t etnaviv_iommu_dump_size(struct etnaviv_iommu_context *context)
{
	return context->global->ops->dump_size(context);
}

void etnaviv_iommu_dump(struct etnaviv_iommu_context *context, void *buf)
{
	context->global->ops->dump(context, buf);
}

int etnaviv_iommu_global_init(struct etnaviv_gpu *gpu)
{
	enum etnaviv_iommu_version version = ETNAVIV_IOMMU_V1;
	struct etnaviv_drm_private *priv = gpu->drm->dev_private;
	struct etnaviv_iommu_global *global;
	struct device *dev = gpu->drm->dev;

	if (gpu->identity.minor_features1 & chipMinorFeatures1_MMU_VERSION)
		version = ETNAVIV_IOMMU_V2;

	if (priv->mmu_global) {
		if (priv->mmu_global->version != version) {
			dev_err(gpu->dev,
				"MMU version doesn't match global version\n");
			return -ENXIO;
		}

		priv->mmu_global->use++;
		return 0;
	}

	global = kzalloc(sizeof(*global), GFP_KERNEL);
	if (!global)
		return -ENOMEM;

	global->bad_page_cpu = dma_alloc_wc(dev, SZ_4K, &global->bad_page_dma,
					    GFP_KERNEL);
	if (!global->bad_page_cpu)
		goto free_global;

	memset32(global->bad_page_cpu, 0xdead55aa, SZ_4K / sizeof(u32));

	if (version == ETNAVIV_IOMMU_V2) {
		global->v2.pta_cpu = dma_alloc_wc(dev, ETNAVIV_PTA_SIZE,
					       &global->v2.pta_dma, GFP_KERNEL);
		if (!global->v2.pta_cpu)
			goto free_bad_page;
	}

	global->dev = dev;
	global->version = version;
	global->use = 1;
	mutex_init(&global->lock);

	if (version == ETNAVIV_IOMMU_V1)
		global->ops = &etnaviv_iommuv1_ops;
	else
		global->ops = &etnaviv_iommuv2_ops;

	priv->mmu_global = global;

	return 0;

free_bad_page:
	dma_free_wc(dev, SZ_4K, global->bad_page_cpu, global->bad_page_dma);
free_global:
	kfree(global);

	return -ENOMEM;
}

void etnaviv_iommu_global_fini(struct etnaviv_gpu *gpu)
{
	struct etnaviv_drm_private *priv = gpu->drm->dev_private;
	struct etnaviv_iommu_global *global = priv->mmu_global;

	if (--global->use > 0)
		return;

	if (global->v2.pta_cpu)
		dma_free_wc(global->dev, ETNAVIV_PTA_SIZE,
			    global->v2.pta_cpu, global->v2.pta_dma);

	if (global->bad_page_cpu)
		dma_free_wc(global->dev, SZ_4K,
			    global->bad_page_cpu, global->bad_page_dma);

	mutex_destroy(&global->lock);
	kfree(global);

	priv->mmu_global = NULL;
>>>>>>> upstream/android-13
}
