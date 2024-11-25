/*
 * Copyright (C) 2015 Red Hat, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

<<<<<<< HEAD
#include <linux/moduleparam.h>

#include <drm/ttm/ttm_execbuf_util.h>

=======
#include <linux/dma-mapping.h>
#include <linux/moduleparam.h>

>>>>>>> upstream/android-13
#include "virtgpu_drv.h"

static int virtio_gpu_virglrenderer_workaround = 1;
module_param_named(virglhack, virtio_gpu_virglrenderer_workaround, int, 0400);

<<<<<<< HEAD
static int virtio_gpu_resource_id_get(struct virtio_gpu_device *vgdev,
				       uint32_t *resid)
=======
int virtio_gpu_resource_id_get(struct virtio_gpu_device *vgdev, uint32_t *resid)
>>>>>>> upstream/android-13
{
	if (virtio_gpu_virglrenderer_workaround) {
		/*
		 * Hack to avoid re-using resource IDs.
		 *
		 * virglrenderer versions up to (and including) 0.7.0
		 * can't deal with that.  virglrenderer commit
		 * "f91a9dd35715 Fix unlinking resources from hash
		 * table." (Feb 2019) fixes the bug.
		 */
		static atomic_t seqno = ATOMIC_INIT(0);
		int handle = atomic_inc_return(&seqno);
		*resid = handle + 1;
	} else {
		int handle = ida_alloc(&vgdev->resource_ida, GFP_KERNEL);
		if (handle < 0)
			return handle;
		*resid = handle + 1;
	}
	return 0;
}

static void virtio_gpu_resource_id_put(struct virtio_gpu_device *vgdev, uint32_t id)
{
	if (!virtio_gpu_virglrenderer_workaround) {
		ida_free(&vgdev->resource_ida, id - 1);
	}
}

<<<<<<< HEAD
static void virtio_gpu_ttm_bo_destroy(struct ttm_buffer_object *tbo)
{
	struct virtio_gpu_object *bo;
	struct virtio_gpu_device *vgdev;

	bo = container_of(tbo, struct virtio_gpu_object, tbo);
	vgdev = (struct virtio_gpu_device *)bo->gem_base.dev->dev_private;

	if (bo->created)
		virtio_gpu_cmd_unref_resource(vgdev, bo->hw_res_handle);
	if (bo->pages)
		virtio_gpu_object_free_sg_table(bo);
	drm_gem_object_release(&bo->gem_base);
	virtio_gpu_resource_id_put(vgdev, bo->hw_res_handle);
	kfree(bo);
}

