/*
 * Copyright © 2008-2018 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#ifndef I915_REQUEST_H
#define I915_REQUEST_H

#include <linux/dma-fence.h>
<<<<<<< HEAD

#include "i915_gem.h"
#include "i915_scheduler.h"
#include "i915_sw_fence.h"
#include "i915_scheduler.h"
=======
#include <linux/hrtimer.h>
#include <linux/irq_work.h>
#include <linux/llist.h>
#include <linux/lockdep.h>

#include "gem/i915_gem_context_types.h"
#include "gt/intel_context_types.h"
#include "gt/intel_engine_types.h"
#include "gt/intel_timeline_types.h"

#include "i915_gem.h"
#include "i915_scheduler.h"
#include "i915_selftest.h"
#include "i915_sw_fence.h"
>>>>>>> upstream/android-13

#include <uapi/drm/i915_drm.h>

struct drm_file;
struct drm_i915_gem_object;
<<<<<<< HEAD
struct i915_request;
struct i915_timeline;

struct intel_wait {
	struct rb_node node;
	struct task_struct *tsk;
	struct i915_request *request;
	u32 seqno;
};

struct intel_signal_node {
	struct intel_wait wait;
	struct list_head link;
};
=======
struct drm_printer;
struct i915_request;
>>>>>>> upstream/android-13

struct i915_capture_list {
	struct i915_capture_list *next;
	struct i915_vma *vma;
};

<<<<<<< HEAD
=======
#define RQ_TRACE(rq, fmt, ...) do {					\
	const struct i915_request *rq__ = (rq);				\
	ENGINE_TRACE(rq__->engine, "fence %llx:%lld, current %d " fmt,	\
		     rq__->fence.context, rq__->fence.seqno,		\
		     hwsp_seqno(rq__), ##__VA_ARGS__);			\
} while (0)

enum {
	/*
	 * I915_FENCE_FLAG_ACTIVE - this request is currently submitted to HW.
	 *
	 * Set by __i915_request_submit() on handing over to HW, and cleared
	 * by __i915_request_unsubmit() if we preempt this request.
	 *
	 * Finally cleared for consistency on retiring the request, when
	 * we know the HW is no longer running this request.
	 *
	 * See i915_request_is_active()
	 */
	I915_FENCE_FLAG_ACTIVE = DMA_FENCE_FLAG_USER_BITS,

	/*
	 * I915_FENCE_FLAG_PQUEUE - this request is ready for execution
	 *
	 * Using the scheduler, when a request is ready for execution it is put
	 * into the priority queue, and removed from that queue when transferred
	 * to the HW runlists. We want to track its membership within the
	 * priority queue so that we can easily check before rescheduling.
	 *
	 * See i915_request_in_priority_queue()
	 */
	I915_FENCE_FLAG_PQUEUE,

	/*
	 * I915_FENCE_FLAG_HOLD - this request is currently on hold
	 *
	 * This request has been suspended, pending an ongoing investigation.
	 */
	I915_FENCE_FLAG_HOLD,

	/*
	 * I915_FENCE_FLAG_INITIAL_BREADCRUMB - this request has the initial
	 * breadcrumb that marks the end of semaphore waits and start of the
	 * user payload.
	 */
	I915_FENCE_FLAG_INITIAL_BREADCRUMB,

	/*
	 * I915_FENCE_FLAG_SIGNAL - this request is currently on signal_list
	 *
	 * Internal bookkeeping used by the breadcrumb code to track when
	 * a request is on the various signal_list.
	 */
	I915_FENCE_FLAG_SIGNAL,

	/*
	 * I915_FENCE_FLAG_NOPREEMPT - this request should not be preempted
	 *
	 * The execution of some requests should not be interrupted. This is
	 * a sensitive operation as it makes the request super important,
	 * blocking other higher priority work. Abuse of this flag will
	 * lead to quality of service issues.
	 */
	I915_FENCE_FLAG_NOPREEMPT,

	/*
	 * I915_FENCE_FLAG_SENTINEL - this request should be last in the queue
	 *
	 * A high priority sentinel request may be submitted to clear the
	 * submission queue. As it will be the only request in-flight, upon
	 * execution all other active requests will have been preempted and
	 * unsubmitted. This preemptive pulse is used to re-evaluate the
	 * in-flight requests, particularly in cases where an active context
	 * is banned and those active requests need to be cancelled.
	 */
	I915_FENCE_FLAG_SENTINEL,

	/*
	 * I915_FENCE_FLAG_BOOST - upclock the gpu for this request
	 *
	 * Some requests are more important than others! In particular, a
	 * request that the user is waiting on is typically required for
	 * interactive latency, for which we want to minimise by upclocking
	 * the GPU. Here we track such boost requests on a per-request basis.
	 */
	I915_FENCE_FLAG_BOOST,
};

