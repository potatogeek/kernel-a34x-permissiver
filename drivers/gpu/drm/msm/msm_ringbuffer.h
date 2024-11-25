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
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef __MSM_RINGBUFFER_H__
#define __MSM_RINGBUFFER_H__

<<<<<<< HEAD
=======
#include "drm/gpu_scheduler.h"
>>>>>>> upstream/android-13
#include "msm_drv.h"

#define rbmemptr(ring, member)  \
	((ring)->memptrs_iova + offsetof(struct msm_rbmemptrs, member))

<<<<<<< HEAD
struct msm_rbmemptrs {
	volatile uint32_t rptr;
	volatile uint32_t fence;
=======
#define rbmemptr_stats(ring, index, member) \
	(rbmemptr((ring), stats) + \
	 ((index) * sizeof(struct msm_gpu_submit_stats)) + \
	 offsetof(struct msm_gpu_submit_stats, member))

struct msm_gpu_submit_stats {
	u64 cpcycles_start;
	u64 cpcycles_end;
	u64 alwayson_start;
	u64 alwayson_end;
};

#define MSM_GPU_SUBMIT_STATS_COUNT 64

struct msm_rbmemptrs {
	volatile uint32_t rptr;
	volatile uint32_t fence;

	volatile struct msm_gpu_submit_stats stats[MSM_GPU_SUBMIT_STATS_COUNT];
	volatile u64 ttbr0;
>>>>>>> upstream/android-13
};

struct msm_ringbuffer {
	struct msm_gpu *gpu;
	int id;
	struct drm_gem_object *bo;
	uint32_t *start, *end, *cur, *next;
<<<<<<< HEAD
	struct list_head submits;
=======

	/*
	 * The job scheduler for this ring.
	 */
	struct drm_gpu_scheduler sched;

	/*
	 * List of in-flight submits on this ring.  Protected by submit_lock.
	 *
	 * Currently just submits that are already written into the ring, not
	 * submits that are still in drm_gpu_scheduler's queues.  At a later
	 * step we could probably move to letting drm_gpu_scheduler manage
	 * hangcheck detection and keep track of submit jobs that are in-
	 * flight.
	 */
	struct list_head submits;
	spinlock_t submit_lock;

>>>>>>> upstream/android-13
	uint64_t iova;
	uint32_t seqno;
	uint32_t hangcheck_fence;
	struct msm_rbmemptrs *memptrs;
	uint64_t memptrs_iova;
	struct msm_fence_context *fctx;
<<<<<<< HEAD
	spinlock_t lock;
=======

	/*
	 * preempt_lock protects preemption and serializes wptr updates against
	 * preemption.  Can be aquired from irq context.
	 */
	spinlock_t preempt_lock;
>>>>>>> upstream/android-13
};

struct msm_ringbuffer *msm_ringbuffer_new(struct msm_gpu *gpu, int id,
		void *memptrs, uint64_t memptrs_iova);
void msm_ringbuffer_destroy(struct msm_ringbuffer *ring);

/* ringbuffer helpers (the parts that are same for a3xx/a2xx/z180..) */

static inline void
OUT_RING(struct msm_ringbuffer *ring, uint32_t data)
{
	/*
	 * ring->next points to the current command being written - it won't be
	 * committed as ring->cur until the flush
	 */
	if (ring->next == ring->end)
		ring->next = ring->start;
	*(ring->next++) = data;
}

#endif /* __MSM_RINGBUFFER_H__ */
