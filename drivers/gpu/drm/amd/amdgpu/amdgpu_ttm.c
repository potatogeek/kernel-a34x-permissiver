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
#include <drm/amdgpu_drm.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/swiotlb.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/debugfs.h>
#include <linux/iommu.h>
=======

#include <linux/dma-mapping.h>
#include <linux/iommu.h>
#include <linux/pagemap.h>
#include <linux/sched/task.h>
#include <linux/sched/mm.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/swiotlb.h>
#include <linux/dma-buf.h>
#include <linux/sizes.h>

#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_range_manager.h>

#include <drm/amdgpu_drm.h>

>>>>>>> upstream/android-13
#include "amdgpu.h"
#include "amdgpu_object.h"
#include "amdgpu_trace.h"
#include "amdgpu_amdkfd.h"
<<<<<<< HEAD
#include "bif/bif_4_1_d.h"

#define DRM_FILE_PAGE_OFFSET (0x100000000ULL >> PAGE_SHIFT)

static int amdgpu_map_buffer(struct ttm_buffer_object *bo,
			     struct ttm_mem_reg *mem, unsigned num_pages,
			     uint64_t offset, unsigned window,
			     struct amdgpu_ring *ring,
			     uint64_t *addr);

static int amdgpu_ttm_debugfs_init(struct amdgpu_device *adev);
static void amdgpu_ttm_debugfs_fini(struct amdgpu_device *adev);

/*
 * Global memory.
 */

/**
 * amdgpu_ttm_mem_global_init - Initialize and acquire reference to
 * memory object
 *
 * @ref: Object for initialization.
 *
 * This is called by drm_global_item_ref() when an object is being
 * initialized.
 */
static int amdgpu_ttm_mem_global_init(struct drm_global_reference *ref)
{
	return ttm_mem_global_init(ref->object);
}

/**
 * amdgpu_ttm_mem_global_release - Drop reference to a memory object
 *
 * @ref: Object being removed
 *
 * This is called by drm_global_item_unref() when an object is being
 * released.
 */
static void amdgpu_ttm_mem_global_release(struct drm_global_reference *ref)
{
	ttm_mem_global_release(ref->object);
}

/**
 * amdgpu_ttm_global_init - Initialize global TTM memory reference structures.
 *
 * @adev: AMDGPU device for which the global structures need to be registered.
 *
 * This is called as part of the AMDGPU ttm init from amdgpu_ttm_init()
 * during bring up.
 */
static int amdgpu_ttm_global_init(struct amdgpu_device *adev)
{
	struct drm_global_reference *global_ref;
	int r;

	/* ensure reference is false in case init fails */
	adev->mman.mem_global_referenced = false;

	global_ref = &adev->mman.mem_global_ref;
	global_ref->global_type = DRM_GLOBAL_TTM_MEM;
	global_ref->size = sizeof(struct ttm_mem_global);
	global_ref->init = &amdgpu_ttm_mem_global_init;
	global_ref->release = &amdgpu_ttm_mem_global_release;
	r = drm_global_item_ref(global_ref);
	if (r) {
		DRM_ERROR("Failed setting up TTM memory accounting "
			  "subsystem.\n");
		goto error_mem;
	}

	adev->mman.bo_global_ref.mem_glob =
		adev->mman.mem_global_ref.object;
	global_ref = &adev->mman.bo_global_ref.ref;
	global_ref->global_type = DRM_GLOBAL_TTM_BO;
	global_ref->size = sizeof(struct ttm_bo_global);
	global_ref->init = &ttm_bo_global_init;
	global_ref->release = &ttm_bo_global_release;
	r = drm_global_item_ref(global_ref);
	if (r) {
		DRM_ERROR("Failed setting up TTM BO subsystem.\n");
		goto error_bo;
	}

	mutex_init(&adev->mman.gtt_window_lock);

	adev->mman.mem_global_referenced = true;

	return 0;

error_bo:
	drm_global_item_unref(&adev->mman.mem_global_ref);
error_mem:
	return r;
}

static void amdgpu_ttm_global_fini(struct amdgpu_device *adev)
{
	if (adev->mman.mem_global_referenced) {
		mutex_destroy(&adev->mman.gtt_window_lock);
		drm_global_item_unref(&adev->mman.bo_global_ref.ref);
		drm_global_item_unref(&adev->mman.mem_global_ref);
		adev->mman.mem_global_referenced = false;
	}
}

static int amdgpu_invalidate_caches(struct ttm_bo_device *bdev, uint32_t flags)
{
	return 0;
}

/**
 * amdgpu_init_mem_type - Initialize a memory manager for a specific type of
 * memory request.
 *
 * @bdev: The TTM BO device object (contains a reference to amdgpu_device)
 * @type: The type of memory requested
 * @man: The memory type manager for each domain
 *
 * This is called by ttm_bo_init_mm() when a buffer object is being
 * initialized.
 */
static int amdgpu_init_mem_type(struct ttm_bo_device *bdev, uint32_t type,
				struct ttm_mem_type_manager *man)
{
	struct amdgpu_device *adev;

	adev = amdgpu_ttm_adev(bdev);

	switch (type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case TTM_PL_TT:
		/* GTT memory  */
		man->func = &amdgpu_gtt_mgr_func;
		man->gpu_offset = adev->gmc.gart_start;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE | TTM_MEMTYPE_FLAG_CMA;
		break;
	case TTM_PL_VRAM:
		/* "On-card" video ram */
		man->func = &amdgpu_vram_mgr_func;
		man->gpu_offset = adev->gmc.vram_start;
		man->flags = TTM_MEMTYPE_FLAG_FIXED |
			     TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_UNCACHED | TTM_PL_FLAG_WC;
		man->default_caching = TTM_PL_FLAG_WC;
		break;
	case AMDGPU_PL_GDS:
	case AMDGPU_PL_GWS:
	case AMDGPU_PL_OA:
		/* On-chip GDS memory*/
		man->func = &ttm_bo_manager_func;
		man->gpu_offset = 0;
		man->flags = TTM_MEMTYPE_FLAG_FIXED | TTM_MEMTYPE_FLAG_CMA;
		man->available_caching = TTM_PL_FLAG_UNCACHED;
		man->default_caching = TTM_PL_FLAG_UNCACHED;
		break;
	default:
		DRM_ERROR("Unsupported memory type %u\n", (unsigned)type);
		return -EINVAL;
	}
	return 0;
=======
#include "amdgpu_sdma.h"
#include "amdgpu_ras.h"
#include "amdgpu_atomfirmware.h"
#include "amdgpu_res_cursor.h"
#include "bif/bif_4_1_d.h"

#define AMDGPU_TTM_VRAM_MAX_DW_READ	(size_t)128

static int amdgpu_ttm_backend_bind(struct ttm_device *bdev,
				   struct ttm_tt *ttm,
				   struct ttm_resource *bo_mem);
static void amdgpu_ttm_backend_unbind(struct ttm_device *bdev,
				      struct ttm_tt *ttm);

static int amdgpu_ttm_init_on_chip(struct amdgpu_device *adev,
				    unsigned int type,
				    uint64_t size_in_page)
{
	return ttm_range_man_init(&adev->mman.bdev, type,
				  false, size_in_page);
>>>>>>> upstream/android-13
}

/**
 * amdgpu_evict_flags - Compute placement flags
 *
 * @bo: The buffer object to evict
 * @placement: Possible destination(s) for evicted BO
 *
 * Fill in placement data when ttm_bo_evict() is called
 */
static void amdgpu_evict_flags(struct ttm_buffer_object *bo,
				struct ttm_placement *placement)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->bdev);
	struct amdgpu_bo *abo;
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

	/* Don't handle scatter gather BOs */
	if (bo->type == ttm_bo_type_sg) {
		placement->num_placement = 0;
		placement->num_busy_placement = 0;
		return;
	}

	/* Object isn't an AMDGPU object so ignore */
	if (!amdgpu_bo_is_amdgpu_bo(bo)) {
		placement->placement = &placements;
		placement->busy_placement = &placements;
		placement->num_placement = 1;
		placement->num_busy_placement = 1;
		return;
	}

	abo = ttm_to_amdgpu_bo(bo);
<<<<<<< HEAD
	switch (bo->mem.mem_type) {
=======
	if (abo->flags & AMDGPU_AMDKFD_CREATE_SVM_BO) {
		struct dma_fence *fence;
		struct dma_resv *resv = &bo->base._resv;

		rcu_read_lock();
		fence = rcu_dereference(resv->fence_excl);
		if (fence && !fence->ops->signaled)
			dma_fence_enable_sw_signaling(fence);

		placement->num_placement = 0;
		placement->num_busy_placement = 0;
		rcu_read_unlock();
		return;
	}

	switch (bo->resource->mem_type) {
	case AMDGPU_PL_GDS:
	case AMDGPU_PL_GWS:
	case AMDGPU_PL_OA:
		placement->num_placement = 0;
		placement->num_busy_placement = 0;
		return;

>>>>>>> upstream/android-13
	case TTM_PL_VRAM:
		if (!adev->mman.buffer_funcs_enabled) {
			/* Move to system memory */
			amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_CPU);
		} else if (!amdgpu_gmc_vram_full_visible(&adev->gmc) &&
			   !(abo->flags & AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED) &&
			   amdgpu_bo_in_cpu_visible_vram(abo)) {

			/* Try evicting to the CPU inaccessible part of VRAM
			 * first, but only set GTT as busy placement, so this
			 * BO will be evicted to GTT rather than causing other
			 * BOs to be evicted from VRAM
			 */
			amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_VRAM |
<<<<<<< HEAD
							 AMDGPU_GEM_DOMAIN_GTT);
=======
							AMDGPU_GEM_DOMAIN_GTT |
							AMDGPU_GEM_DOMAIN_CPU);
>>>>>>> upstream/android-13
			abo->placements[0].fpfn = adev->gmc.visible_vram_size >> PAGE_SHIFT;
			abo->placements[0].lpfn = 0;
			abo->placement.busy_placement = &abo->placements[1];
			abo->placement.num_busy_placement = 1;
		} else {
			/* Move to GTT memory */
<<<<<<< HEAD
			amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_GTT);
		}
		break;
	case TTM_PL_TT:
	default:
		amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_CPU);
=======
			amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_GTT |
							AMDGPU_GEM_DOMAIN_CPU);
		}
		break;
	case TTM_PL_TT:
	case AMDGPU_PL_PREEMPT:
	default:
		amdgpu_bo_placement_from_domain(abo, AMDGPU_GEM_DOMAIN_CPU);
		break;
>>>>>>> upstream/android-13
	}
	*placement = abo->placement;
}

/**
<<<<<<< HEAD
 * amdgpu_verify_access - Verify access for a mmap call
 *
 * @bo:	The buffer object to map
 * @filp: The file pointer from the process performing the mmap
 *
 * This is called by ttm_bo_mmap() to verify whether a process
 * has the right to mmap a BO to their process space.
 */
static int amdgpu_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(bo);

	/*
	 * Don't verify access for KFD BOs. They don't have a GEM
	 * object associated with them.
	 */
	if (abo->kfd_bo)
		return 0;

	if (amdgpu_ttm_tt_get_usermm(bo->ttm))
		return -EPERM;
	return drm_vma_node_verify_access(&abo->gem_base.vma_node,
					  filp->private_data);
}

/**
 * amdgpu_move_null - Register memory for a buffer object
 *
 * @bo: The bo to assign the memory to
 * @new_mem: The memory to be assigned.
 *
 * Assign the memory from new_mem to the memory of the buffer object bo.
 */
static void amdgpu_move_null(struct ttm_buffer_object *bo,
			     struct ttm_mem_reg *new_mem)
{
	struct ttm_mem_reg *old_mem = &bo->mem;

	BUG_ON(old_mem->mm_node != NULL);
	*old_mem = *new_mem;
	new_mem->mm_node = NULL;
}

/**
 * amdgpu_mm_node_addr - Compute the GPU relative offset of a GTT buffer.
 *
 * @bo: The bo to assign the memory to.
 * @mm_node: Memory manager node for drm allocator.
 * @mem: The region where the bo resides.
 *
 */
static uint64_t amdgpu_mm_node_addr(struct ttm_buffer_object *bo,
				    struct drm_mm_node *mm_node,
				    struct ttm_mem_reg *mem)
{
	uint64_t addr = 0;

	if (mem->mem_type != TTM_PL_TT || amdgpu_gtt_mgr_has_gart_addr(mem)) {
		addr = mm_node->start << PAGE_SHIFT;
		addr += bo->bdev->man[mem->mem_type].gpu_offset;
	}
	return addr;
}

/**
 * amdgpu_find_mm_node - Helper function finds the drm_mm_node corresponding to
 * @offset. It also modifies the offset to be within the drm_mm_node returned
 *
 * @mem: The region where the bo resides.
 * @offset: The offset that drm_mm_node is used for finding.
 *
 */
static struct drm_mm_node *amdgpu_find_mm_node(struct ttm_mem_reg *mem,
					       unsigned long *offset)
{
	struct drm_mm_node *mm_node = mem->mm_node;

	while (*offset >= (mm_node->size << PAGE_SHIFT)) {
		*offset -= (mm_node->size << PAGE_SHIFT);
		++mm_node;
	}
	return mm_node;
}

/**
 * amdgpu_copy_ttm_mem_to_mem - Helper function for copy
=======
 * amdgpu_ttm_map_buffer - Map memory into the GART windows
 * @bo: buffer object to map
 * @mem: memory object to map
 * @mm_cur: range to map
 * @num_pages: number of pages to map
 * @window: which GART window to use
 * @ring: DMA ring to use for the copy
 * @tmz: if we should setup a TMZ enabled mapping
 * @addr: resulting address inside the MC address space
 *
 * Setup one of the GART windows to access a specific piece of memory or return
 * the physical address for local memory.
 */
static int amdgpu_ttm_map_buffer(struct ttm_buffer_object *bo,
				 struct ttm_resource *mem,
				 struct amdgpu_res_cursor *mm_cur,
				 unsigned num_pages, unsigned window,
				 struct amdgpu_ring *ring, bool tmz,
				 uint64_t *addr)
{
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_job *job;
	unsigned num_dw, num_bytes;
	struct dma_fence *fence;
	uint64_t src_addr, dst_addr;
	void *cpu_addr;
	uint64_t flags;
	unsigned int i;
	int r;

	BUG_ON(adev->mman.buffer_funcs->copy_max_bytes <
	       AMDGPU_GTT_MAX_TRANSFER_SIZE * 8);
	BUG_ON(mem->mem_type == AMDGPU_PL_PREEMPT);

	/* Map only what can't be accessed directly */
	if (!tmz && mem->start != AMDGPU_BO_INVALID_OFFSET) {
		*addr = amdgpu_ttm_domain_start(adev, mem->mem_type) +
			mm_cur->start;
		return 0;
	}

	*addr = adev->gmc.gart_start;
	*addr += (u64)window * AMDGPU_GTT_MAX_TRANSFER_SIZE *
		AMDGPU_GPU_PAGE_SIZE;
	*addr += mm_cur->start & ~PAGE_MASK;

	num_dw = ALIGN(adev->mman.buffer_funcs->copy_num_dw, 8);
	num_bytes = num_pages * 8 * AMDGPU_GPU_PAGES_IN_CPU_PAGE;

	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4 + num_bytes,
				     AMDGPU_IB_POOL_DELAYED, &job);
	if (r)
		return r;

	src_addr = num_dw * 4;
	src_addr += job->ibs[0].gpu_addr;

	dst_addr = amdgpu_bo_gpu_offset(adev->gart.bo);
	dst_addr += window * AMDGPU_GTT_MAX_TRANSFER_SIZE * 8;
	amdgpu_emit_copy_buffer(adev, &job->ibs[0], src_addr,
				dst_addr, num_bytes, false);

	amdgpu_ring_pad_ib(ring, &job->ibs[0]);
	WARN_ON(job->ibs[0].length_dw > num_dw);

	flags = amdgpu_ttm_tt_pte_flags(adev, bo->ttm, mem);
	if (tmz)
		flags |= AMDGPU_PTE_TMZ;

	cpu_addr = &job->ibs[0].ptr[num_dw];

	if (mem->mem_type == TTM_PL_TT) {
		dma_addr_t *dma_addr;

		dma_addr = &bo->ttm->dma_address[mm_cur->start >> PAGE_SHIFT];
		r = amdgpu_gart_map(adev, 0, num_pages, dma_addr, flags,
				    cpu_addr);
		if (r)
			goto error_free;
	} else {
		dma_addr_t dma_address;

		dma_address = mm_cur->start;
		dma_address += adev->vm_manager.vram_base_offset;

		for (i = 0; i < num_pages; ++i) {
			r = amdgpu_gart_map(adev, i << PAGE_SHIFT, 1,
					    &dma_address, flags, cpu_addr);
			if (r)
				goto error_free;

			dma_address += PAGE_SIZE;
		}
	}

	r = amdgpu_job_submit(job, &adev->mman.entity,
			      AMDGPU_FENCE_OWNER_UNDEFINED, &fence);
	if (r)
		goto error_free;

	dma_fence_put(fence);

	return r;

error_free:
	amdgpu_job_free(job);
	return r;
}