>>>>>>> upstream/android-13
/**
 * Request queue structure.
 *
 * The request queue allows us to note sequence numbers that have been emitted
 * and may be associated with active buffers to be retired.
 *
 * By keeping this list, we can avoid having to do questionable sequence
 * number comparisons on buffer last_read|write_seqno. It also allows an
 * emission time to be associated with the request for tracking how far ahead
 * of the GPU the submission is.
 *
 * When modifying this structure be very aware that we perform a lockless
 * RCU lookup of it that may race against reallocation of the struct
 * from the slab freelist. We intentionally do not zero the structure on
 * allocation so that the lookup can use the dangling pointers (and is
 * cogniscent that those pointers may be wrong). Instead, everything that
 * needs to be initialised must be done so explicitly.
 *
 * The requests are reference counted.
 */
struct i915_request {
	struct dma_fence fence;
	spinlock_t lock;

<<<<<<< HEAD
	/** On Which ring this request was generated */
	struct drm_i915_private *i915;

=======
>>>>>>> upstream/android-13
	/**
	 * Context and ring buffer related to this request
	 * Contexts are refcounted, so when this request is associated with a
	 * context, we must increment the context's refcount, to guarantee that
	 * it persists while any request is linked to it. Requests themselves
	 * are also refcounted, so the request will only be freed when the last
	 * reference to it is dismissed, and the code in
	 * i915_request_free() will then decrement the refcount on the
	 * context.
	 */
<<<<<<< HEAD
	struct i915_gem_context *gem_context;
	struct intel_engine_cs *engine;
	struct intel_context *hw_context;
	struct intel_ring *ring;
	struct i915_timeline *timeline;
	struct intel_signal_node signaling;
=======
	struct intel_engine_cs *engine;
	struct intel_context *context;
	struct intel_ring *ring;
	struct intel_timeline __rcu *timeline;

	struct list_head signal_link;
	struct llist_node signal_node;

	/*
	 * The rcu epoch of when this request was allocated. Used to judiciously
	 * apply backpressure on future allocations to ensure that under
	 * mempressure there is sufficient RCU ticks for us to reclaim our
	 * RCU protected slabs.
	 */
	unsigned long rcustate;

	/*
	 * We pin the timeline->mutex while constructing the request to
	 * ensure that no caller accidentally drops it during construction.
	 * The timeline->mutex must be held to ensure that only this caller
	 * can use the ring and manipulate the associated timeline during
	 * construction.
	 */
	struct pin_cookie cookie;
>>>>>>> upstream/android-13

	/*
	 * Fences for the various phases in the request's lifetime.
	 *
	 * The submit fence is used to await upon all of the request's
	 * dependencies. When it is signaled, the request is ready to run.
	 * It is used by the driver to then queue the request for execution.
	 */
	struct i915_sw_fence submit;
<<<<<<< HEAD
	wait_queue_entry_t submitq;
	wait_queue_head_t execute;
=======
	union {
		wait_queue_entry_t submitq;
		struct i915_sw_dma_fence_cb dmaq;
		struct i915_request_duration_cb {
			struct dma_fence_cb cb;
			ktime_t emitted;
		} duration;
	};
	struct llist_head execute_cb;
	struct i915_sw_fence semaphore;
>>>>>>> upstream/android-13

	/*
	 * A list of everyone we wait upon, and everyone who waits upon us.
	 * Even though we will not be submitted to the hardware before the
	 * submit fence is signaled (it waits for all external events as well
	 * as our own requests), the scheduler still needs to know the
	 * dependency tree for the lifetime of the request (from execbuf
	 * to retirement), i.e. bidirectional dependency information for the
	 * request not tied to individual fences.
	 */
	struct i915_sched_node sched;
	struct i915_dependency dep;
<<<<<<< HEAD

	/**
	 * GEM sequence number associated with this request on the
	 * global execution timeline. It is zero when the request is not
	 * on the HW queue (i.e. not on the engine timeline list).
	 * Its value is guarded by the timeline spinlock.
	 */
	u32 global_seqno;
=======
	intel_engine_mask_t execution_mask;

	/*
	 * A convenience pointer to the current breadcrumb value stored in
	 * the HW status page (or our timeline's local equivalent). The full
	 * path would be rq->hw_context->ring->timeline->hwsp_seqno.
	 */
	const u32 *hwsp_seqno;
>>>>>>> upstream/android-13

	/** Position in the ring of the start of the request */
	u32 head;

	/** Position in the ring of the start of the user packets */
	u32 infix;

	/**
	 * Position in the ring of the start of the postfix.
	 * This is required to calculate the maximum available ring space
	 * without overwriting the postfix.
	 */
	u32 postfix;

	/** Position in the ring of the end of the whole request */
	u32 tail;

	/** Position in the ring of the end of any workarounds after the tail */
	u32 wa_tail;

	/** Preallocate space in the ring for the emitting the request */
	u32 reserved_space;

	/** Batch buffer related to this request if any (used for
	 * error state dump only).
	 */
	struct i915_vma *batch;
	/**
	 * Additional buffers requested by userspace to be captured upon
	 * a GPU hang. The vma/obj on this list are protected by their
	 * active reference - all objects on this list must also be
	 * on the active_list (of their final request).
	 */
	struct i915_capture_list *capture_list;
<<<<<<< HEAD
	struct list_head active_list;
=======
>>>>>>> upstream/android-13

