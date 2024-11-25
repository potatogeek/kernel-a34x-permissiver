/*
 * Copyright 2009 Jerome Glisse.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors:
 *    Jerome Glisse <glisse@freedesktop.org>
 *    Thomas Hellstrom <thomas-at-tungstengraphics-dot-com>
 *    Dave Airlie
 */
<<<<<<< HEAD
#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_module.h>
#include <drm/ttm/ttm_page_alloc.h>
#include <drm/drmP.h>
#include <drm/radeon_drm.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/swiotlb.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/debugfs.h>
#include "radeon_reg.h"
#include "radeon.h"

#define DRM_FILE_PAGE_OFFSET (0x100000000ULL >> PAGE_SHIFT)

static int radeon_ttm_debugfs_init(struct radeon_device *rdev);
static void radeon_ttm_debugfs_fini(struct radeon_device *rdev);

static struct radeon_device *radeon_get_rdev(struct ttm_bo_device *bdev)
=======

#include <linux/dma-mapping.h>
#include <linux/pagemap.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/swiotlb.h>

#include <drm/drm_device.h>
#include <drm/drm_file.h>
#include <drm/drm_prime.h>
#include <drm/radeon_drm.h>
#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_range_manager.h>

#include "radeon_reg.h"
#include "radeon.h"
#include "radeon_ttm.h"

static void radeon_ttm_debugfs_init(struct radeon_device *rdev);

static int radeon_ttm_tt_bind(struct ttm_device *bdev, struct ttm_tt *ttm,
			      struct ttm_resource *bo_mem);
static void radeon_ttm_tt_unbind(struct ttm_device *bdev, struct ttm_tt *ttm);

struct radeon_device *radeon_get_rdev(struct ttm_device *bdev)
>>>>>>> upstream/android-13
{
	struct radeon_mman *mman;
	struct radeon_device *rdev;

	mman = container_of(bdev, struct radeon_mman, bdev);
	rdev = container_of(mman, struct radeon_device, mman);
	return rdev;
}

<<<<<<< HEAD

/*
 * Global memory.
 */
static int radeon_ttm_mem_global_init(struct drm_global_reference *ref)
{
	return ttm_mem_global_init(ref->object);
}

static void radeon_ttm_mem_global_release(struct drm_global_reference *ref)
{
	ttm_mem_global_release(ref->object);
}

static int radeon_ttm_global_init(struct radeon_device *rdev)
{
	struct drm_global_reference *global_ref;
	int r;

	rdev->mman.mem_global_referenced = false;
	global_ref = &rdev->mman.mem_global_ref;
	global_ref->global_type = DRM_GLOBAL_TTM_MEM;
	global_ref->size = sizeof(struct ttm_mem_global);
	global_ref->init = &radeon_ttm_mem_global_init;
	global_ref->release = &radeon_ttm_mem_global_release;
	r = drm_global_item_ref(global_ref);
	if (r != 0) {
		DRM_ERROR("Failed setting up TTM memory accounting "
			  "subsystem.\n");
		return r;
	}

	rdev->mman.bo_global_ref.mem_glob =
		rdev->mman.mem_global_ref.object;
	global_ref = &rdev->mman.bo_global_ref.ref;
	global_ref->global_type = DRM_GLOBAL_TTM_BO;
	global_ref->size = sizeof(struct ttm_bo_global);
	global_ref->init = &ttm_bo_global_init;
	global_ref->release = &ttm_bo_global_release;
	r = drm_global_item_ref(global_ref);
	if (r != 0) {
		DRM_ERROR("Failed setting up TTM BO subsystem.\n");
		drm_global_item_unref(&rdev->mman.mem_global_ref);
		return r;
	}

	rdev->mman.mem_global_referenced = true;
	return 0;
}

static void radeon_ttm_global_fini(struct radeon_device *rdev)
{
	if (rdev->mman.mem_global_referenced) {
		drm_global_item_unref(&rdev->mman.bo_global_ref.ref);
		drm_global_item_unref(&rdev->mman.mem_global_ref);
		rdev->mman.mem_global_referenced = false;
	}
}

static int radeon_invalidate_caches(struct ttm_bo_device *bdev, uint32_t flags)
{
	return 0;
}

static int radeon_init_mem_type(struct ttm_bo_device *bdev, uint32_t type,
				struct ttm_mem_type_manager *man)
{
	struct radeon_device *rdev;

	rdev = radeon_get_rdev(bdev);

	switch (type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case TTM_PL_TT:
		man->func = &ttm_bo_manager_func;
		man->gpu_offset = rdev->mc.gtt_start;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE | TTM_MEMTYPE_FLAG_CMA;
#if IS_ENABLED(CONFIG_AGP)
		if (rdev->flags & RADEON_IS_AGP) {
			if (!rdev->ddev->agp) {
				DRM_ERROR("AGP is not enabled for memory type %u\n",
					  (unsigned)type);
				return -EINVAL;
			}
			if (!rdev->ddev->agp->cant_use_aperture)
				man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
			man->available_caching = TTM_PL_FLAG_UNCACHED |
						 TTM_PL_FLAG_WC;
			man->default_caching = TTM_PL_FLAG_WC;
		}
#endif
		break;
	case TTM_PL_VRAM:
		/* "On-card" video ram */
		man->func = &ttm_bo_manager_func;
		man->gpu_offset = rdev->mc.vram_start;
		man->flags = TTM_MEMTYPE_FLAG_FIXED |
			     TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_UNCACHED | TTM_PL_FLAG_WC;
		man->default_caching = TTM_PL_FLAG_WC;
		break;
	default:
		DRM_ERROR("Unsupported memory type %u\n", (unsigned)type);
		return -EINVAL;
	}
	return 0;
=======
static int radeon_ttm_init_vram(struct radeon_device *rdev)
{
	return ttm_range_man_init(&rdev->mman.bdev, TTM_PL_VRAM,
				  false, rdev->mc.real_vram_size >> PAGE_SHIFT);
}

static int radeon_ttm_init_gtt(struct radeon_device *rdev)
{
	return ttm_range_man_init(&rdev->mman.bdev, TTM_PL_TT,
				  true, rdev->mc.gtt_size >> PAGE_SHIFT);
>>>>>>> upstream/android-13
}

static void radeon_evict_flags(struct ttm_buffer_object *bo,
				struct ttm_placement *placement)
{
	static const struct ttm_place placements = {
		.fpfn = 0,
		.lpfn = 0,
<<<<<<< HEAD
		.flags = TTM_PL_MASK_CACHING | TTM_PL_FLAG_SYSTEM
=======
		.mem_type = TTM_PL_SYSTEM,
		.flags = 0
>>>>>>> upstream/android-13
	};

	struct radeon_bo *rbo;