/**
 * amdgpu_ttm_copy_mem_to_mem - Helper function for copy
 * @adev: amdgpu device
 * @src: buffer/address where to read from
 * @dst: buffer/address where to write to
 * @size: number of bytes to copy
 * @tmz: if a secure copy should be used
 * @resv: resv object to sync to
 * @f: Returns the last fence if multiple jobs are submitted.
>>>>>>> upstream/android-13
 *
 * The function copies @size bytes from {src->mem + src->offset} to
 * {dst->mem + dst->offset}. src->bo and dst->bo could be same BO for a
 * move and different for a BO to BO copy.
 *
<<<<<<< HEAD
 * @f: Returns the last fence if multiple jobs are submitted.
 */
int amdgpu_ttm_copy_mem_to_mem(struct amdgpu_device *adev,
			       struct amdgpu_copy_mem *src,
			       struct amdgpu_copy_mem *dst,
			       uint64_t size,
			       struct reservation_object *resv,
			       struct dma_fence **f)
{
	struct amdgpu_ring *ring = adev->mman.buffer_funcs_ring;
	struct drm_mm_node *src_mm, *dst_mm;
	uint64_t src_node_start, dst_node_start, src_node_size,
		 dst_node_size, src_page_offset, dst_page_offset;
	struct dma_fence *fence = NULL;
	int r = 0;
	const uint64_t GTT_MAX_BYTES = (AMDGPU_GTT_MAX_TRANSFER_SIZE *
					AMDGPU_GPU_PAGE_SIZE);
=======
 */
int amdgpu_ttm_copy_mem_to_mem(struct amdgpu_device *adev,
			       const struct amdgpu_copy_mem *src,
			       const struct amdgpu_copy_mem *dst,
			       uint64_t size, bool tmz,
			       struct dma_resv *resv,
			       struct dma_fence **f)
{
	const uint32_t GTT_MAX_BYTES = (AMDGPU_GTT_MAX_TRANSFER_SIZE *
					AMDGPU_GPU_PAGE_SIZE);

	struct amdgpu_ring *ring = adev->mman.buffer_funcs_ring;
	struct amdgpu_res_cursor src_mm, dst_mm;
	struct dma_fence *fence = NULL;
	int r = 0;
>>>>>>> upstream/android-13

	if (!adev->mman.buffer_funcs_enabled) {
		DRM_ERROR("Trying to move memory with ring turned off.\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	src_mm = amdgpu_find_mm_node(src->mem, &src->offset);
	src_node_start = amdgpu_mm_node_addr(src->bo, src_mm, src->mem) +
					     src->offset;
	src_node_size = (src_mm->size << PAGE_SHIFT) - src->offset;
	src_page_offset = src_node_start & (PAGE_SIZE - 1);

	dst_mm = amdgpu_find_mm_node(dst->mem, &dst->offset);
	dst_node_start = amdgpu_mm_node_addr(dst->bo, dst_mm, dst->mem) +
					     dst->offset;
	dst_node_size = (dst_mm->size << PAGE_SHIFT) - dst->offset;
	dst_page_offset = dst_node_start & (PAGE_SIZE - 1);

	mutex_lock(&adev->mman.gtt_window_lock);

	while (size) {
		unsigned long cur_size;
		uint64_t from = src_node_start, to = dst_node_start;
		struct dma_fence *next;
=======
	amdgpu_res_first(src->mem, src->offset, size, &src_mm);
	amdgpu_res_first(dst->mem, dst->offset, size, &dst_mm);

	mutex_lock(&adev->mman.gtt_window_lock);
	while (src_mm.remaining) {
		uint32_t src_page_offset = src_mm.start & ~PAGE_MASK;
		uint32_t dst_page_offset = dst_mm.start & ~PAGE_MASK;
		struct dma_fence *next;
		uint32_t cur_size;
		uint64_t from, to;
>>>>>>> upstream/android-13

		/* Copy size cannot exceed GTT_MAX_BYTES. So if src or dst
		 * begins at an offset, then adjust the size accordingly
		 */
<<<<<<< HEAD
		cur_size = min3(min(src_node_size, dst_node_size), size,
				GTT_MAX_BYTES);
		if (cur_size + src_page_offset > GTT_MAX_BYTES ||
		    cur_size + dst_page_offset > GTT_MAX_BYTES)
			cur_size -= max(src_page_offset, dst_page_offset);

		/* Map only what needs to be accessed. Map src to window 0 and
		 * dst to window 1
		 */
		if (src->mem->mem_type == TTM_PL_TT &&
		    !amdgpu_gtt_mgr_has_gart_addr(src->mem)) {
			r = amdgpu_map_buffer(src->bo, src->mem,
					PFN_UP(cur_size + src_page_offset),
					src_node_start, 0, ring,
					&from);
			if (r)
				goto error;
			/* Adjust the offset because amdgpu_map_buffer returns
			 * start of mapped page
			 */
			from += src_page_offset;
		}

		if (dst->mem->mem_type == TTM_PL_TT &&
		    !amdgpu_gtt_mgr_has_gart_addr(dst->mem)) {
			r = amdgpu_map_buffer(dst->bo, dst->mem,
					PFN_UP(cur_size + dst_page_offset),
					dst_node_start, 1, ring,
					&to);
			if (r)
				goto error;
			to += dst_page_offset;
		}

		r = amdgpu_copy_buffer(ring, from, to, cur_size,
				       resv, &next, false, true);
=======
		cur_size = max(src_page_offset, dst_page_offset);
		cur_size = min(min3(src_mm.size, dst_mm.size, size),
			       (uint64_t)(GTT_MAX_BYTES - cur_size));

		/* Map src to window 0 and dst to window 1. */
		r = amdgpu_ttm_map_buffer(src->bo, src->mem, &src_mm,
					  PFN_UP(cur_size + src_page_offset),
					  0, ring, tmz, &from);
		if (r)
			goto error;

		r = amdgpu_ttm_map_buffer(dst->bo, dst->mem, &dst_mm,
					  PFN_UP(cur_size + dst_page_offset),
					  1, ring, tmz, &to);
		if (r)
			goto error;

		r = amdgpu_copy_buffer(ring, from, to, cur_size,
				       resv, &next, false, true, tmz);
>>>>>>> upstream/android-13
		if (r)
			goto error;

		dma_fence_put(fence);
		fence = next;

<<<<<<< HEAD
		size -= cur_size;
		if (!size)
			break;

		src_node_size -= cur_size;
		if (!src_node_size) {
			src_node_start = amdgpu_mm_node_addr(src->bo, ++src_mm,
							     src->mem);
			src_node_size = (src_mm->size << PAGE_SHIFT);
		} else {
			src_node_start += cur_size;
			src_page_offset = src_node_start & (PAGE_SIZE - 1);
		}
		dst_node_size -= cur_size;
		if (!dst_node_size) {
			dst_node_start = amdgpu_mm_node_addr(dst->bo, ++dst_mm,
							     dst->mem);
			dst_node_size = (dst_mm->size << PAGE_SHIFT);
		} else {
			dst_node_start += cur_size;
			dst_page_offset = dst_node_start & (PAGE_SIZE - 1);
		}
=======
		amdgpu_res_next(&src_mm, cur_size);
		amdgpu_res_next(&dst_mm, cur_size);
>>>>>>> upstream/android-13
	}
error:
	mutex_unlock(&adev->mman.gtt_window_lock);
	if (f)
		*f = dma_fence_get(fence);
	dma_fence_put(fence);
	return r;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_move_blit - Copy an entire buffer to another buffer
 *
 * This is a helper called by amdgpu_bo_move() and amdgpu_move_vram_ram() to
 * help move buffers to and from VRAM.
 */
static int amdgpu_move_blit(struct ttm_buffer_object *bo,
<<<<<<< HEAD
			    bool evict, bool no_wait_gpu,
			    struct ttm_mem_reg *new_mem,
			    struct ttm_mem_reg *old_mem)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->bdev);
=======
			    bool evict,
			    struct ttm_resource *new_mem,
			    struct ttm_resource *old_mem)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->bdev);
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(bo);
>>>>>>> upstream/android-13
	struct amdgpu_copy_mem src, dst;
	struct dma_fence *fence = NULL;
	int r;

	src.bo = bo;
	dst.bo = bo;
	src.mem = old_mem;
	dst.mem = new_mem;
	src.offset = 0;
	dst.offset = 0;

	r = amdgpu_ttm_copy_mem_to_mem(adev, &src, &dst,
				       new_mem->num_pages << PAGE_SHIFT,
<<<<<<< HEAD
				       bo->resv, &fence);
	if (r)
		goto error;

	r = ttm_bo_pipeline_move(bo, fence, evict, new_mem);
=======
				       amdgpu_bo_encrypted(abo),
				       bo->base.resv, &fence);
	if (r)
		goto error;

	/* clear the space being freed */
	if (old_mem->mem_type == TTM_PL_VRAM &&
	    (abo->flags & AMDGPU_GEM_CREATE_VRAM_WIPE_ON_RELEASE)) {
		struct dma_fence *wipe_fence = NULL;

		r = amdgpu_fill_buffer(ttm_to_amdgpu_bo(bo), AMDGPU_POISON,
				       NULL, &wipe_fence);
		if (r) {
			goto error;
		} else if (wipe_fence) {
			dma_fence_put(fence);
			fence = wipe_fence;
		}
	}

	/* Always block for VM page tables before committing the new location */
	if (bo->type == ttm_bo_type_kernel)
		r = ttm_bo_move_accel_cleanup(bo, fence, true, false, new_mem);
	else
		r = ttm_bo_move_accel_cleanup(bo, fence, evict, true, new_mem);
>>>>>>> upstream/android-13
	dma_fence_put(fence);
	return r;

error:
	if (fence)
		dma_fence_wait(fence, false);
	dma_fence_put(fence);
	return r;
}

<<<<<<< HEAD
/**
 * amdgpu_move_vram_ram - Copy VRAM buffer to RAM buffer
 *
 * Called by amdgpu_bo_move().
 */
static int amdgpu_move_vram_ram(struct ttm_buffer_object *bo, bool evict,
				struct ttm_operation_ctx *ctx,
				struct ttm_mem_reg *new_mem)
{
	struct amdgpu_device *adev;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg tmp_mem;
	struct ttm_place placements;
	struct ttm_placement placement;
	int r;

	adev = amdgpu_ttm_adev(bo->bdev);

	/* create space/pages for new_mem in GTT space */
	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	placement.num_placement = 1;
	placement.placement = &placements;
	placement.num_busy_placement = 1;
	placement.busy_placement = &placements;
	placements.fpfn = 0;
	placements.lpfn = 0;
	placements.flags = TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT;
	r = ttm_bo_mem_space(bo, &placement, &tmp_mem, ctx);
	if (unlikely(r)) {
		return r;
	}

	/* set caching flags */
	r = ttm_tt_set_placement_caching(bo->ttm, tmp_mem.placement);
	if (unlikely(r)) {
		goto out_cleanup;
	}

	/* Bind the memory to the GTT space */
	r = ttm_tt_bind(bo->ttm, &tmp_mem, ctx);
	if (unlikely(r)) {
		goto out_cleanup;
	}

	/* blit VRAM to GTT */
	r = amdgpu_move_blit(bo, evict, ctx->no_wait_gpu, &tmp_mem, old_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}

	/* move BO (in tmp_mem) to new_mem */
	r = ttm_bo_move_ttm(bo, ctx, new_mem);
out_cleanup:
	ttm_bo_mem_put(bo, &tmp_mem);
	return r;
}

/**
 * amdgpu_move_ram_vram - Copy buffer from RAM to VRAM
 *
 * Called by amdgpu_bo_move().
 */
static int amdgpu_move_ram_vram(struct ttm_buffer_object *bo, bool evict,
				struct ttm_operation_ctx *ctx,
				struct ttm_mem_reg *new_mem)
{
	struct amdgpu_device *adev;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg tmp_mem;
	struct ttm_placement placement;
	struct ttm_place placements;
	int r;

	adev = amdgpu_ttm_adev(bo->bdev);

	/* make space in GTT for old_mem buffer */
	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	placement.num_placement = 1;
	placement.placement = &placements;
	placement.num_busy_placement = 1;
	placement.busy_placement = &placements;
	placements.fpfn = 0;
	placements.lpfn = 0;
	placements.flags = TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT;
	r = ttm_bo_mem_space(bo, &placement, &tmp_mem, ctx);
	if (unlikely(r)) {
		return r;
	}

	/* move/bind old memory to GTT space */
	r = ttm_bo_move_ttm(bo, ctx, &tmp_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}

	/* copy to VRAM */
	r = amdgpu_move_blit(bo, evict, ctx->no_wait_gpu, new_mem, old_mem);
	if (unlikely(r)) {
		goto out_cleanup;
	}
out_cleanup:
	ttm_bo_mem_put(bo, &tmp_mem);
	return r;
}

/**
=======
/*
 * amdgpu_mem_visible - Check that memory can be accessed by ttm_bo_move_memcpy
 *
 * Called by amdgpu_bo_move()
 */
static bool amdgpu_mem_visible(struct amdgpu_device *adev,
			       struct ttm_resource *mem)
{
	uint64_t mem_size = (u64)mem->num_pages << PAGE_SHIFT;
	struct amdgpu_res_cursor cursor;

	if (mem->mem_type == TTM_PL_SYSTEM ||
	    mem->mem_type == TTM_PL_TT)
		return true;
	if (mem->mem_type != TTM_PL_VRAM)
		return false;

	amdgpu_res_first(mem, 0, mem_size, &cursor);

	/* ttm_resource_ioremap only supports contiguous memory */
	if (cursor.size != mem_size)
		return false;

	return cursor.start + cursor.size <= adev->gmc.visible_vram_size;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_bo_move - Move a buffer object to a new memory location
 *
 * Called by ttm_bo_handle_move_mem()
 */
static int amdgpu_bo_move(struct ttm_buffer_object *bo, bool evict,
			  struct ttm_operation_ctx *ctx,
<<<<<<< HEAD
			  struct ttm_mem_reg *new_mem)
{
	struct amdgpu_device *adev;
	struct amdgpu_bo *abo;
	struct ttm_mem_reg *old_mem = &bo->mem;
	int r;

	/* Can't move a pinned BO */
	abo = ttm_to_amdgpu_bo(bo);
	if (WARN_ON_ONCE(abo->pin_count > 0))
=======
			  struct ttm_resource *new_mem,
			  struct ttm_place *hop)
{
	struct amdgpu_device *adev;
	struct amdgpu_bo *abo;
	struct ttm_resource *old_mem = bo->resource;
	int r;

	if (new_mem->mem_type == TTM_PL_TT ||
	    new_mem->mem_type == AMDGPU_PL_PREEMPT) {
		r = amdgpu_ttm_backend_bind(bo->bdev, bo->ttm, new_mem);
		if (r)
			return r;
	}

	/* Can't move a pinned BO */
	abo = ttm_to_amdgpu_bo(bo);
	if (WARN_ON_ONCE(abo->tbo.pin_count > 0))
>>>>>>> upstream/android-13
		return -EINVAL;

	adev = amdgpu_ttm_adev(bo->bdev);

	if (old_mem->mem_type == TTM_PL_SYSTEM && bo->ttm == NULL) {
<<<<<<< HEAD
		amdgpu_move_null(bo, new_mem);
		return 0;
	}
	if ((old_mem->mem_type == TTM_PL_TT &&
	     new_mem->mem_type == TTM_PL_SYSTEM) ||
	    (old_mem->mem_type == TTM_PL_SYSTEM &&
	     new_mem->mem_type == TTM_PL_TT)) {
		/* bind is enough */
		amdgpu_move_null(bo, new_mem);
		return 0;
	}

	if (!adev->mman.buffer_funcs_enabled)
		goto memcpy;

	if (old_mem->mem_type == TTM_PL_VRAM &&
	    new_mem->mem_type == TTM_PL_SYSTEM) {
		r = amdgpu_move_vram_ram(bo, evict, ctx, new_mem);
	} else if (old_mem->mem_type == TTM_PL_SYSTEM &&
		   new_mem->mem_type == TTM_PL_VRAM) {
		r = amdgpu_move_ram_vram(bo, evict, ctx, new_mem);
	} else {
		r = amdgpu_move_blit(bo, evict, ctx->no_wait_gpu,
				     new_mem, old_mem);
	}

	if (r) {
memcpy:
		r = ttm_bo_move_memcpy(bo, ctx, new_mem);
		if (r) {
			return r;
		}
=======
		ttm_bo_move_null(bo, new_mem);
		goto out;
	}
	if (old_mem->mem_type == TTM_PL_SYSTEM &&
	    (new_mem->mem_type == TTM_PL_TT ||
	     new_mem->mem_type == AMDGPU_PL_PREEMPT)) {
		ttm_bo_move_null(bo, new_mem);
		goto out;
	}
	if ((old_mem->mem_type == TTM_PL_TT ||
	     old_mem->mem_type == AMDGPU_PL_PREEMPT) &&
	    new_mem->mem_type == TTM_PL_SYSTEM) {
		r = ttm_bo_wait_ctx(bo, ctx);
		if (r)
			return r;

		amdgpu_ttm_backend_unbind(bo->bdev, bo->ttm);
		ttm_resource_free(bo, &bo->resource);
		ttm_bo_assign_mem(bo, new_mem);
		goto out;
	}