	/** Time at which this request was emitted, in jiffies. */
	unsigned long emitted_jiffies;

<<<<<<< HEAD
	bool waitboost;

	/** engine->request_list entry for this request */
	struct list_head link;

	/** ring->request_list entry for this request */
	struct list_head ring_link;

	struct drm_i915_file_private *file_priv;
	/** file_priv list entry for this request */
	struct list_head client_link;
=======
	/** timeline->request entry for this request */
	struct list_head link;

	/** Watchdog support fields. */
	struct i915_request_watchdog {
		struct llist_node link;
		struct hrtimer timer;
	} watchdog;

	/*
	 * Requests may need to be stalled when using GuC submission waiting for
	 * certain GuC operations to complete. If that is the case, stalled
	 * requests are added to a per context list of stalled requests. The
	 * below list_head is the link in that list.
	 */
	struct list_head guc_fence_link;

	/**
	 * Priority level while the request is inflight. Differs from i915
	 * scheduler priority. See comment above
	 * I915_SCHEDULER_CAP_STATIC_PRIORITY_MAP for details.
	 */
#define	GUC_PRIO_INIT	0xff
#define	GUC_PRIO_FINI	0xfe
	u8 guc_prio;

	I915_SELFTEST_DECLARE(struct {
		struct list_head link;
		unsigned long delay;
	} mock;)
>>>>>>> upstream/android-13
};

#define I915_FENCE_GFP (GFP_KERNEL | __GFP_RETRY_MAYFAIL | __GFP_NOWARN)

extern const struct dma_fence_ops i915_fence_ops;

static inline bool dma_fence_is_i915(const struct dma_fence *fence)
{
	return fence->ops == &i915_fence_ops;
}

<<<<<<< HEAD
struct i915_request * __must_check
i915_request_alloc(struct intel_engine_cs *engine,
		   struct i915_gem_context *ctx);
=======
struct kmem_cache *i915_request_slab_cache(void);

struct i915_request * __must_check
__i915_request_create(struct intel_context *ce, gfp_t gfp);
struct i915_request * __must_check
i915_request_create(struct intel_context *ce);

void __i915_request_skip(struct i915_request *rq);
bool i915_request_set_error_once(struct i915_request *rq, int error);
struct i915_request *i915_request_mark_eio(struct i915_request *rq);

struct i915_request *__i915_request_commit(struct i915_request *request);
void __i915_request_queue(struct i915_request *rq,
			  const struct i915_sched_attr *attr);
void __i915_request_queue_bh(struct i915_request *rq);

bool i915_request_retire(struct i915_request *rq);
>>>>>>> upstream/android-13
void i915_request_retire_upto(struct i915_request *rq);

static inline struct i915_request *
to_request(struct dma_fence *fence)
{
	/* We assume that NULL fence/request are interoperable */
	BUILD_BUG_ON(offsetof(struct i915_request, fence) != 0);
	GEM_BUG_ON(fence && !dma_fence_is_i915(fence));
	return container_of(fence, struct i915_request, fence);
}

static inline struct i915_request *
i915_request_get(struct i915_request *rq)
{
	return to_request(dma_fence_get(&rq->fence));
}

static inline struct i915_request *
i915_request_get_rcu(struct i915_request *rq)
{
	return to_request(dma_fence_get_rcu(&rq->fence));
}

static inline void
i915_request_put(struct i915_request *rq)
{
	dma_fence_put(&rq->fence);
}

<<<<<<< HEAD
/**
 * i915_request_global_seqno - report the current global seqno
 * @request - the request
 *
 * A request is assigned a global seqno only when it is on the hardware
 * execution queue. The global seqno can be used to maintain a list of
 * requests on the same engine in retirement order, for example for
 * constructing a priority queue for waiting. Prior to its execution, or
 * if it is subsequently removed in the event of preemption, its global
 * seqno is zero. As both insertion and removal from the execution queue
 * may operate in IRQ context, it is not guarded by the usual struct_mutex
 * BKL. Instead those relying on the global seqno must be prepared for its
 * value to change between reads. Only when the request is complete can
 * the global seqno be stable (due to the memory barriers on submitting
 * the commands to the hardware to write the breadcrumb, if the HWS shows
 * that it has passed the global seqno and the global seqno is unchanged
 * after the read, it is indeed complete).
 */
static u32
i915_request_global_seqno(const struct i915_request *request)
{
	return READ_ONCE(request->global_seqno);
}

=======
>>>>>>> upstream/android-13
int i915_request_await_object(struct i915_request *to,
			      struct drm_i915_gem_object *obj,
			      bool write);
int i915_request_await_dma_fence(struct i915_request *rq,
				 struct dma_fence *fence);
<<<<<<< HEAD

void i915_request_add(struct i915_request *rq);

