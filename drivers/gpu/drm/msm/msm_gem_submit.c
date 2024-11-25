<<<<<<< HEAD
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/sync_file.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#include <linux/file.h>
#include <linux/sync_file.h>
#include <linux/uaccess.h>

#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_syncobj.h>
>>>>>>> upstream/android-13

#include "msm_drv.h"
#include "msm_gpu.h"
#include "msm_gem.h"
<<<<<<< HEAD
=======
#include "msm_gpu_trace.h"
>>>>>>> upstream/android-13

/*
 * Cmdstream submission:
 */

/* make sure these don't conflict w/ MSM_SUBMIT_BO_x */
#define BO_VALID    0x8000   /* is current addr in cmdstream correct/valid? */
<<<<<<< HEAD
#define BO_LOCKED   0x4000
#define BO_PINNED   0x2000

static struct msm_gem_submit *submit_create(struct drm_device *dev,
		struct msm_gpu *gpu, struct msm_gpu_submitqueue *queue,
		uint32_t nr_bos, uint32_t nr_cmds)
{
	struct msm_gem_submit *submit;
	uint64_t sz = sizeof(*submit) + ((u64)nr_bos * sizeof(submit->bos[0])) +
		((u64)nr_cmds * sizeof(submit->cmd[0]));

	if (sz > SIZE_MAX)
		return NULL;

	submit = kmalloc(sz, GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY);
	if (!submit)
		return NULL;

	submit->dev = dev;
	submit->gpu = gpu;
	submit->fence = NULL;
	submit->pid = get_pid(task_pid(current));
	submit->cmd = (void *)&submit->bos[nr_bos];
	submit->queue = queue;
	submit->ring = gpu->rb[queue->prio];

	/* initially, until copy_from_user() and bo lookup succeeds: */
	submit->nr_bos = 0;
	submit->nr_cmds = 0;

	INIT_LIST_HEAD(&submit->node);
	INIT_LIST_HEAD(&submit->bo_list);
	ww_acquire_init(&submit->ticket, &reservation_ww_class);
=======
#define BO_LOCKED   0x4000   /* obj lock is held */
#define BO_ACTIVE   0x2000   /* active refcnt is held */
#define BO_PINNED   0x1000   /* obj is pinned and on active list */

static struct msm_gem_submit *submit_create(struct drm_device *dev,
		struct msm_gpu *gpu,
		struct msm_gpu_submitqueue *queue, uint32_t nr_bos,
		uint32_t nr_cmds)
{
	struct msm_gem_submit *submit;
	uint64_t sz;
	int ret;

	sz = struct_size(submit, bos, nr_bos) +
			((u64)nr_cmds * sizeof(submit->cmd[0]));

	if (sz > SIZE_MAX)
		return ERR_PTR(-ENOMEM);

	submit = kzalloc(sz, GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY);
	if (!submit)
		return ERR_PTR(-ENOMEM);

	ret = drm_sched_job_init(&submit->base, queue->entity, queue);
	if (ret) {
		kfree(submit);
		return ERR_PTR(ret);
	}

	xa_init_flags(&submit->deps, XA_FLAGS_ALLOC);

	kref_init(&submit->ref);
	submit->dev = dev;
	submit->aspace = queue->ctx->aspace;
	submit->gpu = gpu;
	submit->cmd = (void *)&submit->bos[nr_bos];
	submit->queue = queue;
	submit->ring = gpu->rb[queue->ring_nr];
	submit->fault_dumped = false;

	INIT_LIST_HEAD(&submit->node);
>>>>>>> upstream/android-13

	return submit;
}

<<<<<<< HEAD
void msm_gem_submit_free(struct msm_gem_submit *submit)
{
	dma_fence_put(submit->fence);
	list_del(&submit->node);
	put_pid(submit->pid);
	msm_submitqueue_put(submit->queue);

	kfree(submit);
}

static inline unsigned long __must_check
copy_from_user_inatomic(void *to, const void __user *from, unsigned long n)
{
	if (access_ok(VERIFY_READ, from, n))
		return __copy_from_user_inatomic(to, from, n);
	return -EFAULT;
=======
void __msm_gem_submit_destroy(struct kref *kref)
{
	struct msm_gem_submit *submit =
			container_of(kref, struct msm_gem_submit, ref);
	unsigned long index;
	struct dma_fence *fence;
	unsigned i;

	if (submit->fence_id) {
		mutex_lock(&submit->queue->lock);
		idr_remove(&submit->queue->fence_idr, submit->fence_id);
		mutex_unlock(&submit->queue->lock);
	}

	xa_for_each (&submit->deps, index, fence) {
		dma_fence_put(fence);
	}

	xa_destroy(&submit->deps);

	dma_fence_put(submit->user_fence);
	dma_fence_put(submit->hw_fence);

	put_pid(submit->pid);
	msm_submitqueue_put(submit->queue);

	for (i = 0; i < submit->nr_cmds; i++)
		kfree(submit->cmd[i].relocs);

	kfree(submit);
>>>>>>> upstream/android-13
}

static int submit_lookup_objects(struct msm_gem_submit *submit,
		struct drm_msm_gem_submit *args, struct drm_file *file)
{
	unsigned i;
	int ret = 0;

<<<<<<< HEAD
	spin_lock(&file->table_lock);
	pagefault_disable();

	for (i = 0; i < args->nr_bos; i++) {
		struct drm_msm_gem_submit_bo submit_bo;
		struct drm_gem_object *obj;
		struct msm_gem_object *msm_obj;
=======
	for (i = 0; i < args->nr_bos; i++) {
		struct drm_msm_gem_submit_bo submit_bo;
>>>>>>> upstream/android-13
		void __user *userptr =
			u64_to_user_ptr(args->bos + (i * sizeof(submit_bo)));

		/* make sure we don't have garbage flags, in case we hit
		 * error path before flags is initialized:
		 */
		submit->bos[i].flags = 0;

<<<<<<< HEAD
		if (copy_from_user_inatomic(&submit_bo, userptr, sizeof(submit_bo))) {
			pagefault_enable();
			spin_unlock(&file->table_lock);
			if (copy_from_user(&submit_bo, userptr, sizeof(submit_bo))) {
				ret = -EFAULT;
				goto out;
			}
			spin_lock(&file->table_lock);
			pagefault_disable();
		}

		if ((submit_bo.flags & ~MSM_SUBMIT_BO_FLAGS) ||
			!(submit_bo.flags & MSM_SUBMIT_BO_FLAGS)) {
			DRM_ERROR("invalid flags: %x\n", submit_bo.flags);
			ret = -EINVAL;
			goto out_unlock;
		}

		submit->bos[i].flags = submit_bo.flags;
		/* in validate_objects() we figure out if this is true: */
		submit->bos[i].iova  = submit_bo.presumed;
=======
		if (copy_from_user(&submit_bo, userptr, sizeof(submit_bo))) {
			ret = -EFAULT;
			i = 0;
			goto out;
		}

/* at least one of READ and/or WRITE flags should be set: */
#define MANDATORY_FLAGS (MSM_SUBMIT_BO_READ | MSM_SUBMIT_BO_WRITE)

		if ((submit_bo.flags & ~MSM_SUBMIT_BO_FLAGS) ||
			!(submit_bo.flags & MANDATORY_FLAGS)) {
			DRM_ERROR("invalid flags: %x\n", submit_bo.flags);
			ret = -EINVAL;
			i = 0;
			goto out;
		}

		submit->bos[i].handle = submit_bo.handle;
		submit->bos[i].flags = submit_bo.flags;
		/* in validate_objects() we figure out if this is true: */
		submit->bos[i].iova  = submit_bo.presumed;
	}

	spin_lock(&file->table_lock);

	for (i = 0; i < args->nr_bos; i++) {
		struct drm_gem_object *obj;
>>>>>>> upstream/android-13

		/* normally use drm_gem_object_lookup(), but for bulk lookup
		 * all under single table_lock just hit object_idr directly:
		 */
<<<<<<< HEAD
		obj = idr_find(&file->object_idr, submit_bo.handle);
		if (!obj) {
			DRM_ERROR("invalid handle %u at index %u\n", submit_bo.handle, i);
=======
		obj = idr_find(&file->object_idr, submit->bos[i].handle);
		if (!obj) {
			DRM_ERROR("invalid handle %u at index %u\n", submit->bos[i].handle, i);
>>>>>>> upstream/android-13
			ret = -EINVAL;
			goto out_unlock;
		}

<<<<<<< HEAD
		msm_obj = to_msm_bo(obj);

		if (!list_empty(&msm_obj->submit_entry)) {
			DRM_ERROR("handle %u at index %u already on submit list\n",
					submit_bo.handle, i);
			ret = -EINVAL;
			goto out_unlock;
		}

		drm_gem_object_reference(obj);

		submit->bos[i].obj = msm_obj;

		list_add_tail(&msm_obj->submit_entry, &submit->bo_list);
	}

out_unlock:
	pagefault_enable();
=======
		drm_gem_object_get(obj);

		submit->bos[i].obj = to_msm_bo(obj);
	}

out_unlock:
>>>>>>> upstream/android-13
	spin_unlock(&file->table_lock);

out:
	submit->nr_bos = i;

	return ret;
}

<<<<<<< HEAD
static void submit_unlock_unpin_bo(struct msm_gem_submit *submit,
		int i, bool backoff)
{
	struct msm_gem_object *msm_obj = submit->bos[i].obj;

	if (submit->bos[i].flags & BO_PINNED)
		msm_gem_put_iova(&msm_obj->base, submit->gpu->aspace);

	if (submit->bos[i].flags & BO_LOCKED)
		ww_mutex_unlock(&msm_obj->resv->lock);

	if (backoff && !(submit->bos[i].flags & BO_VALID))
		submit->bos[i].iova = 0;

	submit->bos[i].flags &= ~(BO_LOCKED | BO_PINNED);
=======
static int submit_lookup_cmds(struct msm_gem_submit *submit,
		struct drm_msm_gem_submit *args, struct drm_file *file)
{
	unsigned i;
	size_t sz;
	int ret = 0;

	for (i = 0; i < args->nr_cmds; i++) {
		struct drm_msm_gem_submit_cmd submit_cmd;
		void __user *userptr =
			u64_to_user_ptr(args->cmds + (i * sizeof(submit_cmd)));

		ret = copy_from_user(&submit_cmd, userptr, sizeof(submit_cmd));
		if (ret) {
			ret = -EFAULT;
			goto out;
		}

		/* validate input from userspace: */
		switch (submit_cmd.type) {
		case MSM_SUBMIT_CMD_BUF:
		case MSM_SUBMIT_CMD_IB_TARGET_BUF:
		case MSM_SUBMIT_CMD_CTX_RESTORE_BUF:
			break;
		default:
			DRM_ERROR("invalid type: %08x\n", submit_cmd.type);
			return -EINVAL;
		}

		if (submit_cmd.size % 4) {
			DRM_ERROR("non-aligned cmdstream buffer size: %u\n",
					submit_cmd.size);
			ret = -EINVAL;
			goto out;
		}

		submit->cmd[i].type = submit_cmd.type;
		submit->cmd[i].size = submit_cmd.size / 4;
		submit->cmd[i].offset = submit_cmd.submit_offset / 4;
		submit->cmd[i].idx  = submit_cmd.submit_idx;
		submit->cmd[i].nr_relocs = submit_cmd.nr_relocs;

		userptr = u64_to_user_ptr(submit_cmd.relocs);

		sz = array_size(submit_cmd.nr_relocs,
				sizeof(struct drm_msm_gem_submit_reloc));
		/* check for overflow: */
		if (sz == SIZE_MAX) {
			ret = -ENOMEM;
			goto out;
		}
		submit->cmd[i].relocs = kmalloc(sz, GFP_KERNEL);
		ret = copy_from_user(submit->cmd[i].relocs, userptr, sz);
		if (ret) {
			ret = -EFAULT;
			goto out;
		}
	}

out:
	return ret;
}

/* Unwind bo state, according to cleanup_flags.  In the success case, only
 * the lock is dropped at the end of the submit (and active/pin ref is dropped
 * later when the submit is retired).
 */
static void submit_cleanup_bo(struct msm_gem_submit *submit, int i,
		unsigned cleanup_flags)
{
	struct drm_gem_object *obj = &submit->bos[i].obj->base;
	unsigned flags = submit->bos[i].flags & cleanup_flags;

	if (flags & BO_PINNED)
		msm_gem_unpin_iova_locked(obj, submit->aspace);

	if (flags & BO_ACTIVE)
		msm_gem_active_put(obj);

	if (flags & BO_LOCKED)
		dma_resv_unlock(obj->resv);

	submit->bos[i].flags &= ~cleanup_flags;
}

static void submit_unlock_unpin_bo(struct msm_gem_submit *submit, int i)
{
	submit_cleanup_bo(submit, i, BO_PINNED | BO_ACTIVE | BO_LOCKED);

	if (!(submit->bos[i].flags & BO_VALID))
		submit->bos[i].iova = 0;
>>>>>>> upstream/android-13
}

/* This is where we make sure all the bo's are reserved and pin'd: */
static int submit_lock_objects(struct msm_gem_submit *submit)
{
	int contended, slow_locked = -1, i, ret = 0;

retry:
	for (i = 0; i < submit->nr_bos; i++) {
		struct msm_gem_object *msm_obj = submit->bos[i].obj;

		if (slow_locked == i)
			slow_locked = -1;

		contended = i;

		if (!(submit->bos[i].flags & BO_LOCKED)) {
<<<<<<< HEAD
			ret = ww_mutex_lock_interruptible(&msm_obj->resv->lock,
					&submit->ticket);
=======
			ret = dma_resv_lock_interruptible(msm_obj->base.resv,
							  &submit->ticket);
>>>>>>> upstream/android-13
			if (ret)
				goto fail;
			submit->bos[i].flags |= BO_LOCKED;
		}
	}

	ww_acquire_done(&submit->ticket);

	return 0;

fail:
<<<<<<< HEAD
	for (; i >= 0; i--)
		submit_unlock_unpin_bo(submit, i, true);

	if (slow_locked > 0)
		submit_unlock_unpin_bo(submit, slow_locked, true);
=======
	if (ret == -EALREADY) {
		DRM_ERROR("handle %u at index %u already on submit list\n",
				submit->bos[i].handle, i);
		ret = -EINVAL;
	}

	for (; i >= 0; i--)
		submit_unlock_unpin_bo(submit, i);

	if (slow_locked > 0)
		submit_unlock_unpin_bo(submit, slow_locked);
>>>>>>> upstream/android-13

	if (ret == -EDEADLK) {
		struct msm_gem_object *msm_obj = submit->bos[contended].obj;
		/* we lost out in a seqno race, lock and retry.. */
<<<<<<< HEAD
		ret = ww_mutex_lock_slow_interruptible(&msm_obj->resv->lock,
				&submit->ticket);
=======
		ret = dma_resv_lock_slow_interruptible(msm_obj->base.resv,
						       &submit->ticket);
>>>>>>> upstream/android-13
		if (!ret) {
			submit->bos[contended].flags |= BO_LOCKED;
			slow_locked = contended;
			goto retry;
		}
<<<<<<< HEAD
=======

		/* Not expecting -EALREADY here, if the bo was already
		 * locked, we should have gotten -EALREADY already from
		 * the dma_resv_lock_interruptable() call.
		 */
		WARN_ON_ONCE(ret == -EALREADY);
>>>>>>> upstream/android-13
	}

	return ret;
}

static int submit_fence_sync(struct msm_gem_submit *submit, bool no_implicit)
{
	int i, ret = 0;

	for (i = 0; i < submit->nr_bos; i++) {
<<<<<<< HEAD
		struct msm_gem_object *msm_obj = submit->bos[i].obj;
=======
		struct drm_gem_object *obj = &submit->bos[i].obj->base;
>>>>>>> upstream/android-13
		bool write = submit->bos[i].flags & MSM_SUBMIT_BO_WRITE;

		if (!write) {
			/* NOTE: _reserve_shared() must happen before
			 * _add_shared_fence(), which makes this a slightly
			 * strange place to call it.  OTOH this is a
			 * convenient can-fail point to hook it in.
			 */
<<<<<<< HEAD
			ret = reservation_object_reserve_shared(msm_obj->resv);
=======
			ret = dma_resv_reserve_shared(obj->resv, 1);
>>>>>>> upstream/android-13
			if (ret)
				return ret;
		}

		if (no_implicit)
			continue;

<<<<<<< HEAD
		ret = msm_gem_sync_object(&msm_obj->base, submit->ring->fctx,
=======
		ret = drm_gem_fence_array_add_implicit(&submit->deps, obj,
>>>>>>> upstream/android-13
			write);
		if (ret)
			break;
	}

	return ret;
}

static int submit_pin_objects(struct msm_gem_submit *submit)
{
	int i, ret = 0;

	submit->valid = true;

<<<<<<< HEAD
	for (i = 0; i < submit->nr_bos; i++) {
		struct msm_gem_object *msm_obj = submit->bos[i].obj;
		uint64_t iova;

		/* if locking succeeded, pin bo: */
		ret = msm_gem_get_iova(&msm_obj->base,
				submit->gpu->aspace, &iova);
=======
	/*
	 * Increment active_count first, so if under memory pressure, we
	 * don't inadvertently evict a bo needed by the submit in order
	 * to pin an earlier bo in the same submit.
	 */
	for (i = 0; i < submit->nr_bos; i++) {
		struct drm_gem_object *obj = &submit->bos[i].obj->base;

		msm_gem_active_get(obj, submit->gpu);
		submit->bos[i].flags |= BO_ACTIVE;
	}

	for (i = 0; i < submit->nr_bos; i++) {
		struct drm_gem_object *obj = &submit->bos[i].obj->base;
		uint64_t iova;

		/* if locking succeeded, pin bo: */
		ret = msm_gem_get_and_pin_iova_locked(obj,
				submit->aspace, &iova);
>>>>>>> upstream/android-13

		if (ret)
			break;

		submit->bos[i].flags |= BO_PINNED;

		if (iova == submit->bos[i].iova) {
			submit->bos[i].flags |= BO_VALID;
		} else {
			submit->bos[i].iova = iova;
			/* iova changed, so address in cmdstream is not valid: */
			submit->bos[i].flags &= ~BO_VALID;
			submit->valid = false;
		}
	}

	return ret;
}

<<<<<<< HEAD
=======
static void submit_attach_object_fences(struct msm_gem_submit *submit)
{
	int i;

	for (i = 0; i < submit->nr_bos; i++) {
		struct drm_gem_object *obj = &submit->bos[i].obj->base;

		if (submit->bos[i].flags & MSM_SUBMIT_BO_WRITE)
			dma_resv_add_excl_fence(obj->resv, submit->user_fence);
		else if (submit->bos[i].flags & MSM_SUBMIT_BO_READ)
			dma_resv_add_shared_fence(obj->resv, submit->user_fence);
	}
}

>>>>>>> upstream/android-13
static int submit_bo(struct msm_gem_submit *submit, uint32_t idx,
		struct msm_gem_object **obj, uint64_t *iova, bool *valid)
{
	if (idx >= submit->nr_bos) {
		DRM_ERROR("invalid buffer index: %u (out of %u)\n",
				idx, submit->nr_bos);
		return -EINVAL;
	}

	if (obj)
		*obj = submit->bos[idx].obj;
	if (iova)
		*iova = submit->bos[idx].iova;
	if (valid)
		*valid = !!(submit->bos[idx].flags & BO_VALID);

	return 0;
}

/* process the reloc's and patch up the cmdstream as needed: */
static int submit_reloc(struct msm_gem_submit *submit, struct msm_gem_object *obj,
<<<<<<< HEAD
		uint32_t offset, uint32_t nr_relocs, uint64_t relocs)
=======
		uint32_t offset, uint32_t nr_relocs, struct drm_msm_gem_submit_reloc *relocs)
>>>>>>> upstream/android-13
{
	uint32_t i, last_offset = 0;
	uint32_t *ptr;
	int ret = 0;

<<<<<<< HEAD
=======
	if (!nr_relocs)
		return 0;

>>>>>>> upstream/android-13
	if (offset % 4) {
		DRM_ERROR("non-aligned cmdstream buffer: %u\n", offset);
		return -EINVAL;
	}

	/* For now, just map the entire thing.  Eventually we probably
	 * to do it page-by-page, w/ kmap() if not vmap()d..
	 */
<<<<<<< HEAD
	ptr = msm_gem_get_vaddr(&obj->base);
=======
	ptr = msm_gem_get_vaddr_locked(&obj->base);
>>>>>>> upstream/android-13

	if (IS_ERR(ptr)) {
		ret = PTR_ERR(ptr);
		DBG("failed to map: %d", ret);
		return ret;
	}

	for (i = 0; i < nr_relocs; i++) {
<<<<<<< HEAD
		struct drm_msm_gem_submit_reloc submit_reloc;
		void __user *userptr =
			u64_to_user_ptr(relocs + (i * sizeof(submit_reloc)));
=======
		struct drm_msm_gem_submit_reloc submit_reloc = relocs[i];
>>>>>>> upstream/android-13
		uint32_t off;
		uint64_t iova;
		bool valid;

<<<<<<< HEAD
		if (copy_from_user(&submit_reloc, userptr, sizeof(submit_reloc))) {
			ret = -EFAULT;
			goto out;
		}

=======
>>>>>>> upstream/android-13
		if (submit_reloc.submit_offset % 4) {
			DRM_ERROR("non-aligned reloc offset: %u\n",
					submit_reloc.submit_offset);
			ret = -EINVAL;
			goto out;
		}

		/* offset in dwords: */
		off = submit_reloc.submit_offset / 4;

		if ((off >= (obj->base.size / 4)) ||
				(off < last_offset)) {
			DRM_ERROR("invalid offset %u at reloc %u\n", off, i);
			ret = -EINVAL;
			goto out;
		}

		ret = submit_bo(submit, submit_reloc.reloc_idx, NULL, &iova, &valid);
		if (ret)
			goto out;

		if (valid)
			continue;

		iova += submit_reloc.reloc_offset;

		if (submit_reloc.shift < 0)
			iova >>= -submit_reloc.shift;
		else
			iova <<= submit_reloc.shift;

		ptr[off] = iova | submit_reloc.or;

		last_offset = off;
	}

out:
<<<<<<< HEAD
	msm_gem_put_vaddr(&obj->base);
=======
	msm_gem_put_vaddr_locked(&obj->base);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static void submit_cleanup(struct msm_gem_submit *submit)
{
	unsigned i;

	for (i = 0; i < submit->nr_bos; i++) {
		struct msm_gem_object *msm_obj = submit->bos[i].obj;
		submit_unlock_unpin_bo(submit, i, false);
		list_del_init(&msm_obj->submit_entry);
		drm_gem_object_unreference(&msm_obj->base);
	}

	ww_acquire_fini(&submit->ticket);
=======
/* Cleanup submit at end of ioctl.  In the error case, this also drops
 * references, unpins, and drops active refcnt.  In the non-error case,
 * this is done when the submit is retired.
 */
static void submit_cleanup(struct msm_gem_submit *submit, bool error)
{
	unsigned cleanup_flags = BO_LOCKED;
	unsigned i;

	if (error)
		cleanup_flags |= BO_PINNED | BO_ACTIVE;

	for (i = 0; i < submit->nr_bos; i++) {
		struct msm_gem_object *msm_obj = submit->bos[i].obj;
		submit_cleanup_bo(submit, i, cleanup_flags);
		if (error)
			drm_gem_object_put(&msm_obj->base);
	}
}

void msm_submit_retire(struct msm_gem_submit *submit)
{
	int i;

	for (i = 0; i < submit->nr_bos; i++) {
		struct drm_gem_object *obj = &submit->bos[i].obj->base;

		msm_gem_lock(obj);
		submit_cleanup_bo(submit, i, BO_PINNED | BO_ACTIVE);
		msm_gem_unlock(obj);
		drm_gem_object_put(obj);
	}
}

struct msm_submit_post_dep {
	struct drm_syncobj *syncobj;
	uint64_t point;
	struct dma_fence_chain *chain;
};

static struct drm_syncobj **msm_parse_deps(struct msm_gem_submit *submit,
                                           struct drm_file *file,
                                           uint64_t in_syncobjs_addr,
                                           uint32_t nr_in_syncobjs,
                                           size_t syncobj_stride,
                                           struct msm_ringbuffer *ring)
{
	struct drm_syncobj **syncobjs = NULL;
	struct drm_msm_gem_submit_syncobj syncobj_desc = {0};
	int ret = 0;
	uint32_t i, j;

	syncobjs = kcalloc(nr_in_syncobjs, sizeof(*syncobjs),
	                   GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY);
	if (!syncobjs)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < nr_in_syncobjs; ++i) {
		uint64_t address = in_syncobjs_addr + i * syncobj_stride;
		struct dma_fence *fence;

		if (copy_from_user(&syncobj_desc,
			           u64_to_user_ptr(address),
			           min(syncobj_stride, sizeof(syncobj_desc)))) {
			ret = -EFAULT;
			break;
		}

		if (syncobj_desc.point &&
		    !drm_core_check_feature(submit->dev, DRIVER_SYNCOBJ_TIMELINE)) {
			ret = -EOPNOTSUPP;
			break;
		}

		if (syncobj_desc.flags & ~MSM_SUBMIT_SYNCOBJ_FLAGS) {
			ret = -EINVAL;
			break;
		}

		ret = drm_syncobj_find_fence(file, syncobj_desc.handle,
		                             syncobj_desc.point, 0, &fence);
		if (ret)
			break;

		ret = drm_gem_fence_array_add(&submit->deps, fence);
		if (ret)
			break;

		if (syncobj_desc.flags & MSM_SUBMIT_SYNCOBJ_RESET) {
			syncobjs[i] =
				drm_syncobj_find(file, syncobj_desc.handle);
			if (!syncobjs[i]) {
				ret = -EINVAL;
				break;
			}
		}
	}

	if (ret) {
		for (j = 0; j <= i; ++j) {
			if (syncobjs[j])
				drm_syncobj_put(syncobjs[j]);
		}
		kfree(syncobjs);
		return ERR_PTR(ret);
	}
	return syncobjs;
}

static void msm_reset_syncobjs(struct drm_syncobj **syncobjs,
                               uint32_t nr_syncobjs)
{
	uint32_t i;

	for (i = 0; syncobjs && i < nr_syncobjs; ++i) {
		if (syncobjs[i])
			drm_syncobj_replace_fence(syncobjs[i], NULL);
	}
}

static struct msm_submit_post_dep *msm_parse_post_deps(struct drm_device *dev,
                                                       struct drm_file *file,
                                                       uint64_t syncobjs_addr,
                                                       uint32_t nr_syncobjs,
                                                       size_t syncobj_stride)
{
	struct msm_submit_post_dep *post_deps;
	struct drm_msm_gem_submit_syncobj syncobj_desc = {0};
	int ret = 0;
	uint32_t i, j;

	post_deps = kmalloc_array(nr_syncobjs, sizeof(*post_deps),
	                          GFP_KERNEL | __GFP_NOWARN | __GFP_NORETRY);
	if (!post_deps)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < nr_syncobjs; ++i) {
		uint64_t address = syncobjs_addr + i * syncobj_stride;

		if (copy_from_user(&syncobj_desc,
			           u64_to_user_ptr(address),
			           min(syncobj_stride, sizeof(syncobj_desc)))) {
			ret = -EFAULT;
			break;
		}

		post_deps[i].point = syncobj_desc.point;
		post_deps[i].chain = NULL;

		if (syncobj_desc.flags) {
			ret = -EINVAL;
			break;
		}

		if (syncobj_desc.point) {
			if (!drm_core_check_feature(dev,
			                            DRIVER_SYNCOBJ_TIMELINE)) {
				ret = -EOPNOTSUPP;
				break;
			}

			post_deps[i].chain = dma_fence_chain_alloc();
			if (!post_deps[i].chain) {
				ret = -ENOMEM;
				break;
			}
		}

		post_deps[i].syncobj =
			drm_syncobj_find(file, syncobj_desc.handle);
		if (!post_deps[i].syncobj) {
			ret = -EINVAL;
			break;
		}
	}

	if (ret) {
		for (j = 0; j <= i; ++j) {
			dma_fence_chain_free(post_deps[j].chain);
			if (post_deps[j].syncobj)
				drm_syncobj_put(post_deps[j].syncobj);
		}

		kfree(post_deps);
		return ERR_PTR(ret);
	}

	return post_deps;
}

static void msm_process_post_deps(struct msm_submit_post_dep *post_deps,
                                  uint32_t count, struct dma_fence *fence)
{
	uint32_t i;

	for (i = 0; post_deps && i < count; ++i) {
		if (post_deps[i].chain) {
			drm_syncobj_add_point(post_deps[i].syncobj,
			                      post_deps[i].chain,
			                      fence, post_deps[i].point);
			post_deps[i].chain = NULL;
		} else {
			drm_syncobj_replace_fence(post_deps[i].syncobj,
			                          fence);
		}
	}
>>>>>>> upstream/android-13
}

int msm_ioctl_gem_submit(struct drm_device *dev, void *data,
		struct drm_file *file)
{
<<<<<<< HEAD
	struct msm_drm_private *priv = dev->dev_private;
	struct drm_msm_gem_submit *args = data;
	struct msm_file_private *ctx = file->driver_priv;
	struct msm_gem_submit *submit;
	struct msm_gpu *gpu = priv->gpu;
	struct sync_file *sync_file = NULL;
	struct msm_gpu_submitqueue *queue;
	struct msm_ringbuffer *ring;
	int out_fence_fd = -1;
	unsigned i;
	int ret;
=======
	static atomic_t ident = ATOMIC_INIT(0);
	struct msm_drm_private *priv = dev->dev_private;
	struct drm_msm_gem_submit *args = data;
	struct msm_file_private *ctx = file->driver_priv;
	struct msm_gem_submit *submit = NULL;
	struct msm_gpu *gpu = priv->gpu;
	struct msm_gpu_submitqueue *queue;
	struct msm_ringbuffer *ring;
	struct msm_submit_post_dep *post_deps = NULL;
	struct drm_syncobj **syncobjs_to_reset = NULL;
	int out_fence_fd = -1;
	struct pid *pid = get_pid(task_pid(current));
	bool has_ww_ticket = false;
	unsigned i;
	int ret, submitid;
>>>>>>> upstream/android-13

	if (!gpu)
		return -ENXIO;

<<<<<<< HEAD
=======
	if (args->pad)
		return -EINVAL;

>>>>>>> upstream/android-13
	/* for now, we just have 3d pipe.. eventually this would need to
	 * be more clever to dispatch to appropriate gpu module:
	 */
	if (MSM_PIPE_ID(args->flags) != MSM_PIPE_3D0)
		return -EINVAL;

	if (MSM_PIPE_FLAGS(args->flags) & ~MSM_SUBMIT_FLAGS)
		return -EINVAL;

	if (args->flags & MSM_SUBMIT_SUDO) {
		if (!IS_ENABLED(CONFIG_DRM_MSM_GPU_SUDO) ||
		    !capable(CAP_SYS_RAWIO))
			return -EINVAL;
	}

	queue = msm_submitqueue_get(ctx, args->queueid);
	if (!queue)
		return -ENOENT;

<<<<<<< HEAD
	ring = gpu->rb[queue->prio];

	if (args->flags & MSM_SUBMIT_FENCE_FD_IN) {
		struct dma_fence *in_fence;

		in_fence = sync_file_get_fence(args->fence_fd);

		if (!in_fence)
			return -EINVAL;

		/*
		 * Wait if the fence is from a foreign context, or if the fence
		 * array contains any fence from a foreign context.
		 */
		ret = 0;
		if (!dma_fence_match_context(in_fence, ring->fctx->context))
			ret = dma_fence_wait(in_fence, true);

		dma_fence_put(in_fence);
		if (ret)
			return ret;
	}

	ret = mutex_lock_interruptible(&dev->struct_mutex);
	if (ret)
		return ret;
=======
	/* Get a unique identifier for the submission for logging purposes */
	submitid = atomic_inc_return(&ident) - 1;

	ring = gpu->rb[queue->ring_nr];
	trace_msm_gpu_submit(pid_nr(pid), ring->id, submitid,
		args->nr_bos, args->nr_cmds);

	ret = mutex_lock_interruptible(&queue->lock);
	if (ret)
		goto out_post_unlock;
>>>>>>> upstream/android-13

	if (args->flags & MSM_SUBMIT_FENCE_FD_OUT) {
		out_fence_fd = get_unused_fd_flags(O_CLOEXEC);
		if (out_fence_fd < 0) {
			ret = out_fence_fd;
			goto out_unlock;
		}
	}

<<<<<<< HEAD
	submit = submit_create(dev, gpu, queue, args->nr_bos, args->nr_cmds);
	if (!submit) {
		ret = -ENOMEM;
		goto out_unlock;
	}

	if (args->flags & MSM_SUBMIT_SUDO)
		submit->in_rb = true;

=======
	submit = submit_create(dev, gpu, queue, args->nr_bos,
		args->nr_cmds);
	if (IS_ERR(submit)) {
		ret = PTR_ERR(submit);
		submit = NULL;
		goto out_unlock;
	}

	submit->pid = pid;
	submit->ident = submitid;

	if (args->flags & MSM_SUBMIT_SUDO)
		submit->in_rb = true;

	if (args->flags & MSM_SUBMIT_FENCE_FD_IN) {
		struct dma_fence *in_fence;

		in_fence = sync_file_get_fence(args->fence_fd);

		if (!in_fence) {
			ret = -EINVAL;
			goto out_unlock;
		}

		ret = drm_gem_fence_array_add(&submit->deps, in_fence);
		if (ret)
			goto out_unlock;
	}

	if (args->flags & MSM_SUBMIT_SYNCOBJ_IN) {
		syncobjs_to_reset = msm_parse_deps(submit, file,
		                                   args->in_syncobjs,
		                                   args->nr_in_syncobjs,
		                                   args->syncobj_stride, ring);
		if (IS_ERR(syncobjs_to_reset)) {
			ret = PTR_ERR(syncobjs_to_reset);
			goto out_unlock;
		}
	}

	if (args->flags & MSM_SUBMIT_SYNCOBJ_OUT) {
		post_deps = msm_parse_post_deps(dev, file,
		                                args->out_syncobjs,
		                                args->nr_out_syncobjs,
		                                args->syncobj_stride);
		if (IS_ERR(post_deps)) {
			ret = PTR_ERR(post_deps);
			goto out_unlock;
		}
	}

>>>>>>> upstream/android-13
	ret = submit_lookup_objects(submit, args, file);
	if (ret)
		goto out;

<<<<<<< HEAD
=======
	ret = submit_lookup_cmds(submit, args, file);
	if (ret)
		goto out;

	/* copy_*_user while holding a ww ticket upsets lockdep */
	ww_acquire_init(&submit->ticket, &reservation_ww_class);
	has_ww_ticket = true;
>>>>>>> upstream/android-13
	ret = submit_lock_objects(submit);
	if (ret)
		goto out;

	ret = submit_fence_sync(submit, !!(args->flags & MSM_SUBMIT_NO_IMPLICIT));
	if (ret)
		goto out;

	ret = submit_pin_objects(submit);
	if (ret)
		goto out;

	for (i = 0; i < args->nr_cmds; i++) {
<<<<<<< HEAD
		struct drm_msm_gem_submit_cmd submit_cmd;
		void __user *userptr =
			u64_to_user_ptr(args->cmds + (i * sizeof(submit_cmd)));
		struct msm_gem_object *msm_obj;
		uint64_t iova;

		ret = copy_from_user(&submit_cmd, userptr, sizeof(submit_cmd));
		if (ret) {
			ret = -EFAULT;
			goto out;
		}

		/* validate input from userspace: */
		switch (submit_cmd.type) {
		case MSM_SUBMIT_CMD_BUF:
		case MSM_SUBMIT_CMD_IB_TARGET_BUF:
		case MSM_SUBMIT_CMD_CTX_RESTORE_BUF:
			break;
		default:
			DRM_ERROR("invalid type: %08x\n", submit_cmd.type);
			ret = -EINVAL;
			goto out;
		}

		ret = submit_bo(submit, submit_cmd.submit_idx,
=======
		struct msm_gem_object *msm_obj;
		uint64_t iova;

		ret = submit_bo(submit, submit->cmd[i].idx,
>>>>>>> upstream/android-13
				&msm_obj, &iova, NULL);
		if (ret)
			goto out;

<<<<<<< HEAD
		if (submit_cmd.size % 4) {
			DRM_ERROR("non-aligned cmdstream buffer size: %u\n",
					submit_cmd.size);
=======
		if (!submit->cmd[i].size ||
			((submit->cmd[i].size + submit->cmd[i].offset) >
				msm_obj->base.size / 4)) {
			DRM_ERROR("invalid cmdstream size: %u\n", submit->cmd[i].size * 4);
>>>>>>> upstream/android-13
			ret = -EINVAL;
			goto out;
		}

<<<<<<< HEAD
		if (!submit_cmd.size ||
			((submit_cmd.size + submit_cmd.submit_offset) >
				msm_obj->base.size)) {
			DRM_ERROR("invalid cmdstream size: %u\n", submit_cmd.size);
			ret = -EINVAL;
			goto out;
		}

		submit->cmd[i].type = submit_cmd.type;
		submit->cmd[i].size = submit_cmd.size / 4;
		submit->cmd[i].iova = iova + submit_cmd.submit_offset;
		submit->cmd[i].idx  = submit_cmd.submit_idx;
=======
		submit->cmd[i].iova = iova + (submit->cmd[i].offset * 4);
>>>>>>> upstream/android-13

		if (submit->valid)
			continue;

<<<<<<< HEAD
		ret = submit_reloc(submit, msm_obj, submit_cmd.submit_offset,
				submit_cmd.nr_relocs, submit_cmd.relocs);
=======
		ret = submit_reloc(submit, msm_obj, submit->cmd[i].offset * 4,
				submit->cmd[i].nr_relocs, submit->cmd[i].relocs);
>>>>>>> upstream/android-13
		if (ret)
			goto out;
	}

	submit->nr_cmds = i;

<<<<<<< HEAD
	submit->fence = msm_fence_alloc(ring->fctx);
	if (IS_ERR(submit->fence)) {
		ret = PTR_ERR(submit->fence);
		submit->fence = NULL;
=======
	submit->user_fence = dma_fence_get(&submit->base.s_fence->finished);

	/*
	 * Allocate an id which can be used by WAIT_FENCE ioctl to map back
	 * to the underlying fence.
	 */
	submit->fence_id = idr_alloc_cyclic(&queue->fence_idr,
			submit->user_fence, 1, INT_MAX, GFP_KERNEL);
	if (submit->fence_id < 0) {
		ret = submit->fence_id = 0;
		submit->fence_id = 0;
>>>>>>> upstream/android-13
		goto out;
	}

	if (args->flags & MSM_SUBMIT_FENCE_FD_OUT) {
<<<<<<< HEAD
		sync_file = sync_file_create(submit->fence);
=======
		struct sync_file *sync_file = sync_file_create(submit->user_fence);
>>>>>>> upstream/android-13
		if (!sync_file) {
			ret = -ENOMEM;
			goto out;
		}
<<<<<<< HEAD
	}

	msm_gpu_submit(gpu, submit, ctx);

	args->fence = submit->fence->seqno;

	if (args->flags & MSM_SUBMIT_FENCE_FD_OUT) {
=======
>>>>>>> upstream/android-13
		fd_install(out_fence_fd, sync_file->file);
		args->fence_fd = out_fence_fd;
	}

<<<<<<< HEAD
out:
	submit_cleanup(submit);
	if (ret)
		msm_gem_submit_free(submit);
out_unlock:
	if (ret && (out_fence_fd >= 0))
		put_unused_fd(out_fence_fd);
	mutex_unlock(&dev->struct_mutex);
=======
	submit_attach_object_fences(submit);

	/* The scheduler owns a ref now: */
	msm_gem_submit_get(submit);

	drm_sched_entity_push_job(&submit->base, queue->entity);

	args->fence = submit->fence_id;
	queue->last_fence = submit->fence_id;

	msm_reset_syncobjs(syncobjs_to_reset, args->nr_in_syncobjs);
	msm_process_post_deps(post_deps, args->nr_out_syncobjs,
	                      submit->user_fence);


out:
	submit_cleanup(submit, !!ret);
	if (has_ww_ticket)
		ww_acquire_fini(&submit->ticket);
out_unlock:
	if (ret && (out_fence_fd >= 0))
		put_unused_fd(out_fence_fd);
	mutex_unlock(&queue->lock);
	if (submit)
		msm_gem_submit_put(submit);
out_post_unlock:
	if (!IS_ERR_OR_NULL(post_deps)) {
		for (i = 0; i < args->nr_out_syncobjs; ++i) {
			kfree(post_deps[i].chain);
			drm_syncobj_put(post_deps[i].syncobj);
		}
		kfree(post_deps);
	}

	if (!IS_ERR_OR_NULL(syncobjs_to_reset)) {
		for (i = 0; i < args->nr_in_syncobjs; ++i) {
			if (syncobjs_to_reset[i])
				drm_syncobj_put(syncobjs_to_reset[i]);
		}
		kfree(syncobjs_to_reset);
	}

>>>>>>> upstream/android-13
	return ret;
}