	if (!radeon_ttm_bo_is_radeon_bo(bo)) {
		placement->placement = &placements;
		placement->busy_placement = &placements;
		placement->num_placement = 1;
		placement->num_busy_placement = 1;
		return;
	}
	rbo = container_of(bo, struct radeon_bo, tbo);
<<<<<<< HEAD
	switch (bo->mem.mem_type) {
=======
	switch (bo->resource->mem_type) {
>>>>>>> upstream/android-13
	case TTM_PL_VRAM:
		if (rbo->rdev->ring[radeon_copy_ring_index(rbo->rdev)].ready == false)
			radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_CPU);
		else if (rbo->rdev->mc.visible_vram_size < rbo->rdev->mc.real_vram_size &&
<<<<<<< HEAD
			 bo->mem.start < (rbo->rdev->mc.visible_vram_size >> PAGE_SHIFT)) {
=======
			 bo->resource->start < (rbo->rdev->mc.visible_vram_size >> PAGE_SHIFT)) {
>>>>>>> upstream/android-13
			unsigned fpfn = rbo->rdev->mc.visible_vram_size >> PAGE_SHIFT;
			int i;

			/* Try evicting to the CPU inaccessible part of VRAM
			 * first, but only set GTT as busy placement, so this
			 * BO will be evicted to GTT rather than causing other
			 * BOs to be evicted from VRAM
			 */
			radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_VRAM |
							 RADEON_GEM_DOMAIN_GTT);
			rbo->placement.num_busy_placement = 0;
			for (i = 0; i < rbo->placement.num_placement; i++) {
<<<<<<< HEAD
				if (rbo->placements[i].flags & TTM_PL_FLAG_VRAM) {
=======
				if (rbo->placements[i].mem_type == TTM_PL_VRAM) {
>>>>>>> upstream/android-13
					if (rbo->placements[i].fpfn < fpfn)
						rbo->placements[i].fpfn = fpfn;
				} else {
					rbo->placement.busy_placement =
						&rbo->placements[i];
					rbo->placement.num_busy_placement = 1;
				}
			}
		} else
			radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_GTT);
		break;
	case TTM_PL_TT:
	default:
		radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_CPU);
	}
	*placement = rbo->placement;
}

<<<<<<< HEAD
static int radeon_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	struct radeon_bo *rbo = container_of(bo, struct radeon_bo, tbo);

	if (radeon_ttm_tt_has_userptr(bo->ttm))
		return -EPERM;
	return drm_vma_node_verify_access(&rbo->gem_base.vma_node,
					  filp->private_data);
}

static void radeon_move_null(struct ttm_buffer_object *bo,
			     struct ttm_mem_reg *new_mem)
{
	struct ttm_mem_reg *old_mem = &bo->mem;

	BUG_ON(old_mem->mm_node != NULL);
	*old_mem = *new_mem;
	new_mem->mm_node = NULL;
}

static int radeon_move_blit(struct ttm_buffer_object *bo,
			bool evict, bool no_wait_gpu,
			struct ttm_mem_reg *new_mem,
			struct ttm_mem_reg *old_mem)
=======
static int radeon_move_blit(struct ttm_buffer_object *bo,
			bool evict,
			struct ttm_resource *new_mem,
			struct ttm_resource *old_mem)
>>>>>>> upstream/android-13
{
	struct radeon_device *rdev;
	uint64_t old_start, new_start;
	struct radeon_fence *fence;
	unsigned num_pages;
	int r, ridx;

	rdev = radeon_get_rdev(bo->bdev);
	ridx = radeon_copy_ring_index(rdev);
	old_start = (u64)old_mem->start << PAGE_SHIFT;
	new_start = (u64)new_mem->start << PAGE_SHIFT;

	switch (old_mem->mem_type) {
	case TTM_PL_VRAM:
		old_start += rdev->mc.vram_start;
		break;
	case TTM_PL_TT:
		old_start += rdev->mc.gtt_start;
		break;
	default:
		DRM_ERROR("Unknown placement %d\n", old_mem->mem_type);
		return -EINVAL;
	}
	switch (new_mem->mem_type) {
	case TTM_PL_VRAM:
		new_start += rdev->mc.vram_start;
		break;
	case TTM_PL_TT:
		new_start += rdev->mc.gtt_start;
		break;
	default:
		DRM_ERROR("Unknown placement %d\n", old_mem->mem_type);
		return -EINVAL;
	}
	if (!rdev->ring[ridx].ready) {
		DRM_ERROR("Trying to move memory with ring turned off.\n");
		return -EINVAL;
	}

	BUILD_BUG_ON((PAGE_SIZE % RADEON_GPU_PAGE_SIZE) != 0);

	num_pages = new_mem->num_pages * (PAGE_SIZE / RADEON_GPU_PAGE_SIZE);
<<<<<<< HEAD
	fence = radeon_copy(rdev, old_start, new_start, num_pages, bo->resv);
	if (IS_ERR(fence))
		return PTR_ERR(fence);

	r = ttm_bo_move_accel_cleanup(bo, &fence->base, evict, new_mem);
=======
	fence = radeon_copy(rdev, old_start, new_start, num_pages, bo->base.resv);
	if (IS_ERR(fence))
		return PTR_ERR(fence);

	r = ttm_bo_move_accel_cleanup(bo, &fence->base, evict, false, new_mem);
>>>>>>> upstream/android-13
	radeon_fence_unref(&fence);
	return r;
}

<<<<<<< HEAD
static int radeon_move_vram_ram(struct ttm_buffer_object *bo,
				bool evict, bool interruptible,
				bool no_wait_gpu,
				struct ttm_mem_reg *new_mem)
{
	struct ttm_operation_ctx ctx = { interruptible, no_wait_gpu };
	struct radeon_device *rdev;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg tmp_mem;
	struct ttm_place placements;
	struct ttm_placement placement;
	int r;

	rdev = radeon_get_rdev(bo->bdev);
	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	placement.num_placement = 1;
	placement.placement = &placements;
	placement.num_busy_placement = 1;
	placement.busy_placement = &placements;
	placements.fpfn = 0;
	placements.lpfn = 0;
	placements.flags = TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT;
	r = ttm_bo_mem_space(bo, &placement, &tmp_mem, &ctx);
	if (unlikely(r)) {
		return r;
	}

	r = ttm_tt_set_placement_caching(bo->ttm, tmp_mem.placement);
	if (unlikely(r)) {
		goto out_cleanup;
	}

	r = ttm_tt_bind(bo->ttm, &tmp_mem, &ctx);
	if (unlikely(r)) {
		goto out_cleanup;
	}
	r = radeon_move_blit(bo, true, no_wait_gpu, &tmp_mem, old_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}
	r = ttm_bo_move_ttm(bo, &ctx, new_mem);
out_cleanup:
	ttm_bo_mem_put(bo, &tmp_mem);
	return r;
}

static int radeon_move_ram_vram(struct ttm_buffer_object *bo,
				bool evict, bool interruptible,
				bool no_wait_gpu,
				struct ttm_mem_reg *new_mem)
{
	struct ttm_operation_ctx ctx = { interruptible, no_wait_gpu };
	struct radeon_device *rdev;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg tmp_mem;
	struct ttm_placement placement;
	struct ttm_place placements;
	int r;

	rdev = radeon_get_rdev(bo->bdev);
	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	placement.num_placement = 1;
	placement.placement = &placements;
	placement.num_busy_placement = 1;
	placement.busy_placement = &placements;
	placements.fpfn = 0;
	placements.lpfn = 0;
	placements.flags = TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT;
	r = ttm_bo_mem_space(bo, &placement, &tmp_mem, &ctx);
	if (unlikely(r)) {
		return r;
	}
	r = ttm_bo_move_ttm(bo, &ctx, &tmp_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}
	r = radeon_move_blit(bo, true, no_wait_gpu, new_mem, old_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}
out_cleanup:
	ttm_bo_mem_put(bo, &tmp_mem);
	return r;
}