void __i915_request_submit(struct i915_request *request);
void i915_request_submit(struct i915_request *request);

void i915_request_skip(struct i915_request *request, int error);

void __i915_request_unsubmit(struct i915_request *request);
void i915_request_unsubmit(struct i915_request *request);

=======
int i915_request_await_execution(struct i915_request *rq,
				 struct dma_fence *fence);

void i915_request_add(struct i915_request *rq);

bool __i915_request_submit(struct i915_request *request);
void i915_request_submit(struct i915_request *request);

void __i915_request_unsubmit(struct i915_request *request);
void i915_request_unsubmit(struct i915_request *request);

void i915_request_cancel(struct i915_request *rq, int error);

>>>>>>> upstream/android-13
long i915_request_wait(struct i915_request *rq,
		       unsigned int flags,
		       long timeout)
	__attribute__((nonnull(1)));
#define I915_WAIT_INTERRUPTIBLE	BIT(0)
<<<<<<< HEAD
#define I915_WAIT_LOCKED	BIT(1) /* struct_mutex held, handle GPU reset */
#define I915_WAIT_ALL		BIT(2) /* used by i915_gem_object_wait() */
#define I915_WAIT_FOR_IDLE_BOOST BIT(3)

static inline u32 intel_engine_get_seqno(struct intel_engine_cs *engine);
=======
#define I915_WAIT_PRIORITY	BIT(1) /* small priority bump for the request */
#define I915_WAIT_ALL		BIT(2) /* used by i915_gem_object_wait() */

void i915_request_show(struct drm_printer *m,
		       const struct i915_request *rq,
		       const char *prefix,
		       int indent);

static inline bool i915_request_signaled(const struct i915_request *rq)
{
	/* The request may live longer than its HWSP, so check flags first! */
	return test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &rq->fence.flags);
}

static inline bool i915_request_is_active(const struct i915_request *rq)
{
	return test_bit(I915_FENCE_FLAG_ACTIVE, &rq->fence.flags);
}

static inline bool i915_request_in_priority_queue(const struct i915_request *rq)
{
	return test_bit(I915_FENCE_FLAG_PQUEUE, &rq->fence.flags);
}

static inline bool
i915_request_has_initial_breadcrumb(const struct i915_request *rq)
{
	return test_bit(I915_FENCE_FLAG_INITIAL_BREADCRUMB, &rq->fence.flags);
}
>>>>>>> upstream/android-13

/**
 * Returns true if seq1 is later than seq2.
 */
static inline bool i915_seqno_passed(u32 seq1, u32 seq2)
{
	return (s32)(seq1 - seq2) >= 0;
}

