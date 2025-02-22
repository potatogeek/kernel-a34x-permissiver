<<<<<<< HEAD
/*
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 * Author:Mark Yao <mark.yao@rock-chips.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <drm/drm.h>
#include <drm/drmP.h>
#include <drm/drm_gem.h>
#include <drm/drm_vma_manager.h>

#include <linux/dma-buf.h>
#include <linux/iommu.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 * Author:Mark Yao <mark.yao@rock-chips.com>
 */

#include <linux/dma-buf.h>
#include <linux/iommu.h>
#include <linux/vmalloc.h>

#include <drm/drm.h>
#include <drm/drm_gem.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_prime.h>
#include <drm/drm_vma_manager.h>
>>>>>>> upstream/android-13

#include "rockchip_drm_drv.h"
#include "rockchip_drm_gem.h"

static int rockchip_gem_iommu_map(struct rockchip_gem_object *rk_obj)
{
	struct drm_device *drm = rk_obj->base.dev;
	struct rockchip_drm_private *private = drm->dev_private;
	int prot = IOMMU_READ | IOMMU_WRITE;
	ssize_t ret;

	mutex_lock(&private->mm_lock);
	ret = drm_mm_insert_node_generic(&private->mm, &rk_obj->mm,
					 rk_obj->base.size, PAGE_SIZE,
					 0, 0);
	mutex_unlock(&private->mm_lock);

	if (ret < 0) {
		DRM_ERROR("out of I/O virtual memory: %zd\n", ret);
		return ret;
	}

	rk_obj->dma_addr = rk_obj->mm.start;

<<<<<<< HEAD
	ret = iommu_map_sg(private->domain, rk_obj->dma_addr, rk_obj->sgt->sgl,
			   rk_obj->sgt->nents, prot);
=======
	ret = iommu_map_sgtable(private->domain, rk_obj->dma_addr, rk_obj->sgt,
				prot);
>>>>>>> upstream/android-13
	if (ret < rk_obj->base.size) {
		DRM_ERROR("failed to map buffer: size=%zd request_size=%zd\n",
			  ret, rk_obj->base.size);
		ret = -ENOMEM;
		goto err_remove_node;
	}

	rk_obj->size = ret;

	return 0;

err_remove_node:
	mutex_lock(&private->mm_lock);
	drm_mm_remove_node(&rk_obj->mm);
	mutex_unlock(&private->mm_lock);

	return ret;
}

static int rockchip_gem_iommu_unmap(struct rockchip_gem_object *rk_obj)
{
	struct drm_device *drm = rk_obj->base.dev;
	struct rockchip_drm_private *private = drm->dev_private;

	iommu_unmap(private->domain, rk_obj->dma_addr, rk_obj->size);

	mutex_lock(&private->mm_lock);

	drm_mm_remove_node(&rk_obj->mm);

	mutex_unlock(&private->mm_lock);

	return 0;
}

static int rockchip_gem_get_pages(struct rockchip_gem_object *rk_obj)
{
	struct drm_device *drm = rk_obj->base.dev;
	int ret, i;
	struct scatterlist *s;

	rk_obj->pages = drm_gem_get_pages(&rk_obj->base);
	if (IS_ERR(rk_obj->pages))
		return PTR_ERR(rk_obj->pages);

	rk_obj->num_pages = rk_obj->base.size >> PAGE_SHIFT;

<<<<<<< HEAD
	rk_obj->sgt = drm_prime_pages_to_sg(rk_obj->pages, rk_obj->num_pages);
=======
	rk_obj->sgt = drm_prime_pages_to_sg(rk_obj->base.dev,
					    rk_obj->pages, rk_obj->num_pages);
>>>>>>> upstream/android-13
	if (IS_ERR(rk_obj->sgt)) {
		ret = PTR_ERR(rk_obj->sgt);
		goto err_put_pages;
	}

	/*
	 * Fake up the SG table so that dma_sync_sg_for_device() can be used
	 * to flush the pages associated with it.
	 *
	 * TODO: Replace this by drm_clflush_sg() once it can be implemented
	 * without relying on symbols that are not exported.
	 */
<<<<<<< HEAD
	for_each_sg(rk_obj->sgt->sgl, s, rk_obj->sgt->nents, i)
		sg_dma_address(s) = sg_phys(s);

	dma_sync_sg_for_device(drm->dev, rk_obj->sgt->sgl, rk_obj->sgt->nents,
			       DMA_TO_DEVICE);
=======
	for_each_sgtable_sg(rk_obj->sgt, s, i)
		sg_dma_address(s) = sg_phys(s);

	dma_sync_sgtable_for_device(drm->dev, rk_obj->sgt, DMA_TO_DEVICE);
>>>>>>> upstream/android-13

	return 0;

err_put_pages:
	drm_gem_put_pages(&rk_obj->base, rk_obj->pages, false, false);
	return ret;
}