	if (old_mem->mem_type == AMDGPU_PL_GDS ||
	    old_mem->mem_type == AMDGPU_PL_GWS ||
	    old_mem->mem_type == AMDGPU_PL_OA ||
	    new_mem->mem_type == AMDGPU_PL_GDS ||
	    new_mem->mem_type == AMDGPU_PL_GWS ||
	    new_mem->mem_type == AMDGPU_PL_OA) {
		/* Nothing to save here */
		ttm_bo_move_null(bo, new_mem);
		goto out;
>>>>>>> upstream/android-13
	}

	if (bo->type == ttm_bo_type_device &&
	    new_mem->mem_type == TTM_PL_VRAM &&
	    old_mem->mem_type != TTM_PL_VRAM) {
		/* amdgpu_bo_fault_reserve_notify will re-set this if the CPU
		 * accesses the BO after it's moved.
		 */
		abo->flags &= ~AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED;
	}

<<<<<<< HEAD
	/* update statistics */
	atomic64_add((u64)bo->num_pages << PAGE_SHIFT, &adev->num_bytes_moved);
	return 0;
}

/**
=======
	if (adev->mman.buffer_funcs_enabled) {
		if (((old_mem->mem_type == TTM_PL_SYSTEM &&
		      new_mem->mem_type == TTM_PL_VRAM) ||
		     (old_mem->mem_type == TTM_PL_VRAM &&
		      new_mem->mem_type == TTM_PL_SYSTEM))) {
			hop->fpfn = 0;
			hop->lpfn = 0;
			hop->mem_type = TTM_PL_TT;
			hop->flags = TTM_PL_FLAG_TEMPORARY;
			return -EMULTIHOP;
		}

		r = amdgpu_move_blit(bo, evict, new_mem, old_mem);
	} else {
		r = -ENODEV;
	}

	if (r) {
		/* Check that all memory is CPU accessible */
		if (!amdgpu_mem_visible(adev, old_mem) ||
		    !amdgpu_mem_visible(adev, new_mem)) {
			pr_err("Move buffer fallback to memcpy unavailable\n");
			return r;
		}

		r = ttm_bo_move_memcpy(bo, ctx, new_mem);
		if (r)
			return r;
	}

out:
	/* update statistics */
	atomic64_add(bo->base.size, &adev->num_bytes_moved);
	amdgpu_bo_move_notify(bo, evict, new_mem);
	return 0;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_io_mem_reserve - Reserve a block of memory during a fault
 *
 * Called by ttm_mem_io_reserve() ultimately via ttm_bo_vm_fault()
 */