<<<<<<< HEAD
static inline bool
__i915_request_completed(const struct i915_request *rq, u32 seqno)
{
	GEM_BUG_ON(!seqno);
	return i915_seqno_passed(intel_engine_get_seqno(rq->engine), seqno) &&
		seqno == i915_request_global_seqno(rq);
=======
static inline u32 __hwsp_seqno(const struct i915_request *rq)
{
	const u32 *hwsp = READ_ONCE(rq->hwsp_seqno);

	return READ_ONCE(*hwsp);
}

/**
 * hwsp_seqno - the current breadcrumb value in the HW status page
 * @rq: the request, to chase the relevant HW status page
 *
 * The emphasis in naming here is that hwsp_seqno() is not a property of the
 * request, but an indication of the current HW state (associated with this
 * request). Its value will change as the GPU executes more requests.
 *
 * Returns the current breadcrumb value in the associated HW status page (or
 * the local timeline's equivalent) for this request. The request itself
 * has the associated breadcrumb value of rq->fence.seqno, when the HW
 * status page has that breadcrumb or later, this request is complete.
 */
static inline u32 hwsp_seqno(const struct i915_request *rq)
{
	u32 seqno;

	rcu_read_lock(); /* the HWSP may be freed at runtime */
	seqno = __hwsp_seqno(rq);
	rcu_read_unlock();

	return seqno;
}

static inline bool __i915_request_has_started(const struct i915_request *rq)
{
	return i915_seqno_passed(__hwsp_seqno(rq), rq->fence.seqno - 1);
}

/**
 * i915_request_started - check if the request has begun being executed
 * @rq: the request
 *
 * If the timeline is not using initial breadcrumbs, a request is
 * considered started if the previous request on its timeline (i.e.
 * context) has been signaled.
 *
 * If the timeline is using semaphores, it will also be emitting an
 * "initial breadcrumb" after the semaphores are complete and just before
 * it began executing the user payload. A request can therefore be active
 * on the HW and not yet started as it is still busywaiting on its
 * dependencies (via HW semaphores).
 *
 * If the request has started, its dependencies will have been signaled
 * (either by fences or by semaphores) and it will have begun processing
 * the user payload.
 *
 * However, even if a request has started, it may have been preempted and
 * so no longer active, or it may have already completed.
 *
 * See also i915_request_is_active().
 *
 * Returns true if the request has begun executing the user payload, or
 * has completed:
 */
static inline bool i915_request_started(const struct i915_request *rq)
{
	bool result;

	if (i915_request_signaled(rq))
		return true;

	result = true;
	rcu_read_lock(); /* the HWSP may be freed at runtime */
	if (likely(!i915_request_signaled(rq)))
		/* Remember: started but may have since been preempted! */
		result = __i915_request_has_started(rq);
	rcu_read_unlock();

	return result;
}

/**
 * i915_request_is_running - check if the request may actually be executing
 * @rq: the request
 *
 * Returns true if the request is currently submitted to hardware, has passed
 * its start point (i.e. the context is setup and not busywaiting). Note that
 * it may no longer be running by the time the function returns!
 */
static inline bool i915_request_is_running(const struct i915_request *rq)
{
	bool result;

	if (!i915_request_is_active(rq))
		return false;

	rcu_read_lock();
	result = __i915_request_has_started(rq) && i915_request_is_active(rq);
	rcu_read_unlock();

	return result;
}

/**
 * i915_request_is_ready - check if the request is ready for execution
 * @rq: the request
 *
 * Upon construction, the request is instructed to wait upon various
 * signals before it is ready to be executed by the HW. That is, we do
 * not want to start execution and read data before it is written. In practice,
 * this is controlled with a mixture of interrupts and semaphores. Once
 * the submit fence is completed, the backend scheduler will place the
 * request into its queue and from there submit it for execution. So we
 * can detect when a request is eligible for execution (and is under control
 * of the scheduler) by querying where it is in any of the scheduler's lists.
 *
 * Returns true if the request is ready for execution (it may be inflight),
 * false otherwise.
 */
static inline bool i915_request_is_ready(const struct i915_request *rq)
{
	return !list_empty(&rq->sched.link);
}

static inline bool __i915_request_is_complete(const struct i915_request *rq)
{
	return i915_seqno_passed(__hwsp_seqno(rq), rq->fence.seqno);
>>>>>>> upstream/android-13
}

static inline bool i915_request_completed(const struct i915_request *rq)
{
<<<<<<< HEAD
	u32 seqno;

	seqno = i915_request_global_seqno(rq);
	if (!seqno)
		return false;

	return __i915_request_completed(rq, seqno);
}

static inline bool i915_request_started(const struct i915_request *rq)
{
	u32 seqno;

	seqno = i915_request_global_seqno(rq);
	if (!seqno)
		return false;

	return i915_seqno_passed(intel_engine_get_seqno(rq->engine),
				 seqno - 1);
}

static inline bool i915_sched_node_signaled(const struct i915_sched_node *node)
{
	const struct i915_request *rq =
		container_of(node, const struct i915_request, sched);

	return i915_request_completed(rq);
}

void i915_retire_requests(struct drm_i915_private *i915);

/*
 * We treat requests as fences. This is not be to confused with our
 * "fence registers" but pipeline synchronisation objects ala GL_ARB_sync.
 * We use the fences to synchronize access from the CPU with activity on the
 * GPU, for example, we should not rewrite an object's PTE whilst the GPU
 * is reading them. We also track fences at a higher level to provide
 * implicit synchronisation around GEM objects, e.g. set-domain will wait
 * for outstanding GPU rendering before marking the object ready for CPU
 * access, or a pageflip will wait until the GPU is complete before showing
 * the frame on the scanout.
 *
 * In order to use a fence, the object must track the fence it needs to
 * serialise with. For example, GEM objects want to track both read and
 * write access so that we can perform concurrent read operations between
 * the CPU and GPU engines, as well as waiting for all rendering to
 * complete, or waiting for the last GPU user of a "fence register". The
 * object then embeds a #i915_gem_active to track the most recent (in
 * retirement order) request relevant for the desired mode of access.
 * The #i915_gem_active is updated with i915_gem_active_set() to track the
 * most recent fence request, typically this is done as part of
 * i915_vma_move_to_active().
 *
 * When the #i915_gem_active completes (is retired), it will
 * signal its completion to the owner through a callback as well as mark
 * itself as idle (i915_gem_active.request == NULL). The owner
 * can then perform any action, such as delayed freeing of an active
 * resource including itself.
 */
struct i915_gem_active;

typedef void (*i915_gem_retire_fn)(struct i915_gem_active *,
				   struct i915_request *);

struct i915_gem_active {
	struct i915_request __rcu *request;
	struct list_head link;
	i915_gem_retire_fn retire;
};

void i915_gem_retire_noop(struct i915_gem_active *,
			  struct i915_request *request);

/**
 * init_request_active - prepares the activity tracker for use
 * @active - the active tracker
 * @func - a callback when then the tracker is retired (becomes idle),
 *         can be NULL
 *
 * init_request_active() prepares the embedded @active struct for use as
 * an activity tracker, that is for tracking the last known active request
 * associated with it. When the last request becomes idle, when it is retired
 * after completion, the optional callback @func is invoked.
 */
static inline void
init_request_active(struct i915_gem_active *active,
		    i915_gem_retire_fn retire)
{
	RCU_INIT_POINTER(active->request, NULL);
	INIT_LIST_HEAD(&active->link);
	active->retire = retire ?: i915_gem_retire_noop;
}

/**
 * i915_gem_active_set - updates the tracker to watch the current request
 * @active - the active tracker
 * @request - the request to watch
 *
 * i915_gem_active_set() watches the given @request for completion. Whilst
 * that @request is busy, the @active reports busy. When that @request is
 * retired, the @active tracker is updated to report idle.
 */
static inline void
i915_gem_active_set(struct i915_gem_active *active,
		    struct i915_request *request)
{
	list_move(&active->link, &request->active_list);
	rcu_assign_pointer(active->request, request);
}

/**
 * i915_gem_active_set_retire_fn - updates the retirement callback
 * @active - the active tracker
 * @fn - the routine called when the request is retired
 * @mutex - struct_mutex used to guard retirements
 *
 * i915_gem_active_set_retire_fn() updates the function pointer that
 * is called when the final request associated with the @active tracker
 * is retired.
 */
static inline void
i915_gem_active_set_retire_fn(struct i915_gem_active *active,
			      i915_gem_retire_fn fn,
			      struct mutex *mutex)
{
	lockdep_assert_held(mutex);
	active->retire = fn ?: i915_gem_retire_noop;
}

static inline struct i915_request *
__i915_gem_active_peek(const struct i915_gem_active *active)
{
	/*
	 * Inside the error capture (running with the driver in an unknown
	 * state), we want to bend the rules slightly (a lot).
	 *
	 * Work is in progress to make it safer, in the meantime this keeps
	 * the known issue from spamming the logs.
	 */
	return rcu_dereference_protected(active->request, 1);
}

/**
 * i915_gem_active_raw - return the active request
 * @active - the active tracker
 *
 * i915_gem_active_raw() returns the current request being tracked, or NULL.
 * It does not obtain a reference on the request for the caller, so the caller
 * must hold struct_mutex.
 */
static inline struct i915_request *
i915_gem_active_raw(const struct i915_gem_active *active, struct mutex *mutex)
{
	return rcu_dereference_protected(active->request,
					 lockdep_is_held(mutex));
}

/**
 * i915_gem_active_peek - report the active request being monitored
 * @active - the active tracker
 *
 * i915_gem_active_peek() returns the current request being tracked if
 * still active, or NULL. It does not obtain a reference on the request
 * for the caller, so the caller must hold struct_mutex.
 */
static inline struct i915_request *
i915_gem_active_peek(const struct i915_gem_active *active, struct mutex *mutex)
{
	struct i915_request *request;

	request = i915_gem_active_raw(active, mutex);
	if (!request || i915_request_completed(request))
		return NULL;

	return request;
}

/**
 * i915_gem_active_get - return a reference to the active request
 * @active - the active tracker
 *
 * i915_gem_active_get() returns a reference to the active request, or NULL
 * if the active tracker is idle. The caller must hold struct_mutex.
 */
static inline struct i915_request *
i915_gem_active_get(const struct i915_gem_active *active, struct mutex *mutex)
{
	return i915_request_get(i915_gem_active_peek(active, mutex));
}

/**
 * __i915_gem_active_get_rcu - return a reference to the active request
 * @active - the active tracker
 *
 * __i915_gem_active_get() returns a reference to the active request, or NULL
 * if the active tracker is idle. The caller must hold the RCU read lock, but
 * the returned pointer is safe to use outside of RCU.
 */
static inline struct i915_request *
__i915_gem_active_get_rcu(const struct i915_gem_active *active)
{
	/*
	 * Performing a lockless retrieval of the active request is super
	 * tricky. SLAB_TYPESAFE_BY_RCU merely guarantees that the backing
	 * slab of request objects will not be freed whilst we hold the
	 * RCU read lock. It does not guarantee that the request itself
	 * will not be freed and then *reused*. Viz,
	 *
	 * Thread A			Thread B
	 *
	 * rq = active.request
	 *				retire(rq) -> free(rq);
	 *				(rq is now first on the slab freelist)
	 *				active.request = NULL
	 *
	 *				rq = new submission on a new object
	 * ref(rq)
	 *
	 * To prevent the request from being reused whilst the caller
	 * uses it, we take a reference like normal. Whilst acquiring
	 * the reference we check that it is not in a destroyed state
	 * (refcnt == 0). That prevents the request being reallocated
	 * whilst the caller holds on to it. To check that the request
	 * was not reallocated as we acquired the reference we have to
	 * check that our request remains the active request across
	 * the lookup, in the same manner as a seqlock. The visibility
	 * of the pointer versus the reference counting is controlled
	 * by using RCU barriers (rcu_dereference and rcu_assign_pointer).
	 *
	 * In the middle of all that, we inspect whether the request is
	 * complete. Retiring is lazy so the request may be completed long
	 * before the active tracker is updated. Querying whether the
	 * request is complete is far cheaper (as it involves no locked
	 * instructions setting cachelines to exclusive) than acquiring
	 * the reference, so we do it first. The RCU read lock ensures the
	 * pointer dereference is valid, but does not ensure that the
	 * seqno nor HWS is the right one! However, if the request was
	 * reallocated, that means the active tracker's request was complete.
	 * If the new request is also complete, then both are and we can
	 * just report the active tracker is idle. If the new request is
	 * incomplete, then we acquire a reference on it and check that
	 * it remained the active request.
	 *
	 * It is then imperative that we do not zero the request on
	 * reallocation, so that we can chase the dangling pointers!
	 * See i915_request_alloc().
	 */
	do {
		struct i915_request *request;

		request = rcu_dereference(active->request);
		if (!request || i915_request_completed(request))
			return NULL;

		/*
		 * An especially silly compiler could decide to recompute the
		 * result of i915_request_completed, more specifically
		 * re-emit the load for request->fence.seqno. A race would catch
		 * a later seqno value, which could flip the result from true to
		 * false. Which means part of the instructions below might not
		 * be executed, while later on instructions are executed. Due to
		 * barriers within the refcounting the inconsistency can't reach
		 * past the call to i915_request_get_rcu, but not executing
		 * that while still executing i915_request_put() creates
		 * havoc enough.  Prevent this with a compiler barrier.
		 */
		barrier();

		request = i915_request_get_rcu(request);

		/*
		 * What stops the following rcu_access_pointer() from occurring
		 * before the above i915_request_get_rcu()? If we were
		 * to read the value before pausing to get the reference to
		 * the request, we may not notice a change in the active
		 * tracker.
		 *
		 * The rcu_access_pointer() is a mere compiler barrier, which
		 * means both the CPU and compiler are free to perform the
		 * memory read without constraint. The compiler only has to
		 * ensure that any operations after the rcu_access_pointer()
		 * occur afterwards in program order. This means the read may
		 * be performed earlier by an out-of-order CPU, or adventurous
		 * compiler.
		 *
		 * The atomic operation at the heart of
		 * i915_request_get_rcu(), see dma_fence_get_rcu(), is
		 * atomic_inc_not_zero() which is only a full memory barrier
		 * when successful. That is, if i915_request_get_rcu()
		 * returns the request (and so with the reference counted
		 * incremented) then the following read for rcu_access_pointer()
		 * must occur after the atomic operation and so confirm
		 * that this request is the one currently being tracked.
		 *
		 * The corresponding write barrier is part of
		 * rcu_assign_pointer().
		 */
		if (!request || request == rcu_access_pointer(active->request))
			return rcu_pointer_handoff(request);

		i915_request_put(request);
	} while (1);
}

/**
 * i915_gem_active_get_unlocked - return a reference to the active request
 * @active - the active tracker
 *
 * i915_gem_active_get_unlocked() returns a reference to the active request,
 * or NULL if the active tracker is idle. The reference is obtained under RCU,
 * so no locking is required by the caller.
 *
 * The reference should be freed with i915_request_put().
 */
static inline struct i915_request *
i915_gem_active_get_unlocked(const struct i915_gem_active *active)
{
	struct i915_request *request;

	rcu_read_lock();
	request = __i915_gem_active_get_rcu(active);
	rcu_read_unlock();

	return request;
}

/**
 * i915_gem_active_isset - report whether the active tracker is assigned
 * @active - the active tracker
 *
 * i915_gem_active_isset() returns true if the active tracker is currently
 * assigned to a request. Due to the lazy retiring, that request may be idle
 * and this may report stale information.
 */
static inline bool
i915_gem_active_isset(const struct i915_gem_active *active)
{
	return rcu_access_pointer(active->request);
}

/**
 * i915_gem_active_wait - waits until the request is completed
 * @active - the active request on which to wait
 * @flags - how to wait
 * @timeout - how long to wait at most
 * @rps - userspace client to charge for a waitboost
 *
 * i915_gem_active_wait() waits until the request is completed before
 * returning, without requiring any locks to be held. Note that it does not
 * retire any requests before returning.
 *
 * This function relies on RCU in order to acquire the reference to the active
 * request without holding any locks. See __i915_gem_active_get_rcu() for the
 * glory details on how that is managed. Once the reference is acquired, we
 * can then wait upon the request, and afterwards release our reference,
 * free of any locking.
 *
 * This function wraps i915_request_wait(), see it for the full details on
 * the arguments.
 *
 * Returns 0 if successful, or a negative error code.
 */
static inline int
i915_gem_active_wait(const struct i915_gem_active *active, unsigned int flags)
{
	struct i915_request *request;
	long ret = 0;

	request = i915_gem_active_get_unlocked(active);
	if (request) {
		ret = i915_request_wait(request, flags, MAX_SCHEDULE_TIMEOUT);
		i915_request_put(request);
	}

	return ret < 0 ? ret : 0;
}

/**
 * i915_gem_active_retire - waits until the request is retired
 * @active - the active request on which to wait
 *
 * i915_gem_active_retire() waits until the request is completed,
 * and then ensures that at least the retirement handler for this
 * @active tracker is called before returning. If the @active
 * tracker is idle, the function returns immediately.
 */
static inline int __must_check
i915_gem_active_retire(struct i915_gem_active *active,
		       struct mutex *mutex)
{
	struct i915_request *request;
	long ret;

	request = i915_gem_active_raw(active, mutex);
	if (!request)
		return 0;

	ret = i915_request_wait(request,
				I915_WAIT_INTERRUPTIBLE | I915_WAIT_LOCKED,
				MAX_SCHEDULE_TIMEOUT);
	if (ret < 0)
		return ret;

	list_del_init(&active->link);
	RCU_INIT_POINTER(active->request, NULL);

	active->retire(active, request);

	return 0;
}

#define for_each_active(mask, idx) \
	for (; mask ? idx = ffs(mask) - 1, 1 : 0; mask &= ~BIT(idx))
=======
	bool result;

	if (i915_request_signaled(rq))
		return true;

	result = true;
	rcu_read_lock(); /* the HWSP may be freed at runtime */
	if (likely(!i915_request_signaled(rq)))
		result = __i915_request_is_complete(rq);
	rcu_read_unlock();

	return result;
}