static int radeon_bo_move(struct ttm_buffer_object *bo, bool evict,
			  struct ttm_operation_ctx *ctx,
			  struct ttm_mem_reg *new_mem)
{
	struct radeon_device *rdev;
	struct radeon_bo *rbo;
	struct ttm_mem_reg *old_mem = &bo->mem;
	int r;

	r = ttm_bo_wait(bo, ctx->interruptible, ctx->no_wait_gpu);
=======
static int radeon_bo_move(struct ttm_buffer_object *bo, bool evict,
			  struct ttm_operation_ctx *ctx,
			  struct ttm_resource *new_mem,
			  struct ttm_place *hop)
{
	struct ttm_resource *old_mem = bo->resource;
	struct radeon_device *rdev;
	struct radeon_bo *rbo;
	int r, old_type;

	if (new_mem->mem_type == TTM_PL_TT) {
		r = radeon_ttm_tt_bind(bo->bdev, bo->ttm, new_mem);
		if (r)
			return r;
	}

	r = ttm_bo_wait_ctx(bo, ctx);
>>>>>>> upstream/android-13
	if (r)
		return r;

	/* Can't move a pinned BO */
	rbo = container_of(bo, struct radeon_bo, tbo);
<<<<<<< HEAD
	if (WARN_ON_ONCE(rbo->pin_count > 0))
		return -EINVAL;

	rdev = radeon_get_rdev(bo->bdev);
	if (old_mem->mem_type == TTM_PL_SYSTEM && bo->ttm == NULL) {
		radeon_move_null(bo, new_mem);
		return 0;
	}
	if ((old_mem->mem_type == TTM_PL_TT &&
	     new_mem->mem_type == TTM_PL_SYSTEM) ||
	    (old_mem->mem_type == TTM_PL_SYSTEM &&
	     new_mem->mem_type == TTM_PL_TT)) {
		/* bind is enough */
		radeon_move_null(bo, new_mem);
		return 0;
	}
	if (!rdev->ring[radeon_copy_ring_index(rdev)].ready ||
	    rdev->asic->copy.copy == NULL) {
		/* use memcpy */
		goto memcpy;
	}

	if (old_mem->mem_type == TTM_PL_VRAM &&
	    new_mem->mem_type == TTM_PL_SYSTEM) {
		r = radeon_move_vram_ram(bo, evict, ctx->interruptible,
					ctx->no_wait_gpu, new_mem);
	} else if (old_mem->mem_type == TTM_PL_SYSTEM &&
		   new_mem->mem_type == TTM_PL_VRAM) {
		r = radeon_move_ram_vram(bo, evict, ctx->interruptible,
					    ctx->no_wait_gpu, new_mem);
	} else {
		r = radeon_move_blit(bo, evict, ctx->no_wait_gpu,
				     new_mem, old_mem);
	}

	if (r) {
memcpy:
		r = ttm_bo_move_memcpy(bo, ctx, new_mem);
		if (r) {
			return r;
		}
	}