<<<<<<< HEAD
static int amdgpu_ttm_io_mem_reserve(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
	struct drm_mm_node *mm_node = mem->mm_node;

	mem->bus.addr = NULL;
	mem->bus.offset = 0;
	mem->bus.size = mem->num_pages << PAGE_SHIFT;
	mem->bus.base = 0;
	mem->bus.is_iomem = false;
	if (!(man->flags & TTM_MEMTYPE_FLAG_MAPPABLE))
		return -EINVAL;
=======
static int amdgpu_ttm_io_mem_reserve(struct ttm_device *bdev,
				     struct ttm_resource *mem)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
	size_t bus_size = (size_t)mem->num_pages << PAGE_SHIFT;

>>>>>>> upstream/android-13
	switch (mem->mem_type) {
	case TTM_PL_SYSTEM:
		/* system memory */
		return 0;
	case TTM_PL_TT:
<<<<<<< HEAD
=======
	case AMDGPU_PL_PREEMPT:
>>>>>>> upstream/android-13
		break;
	case TTM_PL_VRAM:
		mem->bus.offset = mem->start << PAGE_SHIFT;
		/* check if it's visible */
<<<<<<< HEAD
		if ((mem->bus.offset + mem->bus.size) > adev->gmc.visible_vram_size)
			return -EINVAL;
		/* Only physically contiguous buffers apply. In a contiguous
		 * buffer, size of the first mm_node would match the number of
		 * pages in ttm_mem_reg.
		 */
		if (adev->mman.aper_base_kaddr &&
		    (mm_node->size == mem->num_pages))
			mem->bus.addr = (u8 *)adev->mman.aper_base_kaddr +
					mem->bus.offset;

		mem->bus.base = adev->gmc.aper_base;
=======
		if ((mem->bus.offset + bus_size) > adev->gmc.visible_vram_size)
			return -EINVAL;

		if (adev->mman.aper_base_kaddr &&
		    mem->placement & TTM_PL_FLAG_CONTIGUOUS)
			mem->bus.addr = (u8 *)adev->mman.aper_base_kaddr +
					mem->bus.offset;

		mem->bus.offset += adev->gmc.aper_base;
>>>>>>> upstream/android-13
		mem->bus.is_iomem = true;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

<<<<<<< HEAD
static void amdgpu_ttm_io_mem_free(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
}

static unsigned long amdgpu_ttm_io_mem_pfn(struct ttm_buffer_object *bo,
					   unsigned long page_offset)
{
	struct drm_mm_node *mm;
	unsigned long offset = (page_offset << PAGE_SHIFT);

	mm = amdgpu_find_mm_node(&bo->mem, &offset);
	return (bo->mem.bus.base >> PAGE_SHIFT) + mm->start +
		(offset >> PAGE_SHIFT);
=======
static unsigned long amdgpu_ttm_io_mem_pfn(struct ttm_buffer_object *bo,
					   unsigned long page_offset)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->bdev);
	struct amdgpu_res_cursor cursor;

	amdgpu_res_first(bo->resource, (u64)page_offset << PAGE_SHIFT, 0,
			 &cursor);
	return (adev->gmc.aper_base + cursor.start) >> PAGE_SHIFT;
}

/**
 * amdgpu_ttm_domain_start - Returns GPU start address
 * @adev: amdgpu device object
 * @type: type of the memory
 *
 * Returns:
 * GPU start address of a memory domain
 */

uint64_t amdgpu_ttm_domain_start(struct amdgpu_device *adev, uint32_t type)
{
	switch (type) {
	case TTM_PL_TT:
		return adev->gmc.gart_start;
	case TTM_PL_VRAM:
		return adev->gmc.vram_start;
	}

	return 0;
>>>>>>> upstream/android-13
}

/*
 * TTM backend functions.
 */
<<<<<<< HEAD
struct amdgpu_ttm_gup_task_list {
	struct list_head	list;
	struct task_struct	*task;
};

struct amdgpu_ttm_tt {
	struct ttm_dma_tt	ttm;
=======
struct amdgpu_ttm_tt {
	struct ttm_tt	ttm;
	struct drm_gem_object	*gobj;
>>>>>>> upstream/android-13
	u64			offset;
	uint64_t		userptr;
	struct task_struct	*usertask;
	uint32_t		userflags;
<<<<<<< HEAD
	spinlock_t              guptasklock;
	struct list_head        guptasks;
	atomic_t		mmu_invalidations;
	uint32_t		last_set_pages;
};

/**
 * amdgpu_ttm_tt_get_user_pages - Pin pages of memory pointed to by a USERPTR
 * pointer to memory
 *
 * Called by amdgpu_gem_userptr_ioctl() and amdgpu_cs_parser_bos().
 * This provides a wrapper around the get_user_pages() call to provide
 * device accessible pages that back user memory.
 */
int amdgpu_ttm_tt_get_user_pages(struct ttm_tt *ttm, struct page **pages)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	struct mm_struct *mm = gtt->usertask->mm;
	unsigned int flags = 0;
	unsigned pinned = 0;
	int r;

	if (!mm) /* Happens during process shutdown */
		return -ESRCH;

	if (!(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY))
		flags |= FOLL_WRITE;

	down_read(&mm->mmap_sem);

	if (gtt->userflags & AMDGPU_GEM_USERPTR_ANONONLY) {
		/*
		 * check that we only use anonymous memory to prevent problems
		 * with writeback
		 */
		unsigned long end = gtt->userptr + ttm->num_pages * PAGE_SIZE;
		struct vm_area_struct *vma;

		vma = find_vma(mm, gtt->userptr);
		if (!vma || vma->vm_file || vma->vm_end < end) {
			up_read(&mm->mmap_sem);
			return -EPERM;
		}
	}

	/* loop enough times using contiguous pages of memory */
	do {
		unsigned num_pages = ttm->num_pages - pinned;
		uint64_t userptr = gtt->userptr + pinned * PAGE_SIZE;
		struct page **p = pages + pinned;
		struct amdgpu_ttm_gup_task_list guptask;

		guptask.task = current;
		spin_lock(&gtt->guptasklock);
		list_add(&guptask.list, &gtt->guptasks);
		spin_unlock(&gtt->guptasklock);

		if (mm == current->mm)
			r = get_user_pages(userptr, num_pages, flags, p, NULL);
		else
			r = get_user_pages_remote(gtt->usertask,
					mm, userptr, num_pages,
					flags, p, NULL, NULL);

		spin_lock(&gtt->guptasklock);
		list_del(&guptask.list);
		spin_unlock(&gtt->guptasklock);

		if (r < 0)
			goto release_pages;

		pinned += r;

	} while (pinned < ttm->num_pages);

	up_read(&mm->mmap_sem);
	return 0;

release_pages:
	release_pages(pages, pinned);
	up_read(&mm->mmap_sem);
	return r;
}

/**
=======
	bool			bound;
#if IS_ENABLED(CONFIG_DRM_AMDGPU_USERPTR)
	struct hmm_range	*range;
#endif
};

#ifdef CONFIG_DRM_AMDGPU_USERPTR
/*
 * amdgpu_ttm_tt_get_user_pages - get device accessible pages that back user
 * memory and start HMM tracking CPU page table update
 *
 * Calling function must call amdgpu_ttm_tt_userptr_range_done() once and only
 * once afterwards to stop HMM tracking
 */
int amdgpu_ttm_tt_get_user_pages(struct amdgpu_bo *bo, struct page **pages)
{
	struct ttm_tt *ttm = bo->tbo.ttm;
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	unsigned long start = gtt->userptr;
	struct vm_area_struct *vma;
	struct mm_struct *mm;
	bool readonly;
	int r = 0;

	mm = bo->notifier.mm;
	if (unlikely(!mm)) {
		DRM_DEBUG_DRIVER("BO is not registered?\n");
		return -EFAULT;
	}

	/* Another get_user_pages is running at the same time?? */
	if (WARN_ON(gtt->range))
		return -EFAULT;

	if (!mmget_not_zero(mm)) /* Happens during process shutdown */
		return -ESRCH;

	mmap_read_lock(mm);
	vma = vma_lookup(mm, start);
	if (unlikely(!vma)) {
		r = -EFAULT;
		goto out_unlock;
	}
	if (unlikely((gtt->userflags & AMDGPU_GEM_USERPTR_ANONONLY) &&
		vma->vm_file)) {
		r = -EPERM;
		goto out_unlock;
	}

	readonly = amdgpu_ttm_tt_is_readonly(ttm);
	r = amdgpu_hmm_range_get_pages(&bo->notifier, mm, pages, start,
				       ttm->num_pages, &gtt->range, readonly,
				       true, NULL);
out_unlock:
	mmap_read_unlock(mm);
	mmput(mm);

	return r;
}

/*
 * amdgpu_ttm_tt_userptr_range_done - stop HMM track the CPU page table change
 * Check if the pages backing this ttm range have been invalidated
 *
 * Returns: true if pages are still valid
 */
bool amdgpu_ttm_tt_get_user_pages_done(struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	bool r = false;

	if (!gtt || !gtt->userptr)
		return false;

	DRM_DEBUG_DRIVER("user_pages_done 0x%llx pages 0x%x\n",
		gtt->userptr, ttm->num_pages);

	WARN_ONCE(!gtt->range || !gtt->range->hmm_pfns,
		"No user pages to check\n");

	if (gtt->range) {
		/*
		 * FIXME: Must always hold notifier_lock for this, and must
		 * not ignore the return code.
		 */
		r = amdgpu_hmm_range_get_pages_done(gtt->range);
		gtt->range = NULL;
	}

	return !r;
}
#endif

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_set_user_pages - Copy pages in, putting old pages as necessary.
 *
 * Called by amdgpu_cs_list_validate(). This creates the page list
 * that backs user memory and will ultimately be mapped into the device
 * address space.
 */
void amdgpu_ttm_tt_set_user_pages(struct ttm_tt *ttm, struct page **pages)
{
<<<<<<< HEAD
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	unsigned i;

	gtt->last_set_pages = atomic_read(&gtt->mmu_invalidations);
	for (i = 0; i < ttm->num_pages; ++i) {
		if (ttm->pages[i])
			put_page(ttm->pages[i]);

		ttm->pages[i] = pages ? pages[i] : NULL;
	}
}

/**
 * amdgpu_ttm_tt_mark_user_page - Mark pages as dirty
 *
 * Called while unpinning userptr pages
 */
void amdgpu_ttm_tt_mark_user_pages(struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	unsigned i;

	for (i = 0; i < ttm->num_pages; ++i) {
		struct page *page = ttm->pages[i];

		if (!page)
			continue;

		if (!(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY))
			set_page_dirty(page);

		mark_page_accessed(page);
	}
}

/**
 * amdgpu_ttm_tt_pin_userptr - 	prepare the sg table with the user pages
 *
 * Called by amdgpu_ttm_backend_bind()
 **/
static int amdgpu_ttm_tt_pin_userptr(struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(ttm->bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	unsigned nents;
	int r;

	int write = !(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY);
	enum dma_data_direction direction = write ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;

	/* Allocate an SG array and squash pages into it */
	r = sg_alloc_table_from_pages(ttm->sg, ttm->pages, ttm->num_pages, 0,
				      ttm->num_pages << PAGE_SHIFT,
=======
	unsigned long i;

	for (i = 0; i < ttm->num_pages; ++i)
		ttm->pages[i] = pages ? pages[i] : NULL;
}

/*
 * amdgpu_ttm_tt_pin_userptr - prepare the sg table with the user pages
 *
 * Called by amdgpu_ttm_backend_bind()
 **/
static int amdgpu_ttm_tt_pin_userptr(struct ttm_device *bdev,
				     struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	int write = !(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY);
	enum dma_data_direction direction = write ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;
	int r;

	/* Allocate an SG array and squash pages into it */
	r = sg_alloc_table_from_pages(ttm->sg, ttm->pages, ttm->num_pages, 0,
				      (u64)ttm->num_pages << PAGE_SHIFT,
>>>>>>> upstream/android-13
				      GFP_KERNEL);
	if (r)
		goto release_sg;

	/* Map SG to device */
<<<<<<< HEAD
	r = -ENOMEM;
	nents = dma_map_sg(adev->dev, ttm->sg->sgl, ttm->sg->nents, direction);
	if (nents != ttm->sg->nents)
		goto release_sg;

	/* convert SG to linear array of pages and dma addresses */
	drm_prime_sg_to_page_addr_arrays(ttm->sg, ttm->pages,
					 gtt->ttm.dma_address, ttm->num_pages);
=======
	r = dma_map_sgtable(adev->dev, ttm->sg, direction, 0);
	if (r)
		goto release_sg;

	/* convert SG to linear array of pages and dma addresses */
	drm_prime_sg_to_dma_addr_array(ttm->sg, gtt->ttm.dma_address,
				       ttm->num_pages);
>>>>>>> upstream/android-13

	return 0;

release_sg:
	kfree(ttm->sg);
	ttm->sg = NULL;
	return r;
}

<<<<<<< HEAD
/**
 * amdgpu_ttm_tt_unpin_userptr - Unpin and unmap userptr pages
 */
static void amdgpu_ttm_tt_unpin_userptr(struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(ttm->bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

=======
/*
 * amdgpu_ttm_tt_unpin_userptr - Unpin and unmap userptr pages
 */
static void amdgpu_ttm_tt_unpin_userptr(struct ttm_device *bdev,
					struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
>>>>>>> upstream/android-13
	int write = !(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY);
	enum dma_data_direction direction = write ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;

	/* double check that we don't free the table twice */
	if (!ttm->sg || !ttm->sg->sgl)
		return;

	/* unmap the pages mapped to the device */
<<<<<<< HEAD
	dma_unmap_sg(adev->dev, ttm->sg->sgl, ttm->sg->nents, direction);

	/* mark the pages as dirty */
	amdgpu_ttm_tt_mark_user_pages(ttm);

	sg_free_table(ttm->sg);
}

int amdgpu_ttm_gart_bind(struct amdgpu_device *adev,
=======
	dma_unmap_sgtable(adev->dev, ttm->sg, direction, 0);
	sg_free_table(ttm->sg);

#if IS_ENABLED(CONFIG_DRM_AMDGPU_USERPTR)
	if (gtt->range) {
		unsigned long i;

		for (i = 0; i < ttm->num_pages; i++) {
			if (ttm->pages[i] !=
			    hmm_pfn_to_page(gtt->range->hmm_pfns[i]))
				break;
		}

		WARN((i == ttm->num_pages), "Missing get_user_page_done\n");
	}
#endif
}

static int amdgpu_ttm_gart_bind(struct amdgpu_device *adev,
>>>>>>> upstream/android-13
				struct ttm_buffer_object *tbo,
				uint64_t flags)
{
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(tbo);
	struct ttm_tt *ttm = tbo->ttm;
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	int r;

<<<<<<< HEAD
	if (abo->flags & AMDGPU_GEM_CREATE_MQD_GFX9) {
		uint64_t page_idx = 1;

		r = amdgpu_gart_bind(adev, gtt->offset, page_idx,
				ttm->pages, gtt->ttm.dma_address, flags);
		if (r)
			goto gart_bind_fail;

		/* Patch mtype of the second part BO */
		flags &=  ~AMDGPU_PTE_MTYPE_MASK;
		flags |= AMDGPU_PTE_MTYPE(AMDGPU_MTYPE_NC);
=======
	if (amdgpu_bo_encrypted(abo))
		flags |= AMDGPU_PTE_TMZ;

	if (abo->flags & AMDGPU_GEM_CREATE_CP_MQD_GFX9) {
		uint64_t page_idx = 1;

		r = amdgpu_gart_bind(adev, gtt->offset, page_idx,
				gtt->ttm.dma_address, flags);
		if (r)
			goto gart_bind_fail;

		/* The memory type of the first page defaults to UC. Now
		 * modify the memory type to NC from the second page of
		 * the BO onward.
		 */
		flags &= ~AMDGPU_PTE_MTYPE_VG10_MASK;
		flags |= AMDGPU_PTE_MTYPE_VG10(AMDGPU_MTYPE_NC);
>>>>>>> upstream/android-13

		r = amdgpu_gart_bind(adev,
				gtt->offset + (page_idx << PAGE_SHIFT),
				ttm->num_pages - page_idx,
<<<<<<< HEAD
				&ttm->pages[page_idx],
				&(gtt->ttm.dma_address[page_idx]), flags);
	} else {
		r = amdgpu_gart_bind(adev, gtt->offset, ttm->num_pages,
				     ttm->pages, gtt->ttm.dma_address, flags);
=======
				&(gtt->ttm.dma_address[page_idx]), flags);
	} else {
		r = amdgpu_gart_bind(adev, gtt->offset, ttm->num_pages,
				     gtt->ttm.dma_address, flags);
>>>>>>> upstream/android-13
	}

gart_bind_fail:
	if (r)
<<<<<<< HEAD
		DRM_ERROR("failed to bind %lu pages at 0x%08llX\n",
=======
		DRM_ERROR("failed to bind %u pages at 0x%08llX\n",
>>>>>>> upstream/android-13
			  ttm->num_pages, gtt->offset);

	return r;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_backend_bind - Bind GTT memory
 *
 * Called by ttm_tt_bind() on behalf of ttm_bo_handle_move_mem().
 * This handles binding GTT memory to the device address space.
 */
<<<<<<< HEAD
static int amdgpu_ttm_backend_bind(struct ttm_tt *ttm,
				   struct ttm_mem_reg *bo_mem)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(ttm->bdev);
=======
static int amdgpu_ttm_backend_bind(struct ttm_device *bdev,
				   struct ttm_tt *ttm,
				   struct ttm_resource *bo_mem)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
>>>>>>> upstream/android-13
	struct amdgpu_ttm_tt *gtt = (void*)ttm;
	uint64_t flags;
	int r = 0;

<<<<<<< HEAD
	if (gtt->userptr) {
		r = amdgpu_ttm_tt_pin_userptr(ttm);
=======
	if (!bo_mem)
		return -EINVAL;

	if (gtt->bound)
		return 0;

	if (gtt->userptr) {
		r = amdgpu_ttm_tt_pin_userptr(bdev, ttm);
>>>>>>> upstream/android-13
		if (r) {
			DRM_ERROR("failed to pin userptr\n");
			return r;
		}
<<<<<<< HEAD
	}
	if (!ttm->num_pages) {
		WARN(1, "nothing to bind %lu pages for mreg %p back %p!\n",
=======
	} else if (ttm->page_flags & TTM_PAGE_FLAG_SG) {
		if (!ttm->sg) {
			struct dma_buf_attachment *attach;
			struct sg_table *sgt;

			attach = gtt->gobj->import_attach;
			sgt = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
			if (IS_ERR(sgt))
				return PTR_ERR(sgt);

			ttm->sg = sgt;
		}

		drm_prime_sg_to_dma_addr_array(ttm->sg, gtt->ttm.dma_address,
					       ttm->num_pages);
	}

	if (!ttm->num_pages) {
		WARN(1, "nothing to bind %u pages for mreg %p back %p!\n",
>>>>>>> upstream/android-13
		     ttm->num_pages, bo_mem, ttm);
	}

	if (bo_mem->mem_type == AMDGPU_PL_GDS ||
	    bo_mem->mem_type == AMDGPU_PL_GWS ||
	    bo_mem->mem_type == AMDGPU_PL_OA)
		return -EINVAL;

<<<<<<< HEAD
	if (!amdgpu_gtt_mgr_has_gart_addr(bo_mem)) {
=======
	if (bo_mem->mem_type != TTM_PL_TT ||
	    !amdgpu_gtt_mgr_has_gart_addr(bo_mem)) {
>>>>>>> upstream/android-13
		gtt->offset = AMDGPU_BO_INVALID_OFFSET;
		return 0;
	}

	/* compute PTE flags relevant to this BO memory */
	flags = amdgpu_ttm_tt_pte_flags(adev, ttm, bo_mem);

	/* bind pages into GART page tables */
	gtt->offset = (u64)bo_mem->start << PAGE_SHIFT;
	r = amdgpu_gart_bind(adev, gtt->offset, ttm->num_pages,
<<<<<<< HEAD
		ttm->pages, gtt->ttm.dma_address, flags);

	if (r)
		DRM_ERROR("failed to bind %lu pages at 0x%08llX\n",
			  ttm->num_pages, gtt->offset);
	return r;
}

/**
 * amdgpu_ttm_alloc_gart - Allocate GART memory for buffer object
=======
		gtt->ttm.dma_address, flags);

	if (r)
		DRM_ERROR("failed to bind %u pages at 0x%08llX\n",
			  ttm->num_pages, gtt->offset);
	gtt->bound = true;
	return r;
}

/*
 * amdgpu_ttm_alloc_gart - Make sure buffer object is accessible either
 * through AGP or GART aperture.
 *
 * If bo is accessible through AGP aperture, then use AGP aperture
 * to access bo; otherwise allocate logical space in GART aperture
 * and map bo to GART aperture.
>>>>>>> upstream/android-13
 */
int amdgpu_ttm_alloc_gart(struct ttm_buffer_object *bo)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->bdev);
	struct ttm_operation_ctx ctx = { false, false };
<<<<<<< HEAD
	struct amdgpu_ttm_tt *gtt = (void*)bo->ttm;
	struct ttm_mem_reg tmp;
	struct ttm_placement placement;
	struct ttm_place placements;
	uint64_t flags;
	int r;

	if (bo->mem.mem_type != TTM_PL_TT ||
	    amdgpu_gtt_mgr_has_gart_addr(&bo->mem))
		return 0;

	/* allocate GTT space */
	tmp = bo->mem;
	tmp.mm_node = NULL;
=======
	struct amdgpu_ttm_tt *gtt = (void *)bo->ttm;
	struct ttm_placement placement;
	struct ttm_place placements;
	struct ttm_resource *tmp;
	uint64_t addr, flags;
	int r;

	if (bo->resource->start != AMDGPU_BO_INVALID_OFFSET)
		return 0;

	addr = amdgpu_gmc_agp_addr(bo);
	if (addr != AMDGPU_BO_INVALID_OFFSET) {
		bo->resource->start = addr >> PAGE_SHIFT;
		return 0;
	}

	/* allocate GART space */
>>>>>>> upstream/android-13
	placement.num_placement = 1;
	placement.placement = &placements;
	placement.num_busy_placement = 1;
	placement.busy_placement = &placements;
	placements.fpfn = 0;
	placements.lpfn = adev->gmc.gart_size >> PAGE_SHIFT;
<<<<<<< HEAD
	placements.flags = (bo->mem.placement & ~TTM_PL_MASK_MEM) |
		TTM_PL_FLAG_TT;
=======
	placements.mem_type = TTM_PL_TT;
	placements.flags = bo->resource->placement;
>>>>>>> upstream/android-13

	r = ttm_bo_mem_space(bo, &placement, &tmp, &ctx);
	if (unlikely(r))
		return r;

	/* compute PTE flags for this buffer object */
<<<<<<< HEAD
	flags = amdgpu_ttm_tt_pte_flags(adev, bo->ttm, &tmp);

	/* Bind pages */
	gtt->offset = (u64)tmp.start << PAGE_SHIFT;
	r = amdgpu_ttm_gart_bind(adev, bo, flags);
	if (unlikely(r)) {
		ttm_bo_mem_put(bo, &tmp);
		return r;
	}

	ttm_bo_mem_put(bo, &bo->mem);
	bo->mem = tmp;
	bo->offset = (bo->mem.start << PAGE_SHIFT) +
		bo->bdev->man[bo->mem.mem_type].gpu_offset;
=======
	flags = amdgpu_ttm_tt_pte_flags(adev, bo->ttm, tmp);

	/* Bind pages */
	gtt->offset = (u64)tmp->start << PAGE_SHIFT;
	r = amdgpu_ttm_gart_bind(adev, bo, flags);
	if (unlikely(r)) {
		ttm_resource_free(bo, &tmp);
		return r;
	}

	amdgpu_gart_invalidate_tlb(adev);
	ttm_resource_free(bo, &bo->resource);
	ttm_bo_assign_mem(bo, tmp);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_recover_gart - Rebind GTT pages
 *
 * Called by amdgpu_gtt_mgr_recover() from amdgpu_device_reset() to
 * rebind GTT pages during a GPU reset.
 */
int amdgpu_ttm_recover_gart(struct ttm_buffer_object *tbo)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(tbo->bdev);
	uint64_t flags;
	int r;

	if (!tbo->ttm)
		return 0;

<<<<<<< HEAD
	flags = amdgpu_ttm_tt_pte_flags(adev, tbo->ttm, &tbo->mem);
=======
	flags = amdgpu_ttm_tt_pte_flags(adev, tbo->ttm, tbo->resource);
>>>>>>> upstream/android-13
	r = amdgpu_ttm_gart_bind(adev, tbo, flags);

	return r;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_backend_unbind - Unbind GTT mapped pages
 *
 * Called by ttm_tt_unbind() on behalf of ttm_bo_move_ttm() and
 * ttm_tt_destroy().
 */
<<<<<<< HEAD
static int amdgpu_ttm_backend_unbind(struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(ttm->bdev);
=======
static void amdgpu_ttm_backend_unbind(struct ttm_device *bdev,
				      struct ttm_tt *ttm)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
>>>>>>> upstream/android-13
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	int r;

	/* if the pages have userptr pinning then clear that first */
<<<<<<< HEAD
	if (gtt->userptr)
		amdgpu_ttm_tt_unpin_userptr(ttm);

	if (gtt->offset == AMDGPU_BO_INVALID_OFFSET)
		return 0;
=======
	if (gtt->userptr) {
		amdgpu_ttm_tt_unpin_userptr(bdev, ttm);
	} else if (ttm->sg && gtt->gobj->import_attach) {
		struct dma_buf_attachment *attach;

		attach = gtt->gobj->import_attach;
		dma_buf_unmap_attachment(attach, ttm->sg, DMA_BIDIRECTIONAL);
		ttm->sg = NULL;
	}

	if (!gtt->bound)
		return;

	if (gtt->offset == AMDGPU_BO_INVALID_OFFSET)
		return;
>>>>>>> upstream/android-13

	/* unbind shouldn't be done for GDS/GWS/OA in ttm_bo_clean_mm */
	r = amdgpu_gart_unbind(adev, gtt->offset, ttm->num_pages);
	if (r)
<<<<<<< HEAD
		DRM_ERROR("failed to unbind %lu pages at 0x%08llX\n",
			  gtt->ttm.ttm.num_pages, gtt->offset);
	return r;
}

static void amdgpu_ttm_backend_destroy(struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	if (gtt->usertask)
		put_task_struct(gtt->usertask);

	ttm_dma_tt_fini(&gtt->ttm);
	kfree(gtt);
}

static struct ttm_backend_func amdgpu_backend_func = {
	.bind = &amdgpu_ttm_backend_bind,
	.unbind = &amdgpu_ttm_backend_unbind,
	.destroy = &amdgpu_ttm_backend_destroy,
};

=======
		DRM_ERROR("failed to unbind %u pages at 0x%08llX\n",
			  gtt->ttm.num_pages, gtt->offset);
	gtt->bound = false;
}

static void amdgpu_ttm_backend_destroy(struct ttm_device *bdev,
				       struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	amdgpu_ttm_backend_unbind(bdev, ttm);
	ttm_tt_destroy_common(bdev, ttm);
	if (gtt->usertask)
		put_task_struct(gtt->usertask);

	ttm_tt_fini(&gtt->ttm);
	kfree(gtt);
}

>>>>>>> upstream/android-13
/**
 * amdgpu_ttm_tt_create - Create a ttm_tt object for a given BO
 *
 * @bo: The buffer object to create a GTT ttm_tt object around
<<<<<<< HEAD
=======
 * @page_flags: Page flags to be added to the ttm_tt object
>>>>>>> upstream/android-13
 *
 * Called by ttm_tt_create().
 */
static struct ttm_tt *amdgpu_ttm_tt_create(struct ttm_buffer_object *bo,
					   uint32_t page_flags)
{
<<<<<<< HEAD
	struct amdgpu_device *adev;
	struct amdgpu_ttm_tt *gtt;

	adev = amdgpu_ttm_adev(bo->bdev);
=======
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(bo);
	struct amdgpu_ttm_tt *gtt;
	enum ttm_caching caching;
>>>>>>> upstream/android-13

	gtt = kzalloc(sizeof(struct amdgpu_ttm_tt), GFP_KERNEL);
	if (gtt == NULL) {
		return NULL;
	}
<<<<<<< HEAD
	gtt->ttm.ttm.func = &amdgpu_backend_func;

	/* allocate space for the uninitialized page entries */
	if (ttm_sg_tt_init(&gtt->ttm, bo, page_flags)) {
		kfree(gtt);
		return NULL;
	}
	return &gtt->ttm.ttm;
}

/**
=======
	gtt->gobj = &bo->base;

	if (abo->flags & AMDGPU_GEM_CREATE_CPU_GTT_USWC)
		caching = ttm_write_combined;
	else
		caching = ttm_cached;

	/* allocate space for the uninitialized page entries */
	if (ttm_sg_tt_init(&gtt->ttm, bo, page_flags, caching)) {
		kfree(gtt);
		return NULL;
	}
	return &gtt->ttm;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_populate - Map GTT pages visible to the device
 *
 * Map the pages of a ttm_tt object to an address space visible
 * to the underlying device.
 */
<<<<<<< HEAD
static int amdgpu_ttm_tt_populate(struct ttm_tt *ttm,
			struct ttm_operation_ctx *ctx)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(ttm->bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	/* user pages are bound by amdgpu_ttm_tt_pin_userptr() */
	if (gtt && gtt->userptr) {
		ttm->sg = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
		if (!ttm->sg)
			return -ENOMEM;

		ttm->page_flags |= TTM_PAGE_FLAG_SG;
		ttm->state = tt_unbound;
		return 0;
	}

	if (slave && ttm->sg) {
		drm_prime_sg_to_page_addr_arrays(ttm->sg, ttm->pages,
						 gtt->ttm.dma_address,
						 ttm->num_pages);
		ttm->state = tt_unbound;
		return 0;
	}

#ifdef CONFIG_SWIOTLB
	if (adev->need_swiotlb && swiotlb_nr_tbl()) {
		return ttm_dma_populate(&gtt->ttm, adev->dev, ctx);
	}
#endif

	/* fall back to generic helper to populate the page array
	 * and map them to the device */
	return ttm_populate_and_map_pages(adev->dev, &gtt->ttm, ctx);
}

/**
=======
static int amdgpu_ttm_tt_populate(struct ttm_device *bdev,
				  struct ttm_tt *ttm,
				  struct ttm_operation_ctx *ctx)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bdev);
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	/* user pages are bound by amdgpu_ttm_tt_pin_userptr() */
	if (gtt->userptr) {
		ttm->sg = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
		if (!ttm->sg)
			return -ENOMEM;
		return 0;
	}

	if (ttm->page_flags & TTM_PAGE_FLAG_SG)
		return 0;

	return ttm_pool_alloc(&adev->mman.bdev.pool, ttm, ctx);
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_unpopulate - unmap GTT pages and unpopulate page arrays
 *
 * Unmaps pages of a ttm_tt object from the device address space and
 * unpopulates the page array backing it.
 */
<<<<<<< HEAD
static void amdgpu_ttm_tt_unpopulate(struct ttm_tt *ttm)
{
	struct amdgpu_device *adev;
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (gtt && gtt->userptr) {
		amdgpu_ttm_tt_set_user_pages(ttm, NULL);
		kfree(ttm->sg);
		ttm->page_flags &= ~TTM_PAGE_FLAG_SG;
		return;
	}

	if (slave)
		return;

	adev = amdgpu_ttm_adev(ttm->bdev);

#ifdef CONFIG_SWIOTLB
	if (adev->need_swiotlb && swiotlb_nr_tbl()) {
		ttm_dma_unpopulate(&gtt->ttm, adev->dev);
		return;
	}
#endif

	/* fall back to generic helper to unmap and unpopulate array */
	ttm_unmap_and_unpopulate_pages(adev->dev, &gtt->ttm);
=======
static void amdgpu_ttm_tt_unpopulate(struct ttm_device *bdev,
				     struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	struct amdgpu_device *adev;

	if (gtt->userptr) {
		amdgpu_ttm_tt_set_user_pages(ttm, NULL);
		kfree(ttm->sg);
		ttm->sg = NULL;
		return;
	}

	if (ttm->page_flags & TTM_PAGE_FLAG_SG)
		return;

	adev = amdgpu_ttm_adev(bdev);
	return ttm_pool_free(&adev->mman.bdev.pool, ttm);
>>>>>>> upstream/android-13
}

/**
 * amdgpu_ttm_tt_set_userptr - Initialize userptr GTT ttm_tt for the current
 * task
 *
<<<<<<< HEAD
 * @ttm: The ttm_tt object to bind this userptr object to
=======
 * @bo: The ttm_buffer_object to bind this userptr to
>>>>>>> upstream/android-13
 * @addr:  The address in the current tasks VM space to use
 * @flags: Requirements of userptr object.
 *
 * Called by amdgpu_gem_userptr_ioctl() to bind userptr pages
 * to current task
 */
<<<<<<< HEAD
int amdgpu_ttm_tt_set_userptr(struct ttm_tt *ttm, uint64_t addr,
			      uint32_t flags)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	if (gtt == NULL)
		return -EINVAL;

=======
int amdgpu_ttm_tt_set_userptr(struct ttm_buffer_object *bo,
			      uint64_t addr, uint32_t flags)
{
	struct amdgpu_ttm_tt *gtt;

	if (!bo->ttm) {
		/* TODO: We want a separate TTM object type for userptrs */
		bo->ttm = amdgpu_ttm_tt_create(bo, 0);
		if (bo->ttm == NULL)
			return -ENOMEM;
	}

	/* Set TTM_PAGE_FLAG_SG before populate but after create. */
	bo->ttm->page_flags |= TTM_PAGE_FLAG_SG;

	gtt = (void *)bo->ttm;
>>>>>>> upstream/android-13
	gtt->userptr = addr;
	gtt->userflags = flags;

	if (gtt->usertask)
		put_task_struct(gtt->usertask);
	gtt->usertask = current->group_leader;
	get_task_struct(gtt->usertask);

<<<<<<< HEAD
	spin_lock_init(&gtt->guptasklock);
	INIT_LIST_HEAD(&gtt->guptasks);
	atomic_set(&gtt->mmu_invalidations, 0);
	gtt->last_set_pages = 0;

	return 0;
}

/**
=======
	return 0;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_get_usermm - Return memory manager for ttm_tt object
 */
struct mm_struct *amdgpu_ttm_tt_get_usermm(struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	if (gtt == NULL)
		return NULL;

	if (gtt->usertask == NULL)
		return NULL;

	return gtt->usertask->mm;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_affect_userptr - Determine if a ttm_tt object lays inside an
 * address range for the current task.
 *
 */
bool amdgpu_ttm_tt_affect_userptr(struct ttm_tt *ttm, unsigned long start,
				  unsigned long end)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
<<<<<<< HEAD
	struct amdgpu_ttm_gup_task_list *entry;
=======
>>>>>>> upstream/android-13
	unsigned long size;

	if (gtt == NULL || !gtt->userptr)
		return false;

	/* Return false if no part of the ttm_tt object lies within
	 * the range
	 */
<<<<<<< HEAD
	size = (unsigned long)gtt->ttm.ttm.num_pages * PAGE_SIZE;
	if (gtt->userptr > end || gtt->userptr + size <= start)
		return false;

	/* Search the lists of tasks that hold this mapping and see
	 * if current is one of them.  If it is return false.
	 */
	spin_lock(&gtt->guptasklock);
	list_for_each_entry(entry, &gtt->guptasks, list) {
		if (entry->task == current) {
			spin_unlock(&gtt->guptasklock);
			return false;
		}
	}
	spin_unlock(&gtt->guptasklock);

	atomic_inc(&gtt->mmu_invalidations);

	return true;
}

/**
 * amdgpu_ttm_tt_userptr_invalidated - Has the ttm_tt object been invalidated?
 */
bool amdgpu_ttm_tt_userptr_invalidated(struct ttm_tt *ttm,
				       int *last_invalidated)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;
	int prev_invalidated = *last_invalidated;

	*last_invalidated = atomic_read(&gtt->mmu_invalidations);
	return prev_invalidated != *last_invalidated;
}

/**
 * amdgpu_ttm_tt_userptr_needs_pages - Have the pages backing this ttm_tt object
 * been invalidated since the last time they've been set?
 */
bool amdgpu_ttm_tt_userptr_needs_pages(struct ttm_tt *ttm)
=======
	size = (unsigned long)gtt->ttm.num_pages * PAGE_SIZE;
	if (gtt->userptr > end || gtt->userptr + size <= start)
		return false;

	return true;
}

/*
 * amdgpu_ttm_tt_is_userptr - Have the pages backing by userptr?
 */
bool amdgpu_ttm_tt_is_userptr(struct ttm_tt *ttm)
>>>>>>> upstream/android-13
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	if (gtt == NULL || !gtt->userptr)
		return false;

<<<<<<< HEAD
	return atomic_read(&gtt->mmu_invalidations) != gtt->last_set_pages;
}

/**
=======
	return true;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_tt_is_readonly - Is the ttm_tt object read only?
 */
bool amdgpu_ttm_tt_is_readonly(struct ttm_tt *ttm)
{
	struct amdgpu_ttm_tt *gtt = (void *)ttm;

	if (gtt == NULL)
		return false;

	return !!(gtt->userflags & AMDGPU_GEM_USERPTR_READONLY);
}

/**
<<<<<<< HEAD
 * amdgpu_ttm_tt_pte_flags - Compute PTE flags for ttm_tt object
 *
 * @ttm: The ttm_tt object to compute the flags for
 * @mem: The memory registry backing this ttm_tt object
 */
uint64_t amdgpu_ttm_tt_pte_flags(struct amdgpu_device *adev, struct ttm_tt *ttm,
				 struct ttm_mem_reg *mem)
=======
 * amdgpu_ttm_tt_pde_flags - Compute PDE flags for ttm_tt object
 *
 * @ttm: The ttm_tt object to compute the flags for
 * @mem: The memory registry backing this ttm_tt object
 *
 * Figure out the flags to use for a VM PDE (Page Directory Entry).
 */
uint64_t amdgpu_ttm_tt_pde_flags(struct ttm_tt *ttm, struct ttm_resource *mem)
>>>>>>> upstream/android-13
{
	uint64_t flags = 0;

	if (mem && mem->mem_type != TTM_PL_SYSTEM)
		flags |= AMDGPU_PTE_VALID;

<<<<<<< HEAD
	if (mem && mem->mem_type == TTM_PL_TT) {
		flags |= AMDGPU_PTE_SYSTEM;

		if (ttm->caching_state == tt_cached)
			flags |= AMDGPU_PTE_SNOOPED;
	}

=======
	if (mem && (mem->mem_type == TTM_PL_TT ||
		    mem->mem_type == AMDGPU_PL_PREEMPT)) {
		flags |= AMDGPU_PTE_SYSTEM;

		if (ttm->caching == ttm_cached)
			flags |= AMDGPU_PTE_SNOOPED;
	}

	if (mem && mem->mem_type == TTM_PL_VRAM &&
			mem->bus.caching == ttm_cached)
		flags |= AMDGPU_PTE_SNOOPED;

	return flags;
}

/**
 * amdgpu_ttm_tt_pte_flags - Compute PTE flags for ttm_tt object
 *
 * @adev: amdgpu_device pointer
 * @ttm: The ttm_tt object to compute the flags for
 * @mem: The memory registry backing this ttm_tt object
 *
 * Figure out the flags to use for a VM PTE (Page Table Entry).
 */
uint64_t amdgpu_ttm_tt_pte_flags(struct amdgpu_device *adev, struct ttm_tt *ttm,
				 struct ttm_resource *mem)
{
	uint64_t flags = amdgpu_ttm_tt_pde_flags(ttm, mem);

>>>>>>> upstream/android-13
	flags |= adev->gart.gart_pte_flags;
	flags |= AMDGPU_PTE_READABLE;

	if (!amdgpu_ttm_tt_is_readonly(ttm))
		flags |= AMDGPU_PTE_WRITEABLE;

	return flags;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_bo_eviction_valuable - Check to see if we can evict a buffer
 * object.
 *
 * Return true if eviction is sensible. Called by ttm_mem_evict_first() on
 * behalf of ttm_bo_mem_force_space() which tries to evict buffer objects until
 * it can find space for a new object and by ttm_bo_force_list_clean() which is
 * used to clean out a memory space.
 */
static bool amdgpu_ttm_bo_eviction_valuable(struct ttm_buffer_object *bo,
					    const struct ttm_place *place)
{
<<<<<<< HEAD
	unsigned long num_pages = bo->mem.num_pages;
	struct drm_mm_node *node = bo->mem.mm_node;
	struct reservation_object_list *flist;
	struct dma_fence *f;
	int i;

=======
	unsigned long num_pages = bo->resource->num_pages;
	struct amdgpu_res_cursor cursor;
	struct dma_resv_list *flist;
	struct dma_fence *f;
	int i;

	/* Swapout? */
	if (bo->resource->mem_type == TTM_PL_SYSTEM)
		return true;

	if (bo->type == ttm_bo_type_kernel &&
	    !amdgpu_vm_evictable(ttm_to_amdgpu_bo(bo)))
		return false;

>>>>>>> upstream/android-13
	/* If bo is a KFD BO, check if the bo belongs to the current process.
	 * If true, then return false as any KFD process needs all its BOs to
	 * be resident to run successfully
	 */
<<<<<<< HEAD
	flist = reservation_object_get_list(bo->resv);
	if (flist) {
		for (i = 0; i < flist->shared_count; ++i) {
			f = rcu_dereference_protected(flist->shared[i],
				reservation_object_held(bo->resv));
=======
	flist = dma_resv_shared_list(bo->base.resv);
	if (flist) {
		for (i = 0; i < flist->shared_count; ++i) {
			f = rcu_dereference_protected(flist->shared[i],
				dma_resv_held(bo->base.resv));
>>>>>>> upstream/android-13
			if (amdkfd_fence_check_mm(f, current->mm))
				return false;
		}
	}

<<<<<<< HEAD
	switch (bo->mem.mem_type) {
	case TTM_PL_TT:
=======
	switch (bo->resource->mem_type) {
	case AMDGPU_PL_PREEMPT:
		/* Preemptible BOs don't own system resources managed by the
		 * driver (pages, VRAM, GART space). They point to resources
		 * owned by someone else (e.g. pageable memory in user mode
		 * or a DMABuf). They are used in a preemptible context so we
		 * can guarantee no deadlocks and good QoS in case of MMU
		 * notifiers or DMABuf move notifiers from the resource owner.
		 */
		return false;
	case TTM_PL_TT:
		if (amdgpu_bo_is_amdgpu_bo(bo) &&
		    amdgpu_bo_encrypted(ttm_to_amdgpu_bo(bo)))
			return false;
>>>>>>> upstream/android-13
		return true;

	case TTM_PL_VRAM:
		/* Check each drm MM node individually */
<<<<<<< HEAD
		while (num_pages) {
			if (place->fpfn < (node->start + node->size) &&
			    !(place->lpfn && place->lpfn <= node->start))
				return true;

			num_pages -= node->size;
			++node;
=======
		amdgpu_res_first(bo->resource, 0, (u64)num_pages << PAGE_SHIFT,
				 &cursor);
		while (cursor.remaining) {
			if (place->fpfn < PFN_DOWN(cursor.start + cursor.size)
			    && !(place->lpfn &&
				 place->lpfn <= PFN_DOWN(cursor.start)))
				return true;

			amdgpu_res_next(&cursor, cursor.size);
>>>>>>> upstream/android-13
		}
		return false;

	default:
		break;
	}

	return ttm_bo_eviction_valuable(bo, place);
}

<<<<<<< HEAD
=======
static void amdgpu_ttm_vram_mm_access(struct amdgpu_device *adev, loff_t pos,
				      void *buf, size_t size, bool write)
{
	while (size) {
		uint64_t aligned_pos = ALIGN_DOWN(pos, 4);
		uint64_t bytes = 4 - (pos & 0x3);
		uint32_t shift = (pos & 0x3) * 8;
		uint32_t mask = 0xffffffff << shift;
		uint32_t value = 0;

		if (size < bytes) {
			mask &= 0xffffffff >> (bytes - size) * 8;
			bytes = size;
		}

		if (mask != 0xffffffff) {
			amdgpu_device_mm_access(adev, aligned_pos, &value, 4, false);
			if (write) {
				value &= ~mask;
				value |= (*(uint32_t *)buf << shift) & mask;
				amdgpu_device_mm_access(adev, aligned_pos, &value, 4, true);
			} else {
				value = (value & mask) >> shift;
				memcpy(buf, &value, bytes);
			}
		} else {
			amdgpu_device_mm_access(adev, aligned_pos, buf, 4, write);
		}

		pos += bytes;
		buf += bytes;
		size -= bytes;
	}
}

>>>>>>> upstream/android-13
/**
 * amdgpu_ttm_access_memory - Read or Write memory that backs a buffer object.
 *
 * @bo:  The buffer object to read/write
 * @offset:  Offset into buffer object
 * @buf:  Secondary buffer to write/read from
 * @len: Length in bytes of access
 * @write:  true if writing
 *
 * This is used to access VRAM that backs a buffer object via MMIO
 * access for debugging purposes.
 */
static int amdgpu_ttm_access_memory(struct ttm_buffer_object *bo,
<<<<<<< HEAD
				    unsigned long offset,
				    void *buf, int len, int write)
{
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(bo);
	struct amdgpu_device *adev = amdgpu_ttm_adev(abo->tbo.bdev);
	struct drm_mm_node *nodes;
	uint32_t value = 0;
	int ret = 0;
	uint64_t pos;
	unsigned long flags;

	if (bo->mem.mem_type != TTM_PL_VRAM)
		return -EIO;

	nodes = amdgpu_find_mm_node(&abo->tbo.mem, &offset);
	pos = (nodes->start << PAGE_SHIFT) + offset;

	while (len && pos < adev->gmc.mc_vram_size) {
		uint64_t aligned_pos = pos & ~(uint64_t)3;
		uint32_t bytes = 4 - (pos & 3);
		uint32_t shift = (pos & 3) * 8;
		uint32_t mask = 0xffffffff << shift;

		if (len < bytes) {
			mask &= 0xffffffff >> (bytes - len) * 8;
			bytes = len;
		}

		spin_lock_irqsave(&adev->mmio_idx_lock, flags);
		WREG32_NO_KIQ(mmMM_INDEX, ((uint32_t)aligned_pos) | 0x80000000);
		WREG32_NO_KIQ(mmMM_INDEX_HI, aligned_pos >> 31);
		if (!write || mask != 0xffffffff)
			value = RREG32_NO_KIQ(mmMM_DATA);
		if (write) {
			value &= ~mask;
			value |= (*(uint32_t *)buf << shift) & mask;
			WREG32_NO_KIQ(mmMM_DATA, value);
		}
		spin_unlock_irqrestore(&adev->mmio_idx_lock, flags);
		if (!write) {
			value = (value & mask) >> shift;
			memcpy(buf, &value, bytes);
		}

		ret += bytes;
		buf = (uint8_t *)buf + bytes;
		pos += bytes;
		len -= bytes;
		if (pos >= (nodes->start + nodes->size) << PAGE_SHIFT) {
			++nodes;
			pos = (nodes->start << PAGE_SHIFT);
		}
=======
				    unsigned long offset, void *buf, int len,
				    int write)
{
	struct amdgpu_bo *abo = ttm_to_amdgpu_bo(bo);
	struct amdgpu_device *adev = amdgpu_ttm_adev(abo->tbo.bdev);
	struct amdgpu_res_cursor cursor;
	int ret = 0;

	if (bo->resource->mem_type != TTM_PL_VRAM)
		return -EIO;

	amdgpu_res_first(bo->resource, offset, len, &cursor);
	while (cursor.remaining) {
		size_t count, size = cursor.size;
		loff_t pos = cursor.start;

		count = amdgpu_device_aper_access(adev, pos, buf, size, write);
		size -= count;
		if (size) {
			/* using MM to access rest vram and handle un-aligned address */
			pos += count;
			buf += count;
			amdgpu_ttm_vram_mm_access(adev, pos, buf, size, write);
		}

		ret += cursor.size;
		buf += cursor.size;
		amdgpu_res_next(&cursor, cursor.size);
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
static struct ttm_bo_driver amdgpu_bo_driver = {
	.ttm_tt_create = &amdgpu_ttm_tt_create,
	.ttm_tt_populate = &amdgpu_ttm_tt_populate,
	.ttm_tt_unpopulate = &amdgpu_ttm_tt_unpopulate,
	.invalidate_caches = &amdgpu_invalidate_caches,
	.init_mem_type = &amdgpu_init_mem_type,
	.eviction_valuable = amdgpu_ttm_bo_eviction_valuable,
	.evict_flags = &amdgpu_evict_flags,
	.move = &amdgpu_bo_move,
	.verify_access = &amdgpu_verify_access,
	.move_notify = &amdgpu_bo_move_notify,
	.fault_reserve_notify = &amdgpu_bo_fault_reserve_notify,
	.io_mem_reserve = &amdgpu_ttm_io_mem_reserve,
	.io_mem_free = &amdgpu_ttm_io_mem_free,
	.io_mem_pfn = amdgpu_ttm_io_mem_pfn,
	.access_memory = &amdgpu_ttm_access_memory
=======
static void
amdgpu_bo_delete_mem_notify(struct ttm_buffer_object *bo)
{
	amdgpu_bo_move_notify(bo, false, NULL);
}

static struct ttm_device_funcs amdgpu_bo_driver = {
	.ttm_tt_create = &amdgpu_ttm_tt_create,
	.ttm_tt_populate = &amdgpu_ttm_tt_populate,
	.ttm_tt_unpopulate = &amdgpu_ttm_tt_unpopulate,
	.ttm_tt_destroy = &amdgpu_ttm_backend_destroy,
	.eviction_valuable = amdgpu_ttm_bo_eviction_valuable,
	.evict_flags = &amdgpu_evict_flags,
	.move = &amdgpu_bo_move,
	.delete_mem_notify = &amdgpu_bo_delete_mem_notify,
	.release_notify = &amdgpu_bo_release_notify,
	.io_mem_reserve = &amdgpu_ttm_io_mem_reserve,
	.io_mem_pfn = amdgpu_ttm_io_mem_pfn,
	.access_memory = &amdgpu_ttm_access_memory,
	.del_from_lru_notify = &amdgpu_vm_del_from_lru_notify
>>>>>>> upstream/android-13
};

/*
 * Firmware Reservation functions
 */
/**
 * amdgpu_ttm_fw_reserve_vram_fini - free fw reserved vram
 *
 * @adev: amdgpu_device pointer
 *
 * free fw reserved vram if it has been reserved.
 */
static void amdgpu_ttm_fw_reserve_vram_fini(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	amdgpu_bo_free_kernel(&adev->fw_vram_usage.reserved_bo,
		NULL, &adev->fw_vram_usage.va);
=======
	amdgpu_bo_free_kernel(&adev->mman.fw_vram_usage_reserved_bo,
		NULL, &adev->mman.fw_vram_usage_va);
>>>>>>> upstream/android-13
}

/**
 * amdgpu_ttm_fw_reserve_vram_init - create bo vram reservation from fw
 *
 * @adev: amdgpu_device pointer
 *
 * create bo vram reservation from fw.
 */
static int amdgpu_ttm_fw_reserve_vram_init(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	struct ttm_operation_ctx ctx = { false, false };
	struct amdgpu_bo_param bp;
	int r = 0;
	int i;
	u64 vram_size = adev->gmc.visible_vram_size;
	u64 offset = adev->fw_vram_usage.start_offset;
	u64 size = adev->fw_vram_usage.size;
	struct amdgpu_bo *bo;

	memset(&bp, 0, sizeof(bp));
	bp.size = adev->fw_vram_usage.size;
	bp.byte_align = PAGE_SIZE;
	bp.domain = AMDGPU_GEM_DOMAIN_VRAM;
	bp.flags = AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED |
		AMDGPU_GEM_CREATE_VRAM_CONTIGUOUS;
	bp.type = ttm_bo_type_kernel;
	bp.resv = NULL;
	adev->fw_vram_usage.va = NULL;
	adev->fw_vram_usage.reserved_bo = NULL;

	if (adev->fw_vram_usage.size > 0 &&
		adev->fw_vram_usage.size <= vram_size) {

		r = amdgpu_bo_create(adev, &bp,
				     &adev->fw_vram_usage.reserved_bo);
		if (r)
			goto error_create;

		r = amdgpu_bo_reserve(adev->fw_vram_usage.reserved_bo, false);
		if (r)
			goto error_reserve;

		/* remove the original mem node and create a new one at the
		 * request position
		 */
		bo = adev->fw_vram_usage.reserved_bo;
		offset = ALIGN(offset, PAGE_SIZE);
		for (i = 0; i < bo->placement.num_placement; ++i) {
			bo->placements[i].fpfn = offset >> PAGE_SHIFT;
			bo->placements[i].lpfn = (offset + size) >> PAGE_SHIFT;
		}

		ttm_bo_mem_put(&bo->tbo, &bo->tbo.mem);
		r = ttm_bo_mem_space(&bo->tbo, &bo->placement,
				     &bo->tbo.mem, &ctx);
		if (r)
			goto error_pin;

		r = amdgpu_bo_pin_restricted(adev->fw_vram_usage.reserved_bo,
			AMDGPU_GEM_DOMAIN_VRAM,
			adev->fw_vram_usage.start_offset,
			(adev->fw_vram_usage.start_offset +
			adev->fw_vram_usage.size));
		if (r)
			goto error_pin;
		r = amdgpu_bo_kmap(adev->fw_vram_usage.reserved_bo,
			&adev->fw_vram_usage.va);
		if (r)
			goto error_kmap;

		amdgpu_bo_unreserve(adev->fw_vram_usage.reserved_bo);
	}
	return r;

error_kmap:
	amdgpu_bo_unpin(adev->fw_vram_usage.reserved_bo);
error_pin:
	amdgpu_bo_unreserve(adev->fw_vram_usage.reserved_bo);
error_reserve:
	amdgpu_bo_unref(&adev->fw_vram_usage.reserved_bo);
error_create:
	adev->fw_vram_usage.va = NULL;
	adev->fw_vram_usage.reserved_bo = NULL;
	return r;
}
/**
=======
	uint64_t vram_size = adev->gmc.visible_vram_size;

	adev->mman.fw_vram_usage_va = NULL;
	adev->mman.fw_vram_usage_reserved_bo = NULL;

	if (adev->mman.fw_vram_usage_size == 0 ||
	    adev->mman.fw_vram_usage_size > vram_size)
		return 0;

	return amdgpu_bo_create_kernel_at(adev,
					  adev->mman.fw_vram_usage_start_offset,
					  adev->mman.fw_vram_usage_size,
					  AMDGPU_GEM_DOMAIN_VRAM,
					  &adev->mman.fw_vram_usage_reserved_bo,
					  &adev->mman.fw_vram_usage_va);
}

/*
 * Memoy training reservation functions
 */

/**
 * amdgpu_ttm_training_reserve_vram_fini - free memory training reserved vram
 *
 * @adev: amdgpu_device pointer
 *
 * free memory training reserved vram if it has been reserved.
 */
static int amdgpu_ttm_training_reserve_vram_fini(struct amdgpu_device *adev)
{
	struct psp_memory_training_context *ctx = &adev->psp.mem_train_ctx;

	ctx->init = PSP_MEM_TRAIN_NOT_SUPPORT;
	amdgpu_bo_free_kernel(&ctx->c2p_bo, NULL, NULL);
	ctx->c2p_bo = NULL;

	return 0;
}

static void amdgpu_ttm_training_data_block_init(struct amdgpu_device *adev)
{
	struct psp_memory_training_context *ctx = &adev->psp.mem_train_ctx;

	memset(ctx, 0, sizeof(*ctx));

	ctx->c2p_train_data_offset =
		ALIGN((adev->gmc.mc_vram_size - adev->mman.discovery_tmr_size - SZ_1M), SZ_1M);
	ctx->p2c_train_data_offset =
		(adev->gmc.mc_vram_size - GDDR6_MEM_TRAINING_OFFSET);
	ctx->train_data_size =
		GDDR6_MEM_TRAINING_DATA_SIZE_IN_BYTES;

	DRM_DEBUG("train_data_size:%llx,p2c_train_data_offset:%llx,c2p_train_data_offset:%llx.\n",
			ctx->train_data_size,
			ctx->p2c_train_data_offset,
			ctx->c2p_train_data_offset);
}

/*
 * reserve TMR memory at the top of VRAM which holds
 * IP Discovery data and is protected by PSP.
 */
static int amdgpu_ttm_reserve_tmr(struct amdgpu_device *adev)
{
	int ret;
	struct psp_memory_training_context *ctx = &adev->psp.mem_train_ctx;
	bool mem_train_support = false;

	if (!amdgpu_sriov_vf(adev)) {
		if (amdgpu_atomfirmware_mem_training_supported(adev))
			mem_train_support = true;
		else
			DRM_DEBUG("memory training does not support!\n");
	}

	/*
	 * Query reserved tmr size through atom firmwareinfo for Sienna_Cichlid and onwards for all
	 * the use cases (IP discovery/G6 memory training/profiling/diagnostic data.etc)
	 *
	 * Otherwise, fallback to legacy approach to check and reserve tmr block for ip
	 * discovery data and G6 memory training data respectively
	 */
	adev->mman.discovery_tmr_size =
		amdgpu_atomfirmware_get_fw_reserved_fb_size(adev);
	if (!adev->mman.discovery_tmr_size)
		adev->mman.discovery_tmr_size = DISCOVERY_TMR_OFFSET;

	if (mem_train_support) {
		/* reserve vram for mem train according to TMR location */
		amdgpu_ttm_training_data_block_init(adev);
		ret = amdgpu_bo_create_kernel_at(adev,
					 ctx->c2p_train_data_offset,
					 ctx->train_data_size,
					 AMDGPU_GEM_DOMAIN_VRAM,
					 &ctx->c2p_bo,
					 NULL);
		if (ret) {
			DRM_ERROR("alloc c2p_bo failed(%d)!\n", ret);
			amdgpu_ttm_training_reserve_vram_fini(adev);
			return ret;
		}
		ctx->init = PSP_MEM_TRAIN_RESERVE_SUCCESS;
	}

	ret = amdgpu_bo_create_kernel_at(adev,
				adev->gmc.real_vram_size - adev->mman.discovery_tmr_size,
				adev->mman.discovery_tmr_size,
				AMDGPU_GEM_DOMAIN_VRAM,
				&adev->mman.discovery_memory,
				NULL);
	if (ret) {
		DRM_ERROR("alloc tmr failed(%d)!\n", ret);
		amdgpu_bo_free_kernel(&adev->mman.discovery_memory, NULL, NULL);
		return ret;
	}

	return 0;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_init - Init the memory management (ttm) as well as various
 * gtt/vram related fields.
 *
 * This initializes all of the memory space pools that the TTM layer
 * will need such as the GTT space (system memory mapped to the device),
 * VRAM (on-board memory), and on-chip memories (GDS, GWS, OA) which
 * can be mapped per VMID.
 */
int amdgpu_ttm_init(struct amdgpu_device *adev)
{
	uint64_t gtt_size;
	int r;
	u64 vis_vram_limit;

<<<<<<< HEAD
	/* initialize global references for vram/gtt */
	r = amdgpu_ttm_global_init(adev);
	if (r) {
		return r;
	}
	/* No others user of address space so set it to 0 */
	r = ttm_bo_device_init(&adev->mman.bdev,
			       adev->mman.bo_global_ref.ref.object,
			       &amdgpu_bo_driver,
			       adev->ddev->anon_inode->i_mapping,
			       DRM_FILE_PAGE_OFFSET,
			       adev->need_dma32);
=======
	mutex_init(&adev->mman.gtt_window_lock);

	/* No others user of address space so set it to 0 */
	r = ttm_device_init(&adev->mman.bdev, &amdgpu_bo_driver, adev->dev,
			       adev_to_drm(adev)->anon_inode->i_mapping,
			       adev_to_drm(adev)->vma_offset_manager,
			       adev->need_swiotlb,
			       dma_addressing_limited(adev->dev));
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("failed initializing buffer object driver(%d).\n", r);
		return r;
	}
	adev->mman.initialized = true;

<<<<<<< HEAD
	/* We opt to avoid OOM on system pages allocations */
	adev->mman.bdev.no_retry = true;

	/* Initialize VRAM pool with all of VRAM divided into pages */
	r = ttm_bo_init_mm(&adev->mman.bdev, TTM_PL_VRAM,
				adev->gmc.real_vram_size >> PAGE_SHIFT);
=======
	/* Initialize VRAM pool with all of VRAM divided into pages */
	r = amdgpu_vram_mgr_init(adev);
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("Failed initializing VRAM heap.\n");
		return r;
	}

	/* Reduce size of CPU-visible VRAM if requested */
	vis_vram_limit = (u64)amdgpu_vis_vram_limit * 1024 * 1024;
	if (amdgpu_vis_vram_limit > 0 &&
	    vis_vram_limit <= adev->gmc.visible_vram_size)
		adev->gmc.visible_vram_size = vis_vram_limit;

	/* Change the size here instead of the init above so only lpfn is affected */
	amdgpu_ttm_set_buffer_funcs_status(adev, false);
#ifdef CONFIG_64BIT
<<<<<<< HEAD
	adev->mman.aper_base_kaddr = ioremap_wc(adev->gmc.aper_base,
						adev->gmc.visible_vram_size);
=======
#ifdef CONFIG_X86
	if (adev->gmc.xgmi.connected_to_cpu)
		adev->mman.aper_base_kaddr = ioremap_cache(adev->gmc.aper_base,
				adev->gmc.visible_vram_size);

	else
#endif
		adev->mman.aper_base_kaddr = ioremap_wc(adev->gmc.aper_base,
				adev->gmc.visible_vram_size);
>>>>>>> upstream/android-13
#endif

	/*
	 *The reserved vram for firmware must be pinned to the specified
	 *place on the VRAM, so reserve it early.
	 */
	r = amdgpu_ttm_fw_reserve_vram_init(adev);
	if (r) {
		return r;
	}

<<<<<<< HEAD
=======
	/*
	 * only NAVI10 and onwards ASIC support for IP discovery.
	 * If IP discovery enabled, a block of memory should be
	 * reserved for IP discovey.
	 */
	if (adev->mman.discovery_bin) {
		r = amdgpu_ttm_reserve_tmr(adev);
		if (r)
			return r;
	}

>>>>>>> upstream/android-13
	/* allocate memory as required for VGA
	 * This is used for VGA emulation and pre-OS scanout buffers to
	 * avoid display artifacts while transitioning between pre-OS
	 * and driver.  */
<<<<<<< HEAD
	if (adev->gmc.stolen_size) {
		r = amdgpu_bo_create_kernel(adev, adev->gmc.stolen_size, PAGE_SIZE,
					    AMDGPU_GEM_DOMAIN_VRAM,
					    &adev->stolen_vga_memory,
					    NULL, NULL);
		if (r)
			return r;
	}
=======
	r = amdgpu_bo_create_kernel_at(adev, 0, adev->mman.stolen_vga_size,
				       AMDGPU_GEM_DOMAIN_VRAM,
				       &adev->mman.stolen_vga_memory,
				       NULL);
	if (r)
		return r;
	r = amdgpu_bo_create_kernel_at(adev, adev->mman.stolen_vga_size,
				       adev->mman.stolen_extended_size,
				       AMDGPU_GEM_DOMAIN_VRAM,
				       &adev->mman.stolen_extended_memory,
				       NULL);
	if (r)
		return r;
	r = amdgpu_bo_create_kernel_at(adev, adev->mman.stolen_reserved_offset,
				       adev->mman.stolen_reserved_size,
				       AMDGPU_GEM_DOMAIN_VRAM,
				       &adev->mman.stolen_reserved_memory,
				       NULL);
	if (r)
		return r;

>>>>>>> upstream/android-13
	DRM_INFO("amdgpu: %uM of VRAM memory ready\n",
		 (unsigned) (adev->gmc.real_vram_size / (1024 * 1024)));

	/* Compute GTT size, either bsaed on 3/4th the size of RAM size
	 * or whatever the user passed on module init */
	if (amdgpu_gtt_size == -1) {
		struct sysinfo si;

		si_meminfo(&si);
		gtt_size = min(max((AMDGPU_DEFAULT_GTT_SIZE_MB << 20),
			       adev->gmc.mc_vram_size),
			       ((uint64_t)si.totalram * si.mem_unit * 3/4));
	}
	else
		gtt_size = (uint64_t)amdgpu_gtt_size << 20;

	/* Initialize GTT memory pool */
<<<<<<< HEAD
	r = ttm_bo_init_mm(&adev->mman.bdev, TTM_PL_TT, gtt_size >> PAGE_SHIFT);
=======
	r = amdgpu_gtt_mgr_init(adev, gtt_size);
>>>>>>> upstream/android-13
	if (r) {
		DRM_ERROR("Failed initializing GTT heap.\n");
		return r;
	}
	DRM_INFO("amdgpu: %uM of GTT memory ready.\n",
		 (unsigned)(gtt_size / (1024 * 1024)));

<<<<<<< HEAD
	/* Initialize various on-chip memory pools */
	adev->gds.mem.total_size = adev->gds.mem.total_size << AMDGPU_GDS_SHIFT;
	adev->gds.mem.gfx_partition_size = adev->gds.mem.gfx_partition_size << AMDGPU_GDS_SHIFT;
	adev->gds.mem.cs_partition_size = adev->gds.mem.cs_partition_size << AMDGPU_GDS_SHIFT;
	adev->gds.gws.total_size = adev->gds.gws.total_size << AMDGPU_GWS_SHIFT;
	adev->gds.gws.gfx_partition_size = adev->gds.gws.gfx_partition_size << AMDGPU_GWS_SHIFT;
	adev->gds.gws.cs_partition_size = adev->gds.gws.cs_partition_size << AMDGPU_GWS_SHIFT;
	adev->gds.oa.total_size = adev->gds.oa.total_size << AMDGPU_OA_SHIFT;
	adev->gds.oa.gfx_partition_size = adev->gds.oa.gfx_partition_size << AMDGPU_OA_SHIFT;
	adev->gds.oa.cs_partition_size = adev->gds.oa.cs_partition_size << AMDGPU_OA_SHIFT;
	/* GDS Memory */
	if (adev->gds.mem.total_size) {
		r = ttm_bo_init_mm(&adev->mman.bdev, AMDGPU_PL_GDS,
				   adev->gds.mem.total_size >> PAGE_SHIFT);
		if (r) {
			DRM_ERROR("Failed initializing GDS heap.\n");
			return r;
		}
	}

	/* GWS */
	if (adev->gds.gws.total_size) {
		r = ttm_bo_init_mm(&adev->mman.bdev, AMDGPU_PL_GWS,
				   adev->gds.gws.total_size >> PAGE_SHIFT);
		if (r) {
			DRM_ERROR("Failed initializing gws heap.\n");
			return r;
		}
	}

	/* OA */
	if (adev->gds.oa.total_size) {
		r = ttm_bo_init_mm(&adev->mman.bdev, AMDGPU_PL_OA,
				   adev->gds.oa.total_size >> PAGE_SHIFT);
		if (r) {
			DRM_ERROR("Failed initializing oa heap.\n");
			return r;
		}
	}

	/* Register debugfs entries for amdgpu_ttm */
	r = amdgpu_ttm_debugfs_init(adev);
	if (r) {
		DRM_ERROR("Failed to init debugfs\n");
		return r;
	}
	return 0;
}

/**
 * amdgpu_ttm_late_init - Handle any late initialization for amdgpu_ttm
 */
void amdgpu_ttm_late_init(struct amdgpu_device *adev)
{
	/* return the VGA stolen memory (if any) back to VRAM */
	amdgpu_bo_free_kernel(&adev->stolen_vga_memory, NULL, NULL);
}

/**
=======
	/* Initialize preemptible memory pool */
	r = amdgpu_preempt_mgr_init(adev);
	if (r) {
		DRM_ERROR("Failed initializing PREEMPT heap.\n");
		return r;
	}

	/* Initialize various on-chip memory pools */
	r = amdgpu_ttm_init_on_chip(adev, AMDGPU_PL_GDS, adev->gds.gds_size);
	if (r) {
		DRM_ERROR("Failed initializing GDS heap.\n");
		return r;
	}

	r = amdgpu_ttm_init_on_chip(adev, AMDGPU_PL_GWS, adev->gds.gws_size);
	if (r) {
		DRM_ERROR("Failed initializing gws heap.\n");
		return r;
	}

	r = amdgpu_ttm_init_on_chip(adev, AMDGPU_PL_OA, adev->gds.oa_size);
	if (r) {
		DRM_ERROR("Failed initializing oa heap.\n");
		return r;
	}

	return 0;
}

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_fini - De-initialize the TTM memory pools
 */
void amdgpu_ttm_fini(struct amdgpu_device *adev)
{
	if (!adev->mman.initialized)
		return;

<<<<<<< HEAD
	amdgpu_ttm_debugfs_fini(adev);
	amdgpu_ttm_fw_reserve_vram_fini(adev);
	if (adev->mman.aper_base_kaddr)
		iounmap(adev->mman.aper_base_kaddr);
	adev->mman.aper_base_kaddr = NULL;

	ttm_bo_clean_mm(&adev->mman.bdev, TTM_PL_VRAM);
	ttm_bo_clean_mm(&adev->mman.bdev, TTM_PL_TT);
	if (adev->gds.mem.total_size)
		ttm_bo_clean_mm(&adev->mman.bdev, AMDGPU_PL_GDS);
	if (adev->gds.gws.total_size)
		ttm_bo_clean_mm(&adev->mman.bdev, AMDGPU_PL_GWS);
	if (adev->gds.oa.total_size)
		ttm_bo_clean_mm(&adev->mman.bdev, AMDGPU_PL_OA);
	ttm_bo_device_release(&adev->mman.bdev);
	amdgpu_ttm_global_fini(adev);
=======
	amdgpu_ttm_training_reserve_vram_fini(adev);
	/* return the stolen vga memory back to VRAM */
	amdgpu_bo_free_kernel(&adev->mman.stolen_vga_memory, NULL, NULL);
	amdgpu_bo_free_kernel(&adev->mman.stolen_extended_memory, NULL, NULL);
	/* return the IP Discovery TMR memory back to VRAM */
	amdgpu_bo_free_kernel(&adev->mman.discovery_memory, NULL, NULL);
	if (adev->mman.stolen_reserved_size)
		amdgpu_bo_free_kernel(&adev->mman.stolen_reserved_memory,
				      NULL, NULL);
	amdgpu_ttm_fw_reserve_vram_fini(adev);

	amdgpu_vram_mgr_fini(adev);
	amdgpu_gtt_mgr_fini(adev);
	amdgpu_preempt_mgr_fini(adev);
	ttm_range_man_fini(&adev->mman.bdev, AMDGPU_PL_GDS);
	ttm_range_man_fini(&adev->mman.bdev, AMDGPU_PL_GWS);
	ttm_range_man_fini(&adev->mman.bdev, AMDGPU_PL_OA);
	ttm_device_fini(&adev->mman.bdev);
>>>>>>> upstream/android-13
	adev->mman.initialized = false;
	DRM_INFO("amdgpu: ttm finalized\n");
}

/**
 * amdgpu_ttm_set_buffer_funcs_status - enable/disable use of buffer functions
 *
 * @adev: amdgpu_device pointer
 * @enable: true when we can use buffer functions.
 *
 * Enable/disable use of buffer functions during suspend/resume. This should
 * only be called at bootup or when userspace isn't running.
 */
void amdgpu_ttm_set_buffer_funcs_status(struct amdgpu_device *adev, bool enable)
{
<<<<<<< HEAD
	struct ttm_mem_type_manager *man = &adev->mman.bdev.man[TTM_PL_VRAM];
	uint64_t size;
	int r;

	if (!adev->mman.initialized || adev->in_gpu_reset ||
=======
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev, TTM_PL_VRAM);
	uint64_t size;
	int r;

	if (!adev->mman.initialized || amdgpu_in_reset(adev) ||
>>>>>>> upstream/android-13
	    adev->mman.buffer_funcs_enabled == enable)
		return;

	if (enable) {
		struct amdgpu_ring *ring;
<<<<<<< HEAD
		struct drm_sched_rq *rq;

		ring = adev->mman.buffer_funcs_ring;
		rq = &ring->sched.sched_rq[DRM_SCHED_PRIORITY_KERNEL];
		r = drm_sched_entity_init(&adev->mman.entity, &rq, 1, NULL);
=======
		struct drm_gpu_scheduler *sched;

		ring = adev->mman.buffer_funcs_ring;
		sched = &ring->sched;
		r = drm_sched_entity_init(&adev->mman.entity,
					  DRM_SCHED_PRIORITY_KERNEL, &sched,
					  1, NULL);
>>>>>>> upstream/android-13
		if (r) {
			DRM_ERROR("Failed setting up TTM BO move entity (%d)\n",
				  r);
			return;
		}
	} else {
		drm_sched_entity_destroy(&adev->mman.entity);
		dma_fence_put(man->move);
		man->move = NULL;
	}

	/* this just adjusts TTM size idea, which sets lpfn to the correct value */
	if (enable)
		size = adev->gmc.real_vram_size;
	else
		size = adev->gmc.visible_vram_size;
	man->size = size >> PAGE_SHIFT;
	adev->mman.buffer_funcs_enabled = enable;
}

<<<<<<< HEAD
int amdgpu_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct drm_file *file_priv;
	struct amdgpu_device *adev;

	if (unlikely(vma->vm_pgoff < DRM_FILE_PAGE_OFFSET))
		return -EINVAL;

	file_priv = filp->private_data;
	adev = file_priv->minor->dev->dev_private;
	if (adev == NULL)
		return -EINVAL;

	return ttm_bo_mmap(filp, vma, &adev->mman.bdev);
}

static int amdgpu_map_buffer(struct ttm_buffer_object *bo,
			     struct ttm_mem_reg *mem, unsigned num_pages,
			     uint64_t offset, unsigned window,
			     struct amdgpu_ring *ring,
			     uint64_t *addr)
{
	struct amdgpu_ttm_tt *gtt = (void *)bo->ttm;
	struct amdgpu_device *adev = ring->adev;
	struct ttm_tt *ttm = bo->ttm;
	struct amdgpu_job *job;
	unsigned num_dw, num_bytes;
	dma_addr_t *dma_address;
	struct dma_fence *fence;
	uint64_t src_addr, dst_addr;
	uint64_t flags;
	int r;

	BUG_ON(adev->mman.buffer_funcs->copy_max_bytes <
	       AMDGPU_GTT_MAX_TRANSFER_SIZE * 8);

	*addr = adev->gmc.gart_start;
	*addr += (u64)window * AMDGPU_GTT_MAX_TRANSFER_SIZE *
		AMDGPU_GPU_PAGE_SIZE;

	num_dw = adev->mman.buffer_funcs->copy_num_dw;
	while (num_dw & 0x7)
		num_dw++;

	num_bytes = num_pages * 8;

	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4 + num_bytes, &job);
	if (r)
		return r;

	src_addr = num_dw * 4;
	src_addr += job->ibs[0].gpu_addr;

	dst_addr = adev->gart.table_addr;
	dst_addr += window * AMDGPU_GTT_MAX_TRANSFER_SIZE * 8;
	amdgpu_emit_copy_buffer(adev, &job->ibs[0], src_addr,
				dst_addr, num_bytes);

	amdgpu_ring_pad_ib(ring, &job->ibs[0]);
	WARN_ON(job->ibs[0].length_dw > num_dw);

	dma_address = &gtt->ttm.dma_address[offset >> PAGE_SHIFT];
	flags = amdgpu_ttm_tt_pte_flags(adev, ttm, mem);
	r = amdgpu_gart_map(adev, 0, num_pages, dma_address, flags,
			    &job->ibs[0].ptr[num_dw]);
	if (r)
		goto error_free;

	r = amdgpu_job_submit(job, &adev->mman.entity,
			      AMDGPU_FENCE_OWNER_UNDEFINED, &fence);
	if (r)
		goto error_free;

	dma_fence_put(fence);

	return r;

error_free:
	amdgpu_job_free(job);
	return r;
}

int amdgpu_copy_buffer(struct amdgpu_ring *ring, uint64_t src_offset,
		       uint64_t dst_offset, uint32_t byte_count,
		       struct reservation_object *resv,
		       struct dma_fence **fence, bool direct_submit,
		       bool vm_needs_flush)
{
=======
int amdgpu_copy_buffer(struct amdgpu_ring *ring, uint64_t src_offset,
		       uint64_t dst_offset, uint32_t byte_count,
		       struct dma_resv *resv,
		       struct dma_fence **fence, bool direct_submit,
		       bool vm_needs_flush, bool tmz)
{
	enum amdgpu_ib_pool_type pool = direct_submit ? AMDGPU_IB_POOL_DIRECT :
		AMDGPU_IB_POOL_DELAYED;
>>>>>>> upstream/android-13
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_job *job;

	uint32_t max_bytes;
	unsigned num_loops, num_dw;
	unsigned i;
	int r;

<<<<<<< HEAD
	if (direct_submit && !ring->ready) {
=======
	if (!direct_submit && !ring->sched.ready) {
>>>>>>> upstream/android-13
		DRM_ERROR("Trying to move memory with ring turned off.\n");
		return -EINVAL;
	}

	max_bytes = adev->mman.buffer_funcs->copy_max_bytes;
	num_loops = DIV_ROUND_UP(byte_count, max_bytes);
<<<<<<< HEAD
	num_dw = num_loops * adev->mman.buffer_funcs->copy_num_dw;

	/* for IB padding */
	while (num_dw & 0x7)
		num_dw++;

	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4, &job);
	if (r)
		return r;

	job->vm_needs_flush = vm_needs_flush;
	if (resv) {
		r = amdgpu_sync_resv(adev, &job->sync, resv,
				     AMDGPU_FENCE_OWNER_UNDEFINED,
				     false);
=======
	num_dw = ALIGN(num_loops * adev->mman.buffer_funcs->copy_num_dw, 8);

	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4, pool, &job);
	if (r)
		return r;

	if (vm_needs_flush) {
		job->vm_pd_addr = amdgpu_gmc_pd_addr(adev->gmc.pdb0_bo ?
					adev->gmc.pdb0_bo : adev->gart.bo);
		job->vm_needs_flush = true;
	}
	if (resv) {
		r = amdgpu_sync_resv(adev, &job->sync, resv,
				     AMDGPU_SYNC_ALWAYS,
				     AMDGPU_FENCE_OWNER_UNDEFINED);
>>>>>>> upstream/android-13
		if (r) {
			DRM_ERROR("sync failed (%d).\n", r);
			goto error_free;
		}
	}

	for (i = 0; i < num_loops; i++) {
		uint32_t cur_size_in_bytes = min(byte_count, max_bytes);

		amdgpu_emit_copy_buffer(adev, &job->ibs[0], src_offset,
<<<<<<< HEAD
					dst_offset, cur_size_in_bytes);
=======
					dst_offset, cur_size_in_bytes, tmz);
>>>>>>> upstream/android-13

		src_offset += cur_size_in_bytes;
		dst_offset += cur_size_in_bytes;
		byte_count -= cur_size_in_bytes;
	}

	amdgpu_ring_pad_ib(ring, &job->ibs[0]);
	WARN_ON(job->ibs[0].length_dw > num_dw);
	if (direct_submit)
		r = amdgpu_job_submit_direct(job, ring, fence);
	else
		r = amdgpu_job_submit(job, &adev->mman.entity,
				      AMDGPU_FENCE_OWNER_UNDEFINED, fence);
	if (r)
		goto error_free;

	return r;

error_free:
	amdgpu_job_free(job);
	DRM_ERROR("Error scheduling IBs (%d)\n", r);
	return r;
}

int amdgpu_fill_buffer(struct amdgpu_bo *bo,
		       uint32_t src_data,
<<<<<<< HEAD
		       struct reservation_object *resv,
=======
		       struct dma_resv *resv,
>>>>>>> upstream/android-13
		       struct dma_fence **fence)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->tbo.bdev);
	uint32_t max_bytes = adev->mman.buffer_funcs->fill_max_bytes;
	struct amdgpu_ring *ring = adev->mman.buffer_funcs_ring;

<<<<<<< HEAD
	struct drm_mm_node *mm_node;
	unsigned long num_pages;
	unsigned int num_loops, num_dw;
=======
	struct amdgpu_res_cursor cursor;
	unsigned int num_loops, num_dw;
	uint64_t num_bytes;
>>>>>>> upstream/android-13

	struct amdgpu_job *job;
	int r;

	if (!adev->mman.buffer_funcs_enabled) {
		DRM_ERROR("Trying to clear memory with ring turned off.\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (bo->tbo.mem.mem_type == TTM_PL_TT) {
=======
	if (bo->tbo.resource->mem_type == AMDGPU_PL_PREEMPT) {
		DRM_ERROR("Trying to clear preemptible memory.\n");
		return -EINVAL;
	}

	if (bo->tbo.resource->mem_type == TTM_PL_TT) {
>>>>>>> upstream/android-13
		r = amdgpu_ttm_alloc_gart(&bo->tbo);
		if (r)
			return r;
	}

<<<<<<< HEAD
	num_pages = bo->tbo.num_pages;
	mm_node = bo->tbo.mem.mm_node;
	num_loops = 0;
	while (num_pages) {
		uint32_t byte_count = mm_node->size << PAGE_SHIFT;

		num_loops += DIV_ROUND_UP(byte_count, max_bytes);
		num_pages -= mm_node->size;
		++mm_node;
=======
	num_bytes = bo->tbo.resource->num_pages << PAGE_SHIFT;
	num_loops = 0;

	amdgpu_res_first(bo->tbo.resource, 0, num_bytes, &cursor);
	while (cursor.remaining) {
		num_loops += DIV_ROUND_UP_ULL(cursor.size, max_bytes);
		amdgpu_res_next(&cursor, cursor.size);
>>>>>>> upstream/android-13
	}
	num_dw = num_loops * adev->mman.buffer_funcs->fill_num_dw;

	/* for IB padding */
	num_dw += 64;

<<<<<<< HEAD
	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4, &job);
=======
	r = amdgpu_job_alloc_with_ib(adev, num_dw * 4, AMDGPU_IB_POOL_DELAYED,
				     &job);
>>>>>>> upstream/android-13
	if (r)
		return r;

	if (resv) {
		r = amdgpu_sync_resv(adev, &job->sync, resv,
<<<<<<< HEAD
				     AMDGPU_FENCE_OWNER_UNDEFINED, false);
=======
				     AMDGPU_SYNC_ALWAYS,
				     AMDGPU_FENCE_OWNER_UNDEFINED);
>>>>>>> upstream/android-13
		if (r) {
			DRM_ERROR("sync failed (%d).\n", r);
			goto error_free;
		}
	}

<<<<<<< HEAD
	num_pages = bo->tbo.num_pages;
	mm_node = bo->tbo.mem.mm_node;

	while (num_pages) {
		uint32_t byte_count = mm_node->size << PAGE_SHIFT;
		uint64_t dst_addr;

		dst_addr = amdgpu_mm_node_addr(&bo->tbo, mm_node, &bo->tbo.mem);
		while (byte_count) {
			uint32_t cur_size_in_bytes = min(byte_count, max_bytes);

			amdgpu_emit_fill_buffer(adev, &job->ibs[0], src_data,
						dst_addr, cur_size_in_bytes);

			dst_addr += cur_size_in_bytes;
			byte_count -= cur_size_in_bytes;
		}

		num_pages -= mm_node->size;
		++mm_node;
=======
	amdgpu_res_first(bo->tbo.resource, 0, num_bytes, &cursor);
	while (cursor.remaining) {
		uint32_t cur_size = min_t(uint64_t, cursor.size, max_bytes);
		uint64_t dst_addr = cursor.start;

		dst_addr += amdgpu_ttm_domain_start(adev,
						    bo->tbo.resource->mem_type);
		amdgpu_emit_fill_buffer(adev, &job->ibs[0], src_data, dst_addr,
					cur_size);

		amdgpu_res_next(&cursor, cur_size);
>>>>>>> upstream/android-13
	}

	amdgpu_ring_pad_ib(ring, &job->ibs[0]);
	WARN_ON(job->ibs[0].length_dw > num_dw);
	r = amdgpu_job_submit(job, &adev->mman.entity,
			      AMDGPU_FENCE_OWNER_UNDEFINED, fence);
	if (r)
		goto error_free;

	return 0;

error_free:
	amdgpu_job_free(job);
	return r;
}

<<<<<<< HEAD
#if defined(CONFIG_DEBUG_FS)

static int amdgpu_mm_dump_table(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *)m->private;
	unsigned ttm_pl = *(int *)node->info_ent->data;
	struct drm_device *dev = node->minor->dev;
	struct amdgpu_device *adev = dev->dev_private;
	struct ttm_mem_type_manager *man = &adev->mman.bdev.man[ttm_pl];
=======
/**
 * amdgpu_ttm_evict_resources - evict memory buffers
 * @adev: amdgpu device object
 * @mem_type: evicted BO's memory type
 *
 * Evicts all @mem_type buffers on the lru list of the memory type.
 *
 * Returns:
 * 0 for success or a negative error code on failure.
 */
int amdgpu_ttm_evict_resources(struct amdgpu_device *adev, int mem_type)
{
	struct ttm_resource_manager *man;

	switch (mem_type) {
	case TTM_PL_VRAM:
	case TTM_PL_TT:
	case AMDGPU_PL_GWS:
	case AMDGPU_PL_GDS:
	case AMDGPU_PL_OA:
		man = ttm_manager_type(&adev->mman.bdev, mem_type);
		break;
	default:
		DRM_ERROR("Trying to evict invalid memory type\n");
		return -EINVAL;
	}

	return ttm_resource_manager_evict_all(&adev->mman.bdev, man);
}

#if defined(CONFIG_DEBUG_FS)

static int amdgpu_mm_vram_table_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev,
							    TTM_PL_VRAM);
>>>>>>> upstream/android-13
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

<<<<<<< HEAD
static int ttm_pl_vram = TTM_PL_VRAM;
static int ttm_pl_tt = TTM_PL_TT;

static const struct drm_info_list amdgpu_ttm_debugfs_list[] = {
	{"amdgpu_vram_mm", amdgpu_mm_dump_table, 0, &ttm_pl_vram},
	{"amdgpu_gtt_mm", amdgpu_mm_dump_table, 0, &ttm_pl_tt},
	{"ttm_page_pool", ttm_page_alloc_debugfs, 0, NULL},
#ifdef CONFIG_SWIOTLB
	{"ttm_dma_page_pool", ttm_dma_page_alloc_debugfs, 0, NULL}
#endif
};

/**
=======
static int amdgpu_ttm_page_pool_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;

	return ttm_pool_debugfs(&adev->mman.bdev.pool, m);
}

static int amdgpu_mm_tt_table_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev,
							    TTM_PL_TT);
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

static int amdgpu_mm_gds_table_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev,
							    AMDGPU_PL_GDS);
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

static int amdgpu_mm_gws_table_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev,
							    AMDGPU_PL_GWS);
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

static int amdgpu_mm_oa_table_show(struct seq_file *m, void *unused)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)m->private;
	struct ttm_resource_manager *man = ttm_manager_type(&adev->mman.bdev,
							    AMDGPU_PL_OA);
	struct drm_printer p = drm_seq_file_printer(m);

	man->func->debug(man, &p);
	return 0;
}

DEFINE_SHOW_ATTRIBUTE(amdgpu_mm_vram_table);
DEFINE_SHOW_ATTRIBUTE(amdgpu_mm_tt_table);
DEFINE_SHOW_ATTRIBUTE(amdgpu_mm_gds_table);
DEFINE_SHOW_ATTRIBUTE(amdgpu_mm_gws_table);
DEFINE_SHOW_ATTRIBUTE(amdgpu_mm_oa_table);
DEFINE_SHOW_ATTRIBUTE(amdgpu_ttm_page_pool);

/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_vram_read - Linear read access to VRAM
 *
 * Accesses VRAM via MMIO for debugging purposes.
 */
static ssize_t amdgpu_ttm_vram_read(struct file *f, char __user *buf,
				    size_t size, loff_t *pos)
{
	struct amdgpu_device *adev = file_inode(f)->i_private;
	ssize_t result = 0;
<<<<<<< HEAD
	int r;
=======
>>>>>>> upstream/android-13

	if (size & 0x3 || *pos & 0x3)
		return -EINVAL;

	if (*pos >= adev->gmc.mc_vram_size)
		return -ENXIO;

<<<<<<< HEAD
	while (size) {
		unsigned long flags;
		uint32_t value;

		if (*pos >= adev->gmc.mc_vram_size)
			return result;

		spin_lock_irqsave(&adev->mmio_idx_lock, flags);
		WREG32_NO_KIQ(mmMM_INDEX, ((uint32_t)*pos) | 0x80000000);
		WREG32_NO_KIQ(mmMM_INDEX_HI, *pos >> 31);
		value = RREG32_NO_KIQ(mmMM_DATA);
		spin_unlock_irqrestore(&adev->mmio_idx_lock, flags);

		r = put_user(value, (uint32_t *)buf);
		if (r)
			return r;

		result += 4;
		buf += 4;
		*pos += 4;
		size -= 4;
=======
	size = min(size, (size_t)(adev->gmc.mc_vram_size - *pos));
	while (size) {
		size_t bytes = min(size, AMDGPU_TTM_VRAM_MAX_DW_READ * 4);
		uint32_t value[AMDGPU_TTM_VRAM_MAX_DW_READ];

		amdgpu_device_vram_access(adev, *pos, value, bytes, false);
		if (copy_to_user(buf, value, bytes))
			return -EFAULT;

		result += bytes;
		buf += bytes;
		*pos += bytes;
		size -= bytes;
>>>>>>> upstream/android-13
	}

	return result;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_ttm_vram_write - Linear write access to VRAM
 *
 * Accesses VRAM via MMIO for debugging purposes.
 */
static ssize_t amdgpu_ttm_vram_write(struct file *f, const char __user *buf,
				    size_t size, loff_t *pos)
{
	struct amdgpu_device *adev = file_inode(f)->i_private;
	ssize_t result = 0;
	int r;

	if (size & 0x3 || *pos & 0x3)
		return -EINVAL;

	if (*pos >= adev->gmc.mc_vram_size)
		return -ENXIO;

	while (size) {
<<<<<<< HEAD
		unsigned long flags;
=======
>>>>>>> upstream/android-13
		uint32_t value;

		if (*pos >= adev->gmc.mc_vram_size)
			return result;

		r = get_user(value, (uint32_t *)buf);
		if (r)
			return r;

<<<<<<< HEAD
		spin_lock_irqsave(&adev->mmio_idx_lock, flags);
		WREG32_NO_KIQ(mmMM_INDEX, ((uint32_t)*pos) | 0x80000000);
		WREG32_NO_KIQ(mmMM_INDEX_HI, *pos >> 31);
		WREG32_NO_KIQ(mmMM_DATA, value);
		spin_unlock_irqrestore(&adev->mmio_idx_lock, flags);
=======
		amdgpu_device_mm_access(adev, *pos, &value, 4, true);
>>>>>>> upstream/android-13

		result += 4;
		buf += 4;
		*pos += 4;
		size -= 4;
	}

	return result;
}

static const struct file_operations amdgpu_ttm_vram_fops = {
	.owner = THIS_MODULE,
	.read = amdgpu_ttm_vram_read,
	.write = amdgpu_ttm_vram_write,
	.llseek = default_llseek,
};

<<<<<<< HEAD
#ifdef CONFIG_DRM_AMDGPU_GART_DEBUGFS

/**
 * amdgpu_ttm_gtt_read - Linear read access to GTT memory
 */
static ssize_t amdgpu_ttm_gtt_read(struct file *f, char __user *buf,
				   size_t size, loff_t *pos)
{
	struct amdgpu_device *adev = file_inode(f)->i_private;
	ssize_t result = 0;
	int r;

	while (size) {
		loff_t p = *pos / PAGE_SIZE;
		unsigned off = *pos & ~PAGE_MASK;
		size_t cur_size = min_t(size_t, size, PAGE_SIZE - off);
		struct page *page;
		void *ptr;

		if (p >= adev->gart.num_cpu_pages)
			return result;

		page = adev->gart.pages[p];
		if (page) {
			ptr = kmap(page);
			ptr += off;

			r = copy_to_user(buf, ptr, cur_size);
			kunmap(adev->gart.pages[p]);
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

static const struct file_operations amdgpu_ttm_gtt_fops = {
	.owner = THIS_MODULE,
	.read = amdgpu_ttm_gtt_read,
	.llseek = default_llseek
};

#endif

/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_iomem_read - Virtual read access to GPU mapped memory
 *
 * This function is used to read memory that has been mapped to the
 * GPU and the known addresses are not physical addresses but instead
 * bus addresses (e.g., what you'd put in an IB or ring buffer).
 */
static ssize_t amdgpu_iomem_read(struct file *f, char __user *buf,
				 size_t size, loff_t *pos)
{
	struct amdgpu_device *adev = file_inode(f)->i_private;
	struct iommu_domain *dom;
	ssize_t result = 0;
	int r;

	/* retrieve the IOMMU domain if any for this device */
	dom = iommu_get_domain_for_dev(adev->dev);

	while (size) {
		phys_addr_t addr = *pos & PAGE_MASK;
		loff_t off = *pos & ~PAGE_MASK;
		size_t bytes = PAGE_SIZE - off;
		unsigned long pfn;
		struct page *p;
		void *ptr;

		bytes = bytes < size ? bytes : size;

		/* Translate the bus address to a physical address.  If
		 * the domain is NULL it means there is no IOMMU active
		 * and the address translation is the identity
		 */
		addr = dom ? iommu_iova_to_phys(dom, addr) : addr;

		pfn = addr >> PAGE_SHIFT;
		if (!pfn_valid(pfn))
			return -EPERM;

		p = pfn_to_page(pfn);
		if (p->mapping != adev->mman.bdev.dev_mapping)
			return -EPERM;

		ptr = kmap(p);
		r = copy_to_user(buf, ptr + off, bytes);
		kunmap(p);
		if (r)
			return -EFAULT;

		size -= bytes;
		*pos += bytes;
		result += bytes;
	}

	return result;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * amdgpu_iomem_write - Virtual write access to GPU mapped memory
 *
 * This function is used to write memory that has been mapped to the
 * GPU and the known addresses are not physical addresses but instead
 * bus addresses (e.g., what you'd put in an IB or ring buffer).
 */
static ssize_t amdgpu_iomem_write(struct file *f, const char __user *buf,
				 size_t size, loff_t *pos)
{
	struct amdgpu_device *adev = file_inode(f)->i_private;
	struct iommu_domain *dom;
	ssize_t result = 0;
	int r;

	dom = iommu_get_domain_for_dev(adev->dev);

	while (size) {
		phys_addr_t addr = *pos & PAGE_MASK;
		loff_t off = *pos & ~PAGE_MASK;
		size_t bytes = PAGE_SIZE - off;
		unsigned long pfn;
		struct page *p;
		void *ptr;

		bytes = bytes < size ? bytes : size;

		addr = dom ? iommu_iova_to_phys(dom, addr) : addr;

		pfn = addr >> PAGE_SHIFT;
		if (!pfn_valid(pfn))
			return -EPERM;

		p = pfn_to_page(pfn);
		if (p->mapping != adev->mman.bdev.dev_mapping)
			return -EPERM;

		ptr = kmap(p);
		r = copy_from_user(ptr + off, buf, bytes);
		kunmap(p);
		if (r)
			return -EFAULT;

		size -= bytes;
		*pos += bytes;
		result += bytes;
	}

	return result;
}

static const struct file_operations amdgpu_ttm_iomem_fops = {
	.owner = THIS_MODULE,
	.read = amdgpu_iomem_read,
	.write = amdgpu_iomem_write,
	.llseek = default_llseek
};

<<<<<<< HEAD
static const struct {
	char *name;
	const struct file_operations *fops;
	int domain;
} ttm_debugfs_entries[] = {
	{ "amdgpu_vram", &amdgpu_ttm_vram_fops, TTM_PL_VRAM },
#ifdef CONFIG_DRM_AMDGPU_GART_DEBUGFS
	{ "amdgpu_gtt", &amdgpu_ttm_gtt_fops, TTM_PL_TT },
#endif
	{ "amdgpu_iomem", &amdgpu_ttm_iomem_fops, TTM_PL_SYSTEM },
};

#endif

static int amdgpu_ttm_debugfs_init(struct amdgpu_device *adev)
{
#if defined(CONFIG_DEBUG_FS)
	unsigned count;

	struct drm_minor *minor = adev->ddev->primary;
	struct dentry *ent, *root = minor->debugfs_root;

	for (count = 0; count < ARRAY_SIZE(ttm_debugfs_entries); count++) {
		ent = debugfs_create_file(
				ttm_debugfs_entries[count].name,
				S_IFREG | S_IRUGO, root,
				adev,
				ttm_debugfs_entries[count].fops);
		if (IS_ERR(ent))
			return PTR_ERR(ent);
		if (ttm_debugfs_entries[count].domain == TTM_PL_VRAM)
			i_size_write(ent->d_inode, adev->gmc.mc_vram_size);
		else if (ttm_debugfs_entries[count].domain == TTM_PL_TT)
			i_size_write(ent->d_inode, adev->gmc.gart_size);
		adev->mman.debugfs_entries[count] = ent;
	}

	count = ARRAY_SIZE(amdgpu_ttm_debugfs_list);

#ifdef CONFIG_SWIOTLB
	if (!(adev->need_swiotlb && swiotlb_nr_tbl()))
		--count;
#endif

	return amdgpu_debugfs_add_files(adev, amdgpu_ttm_debugfs_list, count);
#else
	return 0;
#endif
}

static void amdgpu_ttm_debugfs_fini(struct amdgpu_device *adev)
{
#if defined(CONFIG_DEBUG_FS)
	unsigned i;

	for (i = 0; i < ARRAY_SIZE(ttm_debugfs_entries); i++)
		debugfs_remove(adev->mman.debugfs_entries[i]);
=======
#endif

void amdgpu_ttm_debugfs_init(struct amdgpu_device *adev)
{
#if defined(CONFIG_DEBUG_FS)
	struct drm_minor *minor = adev_to_drm(adev)->primary;
	struct dentry *root = minor->debugfs_root;

	debugfs_create_file_size("amdgpu_vram", 0444, root, adev,
				 &amdgpu_ttm_vram_fops, adev->gmc.mc_vram_size);
	debugfs_create_file("amdgpu_iomem", 0444, root, adev,
			    &amdgpu_ttm_iomem_fops);
	debugfs_create_file("amdgpu_vram_mm", 0444, root, adev,
			    &amdgpu_mm_vram_table_fops);
	debugfs_create_file("amdgpu_gtt_mm", 0444, root, adev,
			    &amdgpu_mm_tt_table_fops);
	debugfs_create_file("amdgpu_gds_mm", 0444, root, adev,
			    &amdgpu_mm_gds_table_fops);
	debugfs_create_file("amdgpu_gws_mm", 0444, root, adev,
			    &amdgpu_mm_gws_table_fops);
	debugfs_create_file("amdgpu_oa_mm", 0444, root, adev,
			    &amdgpu_mm_oa_table_fops);
	debugfs_create_file("ttm_page_pool", 0444, root, adev,
			    &amdgpu_ttm_page_pool_fops);
>>>>>>> upstream/android-13
#endif
}