static void rockchip_gem_put_pages(struct rockchip_gem_object *rk_obj)
{
	sg_free_table(rk_obj->sgt);
	kfree(rk_obj->sgt);
	drm_gem_put_pages(&rk_obj->base, rk_obj->pages, true, true);
}

static int rockchip_gem_alloc_iommu(struct rockchip_gem_object *rk_obj,
				    bool alloc_kmap)
{
	int ret;

	ret = rockchip_gem_get_pages(rk_obj);
	if (ret < 0)
		return ret;

	ret = rockchip_gem_iommu_map(rk_obj);
	if (ret < 0)
		goto err_free;

	if (alloc_kmap) {
		rk_obj->kvaddr = vmap(rk_obj->pages, rk_obj->num_pages, VM_MAP,
				      pgprot_writecombine(PAGE_KERNEL));
		if (!rk_obj->kvaddr) {
			DRM_ERROR("failed to vmap() buffer\n");
			ret = -ENOMEM;
			goto err_unmap;
		}
	}

	return 0;

err_unmap:
	rockchip_gem_iommu_unmap(rk_obj);
err_free:
	rockchip_gem_put_pages(rk_obj);

	return ret;
}

static int rockchip_gem_alloc_dma(struct rockchip_gem_object *rk_obj,
				  bool alloc_kmap)
{
	struct drm_gem_object *obj = &rk_obj->base;
	struct drm_device *drm = obj->dev;

	rk_obj->dma_attrs = DMA_ATTR_WRITE_COMBINE;

	if (!alloc_kmap)
		rk_obj->dma_attrs |= DMA_ATTR_NO_KERNEL_MAPPING;

	rk_obj->kvaddr = dma_alloc_attrs(drm->dev, obj->size,
					 &rk_obj->dma_addr, GFP_KERNEL,
					 rk_obj->dma_attrs);
	if (!rk_obj->kvaddr) {
		DRM_ERROR("failed to allocate %zu byte dma buffer", obj->size);
		return -ENOMEM;
	}

	return 0;
}

static int rockchip_gem_alloc_buf(struct rockchip_gem_object *rk_obj,
				  bool alloc_kmap)
{
	struct drm_gem_object *obj = &rk_obj->base;
	struct drm_device *drm = obj->dev;
	struct rockchip_drm_private *private = drm->dev_private;

	if (private->domain)
		return rockchip_gem_alloc_iommu(rk_obj, alloc_kmap);
	else
		return rockchip_gem_alloc_dma(rk_obj, alloc_kmap);
}

static void rockchip_gem_free_iommu(struct rockchip_gem_object *rk_obj)
{
	vunmap(rk_obj->kvaddr);
	rockchip_gem_iommu_unmap(rk_obj);
	rockchip_gem_put_pages(rk_obj);
}

static void rockchip_gem_free_dma(struct rockchip_gem_object *rk_obj)
{
	struct drm_gem_object *obj = &rk_obj->base;
	struct drm_device *drm = obj->dev;

	dma_free_attrs(drm->dev, obj->size, rk_obj->kvaddr, rk_obj->dma_addr,
		       rk_obj->dma_attrs);
}

static void rockchip_gem_free_buf(struct rockchip_gem_object *rk_obj)
{
	if (rk_obj->pages)
		rockchip_gem_free_iommu(rk_obj);
	else
		rockchip_gem_free_dma(rk_obj);
}

static int rockchip_drm_gem_object_mmap_iommu(struct drm_gem_object *obj,
					      struct vm_area_struct *vma)
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);
<<<<<<< HEAD
	unsigned int i, count = obj->size >> PAGE_SHIFT;
	unsigned long user_count = vma_pages(vma);
	unsigned long uaddr = vma->vm_start;
	unsigned long offset = vma->vm_pgoff;
	unsigned long end = user_count + offset;
	int ret;

	if (user_count == 0)
		return -ENXIO;
	if (end > count)
		return -ENXIO;

	for (i = offset; i < end; i++) {
		ret = vm_insert_page(vma, uaddr, rk_obj->pages[i]);
		if (ret)
			return ret;
		uaddr += PAGE_SIZE;
	}

	return 0;
