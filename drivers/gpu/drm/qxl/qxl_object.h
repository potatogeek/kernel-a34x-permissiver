/*
 * Copyright 2013 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Dave Airlie
 *          Alon Levy
 */
#ifndef QXL_OBJECT_H
#define QXL_OBJECT_H

#include "qxl_drv.h"

<<<<<<< HEAD
static inline int qxl_bo_reserve(struct qxl_bo *bo, bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, NULL);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS) {
			struct drm_device *ddev = bo->gem_base.dev;
=======
static inline int qxl_bo_reserve(struct qxl_bo *bo)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, false, NULL);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS) {
			struct drm_device *ddev = bo->tbo.base.dev;

>>>>>>> upstream/android-13
			dev_err(ddev->dev, "%p reserve failed\n", bo);
		}
		return r;
	}
	return 0;
}

static inline void qxl_bo_unreserve(struct qxl_bo *bo)
{
	ttm_bo_unreserve(&bo->tbo);
}

<<<<<<< HEAD
static inline u64 qxl_bo_gpu_offset(struct qxl_bo *bo)
{
	return bo->tbo.offset;
}

static inline unsigned long qxl_bo_size(struct qxl_bo *bo)
{
	return bo->tbo.num_pages << PAGE_SHIFT;
}

static inline u64 qxl_bo_mmap_offset(struct qxl_bo *bo)
{
	return drm_vma_node_offset_addr(&bo->tbo.vma_node);
}

static inline int qxl_bo_wait(struct qxl_bo *bo, u32 *mem_type,
			      bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, NULL);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS) {
			struct drm_device *ddev = bo->gem_base.dev;
			dev_err(ddev->dev, "%p reserve failed for wait\n",
				bo);
		}
		return r;
	}
	if (mem_type)
		*mem_type = bo->tbo.mem.mem_type;

	r = ttm_bo_wait(&bo->tbo, true, no_wait);
	ttm_bo_unreserve(&bo->tbo);
	return r;
=======
static inline unsigned long qxl_bo_size(struct qxl_bo *bo)
{
	return bo->tbo.base.size;
>>>>>>> upstream/android-13
}

extern int qxl_bo_create(struct qxl_device *qdev,
			 unsigned long size,
			 bool kernel, bool pinned, u32 domain,
<<<<<<< HEAD
			 struct qxl_surface *surf,
			 struct qxl_bo **bo_ptr);
extern int qxl_bo_kmap(struct qxl_bo *bo, void **ptr);
extern void qxl_bo_kunmap(struct qxl_bo *bo);
=======
			 u32 priority,
			 struct qxl_surface *surf,
			 struct qxl_bo **bo_ptr);
int qxl_bo_vmap(struct qxl_bo *bo, struct dma_buf_map *map);
int qxl_bo_vmap_locked(struct qxl_bo *bo, struct dma_buf_map *map);
int qxl_bo_vunmap(struct qxl_bo *bo);
void qxl_bo_vunmap_locked(struct qxl_bo *bo);
>>>>>>> upstream/android-13
void *qxl_bo_kmap_atomic_page(struct qxl_device *qdev, struct qxl_bo *bo, int page_offset);
void qxl_bo_kunmap_atomic_page(struct qxl_device *qdev, struct qxl_bo *bo, void *map);
extern struct qxl_bo *qxl_bo_ref(struct qxl_bo *bo);
extern void qxl_bo_unref(struct qxl_bo **bo);
<<<<<<< HEAD
extern int qxl_bo_pin(struct qxl_bo *bo, u32 domain, u64 *gpu_addr);
extern int qxl_bo_unpin(struct qxl_bo *bo);
extern void qxl_ttm_placement_from_domain(struct qxl_bo *qbo, u32 domain, bool pinned);
=======
extern int qxl_bo_pin(struct qxl_bo *bo);
extern int qxl_bo_unpin(struct qxl_bo *bo);
extern void qxl_ttm_placement_from_domain(struct qxl_bo *qbo, u32 domain);
>>>>>>> upstream/android-13
extern bool qxl_ttm_bo_is_qxl_bo(struct ttm_buffer_object *bo);

#endif
