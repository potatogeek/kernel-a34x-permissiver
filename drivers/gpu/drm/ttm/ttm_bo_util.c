/* SPDX-License-Identifier: GPL-2.0 OR MIT */
/**************************************************************************
 *
 * Copyright (c) 2007-2009 VMware, Inc., Palo Alto, CA., USA
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
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
<<<<<<< HEAD
#include <drm/drm_vma_manager.h>
=======
#include <drm/drm_cache.h>
#include <drm/drm_vma_manager.h>
#include <linux/dma-buf-map.h>
>>>>>>> upstream/android-13
#include <linux/io.h>
#include <linux/highmem.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/reservation.h>
=======
#include <linux/dma-resv.h>
>>>>>>> upstream/android-13

struct ttm_transfer_obj {
	struct ttm_buffer_object base;
	struct ttm_buffer_object *bo;
};

<<<<<<< HEAD
void ttm_bo_free_old_node(struct ttm_buffer_object *bo)
{
	ttm_bo_mem_put(bo, &bo->mem);
}

int ttm_bo_move_ttm(struct ttm_buffer_object *bo,
		   struct ttm_operation_ctx *ctx,
		    struct ttm_mem_reg *new_mem)
{
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_mem_reg *old_mem = &bo->mem;
	int ret;

	if (old_mem->mem_type != TTM_PL_SYSTEM) {
		ret = ttm_bo_wait(bo, ctx->interruptible, ctx->no_wait_gpu);

		if (unlikely(ret != 0)) {
			if (ret != -ERESTARTSYS)
				pr_err("Failed to expire sync object before unbinding TTM\n");
			return ret;
		}

		ttm_tt_unbind(ttm);
		ttm_bo_free_old_node(bo);
		ttm_flag_masked(&old_mem->placement, TTM_PL_FLAG_SYSTEM,
				TTM_PL_MASK_MEM);
		old_mem->mem_type = TTM_PL_SYSTEM;
	}

	ret = ttm_tt_set_placement_caching(ttm, new_mem->placement);
	if (unlikely(ret != 0))
		return ret;

	if (new_mem->mem_type != TTM_PL_SYSTEM) {
		ret = ttm_tt_bind(ttm, new_mem, ctx);
		if (unlikely(ret != 0))
			return ret;
	}

	*old_mem = *new_mem;
	new_mem->mm_node = NULL;

	return 0;
}
EXPORT_SYMBOL(ttm_bo_move_ttm);

int ttm_mem_io_lock(struct ttm_mem_type_manager *man, bool interruptible)
{
	if (likely(man->io_reserve_fastpath))
		return 0;

	if (interruptible)
		return mutex_lock_interruptible(&man->io_reserve_mutex);

	mutex_lock(&man->io_reserve_mutex);
	return 0;
}
EXPORT_SYMBOL(ttm_mem_io_lock);

void ttm_mem_io_unlock(struct ttm_mem_type_manager *man)
{
	if (likely(man->io_reserve_fastpath))
		return;

	mutex_unlock(&man->io_reserve_mutex);
}
EXPORT_SYMBOL(ttm_mem_io_unlock);

static int ttm_mem_io_evict(struct ttm_mem_type_manager *man)
{
	struct ttm_buffer_object *bo;

	if (!man->use_io_reserve_lru || list_empty(&man->io_reserve_lru))
		return -EAGAIN;

	bo = list_first_entry(&man->io_reserve_lru,
			      struct ttm_buffer_object,
			      io_reserve_lru);
	list_del_init(&bo->io_reserve_lru);
	ttm_bo_unmap_virtual_locked(bo);

	return 0;
}


int ttm_mem_io_reserve(struct ttm_bo_device *bdev,
		       struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	int ret = 0;

	if (!bdev->driver->io_mem_reserve)
		return 0;
	if (likely(man->io_reserve_fastpath))
		return bdev->driver->io_mem_reserve(bdev, mem);

	if (bdev->driver->io_mem_reserve &&
	    mem->bus.io_reserved_count++ == 0) {
retry:
		ret = bdev->driver->io_mem_reserve(bdev, mem);
		if (ret == -EAGAIN) {
			ret = ttm_mem_io_evict(man);
			if (ret == 0)
				goto retry;
		}
	}
	return ret;
}
EXPORT_SYMBOL(ttm_mem_io_reserve);

void ttm_mem_io_free(struct ttm_bo_device *bdev,
		     struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];

	if (likely(man->io_reserve_fastpath))
		return;

	if (bdev->driver->io_mem_reserve &&
	    --mem->bus.io_reserved_count == 0 &&
	    bdev->driver->io_mem_free)
		bdev->driver->io_mem_free(bdev, mem);

}
EXPORT_SYMBOL(ttm_mem_io_free);

int ttm_mem_io_reserve_vm(struct ttm_buffer_object *bo)
{
	struct ttm_mem_reg *mem = &bo->mem;
	int ret;

	if (!mem->bus.io_reserved_vm) {
		struct ttm_mem_type_manager *man =
			&bo->bdev->man[mem->mem_type];

		ret = ttm_mem_io_reserve(bo->bdev, mem);
		if (unlikely(ret != 0))
			return ret;
		mem->bus.io_reserved_vm = true;
		if (man->use_io_reserve_lru)
			list_add_tail(&bo->io_reserve_lru,
				      &man->io_reserve_lru);
	}
	return 0;
}

void ttm_mem_io_free_vm(struct ttm_buffer_object *bo)
{
	struct ttm_mem_reg *mem = &bo->mem;

	if (mem->bus.io_reserved_vm) {
		mem->bus.io_reserved_vm = false;
		list_del_init(&bo->io_reserve_lru);
		ttm_mem_io_free(bo->bdev, mem);
	}
}

static int ttm_mem_reg_ioremap(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem,
			void **virtual)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	int ret;
	void *addr;

	*virtual = NULL;
	(void) ttm_mem_io_lock(man, false);
	ret = ttm_mem_io_reserve(bdev, mem);
	ttm_mem_io_unlock(man);
	if (ret || !mem->bus.is_iomem)
		return ret;

	if (mem->bus.addr) {
		addr = mem->bus.addr;
	} else {
		if (mem->placement & TTM_PL_FLAG_WC)
			addr = ioremap_wc(mem->bus.base + mem->bus.offset, mem->bus.size);
		else
			addr = ioremap_nocache(mem->bus.base + mem->bus.offset, mem->bus.size);
		if (!addr) {
			(void) ttm_mem_io_lock(man, false);
			ttm_mem_io_free(bdev, mem);
			ttm_mem_io_unlock(man);
			return -ENOMEM;
		}
	}
	*virtual = addr;
	return 0;
}

static void ttm_mem_reg_iounmap(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem,
			 void *virtual)
{
	struct ttm_mem_type_manager *man;

	man = &bdev->man[mem->mem_type];

	if (virtual && mem->bus.addr == NULL)
		iounmap(virtual);
	(void) ttm_mem_io_lock(man, false);
	ttm_mem_io_free(bdev, mem);
	ttm_mem_io_unlock(man);
}

static int ttm_copy_io_page(void *dst, void *src, unsigned long page)
{
	uint32_t *dstP =
	    (uint32_t *) ((unsigned long)dst + (page << PAGE_SHIFT));
	uint32_t *srcP =
	    (uint32_t *) ((unsigned long)src + (page << PAGE_SHIFT));

	int i;
	for (i = 0; i < PAGE_SIZE / sizeof(uint32_t); ++i)
		iowrite32(ioread32(srcP++), dstP++);
	return 0;
}

#ifdef CONFIG_X86
#define __ttm_kmap_atomic_prot(__page, __prot) kmap_atomic_prot(__page, __prot)
#define __ttm_kunmap_atomic(__addr) kunmap_atomic(__addr)
#else
#define __ttm_kmap_atomic_prot(__page, __prot) vmap(&__page, 1, 0,  __prot)
#define __ttm_kunmap_atomic(__addr) vunmap(__addr)
#endif


/**
 * ttm_kmap_atomic_prot - Efficient kernel map of a single page with
 * specified page protection.
 *
 * @page: The page to map.
 * @prot: The page protection.
 *
 * This function maps a TTM page using the kmap_atomic api if available,
 * otherwise falls back to vmap. The user must make sure that the
 * specified page does not have an aliased mapping with a different caching
 * policy unless the architecture explicitly allows it. Also mapping and
 * unmapping using this api must be correctly nested. Unmapping should
 * occur in the reverse order of mapping.
 */