=======
	unsigned int count = obj->size >> PAGE_SHIFT;
	unsigned long user_count = vma_pages(vma);

	if (user_count == 0)
		return -ENXIO;

	return vm_map_pages(vma, rk_obj->pages, count);
>>>>>>> upstream/android-13
}

static int rockchip_drm_gem_object_mmap_dma(struct drm_gem_object *obj,
					    struct vm_area_struct *vma)
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);
	struct drm_device *drm = obj->dev;

	return dma_mmap_attrs(drm->dev, vma, rk_obj->kvaddr, rk_obj->dma_addr,
			      obj->size, rk_obj->dma_attrs);
}

static int rockchip_drm_gem_object_mmap(struct drm_gem_object *obj,
					struct vm_area_struct *vma)
{
	int ret;
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);

	/*
	 * We allocated a struct page table for rk_obj, so clear
	 * VM_PFNMAP flag that was set by drm_gem_mmap_obj()/drm_gem_mmap().
	 */
	vma->vm_flags &= ~VM_PFNMAP;

	if (rk_obj->pages)
		ret = rockchip_drm_gem_object_mmap_iommu(obj, vma);
	else
		ret = rockchip_drm_gem_object_mmap_dma(obj, vma);

	if (ret)
		drm_gem_vm_close(vma);

	return ret;
}

int rockchip_gem_mmap_buf(struct drm_gem_object *obj,
			  struct vm_area_struct *vma)
{
	int ret;

	ret = drm_gem_mmap_obj(obj, obj->size, vma);
	if (ret)
		return ret;

	return rockchip_drm_gem_object_mmap(obj, vma);
}

/* drm driver mmap file operations */
int rockchip_gem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct drm_gem_object *obj;
	int ret;

	ret = drm_gem_mmap(filp, vma);
	if (ret)
		return ret;

	/*
	 * Set vm_pgoff (used as a fake buffer offset by DRM) to 0 and map the
	 * whole buffer from the start.
	 */
	vma->vm_pgoff = 0;

	obj = vma->vm_private_data;

	return rockchip_drm_gem_object_mmap(obj, vma);
}

static void rockchip_gem_release_object(struct rockchip_gem_object *rk_obj)
{
	drm_gem_object_release(&rk_obj->base);
	kfree(rk_obj);
}

<<<<<<< HEAD
struct rockchip_gem_object *
=======
static const struct drm_gem_object_funcs rockchip_gem_object_funcs = {
	.free = rockchip_gem_free_object,
	.get_sg_table = rockchip_gem_prime_get_sg_table,
	.vmap = rockchip_gem_prime_vmap,
	.vunmap	= rockchip_gem_prime_vunmap,
	.vm_ops = &drm_gem_cma_vm_ops,
};

static struct rockchip_gem_object *
>>>>>>> upstream/android-13
	rockchip_gem_alloc_object(struct drm_device *drm, unsigned int size)
{
	struct rockchip_gem_object *rk_obj;
	struct drm_gem_object *obj;

	size = round_up(size, PAGE_SIZE);

	rk_obj = kzalloc(sizeof(*rk_obj), GFP_KERNEL);
	if (!rk_obj)
		return ERR_PTR(-ENOMEM);

	obj = &rk_obj->base;

<<<<<<< HEAD
=======
	obj->funcs = &rockchip_gem_object_funcs;

>>>>>>> upstream/android-13
	drm_gem_object_init(drm, obj, size);

	return rk_obj;
}

struct rockchip_gem_object *
rockchip_gem_create_object(struct drm_device *drm, unsigned int size,
			   bool alloc_kmap)
{
	struct rockchip_gem_object *rk_obj;
	int ret;

	rk_obj = rockchip_gem_alloc_object(drm, size);
	if (IS_ERR(rk_obj))
		return rk_obj;

	ret = rockchip_gem_alloc_buf(rk_obj, alloc_kmap);
	if (ret)
		goto err_free_rk_obj;

	return rk_obj;

err_free_rk_obj:
	rockchip_gem_release_object(rk_obj);
	return ERR_PTR(ret);
}

/*
<<<<<<< HEAD
 * rockchip_gem_free_object - (struct drm_driver)->gem_free_object_unlocked
=======
 * rockchip_gem_free_object - (struct drm_gem_object_funcs)->free
>>>>>>> upstream/android-13
 * callback function
 */