static void virtio_gpu_init_ttm_placement(struct virtio_gpu_object *vgbo)
{
	u32 c = 1;

	vgbo->placement.placement = &vgbo->placement_code;
	vgbo->placement.busy_placement = &vgbo->placement_code;
	vgbo->placement_code.fpfn = 0;
	vgbo->placement_code.lpfn = 0;
	vgbo->placement_code.flags =
		TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT |
		TTM_PL_FLAG_NO_EVICT;
	vgbo->placement.num_placement = c;
	vgbo->placement.num_busy_placement = c;

=======
void virtio_gpu_cleanup_object(struct virtio_gpu_object *bo)
{
	struct virtio_gpu_device *vgdev = bo->base.base.dev->dev_private;

	virtio_gpu_resource_id_put(vgdev, bo->hw_res_handle);
	if (virtio_gpu_is_shmem(bo)) {
		struct virtio_gpu_object_shmem *shmem = to_virtio_gpu_shmem(bo);

		if (shmem->pages) {
			if (shmem->mapped) {
				dma_unmap_sgtable(vgdev->vdev->dev.parent,
					     shmem->pages, DMA_TO_DEVICE, 0);
				shmem->mapped = 0;
			}

			sg_free_table(shmem->pages);
			kfree(shmem->pages);
			shmem->pages = NULL;
			drm_gem_shmem_unpin(&bo->base.base);
		}

		drm_gem_shmem_free_object(&bo->base.base);
	} else if (virtio_gpu_is_vram(bo)) {
		struct virtio_gpu_object_vram *vram = to_virtio_gpu_vram(bo);

		spin_lock(&vgdev->host_visible_lock);
		if (drm_mm_node_allocated(&vram->vram_node))
			drm_mm_remove_node(&vram->vram_node);

		spin_unlock(&vgdev->host_visible_lock);

		drm_gem_free_mmap_offset(&vram->base.base.base);
		drm_gem_object_release(&vram->base.base.base);
		kfree(vram);
	}
}

static void virtio_gpu_free_object(struct drm_gem_object *obj)
{
	struct virtio_gpu_object *bo = gem_to_virtio_gpu_obj(obj);
	struct virtio_gpu_device *vgdev = bo->base.base.dev->dev_private;

	if (bo->created) {
		virtio_gpu_cmd_unref_resource(vgdev, bo);
		virtio_gpu_notify(vgdev);
		/* completion handler calls virtio_gpu_cleanup_object() */
		return;
	}
	virtio_gpu_cleanup_object(bo);
}

static const struct drm_gem_object_funcs virtio_gpu_shmem_funcs = {
	.free = virtio_gpu_free_object,
	.open = virtio_gpu_gem_object_open,
	.close = virtio_gpu_gem_object_close,

	.print_info = drm_gem_shmem_print_info,
	.export = virtgpu_gem_prime_export,
	.pin = drm_gem_shmem_pin,
	.unpin = drm_gem_shmem_unpin,
	.get_sg_table = drm_gem_shmem_get_sg_table,
	.vmap = drm_gem_shmem_vmap,
	.vunmap = drm_gem_shmem_vunmap,
	.mmap = drm_gem_shmem_mmap,
};

bool virtio_gpu_is_shmem(struct virtio_gpu_object *bo)
{
	return bo->base.base.funcs == &virtio_gpu_shmem_funcs;
}

struct drm_gem_object *virtio_gpu_create_object(struct drm_device *dev,
						size_t size)
{
	struct virtio_gpu_object_shmem *shmem;
	struct drm_gem_shmem_object *dshmem;

	shmem = kzalloc(sizeof(*shmem), GFP_KERNEL);
	if (!shmem)
		return NULL;

	dshmem = &shmem->base.base;
	dshmem->base.funcs = &virtio_gpu_shmem_funcs;
	return &dshmem->base;
}

static int virtio_gpu_object_shmem_init(struct virtio_gpu_device *vgdev,
					struct virtio_gpu_object *bo,
					struct virtio_gpu_mem_entry **ents,
					unsigned int *nents)
{
	bool use_dma_api = !virtio_has_dma_quirk(vgdev->vdev);
	struct virtio_gpu_object_shmem *shmem = to_virtio_gpu_shmem(bo);
	struct scatterlist *sg;
	int si, ret;

	ret = drm_gem_shmem_pin(&bo->base.base);
	if (ret < 0)
		return -EINVAL;

	/*
	 * virtio_gpu uses drm_gem_shmem_get_sg_table instead of
	 * drm_gem_shmem_get_pages_sgt because virtio has it's own set of
	 * dma-ops. This is discouraged for other drivers, but should be fine
	 * since virtio_gpu doesn't support dma-buf import from other devices.
	 */
	shmem->pages = drm_gem_shmem_get_sg_table(&bo->base.base);
	if (!shmem->pages) {
		drm_gem_shmem_unpin(&bo->base.base);
		return -EINVAL;
	}

	if (use_dma_api) {
		ret = dma_map_sgtable(vgdev->vdev->dev.parent,
				      shmem->pages, DMA_TO_DEVICE, 0);
		if (ret)
			return ret;
		*nents = shmem->mapped = shmem->pages->nents;
	} else {
		*nents = shmem->pages->orig_nents;
	}

	*ents = kvmalloc_array(*nents,
			       sizeof(struct virtio_gpu_mem_entry),
			       GFP_KERNEL);
	if (!(*ents)) {
		DRM_ERROR("failed to allocate ent list\n");
		return -ENOMEM;
	}

	if (use_dma_api) {
		for_each_sgtable_dma_sg(shmem->pages, sg, si) {
			(*ents)[si].addr = cpu_to_le64(sg_dma_address(sg));
			(*ents)[si].length = cpu_to_le32(sg_dma_len(sg));
			(*ents)[si].padding = 0;
		}
	} else {
		for_each_sgtable_sg(shmem->pages, sg, si) {
			(*ents)[si].addr = cpu_to_le64(sg_phys(sg));
			(*ents)[si].length = cpu_to_le32(sg->length);
			(*ents)[si].padding = 0;
		}
	}

	return 0;
>>>>>>> upstream/android-13
}

int virtio_gpu_object_create(struct virtio_gpu_device *vgdev,
			     struct virtio_gpu_object_params *params,
			     struct virtio_gpu_object **bo_ptr,
			     struct virtio_gpu_fence *fence)
{
<<<<<<< HEAD
	struct virtio_gpu_object *bo;
	size_t acc_size;
=======
	struct virtio_gpu_object_array *objs = NULL;
	struct drm_gem_shmem_object *shmem_obj;
	struct virtio_gpu_object *bo;
	struct virtio_gpu_mem_entry *ents;
	unsigned int nents;
>>>>>>> upstream/android-13
	int ret;

	*bo_ptr = NULL;

<<<<<<< HEAD
	acc_size = ttm_bo_dma_acc_size(&vgdev->mman.bdev, params->size,
				       sizeof(struct virtio_gpu_object));

	bo = kzalloc(sizeof(struct virtio_gpu_object), GFP_KERNEL);
	if (bo == NULL)
		return -ENOMEM;
	ret = virtio_gpu_resource_id_get(vgdev, &bo->hw_res_handle);
	if (ret < 0) {
		kfree(bo);
		return ret;
	}
	params->size = roundup(params->size, PAGE_SIZE);
	ret = drm_gem_object_init(vgdev->ddev, &bo->gem_base, params->size);
	if (ret != 0) {
		virtio_gpu_resource_id_put(vgdev, bo->hw_res_handle);
		kfree(bo);
		return ret;
	}
	bo->dumb = params->dumb;

	if (params->virgl) {
		virtio_gpu_cmd_resource_create_3d(vgdev, bo, params, fence);
	} else {
		virtio_gpu_cmd_create_resource(vgdev, bo, params, fence);
	}

	virtio_gpu_init_ttm_placement(bo);
	ret = ttm_bo_init(&vgdev->mman.bdev, &bo->tbo, params->size,
			  ttm_bo_type_device, &bo->placement, 0,
			  true, acc_size, NULL, NULL,
			  &virtio_gpu_ttm_bo_destroy);
	/* ttm_bo_init failure will call the destroy */
	if (ret != 0)
		return ret;

	if (fence) {
		struct virtio_gpu_fence_driver *drv = &vgdev->fence_drv;
		struct list_head validate_list;
		struct ttm_validate_buffer mainbuf;
		struct ww_acquire_ctx ticket;
		unsigned long irq_flags;
		bool signaled;

		INIT_LIST_HEAD(&validate_list);
		memset(&mainbuf, 0, sizeof(struct ttm_validate_buffer));

		/* use a gem reference since unref list undoes them */
		drm_gem_object_get(&bo->gem_base);
		mainbuf.bo = &bo->tbo;
		list_add(&mainbuf.head, &validate_list);

		ret = virtio_gpu_object_list_validate(&ticket, &validate_list);
		if (ret == 0) {
			spin_lock_irqsave(&drv->lock, irq_flags);
			signaled = virtio_fence_signaled(&fence->f);
			if (!signaled)
				/* virtio create command still in flight */
				ttm_eu_fence_buffer_objects(&ticket, &validate_list,
							    &fence->f);
			spin_unlock_irqrestore(&drv->lock, irq_flags);
			if (signaled)
				/* virtio create command finished */
				ttm_eu_backoff_reservation(&ticket, &validate_list);
		}
		virtio_gpu_unref_list(&validate_list);
=======
	params->size = roundup(params->size, PAGE_SIZE);
	shmem_obj = drm_gem_shmem_create(vgdev->ddev, params->size);
	if (IS_ERR(shmem_obj))
		return PTR_ERR(shmem_obj);
	bo = gem_to_virtio_gpu_obj(&shmem_obj->base);

	ret = virtio_gpu_resource_id_get(vgdev, &bo->hw_res_handle);
	if (ret < 0)
		goto err_free_gem;

	bo->dumb = params->dumb;

	if (fence) {
		ret = -ENOMEM;
		objs = virtio_gpu_array_alloc(1);
		if (!objs)
			goto err_put_id;
		virtio_gpu_array_add_obj(objs, &bo->base.base);

		ret = virtio_gpu_array_lock_resv(objs);
		if (ret != 0)
			goto err_put_objs;
	}

	ret = virtio_gpu_object_shmem_init(vgdev, bo, &ents, &nents);
	if (ret != 0) {
		virtio_gpu_array_put_free(objs);
		virtio_gpu_free_object(&shmem_obj->base);
		return ret;
	}

	if (params->blob) {
		if (params->blob_mem == VIRTGPU_BLOB_MEM_GUEST)
			bo->guest_blob = true;

		virtio_gpu_cmd_resource_create_blob(vgdev, bo, params,
						    ents, nents);
	} else if (params->virgl) {
		virtio_gpu_cmd_resource_create_3d(vgdev, bo, params,
						  objs, fence);
		virtio_gpu_object_attach(vgdev, bo, ents, nents);
	} else {
		virtio_gpu_cmd_create_resource(vgdev, bo, params,
					       objs, fence);
		virtio_gpu_object_attach(vgdev, bo, ents, nents);
>>>>>>> upstream/android-13
	}

	*bo_ptr = bo;
	return 0;
<<<<<<< HEAD
}

void virtio_gpu_object_kunmap(struct virtio_gpu_object *bo)
{
	bo->vmap = NULL;
	ttm_bo_kunmap(&bo->kmap);
}

int virtio_gpu_object_kmap(struct virtio_gpu_object *bo)
{
	bool is_iomem;
	int r;

	WARN_ON(bo->vmap);

	r = ttm_bo_kmap(&bo->tbo, 0, bo->tbo.num_pages, &bo->kmap);
	if (r)
		return r;
	bo->vmap = ttm_kmap_obj_virtual(&bo->kmap, &is_iomem);
	return 0;
}

int virtio_gpu_object_get_sg_table(struct virtio_gpu_device *qdev,
				   struct virtio_gpu_object *bo)
{
	int ret;
	struct page **pages = bo->tbo.ttm->pages;
	int nr_pages = bo->tbo.num_pages;
	struct ttm_operation_ctx ctx = {
		.interruptible = false,
		.no_wait_gpu = false
	};

	/* wtf swapping */
	if (bo->pages)
		return 0;

	if (bo->tbo.ttm->state == tt_unpopulated)
		bo->tbo.ttm->bdev->driver->ttm_tt_populate(bo->tbo.ttm, &ctx);
	bo->pages = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!bo->pages)
		goto out;

	ret = sg_alloc_table_from_pages(bo->pages, pages, nr_pages, 0,
					nr_pages << PAGE_SHIFT, GFP_KERNEL);
	if (ret)
		goto out;
	return 0;
out:
	kfree(bo->pages);
	bo->pages = NULL;
	return -ENOMEM;
}

void virtio_gpu_object_free_sg_table(struct virtio_gpu_object *bo)
{
	sg_free_table(bo->pages);
	kfree(bo->pages);
	bo->pages = NULL;
}

int virtio_gpu_object_wait(struct virtio_gpu_object *bo, bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, NULL);
	if (unlikely(r != 0))
		return r;
	r = ttm_bo_wait(&bo->tbo, true, no_wait);
	ttm_bo_unreserve(&bo->tbo);
	return r;
}

=======

err_put_objs:
	virtio_gpu_array_put_free(objs);
err_put_id:
	virtio_gpu_resource_id_put(vgdev, bo->hw_res_handle);
err_free_gem:
	drm_gem_shmem_free_object(&shmem_obj->base);
	return ret;
}
>>>>>>> upstream/android-13