void *ttm_kmap_atomic_prot(struct page *page, pgprot_t prot)
{
	if (pgprot_val(prot) == pgprot_val(PAGE_KERNEL))
		return kmap_atomic(page);
	else
		return __ttm_kmap_atomic_prot(page, prot);
}
EXPORT_SYMBOL(ttm_kmap_atomic_prot);

/**
 * ttm_kunmap_atomic_prot - Unmap a page that was mapped using
 * ttm_kmap_atomic_prot.
 *
 * @addr: The virtual address from the map.
 * @prot: The page protection.
 */
void ttm_kunmap_atomic_prot(void *addr, pgprot_t prot)
{
	if (pgprot_val(prot) == pgprot_val(PAGE_KERNEL))
		kunmap_atomic(addr);
	else
		__ttm_kunmap_atomic(addr);
}
EXPORT_SYMBOL(ttm_kunmap_atomic_prot);

static int ttm_copy_io_ttm_page(struct ttm_tt *ttm, void *src,
				unsigned long page,
				pgprot_t prot)
{
	struct page *d = ttm->pages[page];
	void *dst;

	if (!d)
		return -ENOMEM;

	src = (void *)((unsigned long)src + (page << PAGE_SHIFT));
	dst = ttm_kmap_atomic_prot(d, prot);
	if (!dst)
		return -ENOMEM;

	memcpy_fromio(dst, src, PAGE_SIZE);

	ttm_kunmap_atomic_prot(dst, prot);

	return 0;
}

static int ttm_copy_ttm_io_page(struct ttm_tt *ttm, void *dst,
				unsigned long page,
				pgprot_t prot)
{
	struct page *s = ttm->pages[page];
	void *src;

	if (!s)
		return -ENOMEM;

	dst = (void *)((unsigned long)dst + (page << PAGE_SHIFT));
	src = ttm_kmap_atomic_prot(s, prot);
	if (!src)
		return -ENOMEM;

	memcpy_toio(dst, src, PAGE_SIZE);

	ttm_kunmap_atomic_prot(src, prot);

	return 0;
}