void rockchip_gem_free_object(struct drm_gem_object *obj)
{
	struct drm_device *drm = obj->dev;
	struct rockchip_drm_private *private = drm->dev_private;
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);

	if (obj->import_attach) {
		if (private->domain) {
			rockchip_gem_iommu_unmap(rk_obj);
		} else {
<<<<<<< HEAD
			dma_unmap_sg(drm->dev, rk_obj->sgt->sgl,
				     rk_obj->sgt->nents, DMA_BIDIRECTIONAL);
=======
			dma_unmap_sgtable(drm->dev, rk_obj->sgt,
					  DMA_BIDIRECTIONAL, 0);
>>>>>>> upstream/android-13
		}
		drm_prime_gem_destroy(obj, rk_obj->sgt);
	} else {
		rockchip_gem_free_buf(rk_obj);
	}

	rockchip_gem_release_object(rk_obj);
}

/*
 * rockchip_gem_create_with_handle - allocate an object with the given
 * size and create a gem handle on it
 *
 * returns a struct rockchip_gem_object* on success or ERR_PTR values
 * on failure.
 */
static struct rockchip_gem_object *
rockchip_gem_create_with_handle(struct drm_file *file_priv,
				struct drm_device *drm, unsigned int size,
				unsigned int *handle)
{
	struct rockchip_gem_object *rk_obj;
	struct drm_gem_object *obj;
	int ret;

	rk_obj = rockchip_gem_create_object(drm, size, false);
	if (IS_ERR(rk_obj))
		return ERR_CAST(rk_obj);

	obj = &rk_obj->base;

	/*
	 * allocate a id of idr table where the obj is registered
	 * and handle has the id what user can see.
	 */
	ret = drm_gem_handle_create(file_priv, obj, handle);
	if (ret)
		goto err_handle_create;

	/* drop reference from allocate - handle holds it now. */
<<<<<<< HEAD
	drm_gem_object_put_unlocked(obj);
=======
	drm_gem_object_put(obj);
>>>>>>> upstream/android-13

	return rk_obj;

err_handle_create:
	rockchip_gem_free_object(obj);

	return ERR_PTR(ret);
}

/*
 * rockchip_gem_dumb_create - (struct drm_driver)->dumb_create callback
 * function
 *
 * This aligns the pitch and size arguments to the minimum required. wrap
 * this into your own function if you need bigger alignment.
 */
int rockchip_gem_dumb_create(struct drm_file *file_priv,
			     struct drm_device *dev,
			     struct drm_mode_create_dumb *args)
{
	struct rockchip_gem_object *rk_obj;
	int min_pitch = DIV_ROUND_UP(args->width * args->bpp, 8);

	/*
	 * align to 64 bytes since Mali requires it.
	 */
	args->pitch = ALIGN(min_pitch, 64);
	args->size = args->pitch * args->height;

	rk_obj = rockchip_gem_create_with_handle(file_priv, dev, args->size,
						 &args->handle);

	return PTR_ERR_OR_ZERO(rk_obj);
}

/*
 * Allocate a sg_table for this GEM object.
 * Note: Both the table's contents, and the sg_table itself must be freed by
 *       the caller.
 * Returns a pointer to the newly allocated sg_table, or an ERR_PTR() error.
 */
struct sg_table *rockchip_gem_prime_get_sg_table(struct drm_gem_object *obj)
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);
	struct drm_device *drm = obj->dev;
	struct sg_table *sgt;
	int ret;

	if (rk_obj->pages)
<<<<<<< HEAD
		return drm_prime_pages_to_sg(rk_obj->pages, rk_obj->num_pages);
=======
		return drm_prime_pages_to_sg(obj->dev, rk_obj->pages, rk_obj->num_pages);
>>>>>>> upstream/android-13

	sgt = kzalloc(sizeof(*sgt), GFP_KERNEL);
	if (!sgt)
		return ERR_PTR(-ENOMEM);

	ret = dma_get_sgtable_attrs(drm->dev, sgt, rk_obj->kvaddr,
				    rk_obj->dma_addr, obj->size,
				    rk_obj->dma_attrs);
	if (ret) {
		DRM_ERROR("failed to allocate sgt, %d\n", ret);
		kfree(sgt);
		return ERR_PTR(ret);
	}

	return sgt;
}

<<<<<<< HEAD
static unsigned long rockchip_sg_get_contiguous_size(struct sg_table *sgt,
						     int count)
{
	struct scatterlist *s;
	dma_addr_t expected = sg_dma_address(sgt->sgl);
	unsigned int i;
	unsigned long size = 0;

	for_each_sg(sgt->sgl, s, count, i) {
		if (sg_dma_address(s) != expected)
			break;
		expected = sg_dma_address(s) + sg_dma_len(s);
		size += sg_dma_len(s);
	}
	return size;
}