static inline void i915_request_mark_complete(struct i915_request *rq)
{
	WRITE_ONCE(rq->hwsp_seqno, /* decouple from HWSP */
		   (u32 *)&rq->fence.seqno);
}

static inline bool i915_request_has_waitboost(const struct i915_request *rq)
{
	return test_bit(I915_FENCE_FLAG_BOOST, &rq->fence.flags);
}

static inline bool i915_request_has_nopreempt(const struct i915_request *rq)
{
	/* Preemption should only be disabled very rarely */
	return unlikely(test_bit(I915_FENCE_FLAG_NOPREEMPT, &rq->fence.flags));
}

static inline bool i915_request_has_sentinel(const struct i915_request *rq)
{
	return unlikely(test_bit(I915_FENCE_FLAG_SENTINEL, &rq->fence.flags));
}

static inline bool i915_request_on_hold(const struct i915_request *rq)
{
	return unlikely(test_bit(I915_FENCE_FLAG_HOLD, &rq->fence.flags));
}

static inline void i915_request_set_hold(struct i915_request *rq)
{
	set_bit(I915_FENCE_FLAG_HOLD, &rq->fence.flags);
}

static inline void i915_request_clear_hold(struct i915_request *rq)
{
	clear_bit(I915_FENCE_FLAG_HOLD, &rq->fence.flags);
}