int ttm_bo_move_memcpy(struct ttm_buffer_object *bo,
		       struct ttm_operation_ctx *ctx,
		       struct ttm_mem_reg *new_mem)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[new_mem->mem_type];
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg old_copy = *old_mem;
	void *old_iomap;
	void *new_iomap;
	int ret;
	unsigned long i;
	unsigned long page;
	unsigned long add = 0;
	int dir;

	ret = ttm_bo_wait(bo, ctx->interruptible, ctx->no_wait_gpu);
	if (ret)
		return ret;

	ret = ttm_mem_reg_ioremap(bdev, old_mem, &old_iomap);
	if (ret)
		return ret;
	ret = ttm_mem_reg_ioremap(bdev, new_mem, &new_iomap);
	if (ret)
		goto out;

	/*
	 * Single TTM move. NOP.
	 */
	if (old_iomap == NULL && new_iomap == NULL)
		goto out2;

	/*
	 * Don't move nonexistent data. Clear destination instead.
	 */
	if (old_iomap == NULL &&
	    (ttm == NULL || (ttm->state == tt_unpopulated &&
			     !(ttm->page_flags & TTM_PAGE_FLAG_SWAPPED)))) {
		memset_io(new_iomap, 0, new_mem->num_pages*PAGE_SIZE);
		goto out2;
	}

	/*
	 * TTM might be null for moves within the same region.
	 */
	if (ttm) {
		ret = ttm_tt_populate(ttm, ctx);
		if (ret)
			goto out1;
	}

	add = 0;
	dir = 1;

	if ((old_mem->mem_type == new_mem->mem_type) &&
	    (new_mem->start < old_mem->start + old_mem->size)) {
		dir = -1;
		add = new_mem->num_pages - 1;
	}

	for (i = 0; i < new_mem->num_pages; ++i) {
		page = i * dir + add;
		if (old_iomap == NULL) {
			pgprot_t prot = ttm_io_prot(old_mem->placement,
						    PAGE_KERNEL);
			ret = ttm_copy_ttm_io_page(ttm, new_iomap, page,
						   prot);
		} else if (new_iomap == NULL) {
			pgprot_t prot = ttm_io_prot(new_mem->placement,
						    PAGE_KERNEL);
			ret = ttm_copy_io_ttm_page(ttm, old_iomap, page,
						   prot);
		} else {
			ret = ttm_copy_io_page(new_iomap, old_iomap, page);
		}
		if (ret)
			goto out1;
	}
	mb();
out2:
	old_copy = *old_mem;
	*old_mem = *new_mem;
	new_mem->mm_node = NULL;

	if (man->flags & TTM_MEMTYPE_FLAG_FIXED) {
		ttm_tt_destroy(ttm);
		bo->ttm = NULL;
	}

out1:
	ttm_mem_reg_iounmap(bdev, old_mem, new_iomap);
out:
	ttm_mem_reg_iounmap(bdev, &old_copy, old_iomap);

	/*
	 * On error, keep the mm node!
	 */
	if (!ret)
		ttm_bo_mem_put(bo, &old_copy);
=======
int ttm_mem_io_reserve(struct ttm_device *bdev,
		       struct ttm_resource *mem)
{
	if (mem->bus.offset || mem->bus.addr)
		return 0;

	mem->bus.is_iomem = false;
	if (!bdev->funcs->io_mem_reserve)
		return 0;

	return bdev->funcs->io_mem_reserve(bdev, mem);
}

void ttm_mem_io_free(struct ttm_device *bdev,
		     struct ttm_resource *mem)
{
	if (!mem)
		return;

	if (!mem->bus.offset && !mem->bus.addr)
		return;

	if (bdev->funcs->io_mem_free)
		bdev->funcs->io_mem_free(bdev, mem);

	mem->bus.offset = 0;
	mem->bus.addr = NULL;
}

/**
 * ttm_move_memcpy - Helper to perform a memcpy ttm move operation.
 * @bo: The struct ttm_buffer_object.
 * @new_mem: The struct ttm_resource we're moving to (copy destination).
 * @new_iter: A struct ttm_kmap_iter representing the destination resource.
 * @src_iter: A struct ttm_kmap_iter representing the source resource.
 *
 * This function is intended to be able to move out async under a
 * dma-fence if desired.
 */
void ttm_move_memcpy(struct ttm_buffer_object *bo,
		     u32 num_pages,
		     struct ttm_kmap_iter *dst_iter,
		     struct ttm_kmap_iter *src_iter)
{
	const struct ttm_kmap_iter_ops *dst_ops = dst_iter->ops;
	const struct ttm_kmap_iter_ops *src_ops = src_iter->ops;
	struct ttm_tt *ttm = bo->ttm;
	struct dma_buf_map src_map, dst_map;
	pgoff_t i;

	/* Single TTM move. NOP */
	if (dst_ops->maps_tt && src_ops->maps_tt)
		return;

	/* Don't move nonexistent data. Clear destination instead. */
	if (src_ops->maps_tt && (!ttm || !ttm_tt_is_populated(ttm))) {
		if (ttm && !(ttm->page_flags & TTM_PAGE_FLAG_ZERO_ALLOC))
			return;

		for (i = 0; i < num_pages; ++i) {
			dst_ops->map_local(dst_iter, &dst_map, i);
			if (dst_map.is_iomem)
				memset_io(dst_map.vaddr_iomem, 0, PAGE_SIZE);
			else
				memset(dst_map.vaddr, 0, PAGE_SIZE);
			if (dst_ops->unmap_local)
				dst_ops->unmap_local(dst_iter, &dst_map);
		}
		return;
	}

