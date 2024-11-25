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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2013-2016 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef __MSM_FENCE_H__
#define __MSM_FENCE_H__

#include "msm_drv.h"

<<<<<<< HEAD
struct msm_fence_context {
	struct drm_device *dev;
	char name[32];
	unsigned context;
	/* last_fence == completed_fence --> no pending work */
	uint32_t last_fence;          /* last assigned fence */
	uint32_t completed_fence;     /* last completed fence */
	wait_queue_head_t event;
=======
/**
 * struct msm_fence_context - fence context for gpu
 *
 * Each ringbuffer has a single fence context, with the GPU writing an
 * incrementing fence seqno at the end of each submit
 */
struct msm_fence_context {
	struct drm_device *dev;
	/** name: human readable name for fence timeline */
	char name[32];
	/** context: see dma_fence_context_alloc() */
	unsigned context;

	/**
	 * last_fence:
	 *
	 * Last assigned fence, incremented each time a fence is created
	 * on this fence context.  If last_fence == completed_fence,
	 * there is no remaining pending work
	 */
	uint32_t last_fence;

	/**
	 * completed_fence:
	 *
	 * The last completed fence, updated from the CPU after interrupt
	 * from GPU
	 */
	uint32_t completed_fence;

	/**
	 * fenceptr:
	 *
	 * The address that the GPU directly writes with completed fence
	 * seqno.  This can be ahead of completed_fence.  We can peek at
	 * this to see if a fence has already signaled but the CPU hasn't
	 * gotten around to handling the irq and updating completed_fence
	 */
	volatile uint32_t *fenceptr;

>>>>>>> upstream/android-13
	spinlock_t spinlock;
};

struct msm_fence_context * msm_fence_context_alloc(struct drm_device *dev,
<<<<<<< HEAD
		const char *name);
void msm_fence_context_free(struct msm_fence_context *fctx);

int msm_wait_fence(struct msm_fence_context *fctx, uint32_t fence,
		ktime_t *timeout, bool interruptible);
=======
		volatile uint32_t *fenceptr, const char *name);
void msm_fence_context_free(struct msm_fence_context *fctx);

>>>>>>> upstream/android-13
void msm_update_fence(struct msm_fence_context *fctx, uint32_t fence);

struct dma_fence * msm_fence_alloc(struct msm_fence_context *fctx);

#endif
