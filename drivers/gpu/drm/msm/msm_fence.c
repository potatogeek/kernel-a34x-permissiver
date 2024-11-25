<<<<<<< HEAD
/*
 * Copyright (C) 2013-2016 Red Hat
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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013-2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

#include <linux/dma-fence.h>

#include "msm_drv.h"
#include "msm_fence.h"


struct msm_fence_context *
<<<<<<< HEAD
msm_fence_context_alloc(struct drm_device *dev, const char *name)
=======
msm_fence_context_alloc(struct drm_device *dev, volatile uint32_t *fenceptr,
		const char *name)
>>>>>>> upstream/android-13
{
	struct msm_fence_context *fctx;

	fctx = kzalloc(sizeof(*fctx), GFP_KERNEL);
	if (!fctx)
		return ERR_PTR(-ENOMEM);

	fctx->dev = dev;
	strncpy(fctx->name, name, sizeof(fctx->name));
	fctx->context = dma_fence_context_alloc(1);
<<<<<<< HEAD
	init_waitqueue_head(&fctx->event);
=======
	fctx->fenceptr = fenceptr;
>>>>>>> upstream/android-13
	spin_lock_init(&fctx->spinlock);

	return fctx;
}

void msm_fence_context_free(struct msm_fence_context *fctx)
{
	kfree(fctx);
}

static inline bool fence_completed(struct msm_fence_context *fctx, uint32_t fence)
{
<<<<<<< HEAD
	return (int32_t)(fctx->completed_fence - fence) >= 0;
}

/* legacy path for WAIT_FENCE ioctl: */
int msm_wait_fence(struct msm_fence_context *fctx, uint32_t fence,
		ktime_t *timeout, bool interruptible)
{
	int ret;

	if (fence > fctx->last_fence) {
		DRM_ERROR_RATELIMITED("%s: waiting on invalid fence: %u (of %u)\n",
				fctx->name, fence, fctx->last_fence);
		return -EINVAL;
	}

	if (!timeout) {
		/* no-wait: */
		ret = fence_completed(fctx, fence) ? 0 : -EBUSY;
	} else {
		unsigned long remaining_jiffies = timeout_to_jiffies(timeout);

		if (interruptible)
			ret = wait_event_interruptible_timeout(fctx->event,
				fence_completed(fctx, fence),
				remaining_jiffies);
		else
			ret = wait_event_timeout(fctx->event,
				fence_completed(fctx, fence),
				remaining_jiffies);

		if (ret == 0) {
			DBG("timeout waiting for fence: %u (completed: %u)",
					fence, fctx->completed_fence);
			ret = -ETIMEDOUT;
		} else if (ret != -ERESTARTSYS) {
			ret = 0;
		}
	}

	return ret;
=======
	/*
	 * Note: Check completed_fence first, as fenceptr is in a write-combine
	 * mapping, so it will be more expensive to read.
	 */
	return (int32_t)(fctx->completed_fence - fence) >= 0 ||
		(int32_t)(*fctx->fenceptr - fence) >= 0;
>>>>>>> upstream/android-13
}

/* called from workqueue */
void msm_update_fence(struct msm_fence_context *fctx, uint32_t fence)
{
	spin_lock(&fctx->spinlock);
	fctx->completed_fence = max(fence, fctx->completed_fence);
	spin_unlock(&fctx->spinlock);
<<<<<<< HEAD

	wake_up_all(&fctx->event);
=======
>>>>>>> upstream/android-13
}

struct msm_fence {
	struct dma_fence base;
	struct msm_fence_context *fctx;
};

static inline struct msm_fence *to_msm_fence(struct dma_fence *fence)
{
	return container_of(fence, struct msm_fence, base);
}

static const char *msm_fence_get_driver_name(struct dma_fence *fence)
{
	return "msm";
}

static const char *msm_fence_get_timeline_name(struct dma_fence *fence)
{
	struct msm_fence *f = to_msm_fence(fence);
	return f->fctx->name;
}

<<<<<<< HEAD
static bool msm_fence_enable_signaling(struct dma_fence *fence)
{
	return true;
}

=======
>>>>>>> upstream/android-13
static bool msm_fence_signaled(struct dma_fence *fence)
{
	struct msm_fence *f = to_msm_fence(fence);
	return fence_completed(f->fctx, f->base.seqno);
}

static const struct dma_fence_ops msm_fence_ops = {
	.get_driver_name = msm_fence_get_driver_name,
	.get_timeline_name = msm_fence_get_timeline_name,
<<<<<<< HEAD
	.enable_signaling = msm_fence_enable_signaling,
	.signaled = msm_fence_signaled,
	.wait = dma_fence_default_wait,
	.release = dma_fence_free,
=======
	.signaled = msm_fence_signaled,
>>>>>>> upstream/android-13
};

struct dma_fence *
msm_fence_alloc(struct msm_fence_context *fctx)
{
	struct msm_fence *f;

	f = kzalloc(sizeof(*f), GFP_KERNEL);
	if (!f)
		return ERR_PTR(-ENOMEM);

	f->fctx = fctx;

	dma_fence_init(&f->base, &msm_fence_ops, &fctx->spinlock,
		       fctx->context, ++fctx->last_fence);

	return &f->base;
}