=======
>>>>>>> upstream/android-13
static int
rockchip_gem_iommu_map_sg(struct drm_device *drm,
			  struct dma_buf_attachment *attach,
			  struct sg_table *sg,
			  struct rockchip_gem_object *rk_obj)
{
	rk_obj->sgt = sg;
	return rockchip_gem_iommu_map(rk_obj);
}

static int
rockchip_gem_dma_map_sg(struct drm_device *drm,
			struct dma_buf_attachment *attach,
			struct sg_table *sg,
			struct rockchip_gem_object *rk_obj)
{
<<<<<<< HEAD
	int count = dma_map_sg(drm->dev, sg->sgl, sg->nents,
			       DMA_BIDIRECTIONAL);
	if (!count)
		return -EINVAL;

	if (rockchip_sg_get_contiguous_size(sg, count) < attach->dmabuf->size) {
		DRM_ERROR("failed to map sg_table to contiguous linear address.\n");
		dma_unmap_sg(drm->dev, sg->sgl, sg->nents,
			     DMA_BIDIRECTIONAL);
=======
	int err = dma_map_sgtable(drm->dev, sg, DMA_BIDIRECTIONAL, 0);
	if (err)
		return err;

	if (drm_prime_get_contiguous_size(sg) < attach->dmabuf->size) {
		DRM_ERROR("failed to map sg_table to contiguous linear address.\n");
		dma_unmap_sgtable(drm->dev, sg, DMA_BIDIRECTIONAL, 0);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	rk_obj->dma_addr = sg_dma_address(sg->sgl);
	rk_obj->sgt = sg;
	return 0;
}

struct drm_gem_object *
rockchip_gem_prime_import_sg_table(struct drm_device *drm,
				   struct dma_buf_attachment *attach,
				   struct sg_table *sg)
{
	struct rockchip_drm_private *private = drm->dev_private;
	struct rockchip_gem_object *rk_obj;
	int ret;

	rk_obj = rockchip_gem_alloc_object(drm, attach->dmabuf->size);
	if (IS_ERR(rk_obj))
		return ERR_CAST(rk_obj);

	if (private->domain)
		ret = rockchip_gem_iommu_map_sg(drm, attach, sg, rk_obj);
	else
		ret = rockchip_gem_dma_map_sg(drm, attach, sg, rk_obj);

	if (ret < 0) {
		DRM_ERROR("failed to import sg table: %d\n", ret);
		goto err_free_rk_obj;
	}

	return &rk_obj->base;

err_free_rk_obj:
	rockchip_gem_release_object(rk_obj);
	return ERR_PTR(ret);
}

<<<<<<< HEAD
void *rockchip_gem_prime_vmap(struct drm_gem_object *obj)
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);

	if (rk_obj->pages)
		return vmap(rk_obj->pages, rk_obj->num_pages, VM_MAP,
			    pgprot_writecombine(PAGE_KERNEL));

	if (rk_obj->dma_attrs & DMA_ATTR_NO_KERNEL_MAPPING)
		return NULL;

	return rk_obj->kvaddr;
}

void rockchip_gem_prime_vunmap(struct drm_gem_object *obj, void *vaddr)
=======
int rockchip_gem_prime_vmap(struct drm_gem_object *obj, struct dma_buf_map *map)
>>>>>>> upstream/android-13
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);

	if (rk_obj->pages) {
<<<<<<< HEAD
		vunmap(vaddr);
=======
		void *vaddr = vmap(rk_obj->pages, rk_obj->num_pages, VM_MAP,
				  pgprot_writecombine(PAGE_KERNEL));
		if (!vaddr)
			return -ENOMEM;
		dma_buf_map_set_vaddr(map, vaddr);
		return 0;
	}

	if (rk_obj->dma_attrs & DMA_ATTR_NO_KERNEL_MAPPING)
		return -ENOMEM;
	dma_buf_map_set_vaddr(map, rk_obj->kvaddr);

	return 0;
}

void rockchip_gem_prime_vunmap(struct drm_gem_object *obj, struct dma_buf_map *map)
{
	struct rockchip_gem_object *rk_obj = to_rockchip_obj(obj);

	if (rk_obj->pages) {
		vunmap(map->vaddr);
>>>>>>> upstream/android-13
		return;
	}

	/* Nothing to do if allocated by DMA mapping API. */
}