	/* update statistics */
	atomic64_add((u64)bo->num_pages << PAGE_SHIFT, &rdev->num_bytes_moved);
	return 0;
}

static int radeon_ttm_io_mem_reserve(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	struct radeon_device *rdev = radeon_get_rdev(bdev);

	mem->bus.addr = NULL;
	mem->bus.offset = 0;
	mem->bus.size = mem->num_pages << PAGE_SHIFT;
	mem->bus.base = 0;
	mem->bus.is_iomem = false;
	if (!(man->flags & TTM_MEMTYPE_FLAG_MAPPABLE))
		return -EINVAL;
=======
	if (WARN_ON_ONCE(rbo->tbo.pin_count > 0))
		return -EINVAL;

	/* Save old type for statistics update */
	old_type = old_mem->mem_type;

	rdev = radeon_get_rdev(bo->bdev);
	if (old_mem->mem_type == TTM_PL_SYSTEM && bo->ttm == NULL) {
		ttm_bo_move_null(bo, new_mem);
		goto out;
	}
	if (old_mem->mem_type == TTM_PL_SYSTEM &&
	    new_mem->mem_type == TTM_PL_TT) {
		ttm_bo_move_null(bo, new_mem);
		goto out;
	}

	if (old_mem->mem_type == TTM_PL_TT &&
	    new_mem->mem_type == TTM_PL_SYSTEM) {
		radeon_ttm_tt_unbind(bo->bdev, bo->ttm);
		ttm_resource_free(bo, &bo->resource);
		ttm_bo_assign_mem(bo, new_mem);
		goto out;
	}
	if (rdev->ring[radeon_copy_ring_index(rdev)].ready &&
	    rdev->asic->copy.copy != NULL) {
		if ((old_mem->mem_type == TTM_PL_SYSTEM &&
		     new_mem->mem_type == TTM_PL_VRAM) ||
		    (old_mem->mem_type == TTM_PL_VRAM &&
		     new_mem->mem_type == TTM_PL_SYSTEM)) {
			hop->fpfn = 0;
			hop->lpfn = 0;
			hop->mem_type = TTM_PL_TT;
			hop->flags = 0;
			return -EMULTIHOP;
		}

		r = radeon_move_blit(bo, evict, new_mem, old_mem);
	} else {
		r = -ENODEV;
	}

	if (r) {
		r = ttm_bo_move_memcpy(bo, ctx, new_mem);
		if (r)
			return r;
	}

out:
	/* update statistics */
	atomic64_add(bo->base.size, &rdev->num_bytes_moved);
	radeon_bo_move_notify(bo, old_type, new_mem);
	return 0;
}

static int radeon_ttm_io_mem_reserve(struct ttm_device *bdev, struct ttm_resource *mem)
{
	struct radeon_device *rdev = radeon_get_rdev(bdev);
	size_t bus_size = (size_t)mem->num_pages << PAGE_SHIFT;

>>>>>>> upstream/android-13
	switch (mem->mem_type) {
	case TTM_PL_SYSTEM:
		/* system memory */
		return 0;
	case TTM_PL_TT:
#if IS_ENABLED(CONFIG_AGP)
		if (rdev->flags & RADEON_IS_AGP) {
			/* RADEON_IS_AGP is set only if AGP is active */
<<<<<<< HEAD
			mem->bus.offset = mem->start << PAGE_SHIFT;
			mem->bus.base = rdev->mc.agp_base;
			mem->bus.is_iomem = !rdev->ddev->agp->cant_use_aperture;
=======
			mem->bus.offset = (mem->start << PAGE_SHIFT) +
				rdev->mc.agp_base;
			mem->bus.is_iomem = !rdev->agp->cant_use_aperture;
			mem->bus.caching = ttm_write_combined;
>>>>>>> upstream/android-13
		}
#endif
		break;
	case TTM_PL_VRAM:
		mem->bus.offset = mem->start << PAGE_SHIFT;
		/* check if it's visible */
<<<<<<< HEAD
		if ((mem->bus.offset + mem->bus.size) > rdev->mc.visible_vram_size)
			return -EINVAL;
		mem->bus.base = rdev->mc.aper_base;
		mem->bus.is_iomem = true;
=======
		if ((mem->bus.offset + bus_size) > rdev->mc.visible_vram_size)
			return -EINVAL;
		mem->bus.offset += rdev->mc.aper_base;
		mem->bus.is_iomem = true;
		mem->bus.caching = ttm_write_combined;
>>>>>>> upstream/android-13
#ifdef __alpha__
		/*
		 * Alpha: use bus.addr to hold the ioremap() return,
		 * so we can modify bus.base below.
		 */
<<<<<<< HEAD
		if (mem->placement & TTM_PL_FLAG_WC)
			mem->bus.addr =
				ioremap_wc(mem->bus.base + mem->bus.offset,
					   mem->bus.size);
		else
			mem->bus.addr =
				ioremap_nocache(mem->bus.base + mem->bus.offset,
						mem->bus.size);
=======
		mem->bus.addr = ioremap_wc(mem->bus.offset, bus_size);
>>>>>>> upstream/android-13
		if (!mem->bus.addr)
			return -ENOMEM;

		/*
		 * Alpha: Use just the bus offset plus
		 * the hose/domain memory base for bus.base.
		 * It then can be used to build PTEs for VRAM
		 * access, as done in ttm_bo_vm_fault().
		 */
<<<<<<< HEAD
		mem->bus.base = (mem->bus.base & 0x0ffffffffUL) +
			rdev->ddev->hose->dense_mem_base;
=======
		mem->bus.offset = (mem->bus.offset & 0x0ffffffffUL) +
			rdev->hose->dense_mem_base;
>>>>>>> upstream/android-13
#endif
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

<<<<<<< HEAD
static void radeon_ttm_io_mem_free(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
}

=======
>>>>>>> upstream/android-13
/*
 * TTM backend functions.
 */
struct radeon_ttm_tt {
<<<<<<< HEAD
	struct ttm_dma_tt		ttm;
	struct radeon_device		*rdev;
=======
	struct ttm_tt		ttm;
>>>>>>> upstream/android-13
	u64				offset;

	uint64_t			userptr;
	struct mm_struct		*usermm;
	uint32_t			userflags;
<<<<<<< HEAD
};

/* prepare the sg table with the user pages */
static int radeon_ttm_tt_pin_userptr(struct ttm_tt *ttm)
{
	struct radeon_device *rdev = radeon_get_rdev(ttm->bdev);
	struct radeon_ttm_tt *gtt = (void *)ttm;
	unsigned pinned = 0, nents;
=======
	bool bound;
};

/* prepare the sg table with the user pages */
static int radeon_ttm_tt_pin_userptr(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct radeon_device *rdev = radeon_get_rdev(bdev);
	struct radeon_ttm_tt *gtt = (void *)ttm;
	unsigned pinned = 0;
>>>>>>> upstream/android-13
	int r;

	int write = !(gtt->userflags & RADEON_GEM_USERPTR_READONLY);
	enum dma_data_direction direction = write ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;

	if (current->mm != gtt->usermm)
		return -EPERM;

	if (gtt->userflags & RADEON_GEM_USERPTR_ANONONLY) {
		/* check that we only pin down anonymous memory
		   to prevent problems with writeback */
<<<<<<< HEAD
		unsigned long end = gtt->userptr + ttm->num_pages * PAGE_SIZE;
=======
		unsigned long end = gtt->userptr + (u64)ttm->num_pages * PAGE_SIZE;
>>>>>>> upstream/android-13
		struct vm_area_struct *vma;
		vma = find_vma(gtt->usermm, gtt->userptr);
		if (!vma || vma->vm_file || vma->vm_end < end)
			return -EPERM;
	}

	do {
		unsigned num_pages = ttm->num_pages - pinned;
		uint64_t userptr = gtt->userptr + pinned * PAGE_SIZE;
		struct page **pages = ttm->pages + pinned;

		r = get_user_pages(userptr, num_pages, write ? FOLL_WRITE : 0,
				   pages, NULL);
		if (r < 0)
			goto release_pages;

		pinned += r;

	} while (pinned < ttm->num_pages);

	r = sg_alloc_table_from_pages(ttm->sg, ttm->pages, ttm->num_pages, 0,
<<<<<<< HEAD
				      ttm->num_pages << PAGE_SHIFT,
=======
				      (u64)ttm->num_pages << PAGE_SHIFT,
>>>>>>> upstream/android-13
				      GFP_KERNEL);
	if (r)
		goto release_sg;

<<<<<<< HEAD
	r = -ENOMEM;
	nents = dma_map_sg(rdev->dev, ttm->sg->sgl, ttm->sg->nents, direction);
	if (nents != ttm->sg->nents)
		goto release_sg;

	drm_prime_sg_to_page_addr_arrays(ttm->sg, ttm->pages,
					 gtt->ttm.dma_address, ttm->num_pages);
=======
	r = dma_map_sgtable(rdev->dev, ttm->sg, direction, 0);
	if (r)
		goto release_sg;

	drm_prime_sg_to_dma_addr_array(ttm->sg, gtt->ttm.dma_address,
				       ttm->num_pages);
>>>>>>> upstream/android-13

	return 0;

release_sg:
	kfree(ttm->sg);

release_pages:
	release_pages(ttm->pages, pinned);
	return r;
}

<<<<<<< HEAD
static void radeon_ttm_tt_unpin_userptr(struct ttm_tt *ttm)
{
	struct radeon_device *rdev = radeon_get_rdev(ttm->bdev);
=======
static void radeon_ttm_tt_unpin_userptr(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct radeon_device *rdev = radeon_get_rdev(bdev);
>>>>>>> upstream/android-13
	struct radeon_ttm_tt *gtt = (void *)ttm;
	struct sg_page_iter sg_iter;

	int write = !(gtt->userflags & RADEON_GEM_USERPTR_READONLY);
	enum dma_data_direction direction = write ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;

	/* double check that we don't free the table twice */
<<<<<<< HEAD
	if (!ttm->sg->sgl)
		return;

	/* free the sg table and pages again */
	dma_unmap_sg(rdev->dev, ttm->sg->sgl, ttm->sg->nents, direction);

	for_each_sg_page(ttm->sg->sgl, &sg_iter, ttm->sg->nents, 0) {
=======
	if (!ttm->sg || !ttm->sg->sgl)
		return;

	/* free the sg table and pages again */
	dma_unmap_sgtable(rdev->dev, ttm->sg, direction, 0);

	for_each_sgtable_page(ttm->sg, &sg_iter, 0) {
>>>>>>> upstream/android-13
		struct page *page = sg_page_iter_page(&sg_iter);
		if (!(gtt->userflags & RADEON_GEM_USERPTR_READONLY))
			set_page_dirty(page);

		mark_page_accessed(page);
		put_page(page);
	}

	sg_free_table(ttm->sg);
}

<<<<<<< HEAD
static int radeon_ttm_backend_bind(struct ttm_tt *ttm,
				   struct ttm_mem_reg *bo_mem)
{
	struct radeon_ttm_tt *gtt = (void*)ttm;
=======
static bool radeon_ttm_backend_is_bound(struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = (void*)ttm;

	return (gtt->bound);
}

static int radeon_ttm_backend_bind(struct ttm_device *bdev,
				   struct ttm_tt *ttm,
				   struct ttm_resource *bo_mem)
{
	struct radeon_ttm_tt *gtt = (void*)ttm;
	struct radeon_device *rdev = radeon_get_rdev(bdev);
>>>>>>> upstream/android-13
	uint32_t flags = RADEON_GART_PAGE_VALID | RADEON_GART_PAGE_READ |
		RADEON_GART_PAGE_WRITE;
	int r;

<<<<<<< HEAD
	if (gtt->userptr) {
		radeon_ttm_tt_pin_userptr(ttm);
=======
	if (gtt->bound)
		return 0;

	if (gtt->userptr) {
		radeon_ttm_tt_pin_userptr(bdev, ttm);
>>>>>>> upstream/android-13
		flags &= ~RADEON_GART_PAGE_WRITE;
	}

	gtt->offset = (unsigned long)(bo_mem->start << PAGE_SHIFT);
	if (!ttm->num_pages) {
<<<<<<< HEAD
		WARN(1, "nothing to bind %lu pages for mreg %p back %p!\n",
		     ttm->num_pages, bo_mem, ttm);
	}
	if (ttm->caching_state == tt_cached)
		flags |= RADEON_GART_PAGE_SNOOP;
	r = radeon_gart_bind(gtt->rdev, gtt->offset, ttm->num_pages,
			     ttm->pages, gtt->ttm.dma_address, flags);
	if (r) {
		DRM_ERROR("failed to bind %lu pages at 0x%08X\n",
			  ttm->num_pages, (unsigned)gtt->offset);
		return r;
	}
	return 0;
}

static int radeon_ttm_backend_unbind(struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = (void *)ttm;

	radeon_gart_unbind(gtt->rdev, gtt->offset, ttm->num_pages);

	if (gtt->userptr)
		radeon_ttm_tt_unpin_userptr(ttm);

	return 0;
}

static void radeon_ttm_backend_destroy(struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = (void *)ttm;

	ttm_dma_tt_fini(&gtt->ttm);
	kfree(gtt);
}

static struct ttm_backend_func radeon_backend_func = {
	.bind = &radeon_ttm_backend_bind,
	.unbind = &radeon_ttm_backend_unbind,
	.destroy = &radeon_ttm_backend_destroy,
};

static struct ttm_tt *radeon_ttm_tt_create(struct ttm_buffer_object *bo,
					   uint32_t page_flags)
{
	struct radeon_device *rdev;
	struct radeon_ttm_tt *gtt;

	rdev = radeon_get_rdev(bo->bdev);
#if IS_ENABLED(CONFIG_AGP)
	if (rdev->flags & RADEON_IS_AGP) {
		return ttm_agp_tt_create(bo, rdev->ddev->agp->bridge,
					 page_flags);
	}
#endif
=======
		WARN(1, "nothing to bind %u pages for mreg %p back %p!\n",
		     ttm->num_pages, bo_mem, ttm);
	}
	if (ttm->caching == ttm_cached)
		flags |= RADEON_GART_PAGE_SNOOP;
	r = radeon_gart_bind(rdev, gtt->offset, ttm->num_pages,
			     ttm->pages, gtt->ttm.dma_address, flags);
	if (r) {
		DRM_ERROR("failed to bind %u pages at 0x%08X\n",
			  ttm->num_pages, (unsigned)gtt->offset);
		return r;
	}
	gtt->bound = true;
	return 0;
}

static void radeon_ttm_backend_unbind(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = (void *)ttm;
	struct radeon_device *rdev = radeon_get_rdev(bdev);

	if (gtt->userptr)
		radeon_ttm_tt_unpin_userptr(bdev, ttm);

	if (!gtt->bound)
		return;

	radeon_gart_unbind(rdev, gtt->offset, ttm->num_pages);

	gtt->bound = false;
}

static void radeon_ttm_backend_destroy(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = (void *)ttm;

	radeon_ttm_backend_unbind(bdev, ttm);
	ttm_tt_destroy_common(bdev, ttm);

	ttm_tt_fini(&gtt->ttm);
	kfree(gtt);
}

static struct ttm_tt *radeon_ttm_tt_create(struct ttm_buffer_object *bo,
					   uint32_t page_flags)
{
	struct radeon_ttm_tt *gtt;
	enum ttm_caching caching;
	struct radeon_bo *rbo;
#if IS_ENABLED(CONFIG_AGP)
	struct radeon_device *rdev = radeon_get_rdev(bo->bdev);

	if (rdev->flags & RADEON_IS_AGP) {
		return ttm_agp_tt_create(bo, rdev->agp->bridge, page_flags);
	}
#endif
	rbo = container_of(bo, struct radeon_bo, tbo);
>>>>>>> upstream/android-13

	gtt = kzalloc(sizeof(struct radeon_ttm_tt), GFP_KERNEL);
	if (gtt == NULL) {
		return NULL;
	}
<<<<<<< HEAD
	gtt->ttm.ttm.func = &radeon_backend_func;
	gtt->rdev = rdev;
	if (ttm_dma_tt_init(&gtt->ttm, bo, page_flags)) {
		kfree(gtt);
		return NULL;
	}
	return &gtt->ttm.ttm;
}

static struct radeon_ttm_tt *radeon_ttm_tt_to_gtt(struct ttm_tt *ttm)
{
	if (!ttm || ttm->func != &radeon_backend_func)
		return NULL;
	return (struct radeon_ttm_tt *)ttm;
}

static int radeon_ttm_tt_populate(struct ttm_tt *ttm,
			struct ttm_operation_ctx *ctx)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(ttm);
	struct radeon_device *rdev;
=======

	if (rbo->flags & RADEON_GEM_GTT_UC)
		caching = ttm_uncached;
	else if (rbo->flags & RADEON_GEM_GTT_WC)
		caching = ttm_write_combined;
	else
		caching = ttm_cached;

	if (ttm_sg_tt_init(&gtt->ttm, bo, page_flags, caching)) {
		kfree(gtt);
		return NULL;
	}
	return &gtt->ttm;
}

static struct radeon_ttm_tt *radeon_ttm_tt_to_gtt(struct radeon_device *rdev,
						  struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	if (rdev->flags & RADEON_IS_AGP)
		return NULL;
#endif

	if (!ttm)
		return NULL;
	return container_of(ttm, struct radeon_ttm_tt, ttm);
}

static int radeon_ttm_tt_populate(struct ttm_device *bdev,
				  struct ttm_tt *ttm,
				  struct ttm_operation_ctx *ctx)
{
	struct radeon_device *rdev = radeon_get_rdev(bdev);
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(rdev, ttm);
>>>>>>> upstream/android-13
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (gtt && gtt->userptr) {
		ttm->sg = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
		if (!ttm->sg)
			return -ENOMEM;

		ttm->page_flags |= TTM_PAGE_FLAG_SG;
<<<<<<< HEAD
		ttm->state = tt_unbound;
=======
>>>>>>> upstream/android-13
		return 0;
	}

	if (slave && ttm->sg) {
<<<<<<< HEAD
		drm_prime_sg_to_page_addr_arrays(ttm->sg, ttm->pages,
						 gtt->ttm.dma_address, ttm->num_pages);
		ttm->state = tt_unbound;
		return 0;
	}

	rdev = radeon_get_rdev(ttm->bdev);
#if IS_ENABLED(CONFIG_AGP)
	if (rdev->flags & RADEON_IS_AGP) {
		return ttm_agp_tt_populate(ttm, ctx);
	}
#endif

#ifdef CONFIG_SWIOTLB
	if (rdev->need_swiotlb && swiotlb_nr_tbl()) {
		return ttm_dma_populate(&gtt->ttm, rdev->dev, ctx);
	}
#endif

	return ttm_populate_and_map_pages(rdev->dev, &gtt->ttm, ctx);
}

static void radeon_ttm_tt_unpopulate(struct ttm_tt *ttm)
{
	struct radeon_device *rdev;
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(ttm);
=======
		drm_prime_sg_to_dma_addr_array(ttm->sg, gtt->ttm.dma_address,
					       ttm->num_pages);
		return 0;
	}

	return ttm_pool_alloc(&rdev->mman.bdev.pool, ttm, ctx);
}

static void radeon_ttm_tt_unpopulate(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct radeon_device *rdev = radeon_get_rdev(bdev);
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(rdev, ttm);
>>>>>>> upstream/android-13
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (gtt && gtt->userptr) {
		kfree(ttm->sg);
		ttm->page_flags &= ~TTM_PAGE_FLAG_SG;
		return;
	}

	if (slave)
		return;

<<<<<<< HEAD
	rdev = radeon_get_rdev(ttm->bdev);
#if IS_ENABLED(CONFIG_AGP)
	if (rdev->flags & RADEON_IS_AGP) {
		ttm_agp_tt_unpopulate(ttm);
		return;
	}
#endif

#ifdef CONFIG_SWIOTLB
	if (rdev->need_swiotlb && swiotlb_nr_tbl()) {
		ttm_dma_unpopulate(&gtt->ttm, rdev->dev);
		return;
	}
#endif

	ttm_unmap_and_unpopulate_pages(rdev->dev, &gtt->ttm);
}

int radeon_ttm_tt_set_userptr(struct ttm_tt *ttm, uint64_t addr,
			      uint32_t flags)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(ttm);
=======
	return ttm_pool_free(&rdev->mman.bdev.pool, ttm);
}

int radeon_ttm_tt_set_userptr(struct radeon_device *rdev,
			      struct ttm_tt *ttm, uint64_t addr,
			      uint32_t flags)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(rdev, ttm);
>>>>>>> upstream/android-13

	if (gtt == NULL)
		return -EINVAL;

	gtt->userptr = addr;
	gtt->usermm = current->mm;
	gtt->userflags = flags;
	return 0;
}

<<<<<<< HEAD
bool radeon_ttm_tt_has_userptr(struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(ttm);
=======
bool radeon_ttm_tt_is_bound(struct ttm_device *bdev,
			    struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct radeon_device *rdev = radeon_get_rdev(bdev);
	if (rdev->flags & RADEON_IS_AGP)
		return ttm_agp_is_bound(ttm);
#endif
	return radeon_ttm_backend_is_bound(ttm);
}

static int radeon_ttm_tt_bind(struct ttm_device *bdev,
			      struct ttm_tt *ttm,
			      struct ttm_resource *bo_mem)
{
#if IS_ENABLED(CONFIG_AGP)
	struct radeon_device *rdev = radeon_get_rdev(bdev);
#endif

	if (!bo_mem)
		return -EINVAL;
#if IS_ENABLED(CONFIG_AGP)
	if (rdev->flags & RADEON_IS_AGP)
		return ttm_agp_bind(ttm, bo_mem);
#endif

	return radeon_ttm_backend_bind(bdev, ttm, bo_mem);
}

static void radeon_ttm_tt_unbind(struct ttm_device *bdev,
				 struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct radeon_device *rdev = radeon_get_rdev(bdev);

	if (rdev->flags & RADEON_IS_AGP) {
		ttm_agp_unbind(ttm);
		return;
	}
#endif
	radeon_ttm_backend_unbind(bdev, ttm);
}

static void radeon_ttm_tt_destroy(struct ttm_device *bdev,
				  struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct radeon_device *rdev = radeon_get_rdev(bdev);

	if (rdev->flags & RADEON_IS_AGP) {
		ttm_agp_unbind(ttm);
		ttm_tt_destroy_common(bdev, ttm);
		ttm_agp_destroy(ttm);
		return;
	}
#endif
	radeon_ttm_backend_destroy(bdev, ttm);
}

bool radeon_ttm_tt_has_userptr(struct radeon_device *rdev,
			       struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(rdev, ttm);
>>>>>>> upstream/android-13

	if (gtt == NULL)
		return false;

	return !!gtt->userptr;
}

<<<<<<< HEAD
bool radeon_ttm_tt_is_readonly(struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(ttm);
=======
bool radeon_ttm_tt_is_readonly(struct radeon_device *rdev,
			       struct ttm_tt *ttm)
{
	struct radeon_ttm_tt *gtt = radeon_ttm_tt_to_gtt(rdev, ttm);
>>>>>>> upstream/android-13

	if (gtt == NULL)
		return false;

	return !!(gtt->userflags & RADEON_GEM_USERPTR_READONLY);
}

<<<<<<< HEAD
static struct ttm_bo_driver radeon_bo_driver = {
	.ttm_tt_create = &radeon_ttm_tt_create,
	.ttm_tt_populate = &radeon_ttm_tt_populate,
	.ttm_tt_unpopulate = &radeon_ttm_tt_unpopulate,
	.invalidate_caches = &radeon_invalidate_caches,
	.init_mem_type = &radeon_init_mem_type,
	.eviction_valuable = ttm_bo_eviction_valuable,
	.evict_flags = &radeon_evict_flags,
	.move = &radeon_bo_move,
	.verify_access = &radeon_verify_access,
	.move_notify = &radeon_bo_move_notify,
	.fault_reserve_notify = &radeon_bo_fault_reserve_notify,
	.io_mem_reserve = &radeon_ttm_io_mem_reserve,
	.io_mem_free = &radeon_ttm_io_mem_free,
=======
static void
radeon_bo_delete_mem_notify(struct ttm_buffer_object *bo)
{
	unsigned int old_type = TTM_PL_SYSTEM;

	if (bo->resource)
		old_type = bo->resource->mem_type;
	radeon_bo_move_notify(bo, old_type, NULL);
}

static struct ttm_device_funcs radeon_bo_driver = {
	.ttm_tt_create = &radeon_ttm_tt_create,
	.ttm_tt_populate = &radeon_ttm_tt_populate,
	.ttm_tt_unpopulate = &radeon_ttm_tt_unpopulate,
	.ttm_tt_destroy = &radeon_ttm_tt_destroy,
	.eviction_valuable = ttm_bo_eviction_valuable,
	.evict_flags = &radeon_evict_flags,
	.move = &radeon_bo_move,
	.delete_mem_notify = &radeon_bo_delete_mem_notify,
	.io_mem_reserve = &radeon_ttm_io_mem_reserve,
>>>>>>> upstream/android-13
};

int radeon_ttm_init(struct radeon_device *rdev)
{
	int r;

<<<<<<< HEAD
	r = radeon_ttm_global_init(rdev);
	if (r) {
		return r;
	}
	/* No others user of address space so set it to 0 */
	r = ttm_bo_device_init(&rdev->mman.bdev,
			       rdev->mman.bo_global_ref.ref.object,
			       &radeon_bo_driver,
			       rdev->ddev->anon_inode->i_mapping,
			       DRM_FILE_PAGE_OFFSET,
			       rdev->need_dma32);
=======
	/* No others user of address space so set it to 0 */
	r = ttm_device_init(&rdev->mman.bdev, &radeon_bo_driver, rdev->dev,
			       rdev->ddev->anon_inode->i_mapping,
			       rdev->ddev->vma_offset_manager,
			       rdev->need_swiotlb,
			       dma_addressing_limited(&rdev->pdev->dev));
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("failed initializing buffer object driver(%d).\n", r);
		return r;
	}
	rdev->mman.initialized = true;
<<<<<<< HEAD
	r = ttm_bo_init_mm(&rdev->mman.bdev, TTM_PL_VRAM,
				rdev->mc.real_vram_size >> PAGE_SHIFT);
=======

	r = radeon_ttm_init_vram(rdev);
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("Failed initializing VRAM heap.\n");
		return r;
	}
	/* Change the size here instead of the init above so only lpfn is affected */
	radeon_ttm_set_active_vram_size(rdev, rdev->mc.visible_vram_size);

	r = radeon_bo_create(rdev, 256 * 1024, PAGE_SIZE, true,
			     RADEON_GEM_DOMAIN_VRAM, 0, NULL,
			     NULL, &rdev->stolen_vga_memory);
	if (r) {
		return r;
	}
	r = radeon_bo_reserve(rdev->stolen_vga_memory, false);
	if (r)
		return r;
	r = radeon_bo_pin(rdev->stolen_vga_memory, RADEON_GEM_DOMAIN_VRAM, NULL);
	radeon_bo_unreserve(rdev->stolen_vga_memory);
	if (r) {
		radeon_bo_unref(&rdev->stolen_vga_memory);
		return r;
	}
	DRM_INFO("radeon: %uM of VRAM memory ready\n",
		 (unsigned) (rdev->mc.real_vram_size / (1024 * 1024)));
<<<<<<< HEAD
	r = ttm_bo_init_mm(&rdev->mman.bdev, TTM_PL_TT,
				rdev->mc.gtt_size >> PAGE_SHIFT);
=======

	r = radeon_ttm_init_gtt(rdev);
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("Failed initializing GTT heap.\n");
		return r;
	}
	DRM_INFO("radeon: %uM of GTT memory ready.\n",
		 (unsigned)(rdev->mc.gtt_size / (1024 * 1024)));

<<<<<<< HEAD
	r = radeon_ttm_debugfs_init(rdev);
	if (r) {
		DRM_ERROR("Failed to init debugfs\n");
		return r;
	}
=======
	radeon_ttm_debugfs_init(rdev);

>>>>>>> upstream/android-13
	return 0;
}

void radeon_ttm_fini(struct radeon_device *rdev)
{
	int r;

	if (!rdev->mman.initialized)
		return;
<<<<<<< HEAD
	radeon_ttm_debugfs_fini(rdev);
=======

>>>>>>> upstream/android-13
	if (rdev->stolen_vga_memory) {
		r = radeon_bo_reserve(rdev->stolen_vga_memory, false);
		if (r == 0) {
			radeon_bo_unpin(rdev->stolen_vga_memory);
			radeon_bo_unreserve(rdev->stolen_vga_memory);
		}
		radeon_bo_unref(&rdev->stolen_vga_memory);
	}
<<<<<<< HEAD
	ttm_bo_clean_mm(&rdev->mman.bdev, TTM_PL_VRAM);
	ttm_bo_clean_mm(&rdev->mman.bdev, TTM_PL_TT);
	ttm_bo_device_release(&rdev->mman.bdev);
	radeon_gart_fini(rdev);
	radeon_ttm_global_fini(rdev);
=======
	ttm_range_man_fini(&rdev->mman.bdev, TTM_PL_VRAM);
	ttm_range_man_fini(&rdev->mman.bdev, TTM_PL_TT);
	ttm_device_fini(&rdev->mman.bdev);
	radeon_gart_fini(rdev);
>>>>>>> upstream/android-13
	rdev->mman.initialized = false;
	DRM_INFO("radeon: ttm finalized\n");
}

/* this should only be called at bootup or when userspace
 * isn't running */
void radeon_ttm_set_active_vram_size(struct radeon_device *rdev, u64 size)
{
<<<<<<< HEAD
	struct ttm_mem_type_manager *man;
=======
	struct ttm_resource_manager *man;
>>>>>>> upstream/android-13

	if (!rdev->mman.initialized)
		return;

<<<<<<< HEAD
	man = &rdev->mman.bdev.man[TTM_PL_VRAM];
=======
	man = ttm_manager_type(&rdev->mman.bdev, TTM_PL_VRAM);
>>>>>>> upstream/android-13
	/* this just adjusts TTM size idea, which sets lpfn to the correct value */
	man->size = size >> PAGE_SHIFT;
}

<<<<<<< HEAD
static struct vm_operations_struct radeon_ttm_vm_ops;
static const struct vm_operations_struct *ttm_vm_ops = NULL;

static vm_fault_t radeon_ttm_fault(struct vm_fault *vmf)
{
	struct ttm_buffer_object *bo;
	struct radeon_device *rdev;
	vm_fault_t ret;

	bo = (struct ttm_buffer_object *)vmf->vma->vm_private_data;
	if (bo == NULL) {
		return VM_FAULT_NOPAGE;
	}
	rdev = radeon_get_rdev(bo->bdev);
	down_read(&rdev->pm.mclk_lock);
	ret = ttm_vm_ops->fault(vmf);
	up_read(&rdev->pm.mclk_lock);
	return ret;
}

int radeon_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct drm_file *file_priv;
	struct radeon_device *rdev;
	int r;

	if (unlikely(vma->vm_pgoff < DRM_FILE_PAGE_OFFSET)) {
		return -EINVAL;
	}

	file_priv = filp->private_data;
	rdev = file_priv->minor->dev->dev_private;
	if (rdev == NULL) {
		return -EINVAL;
	}
	r = ttm_bo_mmap(filp, vma, &rdev->mman.bdev);
	if (unlikely(r != 0)) {
		return r;
	}
	if (unlikely(ttm_vm_ops == NULL)) {
		ttm_vm_ops = vma->vm_ops;
		radeon_ttm_vm_ops = *ttm_vm_ops;
		radeon_ttm_vm_ops.fault = &radeon_ttm_fault;
	}
	vma->vm_ops = &radeon_ttm_vm_ops;
	return 0;
}

#if defined(CONFIG_DEBUG_FS)

static int radeon_mm_dump_table(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *)m->private;
	unsigned ttm_pl = *(int*)node->info_ent->data;
	struct drm_device *dev = node->minor->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct ttm_mem_type_manager *man = &rdev->mman.bdev.man[ttm_pl];
=======
#if defined(CONFIG_DEBUG_FS)

static int radeon_mm_vram_dump_table_show(struct seq_file *m, void *unused)
{
	struct radeon_device *rdev = (struct radeon_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&rdev->mman.bdev,
							    TTM_PL_VRAM);
>>>>>>> upstream/android-13
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

<<<<<<< HEAD

static int ttm_pl_vram = TTM_PL_VRAM;
static int ttm_pl_tt = TTM_PL_TT;

static struct drm_info_list radeon_ttm_debugfs_list[] = {
	{"radeon_vram_mm", radeon_mm_dump_table, 0, &ttm_pl_vram},
	{"radeon_gtt_mm", radeon_mm_dump_table, 0, &ttm_pl_tt},
	{"ttm_page_pool", ttm_page_alloc_debugfs, 0, NULL},
#ifdef CONFIG_SWIOTLB
	{"ttm_dma_page_pool", ttm_dma_page_alloc_debugfs, 0, NULL}
#endif
};
=======
static int radeon_ttm_page_pool_show(struct seq_file *m, void *data)
{
	struct radeon_device *rdev = (struct radeon_device *)m->private;

	return ttm_pool_debugfs(&rdev->mman.bdev.pool, m);
}

static int radeon_mm_gtt_dump_table_show(struct seq_file *m, void *unused)
{
	struct radeon_device *rdev = (struct radeon_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&rdev->mman.bdev,
							    TTM_PL_TT);
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

DEFINE_SHOW_ATTRIBUTE(radeon_mm_vram_dump_table);
DEFINE_SHOW_ATTRIBUTE(radeon_mm_gtt_dump_table);
DEFINE_SHOW_ATTRIBUTE(radeon_ttm_page_pool);
>>>>>>> upstream/android-13

static int radeon_ttm_vram_open(struct inode *inode, struct file *filep)
{
	struct radeon_device *rdev = inode->i_private;
	i_size_write(inode, rdev->mc.mc_vram_size);
	filep->private_data = inode->i_private;
	return 0;
}

static ssize_t radeon_ttm_vram_read(struct file *f, char __user *buf,
				    size_t size, loff_t *pos)
{
	struct radeon_device *rdev = f->private_data;
	ssize_t result = 0;
	int r;

	if (size & 0x3 || *pos & 0x3)
		return -EINVAL;

	while (size) {
		unsigned long flags;
		uint32_t value;

		if (*pos >= rdev->mc.mc_vram_size)
			return result;

		spin_lock_irqsave(&rdev->mmio_idx_lock, flags);
		WREG32(RADEON_MM_INDEX, ((uint32_t)*pos) | 0x80000000);
		if (rdev->family >= CHIP_CEDAR)
			WREG32(EVERGREEN_MM_INDEX_HI, *pos >> 31);
		value = RREG32(RADEON_MM_DATA);
		spin_unlock_irqrestore(&rdev->mmio_idx_lock, flags);

<<<<<<< HEAD
		r = put_user(value, (uint32_t *)buf);
=======
		r = put_user(value, (uint32_t __user *)buf);
>>>>>>> upstream/android-13
		if (r)
			return r;

		result += 4;
		buf += 4;
		*pos += 4;
		size -= 4;
	}

	return result;
}

static const struct file_operations radeon_ttm_vram_fops = {
	.owner = THIS_MODULE,
	.open = radeon_ttm_vram_open,
	.read = radeon_ttm_vram_read,
	.llseek = default_llseek
};

static int radeon_ttm_gtt_open(struct inode *inode, struct file *filep)
{
	struct radeon_device *rdev = inode->i_private;
	i_size_write(inode, rdev->mc.gtt_size);
	filep->private_data = inode->i_private;
	return 0;
}

static ssize_t radeon_ttm_gtt_read(struct file *f, char __user *buf,
				   size_t size, loff_t *pos)
{
	struct radeon_device *rdev = f->private_data;
	ssize_t result = 0;
	int r;

	while (size) {
		loff_t p = *pos / PAGE_SIZE;
		unsigned off = *pos & ~PAGE_MASK;
		size_t cur_size = min_t(size_t, size, PAGE_SIZE - off);
		struct page *page;
		void *ptr;

		if (p >= rdev->gart.num_cpu_pages)
			return result;

		page = rdev->gart.pages[p];
		if (page) {
			ptr = kmap(page);
			ptr += off;

			r = copy_to_user(buf, ptr, cur_size);
			kunmap(rdev->gart.pages[p]);
		} else
			r = clear_user(buf, cur_size);

		if (r)
			return -EFAULT;

		result += cur_size;
		buf += cur_size;
		*pos += cur_size;
		size -= cur_size;
	}

	return result;
}

static const struct file_operations radeon_ttm_gtt_fops = {
	.owner = THIS_MODULE,
	.open = radeon_ttm_gtt_open,
	.read = radeon_ttm_gtt_read,
	.llseek = default_llseek
};

#endif

<<<<<<< HEAD
static int radeon_ttm_debugfs_init(struct radeon_device *rdev)
{
#if defined(CONFIG_DEBUG_FS)
	unsigned count;

	struct drm_minor *minor = rdev->ddev->primary;
	struct dentry *ent, *root = minor->debugfs_root;

	ent = debugfs_create_file("radeon_vram", S_IFREG | S_IRUGO, root,
				  rdev, &radeon_ttm_vram_fops);
	if (IS_ERR(ent))
		return PTR_ERR(ent);
	rdev->mman.vram = ent;

	ent = debugfs_create_file("radeon_gtt", S_IFREG | S_IRUGO, root,
				  rdev, &radeon_ttm_gtt_fops);
	if (IS_ERR(ent))
		return PTR_ERR(ent);
	rdev->mman.gtt = ent;

	count = ARRAY_SIZE(radeon_ttm_debugfs_list);

#ifdef CONFIG_SWIOTLB
	if (!(rdev->need_swiotlb && swiotlb_nr_tbl()))
		--count;
#endif

	return radeon_debugfs_add_files(rdev, radeon_ttm_debugfs_list, count);
#else

	return 0;
#endif
}

static void radeon_ttm_debugfs_fini(struct radeon_device *rdev)
{
#if defined(CONFIG_DEBUG_FS)

	debugfs_remove(rdev->mman.vram);
	rdev->mman.vram = NULL;

	debugfs_remove(rdev->mman.gtt);
	rdev->mman.gtt = NULL;
=======
static void radeon_ttm_debugfs_init(struct radeon_device *rdev)
{
#if defined(CONFIG_DEBUG_FS)
	struct drm_minor *minor = rdev->ddev->primary;
	struct dentry *root = minor->debugfs_root;

	debugfs_create_file("radeon_vram", 0444, root, rdev,
			    &radeon_ttm_vram_fops);

	debugfs_create_file("radeon_gtt", 0444, root, rdev,
			    &radeon_ttm_gtt_fops);

	debugfs_create_file("radeon_vram_mm", 0444, root, rdev,
			    &radeon_mm_vram_dump_table_fops);
	debugfs_create_file("radeon_gtt_mm", 0444, root, rdev,
			    &radeon_mm_gtt_dump_table_fops);
	debugfs_create_file("ttm_page_pool", 0444, root, rdev,
			    &radeon_ttm_page_pool_fops);
>>>>>>> upstream/android-13
#endif
}