	for (i = 0; i < num_pages; ++i) {
		dst_ops->map_local(dst_iter, &dst_map, i);
		src_ops->map_local(src_iter, &src_map, i);

		drm_memcpy_from_wc(&dst_map, &src_map, PAGE_SIZE);

		if (src_ops->unmap_local)
			src_ops->unmap_local(src_iter, &src_map);
		if (dst_ops->unmap_local)
			dst_ops->unmap_local(dst_iter, &dst_map);
	}
}
EXPORT_SYMBOL(ttm_move_memcpy);

int ttm_bo_move_memcpy(struct ttm_buffer_object *bo,
		       struct ttm_operation_ctx *ctx,
		       struct ttm_resource *dst_mem)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *dst_man =
		ttm_manager_type(bo->bdev, dst_mem->mem_type);
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_resource *src_mem = bo->resource;
	struct ttm_resource_manager *src_man =
		ttm_manager_type(bdev, src_mem->mem_type);
	union {
		struct ttm_kmap_iter_tt tt;
		struct ttm_kmap_iter_linear_io io;
	} _dst_iter, _src_iter;
	struct ttm_kmap_iter *dst_iter, *src_iter;
	int ret = 0;

	if (ttm && ((ttm->page_flags & TTM_PAGE_FLAG_SWAPPED) ||
		    dst_man->use_tt)) {
		ret = ttm_tt_populate(bdev, ttm, ctx);
		if (ret)
			return ret;
	}

	dst_iter = ttm_kmap_iter_linear_io_init(&_dst_iter.io, bdev, dst_mem);
	if (PTR_ERR(dst_iter) == -EINVAL && dst_man->use_tt)
		dst_iter = ttm_kmap_iter_tt_init(&_dst_iter.tt, bo->ttm);
	if (IS_ERR(dst_iter))
		return PTR_ERR(dst_iter);

	src_iter = ttm_kmap_iter_linear_io_init(&_src_iter.io, bdev, src_mem);
	if (PTR_ERR(src_iter) == -EINVAL && src_man->use_tt)
		src_iter = ttm_kmap_iter_tt_init(&_src_iter.tt, bo->ttm);
	if (IS_ERR(src_iter)) {
		ret = PTR_ERR(src_iter);
		goto out_src_iter;
	}

	ttm_move_memcpy(bo, dst_mem->num_pages, dst_iter, src_iter);

	if (!src_iter->ops->maps_tt)
		ttm_kmap_iter_linear_io_fini(&_src_iter.io, bdev, src_mem);
	ttm_bo_move_sync_cleanup(bo, dst_mem);

out_src_iter:
	if (!dst_iter->ops->maps_tt)
		ttm_kmap_iter_linear_io_fini(&_dst_iter.io, bdev, dst_mem);

>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL(ttm_bo_move_memcpy);

static void ttm_transfered_destroy(struct ttm_buffer_object *bo)
{
	struct ttm_transfer_obj *fbo;

	fbo = container_of(bo, struct ttm_transfer_obj, base);
<<<<<<< HEAD
=======
	dma_resv_fini(&fbo->base.base._resv);
>>>>>>> upstream/android-13
	ttm_bo_put(fbo->bo);
	kfree(fbo);
}

/**
 * ttm_buffer_object_transfer
 *
 * @bo: A pointer to a struct ttm_buffer_object.
 * @new_obj: A pointer to a pointer to a newly created ttm_buffer_object,
 * holding the data of @bo with the old placement.
 *
 * This is a utility function that may be called after an accelerated move
 * has been scheduled. A new buffer object is created as a placeholder for
 * the old data while it's being copied. When that buffer object is idle,
 * it can be destroyed, releasing the space of the old placement.
 * Returns:
 * !0: Failure.
 */

static int ttm_buffer_object_transfer(struct ttm_buffer_object *bo,
				      struct ttm_buffer_object **new_obj)
{
	struct ttm_transfer_obj *fbo;
	int ret;

	fbo = kmalloc(sizeof(*fbo), GFP_KERNEL);
	if (!fbo)
		return -ENOMEM;

	fbo->base = *bo;
<<<<<<< HEAD
	fbo->base.mem.placement |= TTM_PL_FLAG_NO_EVICT;
=======
>>>>>>> upstream/android-13

	ttm_bo_get(bo);
	fbo->bo = bo;

	/**
	 * Fix up members that we shouldn't copy directly:
	 * TODO: Explicit member copy would probably be better here.
	 */

<<<<<<< HEAD
	atomic_inc(&bo->bdev->glob->bo_count);
	INIT_LIST_HEAD(&fbo->base.ddestroy);
	INIT_LIST_HEAD(&fbo->base.lru);
	INIT_LIST_HEAD(&fbo->base.swap);
	INIT_LIST_HEAD(&fbo->base.io_reserve_lru);
	mutex_init(&fbo->base.wu_mutex);
	fbo->base.moving = NULL;
	drm_vma_node_reset(&fbo->base.vma_node);
	atomic_set(&fbo->base.cpu_writers, 0);

	kref_init(&fbo->base.list_kref);
	kref_init(&fbo->base.kref);
	fbo->base.destroy = &ttm_transfered_destroy;
	fbo->base.acc_size = 0;
	fbo->base.resv = &fbo->base.ttm_resv;
	reservation_object_init(fbo->base.resv);
	ret = reservation_object_trylock(fbo->base.resv);
	WARN_ON(!ret);

=======
	atomic_inc(&ttm_glob.bo_count);
	INIT_LIST_HEAD(&fbo->base.ddestroy);
	INIT_LIST_HEAD(&fbo->base.lru);
	fbo->base.moving = NULL;
	drm_vma_node_reset(&fbo->base.base.vma_node);