static inline struct intel_timeline *
i915_request_timeline(const struct i915_request *rq)
{
	/* Valid only while the request is being constructed (or retired). */
	return rcu_dereference_protected(rq->timeline,
					 lockdep_is_held(&rcu_access_pointer(rq->timeline)->mutex));
}

static inline struct i915_gem_context *
i915_request_gem_context(const struct i915_request *rq)
{
	/* Valid only while the request is being constructed (or retired). */
	return rcu_dereference_protected(rq->context->gem_context, true);
}

static inline struct intel_timeline *
i915_request_active_timeline(const struct i915_request *rq)
{
	/*
	 * When in use during submission, we are protected by a guarantee that
	 * the context/timeline is pinned and must remain pinned until after
	 * this submission.
	 */
	return rcu_dereference_protected(rq->timeline,
					 lockdep_is_held(&rq->engine->sched_engine->lock));
}

static inline u32
i915_request_active_seqno(const struct i915_request *rq)
{
	u32 hwsp_phys_base =
		page_mask_bits(i915_request_active_timeline(rq)->hwsp_offset);
	u32 hwsp_relative_offset = offset_in_page(rq->hwsp_seqno);

	/*
	 * Because of wraparound, we cannot simply take tl->hwsp_offset,
	 * but instead use the fact that the relative for vaddr is the
	 * offset as for hwsp_offset. Take the top bits from tl->hwsp_offset
	 * and combine them with the relative offset in rq->hwsp_seqno.
	 *
	 * As rw->hwsp_seqno is rewritten when signaled, this only works
	 * when the request isn't signaled yet, but at that point you
	 * no longer need the offset.
	 */

	return hwsp_phys_base + hwsp_relative_offset;
}

bool
i915_request_active_engine(struct i915_request *rq,
			   struct intel_engine_cs **active);

void i915_request_notify_execute_cb_imm(struct i915_request *rq);

enum i915_request_state {
	I915_REQUEST_UNKNOWN = 0,
	I915_REQUEST_COMPLETE,
	I915_REQUEST_PENDING,
	I915_REQUEST_QUEUED,
	I915_REQUEST_ACTIVE,
};

enum i915_request_state i915_test_request_state(struct i915_request *rq);

void i915_request_module_exit(void);
int i915_request_module_init(void);
>>>>>>> upstream/android-13

#endif /* I915_REQUEST_H */