	kref_init(&fbo->base.kref);
	fbo->base.destroy = &ttm_transfered_destroy;
	fbo->base.pin_count = 0;
	if (bo->type != ttm_bo_type_sg)
		fbo->base.base.resv = &fbo->base.base._resv;

	dma_resv_init(&fbo->base.base._resv);
	fbo->base.base.dev = NULL;
	ret = dma_resv_trylock(&fbo->base.base._resv);
	WARN_ON(!ret);

	ttm_bo_move_to_lru_tail_unlocked(&fbo->base);

>>>>>>> upstream/android-13
	*new_obj = &fbo->base;
	return 0;
}

<<<<<<< HEAD
pgprot_t ttm_io_prot(uint32_t caching_flags, pgprot_t tmp)
{
	/* Cached mappings need no adjustment */
	if (caching_flags & TTM_PL_FLAG_CACHED)
		return tmp;

#if defined(__i386__) || defined(__x86_64__)
	if (caching_flags & TTM_PL_FLAG_WC)
		tmp = pgprot_writecombine(tmp);
	else if (boot_cpu_data.x86 > 3)
		tmp = pgprot_noncached(tmp);
#endif
#if defined(__ia64__) || defined(__arm__) || defined(__aarch64__) || \
    defined(__powerpc__)
	if (caching_flags & TTM_PL_FLAG_WC)
		tmp = pgprot_writecombine(tmp);
	else
		tmp = pgprot_noncached(tmp);
#endif
#if defined(__sparc__) || defined(__mips__)
	tmp = pgprot_noncached(tmp);
#endif
	return tmp;
=======
pgprot_t ttm_io_prot(struct ttm_buffer_object *bo, struct ttm_resource *res,
		     pgprot_t tmp)
{
	struct ttm_resource_manager *man;
	enum ttm_caching caching;

	man = ttm_manager_type(bo->bdev, res->mem_type);
	caching = man->use_tt ? bo->ttm->caching : res->bus.caching;

	return ttm_prot_from_caching(caching, tmp);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(ttm_io_prot);

static int ttm_bo_ioremap(struct ttm_buffer_object *bo,
			  unsigned long offset,
			  unsigned long size,
			  struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_reg *mem = &bo->mem;

	if (bo->mem.bus.addr) {
		map->bo_kmap_type = ttm_bo_map_premapped;
		map->virtual = (void *)(((u8 *)bo->mem.bus.addr) + offset);
	} else {
		map->bo_kmap_type = ttm_bo_map_iomap;
		if (mem->placement & TTM_PL_FLAG_WC)
			map->virtual = ioremap_wc(bo->mem.bus.base + bo->mem.bus.offset + offset,
						  size);
		else
			map->virtual = ioremap_nocache(bo->mem.bus.base + bo->mem.bus.offset + offset,
						       size);
=======
	struct ttm_resource *mem = bo->resource;

	if (bo->resource->bus.addr) {
		map->bo_kmap_type = ttm_bo_map_premapped;
		map->virtual = ((u8 *)bo->resource->bus.addr) + offset;
	} else {
		resource_size_t res = bo->resource->bus.offset + offset;

		map->bo_kmap_type = ttm_bo_map_iomap;
		if (mem->bus.caching == ttm_write_combined)
			map->virtual = ioremap_wc(res, size);
#ifdef CONFIG_X86
		else if (mem->bus.caching == ttm_cached)
			map->virtual = ioremap_cache(res, size);
#endif
		else
			map->virtual = ioremap(res, size);
>>>>>>> upstream/android-13
	}
	return (!map->virtual) ? -ENOMEM : 0;
}

static int ttm_bo_kmap_ttm(struct ttm_buffer_object *bo,
			   unsigned long start_page,
			   unsigned long num_pages,
			   struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_reg *mem = &bo->mem;
=======
	struct ttm_resource *mem = bo->resource;
>>>>>>> upstream/android-13
	struct ttm_operation_ctx ctx = {
		.interruptible = false,
		.no_wait_gpu = false
	};
	struct ttm_tt *ttm = bo->ttm;
	pgprot_t prot;
	int ret;

	BUG_ON(!ttm);

<<<<<<< HEAD
	ret = ttm_tt_populate(ttm, &ctx);
	if (ret)
		return ret;

	if (num_pages == 1 && (mem->placement & TTM_PL_FLAG_CACHED)) {
=======
	ret = ttm_tt_populate(bo->bdev, ttm, &ctx);
	if (ret)
		return ret;

	if (num_pages == 1 && ttm->caching == ttm_cached) {
>>>>>>> upstream/android-13
		/*
		 * We're mapping a single page, and the desired
		 * page protection is consistent with the bo.
		 */

		map->bo_kmap_type = ttm_bo_map_kmap;
		map->page = ttm->pages[start_page];
		map->virtual = kmap(map->page);
	} else {
		/*
		 * We need to use vmap to get the desired page protection
		 * or to make the buffer object look contiguous.
		 */
<<<<<<< HEAD
		prot = ttm_io_prot(mem->placement, PAGE_KERNEL);
=======
		prot = ttm_io_prot(bo, mem, PAGE_KERNEL);
>>>>>>> upstream/android-13
		map->bo_kmap_type = ttm_bo_map_vmap;
		map->virtual = vmap(ttm->pages + start_page, num_pages,
				    0, prot);
	}
	return (!map->virtual) ? -ENOMEM : 0;
}

int ttm_bo_kmap(struct ttm_buffer_object *bo,
		unsigned long start_page, unsigned long num_pages,
		struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_type_manager *man =
		&bo->bdev->man[bo->mem.mem_type];
=======
>>>>>>> upstream/android-13
	unsigned long offset, size;
	int ret;

	map->virtual = NULL;
	map->bo = bo;
<<<<<<< HEAD
	if (num_pages > bo->num_pages)
		return -EINVAL;
	if (start_page > bo->num_pages)
		return -EINVAL;
#if 0
	if (num_pages > 1 && !capable(CAP_SYS_ADMIN))
		return -EPERM;
#endif
	(void) ttm_mem_io_lock(man, false);
	ret = ttm_mem_io_reserve(bo->bdev, &bo->mem);
	ttm_mem_io_unlock(man);
	if (ret)
		return ret;
	if (!bo->mem.bus.is_iomem) {
=======
	if (num_pages > bo->resource->num_pages)
		return -EINVAL;
	if ((start_page + num_pages) > bo->resource->num_pages)
		return -EINVAL;

	ret = ttm_mem_io_reserve(bo->bdev, bo->resource);
	if (ret)
		return ret;
	if (!bo->resource->bus.is_iomem) {
>>>>>>> upstream/android-13
		return ttm_bo_kmap_ttm(bo, start_page, num_pages, map);
	} else {
		offset = start_page << PAGE_SHIFT;
		size = num_pages << PAGE_SHIFT;
		return ttm_bo_ioremap(bo, offset, size, map);
	}
}
EXPORT_SYMBOL(ttm_bo_kmap);

void ttm_bo_kunmap(struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_buffer_object *bo = map->bo;
	struct ttm_mem_type_manager *man =
		&bo->bdev->man[bo->mem.mem_type];

=======
>>>>>>> upstream/android-13
	if (!map->virtual)
		return;
	switch (map->bo_kmap_type) {
	case ttm_bo_map_iomap:
		iounmap(map->virtual);
		break;
	case ttm_bo_map_vmap:
		vunmap(map->virtual);
		break;
	case ttm_bo_map_kmap:
		kunmap(map->page);
		break;
	case ttm_bo_map_premapped:
		break;
	default:
		BUG();
	}
<<<<<<< HEAD
	(void) ttm_mem_io_lock(man, false);
	ttm_mem_io_free(map->bo->bdev, &map->bo->mem);
	ttm_mem_io_unlock(man);
=======
	ttm_mem_io_free(map->bo->bdev, map->bo->resource);
>>>>>>> upstream/android-13
	map->virtual = NULL;
	map->page = NULL;
}
EXPORT_SYMBOL(ttm_bo_kunmap);

<<<<<<< HEAD
int ttm_bo_move_accel_cleanup(struct ttm_buffer_object *bo,
			      struct dma_fence *fence,
			      bool evict,
			      struct ttm_mem_reg *new_mem)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[new_mem->mem_type];
	struct ttm_mem_reg *old_mem = &bo->mem;
	int ret;
	struct ttm_buffer_object *ghost_obj;

	reservation_object_add_excl_fence(bo->resv, fence);
	if (evict) {
		ret = ttm_bo_wait(bo, false, false);
		if (ret)
			return ret;

		if (man->flags & TTM_MEMTYPE_FLAG_FIXED) {
			ttm_tt_destroy(bo->ttm);
			bo->ttm = NULL;
		}
		ttm_bo_free_old_node(bo);
	} else {
		/**
		 * This should help pipeline ordinary buffer moves.
		 *
		 * Hang old buffer memory on a new buffer object,
		 * and leave it to be released when the GPU
		 * operation has completed.
		 */

		dma_fence_put(bo->moving);
		bo->moving = dma_fence_get(fence);

		ret = ttm_buffer_object_transfer(bo, &ghost_obj);
		if (ret)
			return ret;

		reservation_object_add_excl_fence(ghost_obj->resv, fence);

		/**
		 * If we're not moving to fixed memory, the TTM object
		 * needs to stay alive. Otherwhise hang it on the ghost
		 * bo to be unbound and destroyed.
		 */

		if (!(man->flags & TTM_MEMTYPE_FLAG_FIXED))
			ghost_obj->ttm = NULL;
		else
			bo->ttm = NULL;

		ttm_bo_unreserve(ghost_obj);
		ttm_bo_put(ghost_obj);
	}

	*old_mem = *new_mem;
	new_mem->mm_node = NULL;
=======
int ttm_bo_vmap(struct ttm_buffer_object *bo, struct dma_buf_map *map)
{
	struct ttm_resource *mem = bo->resource;
	int ret;

	ret = ttm_mem_io_reserve(bo->bdev, mem);
	if (ret)
		return ret;

	if (mem->bus.is_iomem) {
		void __iomem *vaddr_iomem;

		if (mem->bus.addr)
			vaddr_iomem = (void __iomem *)mem->bus.addr;
		else if (mem->bus.caching == ttm_write_combined)
			vaddr_iomem = ioremap_wc(mem->bus.offset,
						 bo->base.size);
#ifdef CONFIG_X86
		else if (mem->bus.caching == ttm_cached)
			vaddr_iomem = ioremap_cache(mem->bus.offset,
						  bo->base.size);
#endif
		else
			vaddr_iomem = ioremap(mem->bus.offset, bo->base.size);

		if (!vaddr_iomem)
			return -ENOMEM;

		dma_buf_map_set_vaddr_iomem(map, vaddr_iomem);

	} else {
		struct ttm_operation_ctx ctx = {
			.interruptible = false,
			.no_wait_gpu = false
		};
		struct ttm_tt *ttm = bo->ttm;
		pgprot_t prot;
		void *vaddr;

		ret = ttm_tt_populate(bo->bdev, ttm, &ctx);
		if (ret)
			return ret;

		/*
		 * We need to use vmap to get the desired page protection
		 * or to make the buffer object look contiguous.
		 */
		prot = ttm_io_prot(bo, mem, PAGE_KERNEL);
		vaddr = vmap(ttm->pages, ttm->num_pages, 0, prot);
		if (!vaddr)
			return -ENOMEM;

		dma_buf_map_set_vaddr(map, vaddr);
	}

	return 0;
}
EXPORT_SYMBOL(ttm_bo_vmap);

void ttm_bo_vunmap(struct ttm_buffer_object *bo, struct dma_buf_map *map)
{
	struct ttm_resource *mem = bo->resource;

	if (dma_buf_map_is_null(map))
		return;

	if (!map->is_iomem)
		vunmap(map->vaddr);
	else if (!mem->bus.addr)
		iounmap(map->vaddr_iomem);
	dma_buf_map_clear(map);

	ttm_mem_io_free(bo->bdev, bo->resource);
}
EXPORT_SYMBOL(ttm_bo_vunmap);

static int ttm_bo_wait_free_node(struct ttm_buffer_object *bo,
				 bool dst_use_tt)
{
	int ret;
	ret = ttm_bo_wait(bo, false, false);
	if (ret)
		return ret;

	if (!dst_use_tt)
		ttm_bo_tt_destroy(bo);
	ttm_resource_free(bo, &bo->resource);
	return 0;
}

static int ttm_bo_move_to_ghost(struct ttm_buffer_object *bo,
				struct dma_fence *fence,
				bool dst_use_tt)
{
	struct ttm_buffer_object *ghost_obj;
	int ret;

	/**
	 * This should help pipeline ordinary buffer moves.
	 *
	 * Hang old buffer memory on a new buffer object,
	 * and leave it to be released when the GPU
	 * operation has completed.
	 */

	dma_fence_put(bo->moving);
	bo->moving = dma_fence_get(fence);

	ret = ttm_buffer_object_transfer(bo, &ghost_obj);
	if (ret)
		return ret;

	dma_resv_add_excl_fence(&ghost_obj->base._resv, fence);

	/**
	 * If we're not moving to fixed memory, the TTM object
	 * needs to stay alive. Otherwhise hang it on the ghost
	 * bo to be unbound and destroyed.
	 */

	if (dst_use_tt)
		ghost_obj->ttm = NULL;
	else
		bo->ttm = NULL;
	bo->resource = NULL;

	dma_resv_unlock(&ghost_obj->base._resv);
	ttm_bo_put(ghost_obj);
	return 0;
}

static void ttm_bo_move_pipeline_evict(struct ttm_buffer_object *bo,
				       struct dma_fence *fence)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *from;

	from = ttm_manager_type(bdev, bo->resource->mem_type);

	/**
	 * BO doesn't have a TTM we need to bind/unbind. Just remember
	 * this eviction and free up the allocation
	 */
	spin_lock(&from->move_lock);
	if (!from->move || dma_fence_is_later(fence, from->move)) {
		dma_fence_put(from->move);
		from->move = dma_fence_get(fence);
	}
	spin_unlock(&from->move_lock);

	ttm_resource_free(bo, &bo->resource);

	dma_fence_put(bo->moving);
	bo->moving = dma_fence_get(fence);
}

int ttm_bo_move_accel_cleanup(struct ttm_buffer_object *bo,
			      struct dma_fence *fence,
			      bool evict,
			      bool pipeline,
			      struct ttm_resource *new_mem)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *from = ttm_manager_type(bdev, bo->resource->mem_type);
	struct ttm_resource_manager *man = ttm_manager_type(bdev, new_mem->mem_type);
	int ret = 0;

	dma_resv_add_excl_fence(bo->base.resv, fence);
	if (!evict)
		ret = ttm_bo_move_to_ghost(bo, fence, man->use_tt);
	else if (!from->use_tt && pipeline)
		ttm_bo_move_pipeline_evict(bo, fence);
	else
		ret = ttm_bo_wait_free_node(bo, man->use_tt);

	if (ret)
		return ret;

	ttm_bo_assign_mem(bo, new_mem);
>>>>>>> upstream/android-13

	return 0;
}
EXPORT_SYMBOL(ttm_bo_move_accel_cleanup);

<<<<<<< HEAD
int ttm_bo_pipeline_move(struct ttm_buffer_object *bo,
			 struct dma_fence *fence, bool evict,
			 struct ttm_mem_reg *new_mem)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_reg *old_mem = &bo->mem;

	struct ttm_mem_type_manager *from = &bdev->man[old_mem->mem_type];
	struct ttm_mem_type_manager *to = &bdev->man[new_mem->mem_type];

	int ret;

	reservation_object_add_excl_fence(bo->resv, fence);

	if (!evict) {
		struct ttm_buffer_object *ghost_obj;

		/**
		 * This should help pipeline ordinary buffer moves.
		 *
		 * Hang old buffer memory on a new buffer object,
		 * and leave it to be released when the GPU
		 * operation has completed.
		 */

		dma_fence_put(bo->moving);
		bo->moving = dma_fence_get(fence);

		ret = ttm_buffer_object_transfer(bo, &ghost_obj);
		if (ret)
			return ret;

		reservation_object_add_excl_fence(ghost_obj->resv, fence);

		/**
		 * If we're not moving to fixed memory, the TTM object
		 * needs to stay alive. Otherwhise hang it on the ghost
		 * bo to be unbound and destroyed.
		 */

		if (!(to->flags & TTM_MEMTYPE_FLAG_FIXED))
			ghost_obj->ttm = NULL;
		else
			bo->ttm = NULL;

		ttm_bo_unreserve(ghost_obj);
		ttm_bo_put(ghost_obj);

	} else if (from->flags & TTM_MEMTYPE_FLAG_FIXED) {

		/**
		 * BO doesn't have a TTM we need to bind/unbind. Just remember
		 * this eviction and free up the allocation
		 */

		spin_lock(&from->move_lock);
		if (!from->move || dma_fence_is_later(fence, from->move)) {
			dma_fence_put(from->move);
			from->move = dma_fence_get(fence);
		}
		spin_unlock(&from->move_lock);

		ttm_bo_free_old_node(bo);

		dma_fence_put(bo->moving);
		bo->moving = dma_fence_get(fence);

	} else {
		/**
		 * Last resort, wait for the move to be completed.
		 *
		 * Should never happen in pratice.
		 */

		ret = ttm_bo_wait(bo, false, false);
		if (ret)
			return ret;

		if (to->flags & TTM_MEMTYPE_FLAG_FIXED) {
			ttm_tt_destroy(bo->ttm);
			bo->ttm = NULL;
		}
		ttm_bo_free_old_node(bo);
	}

	*old_mem = *new_mem;
	new_mem->mm_node = NULL;

	return 0;
}
EXPORT_SYMBOL(ttm_bo_pipeline_move);

int ttm_bo_pipeline_gutting(struct ttm_buffer_object *bo)
{
	struct ttm_buffer_object *ghost;
	int ret;

	ret = ttm_buffer_object_transfer(bo, &ghost);
	if (ret)
		return ret;

	ret = reservation_object_copy_fences(ghost->resv, bo->resv);
=======
/**
 * ttm_bo_pipeline_gutting - purge the contents of a bo
 * @bo: The buffer object
 *
 * Purge the contents of a bo, async if the bo is not idle.
 * After a successful call, the bo is left unpopulated in
 * system placement. The function may wait uninterruptible
 * for idle on OOM.
 *
 * Return: 0 if successful, negative error code on failure.
 */
int ttm_bo_pipeline_gutting(struct ttm_buffer_object *bo)
{
	static const struct ttm_place sys_mem = { .mem_type = TTM_PL_SYSTEM };
	struct ttm_buffer_object *ghost;
	struct ttm_resource *sys_res;
	struct ttm_tt *ttm;
	int ret;

	ret = ttm_resource_alloc(bo, &sys_mem, &sys_res);
	if (ret)
		return ret;

	/* If already idle, no need for ghost object dance. */
	ret = ttm_bo_wait(bo, false, true);
	if (ret != -EBUSY) {
		if (!bo->ttm) {
			/* See comment below about clearing. */
			ret = ttm_tt_create(bo, true);
			if (ret)
				goto error_free_sys_mem;
		} else {
			ttm_tt_unpopulate(bo->bdev, bo->ttm);
			if (bo->type == ttm_bo_type_device)
				ttm_tt_mark_for_clear(bo->ttm);
		}
		ttm_resource_free(bo, &bo->resource);
		ttm_bo_assign_mem(bo, sys_res);
		return 0;
	}

	/*
	 * We need an unpopulated ttm_tt after giving our current one,
	 * if any, to the ghost object. And we can't afford to fail
	 * creating one *after* the operation. If the bo subsequently gets
	 * resurrected, make sure it's cleared (if ttm_bo_type_device)
	 * to avoid leaking sensitive information to user-space.
	 */

	ttm = bo->ttm;
	bo->ttm = NULL;
	ret = ttm_tt_create(bo, true);
	swap(bo->ttm, ttm);
	if (ret)
		goto error_free_sys_mem;

	ret = ttm_buffer_object_transfer(bo, &ghost);
	if (ret)
		goto error_destroy_tt;

	ret = dma_resv_copy_fences(&ghost->base._resv, bo->base.resv);
>>>>>>> upstream/android-13
	/* Last resort, wait for the BO to be idle when we are OOM */
	if (ret)
		ttm_bo_wait(bo, false, false);

<<<<<<< HEAD
	memset(&bo->mem, 0, sizeof(bo->mem));
	bo->mem.mem_type = TTM_PL_SYSTEM;
	bo->ttm = NULL;

	ttm_bo_unreserve(ghost);
	ttm_bo_put(ghost);

	return 0;
=======
	dma_resv_unlock(&ghost->base._resv);
	ttm_bo_put(ghost);
	bo->ttm = ttm;
	bo->resource = NULL;
	ttm_bo_assign_mem(bo, sys_res);
	return 0;

error_destroy_tt:
	ttm_tt_destroy(bo->bdev, ttm);

error_free_sys_mem:
	ttm_resource_free(bo, &sys_res);
	return ret;
>>>>>>> upstream/android-13
}
