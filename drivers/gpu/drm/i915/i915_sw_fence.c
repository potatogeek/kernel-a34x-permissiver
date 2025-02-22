/*
<<<<<<< HEAD
 * (C) Copyright 2016 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
 * SPDX-License-Identifier: MIT
 *
 * (C) Copyright 2016 Intel Corporation
>>>>>>> upstream/android-13
 */

#include <linux/slab.h>
#include <linux/dma-fence.h>
#include <linux/irq_work.h>
<<<<<<< HEAD
#include <linux/reservation.h>
=======
#include <linux/dma-resv.h>
>>>>>>> upstream/android-13

#include "i915_sw_fence.h"
#include "i915_selftest.h"

<<<<<<< HEAD
#define I915_SW_FENCE_FLAG_ALLOC BIT(3) /* after WQ_FLAG_* for safety */

static DEFINE_SPINLOCK(i915_sw_fence_lock);

=======
#if IS_ENABLED(CONFIG_DRM_I915_DEBUG)
#define I915_SW_FENCE_BUG_ON(expr) BUG_ON(expr)
#else
#define I915_SW_FENCE_BUG_ON(expr) BUILD_BUG_ON_INVALID(expr)
#endif

static DEFINE_SPINLOCK(i915_sw_fence_lock);

#define WQ_FLAG_BITS \
	BITS_PER_TYPE(typeof_member(struct wait_queue_entry, flags))

/* after WQ_FLAG_* for safety */
#define I915_SW_FENCE_FLAG_FENCE BIT(WQ_FLAG_BITS - 1)
#define I915_SW_FENCE_FLAG_ALLOC BIT(WQ_FLAG_BITS - 2)

>>>>>>> upstream/android-13
enum {
	DEBUG_FENCE_IDLE = 0,
	DEBUG_FENCE_NOTIFY,
};

<<<<<<< HEAD
#ifdef CONFIG_DRM_I915_SW_FENCE_DEBUG_OBJECTS

=======
>>>>>>> upstream/android-13
static void *i915_sw_fence_debug_hint(void *addr)
{
	return (void *)(((struct i915_sw_fence *)addr)->flags & I915_SW_FENCE_MASK);
}

<<<<<<< HEAD
static struct debug_obj_descr i915_sw_fence_debug_descr = {
=======
#ifdef CONFIG_DRM_I915_SW_FENCE_DEBUG_OBJECTS

static const struct debug_obj_descr i915_sw_fence_debug_descr = {
>>>>>>> upstream/android-13
	.name = "i915_sw_fence",
	.debug_hint = i915_sw_fence_debug_hint,
};

static inline void debug_fence_init(struct i915_sw_fence *fence)
{
	debug_object_init(fence, &i915_sw_fence_debug_descr);
}

static inline void debug_fence_init_onstack(struct i915_sw_fence *fence)
{
	debug_object_init_on_stack(fence, &i915_sw_fence_debug_descr);
}

static inline void debug_fence_activate(struct i915_sw_fence *fence)
{
	debug_object_activate(fence, &i915_sw_fence_debug_descr);
}

static inline void debug_fence_set_state(struct i915_sw_fence *fence,
					 int old, int new)
{
	debug_object_active_state(fence, &i915_sw_fence_debug_descr, old, new);
}

static inline void debug_fence_deactivate(struct i915_sw_fence *fence)
{
	debug_object_deactivate(fence, &i915_sw_fence_debug_descr);
}

static inline void debug_fence_destroy(struct i915_sw_fence *fence)
{
	debug_object_destroy(fence, &i915_sw_fence_debug_descr);
}

static inline void debug_fence_free(struct i915_sw_fence *fence)
{
	debug_object_free(fence, &i915_sw_fence_debug_descr);
	smp_wmb(); /* flush the change in state before reallocation */
}

static inline void debug_fence_assert(struct i915_sw_fence *fence)
{
	debug_object_assert_init(fence, &i915_sw_fence_debug_descr);
}

#else

static inline void debug_fence_init(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_init_onstack(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_activate(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_set_state(struct i915_sw_fence *fence,
					 int old, int new)
{
}

static inline void debug_fence_deactivate(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_destroy(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_free(struct i915_sw_fence *fence)
{
}

static inline void debug_fence_assert(struct i915_sw_fence *fence)
{
}

#endif

static int __i915_sw_fence_notify(struct i915_sw_fence *fence,
				  enum i915_sw_fence_notify state)
{
	i915_sw_fence_notify_t fn;

	fn = (i915_sw_fence_notify_t)(fence->flags & I915_SW_FENCE_MASK);
	return fn(fence, state);
}

#ifdef CONFIG_DRM_I915_SW_FENCE_DEBUG_OBJECTS
void i915_sw_fence_fini(struct i915_sw_fence *fence)
{
	debug_fence_free(fence);
}
#endif

static void __i915_sw_fence_wake_up_all(struct i915_sw_fence *fence,
					struct list_head *continuation)
{
	wait_queue_head_t *x = &fence->wait;
	wait_queue_entry_t *pos, *next;
	unsigned long flags;

	debug_fence_deactivate(fence);
	atomic_set_release(&fence->pending, -1); /* 0 -> -1 [done] */

	/*
	 * To prevent unbounded recursion as we traverse the graph of
	 * i915_sw_fences, we move the entry list from this, the next ready
	 * fence, to the tail of the original fence's entry list
	 * (and so added to the list to be woken).
	 */

	spin_lock_irqsave_nested(&x->lock, flags, 1 + !!continuation);
	if (continuation) {
		list_for_each_entry_safe(pos, next, &x->head, entry) {
<<<<<<< HEAD
			if (pos->func == autoremove_wake_function)
				pos->func(pos, TASK_NORMAL, 0, continuation);
			else
				list_move_tail(&pos->entry, continuation);
=======
			if (pos->flags & I915_SW_FENCE_FLAG_FENCE)
				list_move_tail(&pos->entry, continuation);
			else
				pos->func(pos, TASK_NORMAL, 0, continuation);
>>>>>>> upstream/android-13
		}
	} else {
		LIST_HEAD(extra);

		do {
<<<<<<< HEAD
			list_for_each_entry_safe(pos, next, &x->head, entry)
				pos->func(pos, TASK_NORMAL, 0, &extra);
=======
			list_for_each_entry_safe(pos, next, &x->head, entry) {
				int wake_flags;

				wake_flags = 0;
				if (pos->flags & I915_SW_FENCE_FLAG_FENCE)
					wake_flags = fence->error;

				pos->func(pos, TASK_NORMAL, wake_flags, &extra);
			}
>>>>>>> upstream/android-13

			if (list_empty(&extra))
				break;

			list_splice_tail_init(&extra, &x->head);
		} while (1);
	}
	spin_unlock_irqrestore(&x->lock, flags);

	debug_fence_assert(fence);
}

static void __i915_sw_fence_complete(struct i915_sw_fence *fence,
				     struct list_head *continuation)
{
	debug_fence_assert(fence);

	if (!atomic_dec_and_test(&fence->pending))
		return;

	debug_fence_set_state(fence, DEBUG_FENCE_IDLE, DEBUG_FENCE_NOTIFY);

	if (__i915_sw_fence_notify(fence, FENCE_COMPLETE) != NOTIFY_DONE)
		return;

	debug_fence_set_state(fence, DEBUG_FENCE_NOTIFY, DEBUG_FENCE_IDLE);

	__i915_sw_fence_wake_up_all(fence, continuation);

	debug_fence_destroy(fence);
	__i915_sw_fence_notify(fence, FENCE_FREE);
}

<<<<<<< HEAD
static void i915_sw_fence_complete(struct i915_sw_fence *fence)
=======
void i915_sw_fence_complete(struct i915_sw_fence *fence)
>>>>>>> upstream/android-13
{
	debug_fence_assert(fence);

	if (WARN_ON(i915_sw_fence_done(fence)))
		return;

	__i915_sw_fence_complete(fence, NULL);
}

<<<<<<< HEAD
static void i915_sw_fence_await(struct i915_sw_fence *fence)
{
	debug_fence_assert(fence);
	WARN_ON(atomic_inc_return(&fence->pending) <= 1);
=======
bool i915_sw_fence_await(struct i915_sw_fence *fence)
{
	int pending;

	/*
	 * It is only safe to add a new await to the fence while it has
	 * not yet been signaled (i.e. there are still existing signalers).
	 */
	pending = atomic_read(&fence->pending);
	do {
		if (pending < 1)
			return false;
	} while (!atomic_try_cmpxchg(&fence->pending, &pending, pending + 1));

	return true;
>>>>>>> upstream/android-13
}

void __i915_sw_fence_init(struct i915_sw_fence *fence,
			  i915_sw_fence_notify_t fn,
			  const char *name,
			  struct lock_class_key *key)
{
	BUG_ON(!fn || (unsigned long)fn & ~I915_SW_FENCE_MASK);

<<<<<<< HEAD
	debug_fence_init(fence);

	__init_waitqueue_head(&fence->wait, name, key);
	atomic_set(&fence->pending, 1);
	fence->flags = (unsigned long)fn;
=======
	__init_waitqueue_head(&fence->wait, name, key);
	fence->flags = (unsigned long)fn;

	i915_sw_fence_reinit(fence);
}

void i915_sw_fence_reinit(struct i915_sw_fence *fence)
{
	debug_fence_init(fence);

	atomic_set(&fence->pending, 1);
	fence->error = 0;

	I915_SW_FENCE_BUG_ON(!fence->flags);
	I915_SW_FENCE_BUG_ON(!list_empty(&fence->wait.head));
>>>>>>> upstream/android-13
}

void i915_sw_fence_commit(struct i915_sw_fence *fence)
{
	debug_fence_activate(fence);
	i915_sw_fence_complete(fence);
}

static int i915_sw_fence_wake(wait_queue_entry_t *wq, unsigned mode, int flags, void *key)
{
<<<<<<< HEAD
=======
	i915_sw_fence_set_error_once(wq->private, flags);

>>>>>>> upstream/android-13
	list_del(&wq->entry);
	__i915_sw_fence_complete(wq->private, key);

	if (wq->flags & I915_SW_FENCE_FLAG_ALLOC)
		kfree(wq);
	return 0;
}

static bool __i915_sw_fence_check_if_after(struct i915_sw_fence *fence,
				    const struct i915_sw_fence * const signaler)
{
	wait_queue_entry_t *wq;

	if (__test_and_set_bit(I915_SW_FENCE_CHECKED_BIT, &fence->flags))
		return false;

	if (fence == signaler)
		return true;

	list_for_each_entry(wq, &fence->wait.head, entry) {
		if (wq->func != i915_sw_fence_wake)
			continue;

		if (__i915_sw_fence_check_if_after(wq->private, signaler))
			return true;
	}

	return false;
}

static void __i915_sw_fence_clear_checked_bit(struct i915_sw_fence *fence)
{
	wait_queue_entry_t *wq;

	if (!__test_and_clear_bit(I915_SW_FENCE_CHECKED_BIT, &fence->flags))
		return;

	list_for_each_entry(wq, &fence->wait.head, entry) {
		if (wq->func != i915_sw_fence_wake)
			continue;

		__i915_sw_fence_clear_checked_bit(wq->private);
	}
}

static bool i915_sw_fence_check_if_after(struct i915_sw_fence *fence,
				  const struct i915_sw_fence * const signaler)
{
	unsigned long flags;
	bool err;

	if (!IS_ENABLED(CONFIG_DRM_I915_SW_FENCE_CHECK_DAG))
		return false;

	spin_lock_irqsave(&i915_sw_fence_lock, flags);
	err = __i915_sw_fence_check_if_after(fence, signaler);
	__i915_sw_fence_clear_checked_bit(fence);
	spin_unlock_irqrestore(&i915_sw_fence_lock, flags);

	return err;
}

static int __i915_sw_fence_await_sw_fence(struct i915_sw_fence *fence,
					  struct i915_sw_fence *signaler,
					  wait_queue_entry_t *wq, gfp_t gfp)
{
<<<<<<< HEAD
	unsigned long flags;
	int pending;
=======
	unsigned int pending;
	unsigned long flags;
>>>>>>> upstream/android-13

	debug_fence_assert(fence);
	might_sleep_if(gfpflags_allow_blocking(gfp));

<<<<<<< HEAD
	if (i915_sw_fence_done(signaler))
		return 0;
=======
	if (i915_sw_fence_done(signaler)) {
		i915_sw_fence_set_error_once(fence, signaler->error);
		return 0;
	}
>>>>>>> upstream/android-13

	debug_fence_assert(signaler);

	/* The dependency graph must be acyclic. */
	if (unlikely(i915_sw_fence_check_if_after(fence, signaler)))
		return -EINVAL;

<<<<<<< HEAD
	pending = 0;
=======
	pending = I915_SW_FENCE_FLAG_FENCE;
>>>>>>> upstream/android-13
	if (!wq) {
		wq = kmalloc(sizeof(*wq), gfp);
		if (!wq) {
			if (!gfpflags_allow_blocking(gfp))
				return -ENOMEM;

			i915_sw_fence_wait(signaler);
<<<<<<< HEAD
=======
			i915_sw_fence_set_error_once(fence, signaler->error);
>>>>>>> upstream/android-13
			return 0;
		}

		pending |= I915_SW_FENCE_FLAG_ALLOC;
	}

	INIT_LIST_HEAD(&wq->entry);
	wq->flags = pending;
	wq->func = i915_sw_fence_wake;
	wq->private = fence;

	i915_sw_fence_await(fence);

	spin_lock_irqsave(&signaler->wait.lock, flags);
	if (likely(!i915_sw_fence_done(signaler))) {
		__add_wait_queue_entry_tail(&signaler->wait, wq);
		pending = 1;
	} else {
<<<<<<< HEAD
		i915_sw_fence_wake(wq, 0, 0, NULL);
=======
		i915_sw_fence_wake(wq, 0, signaler->error, NULL);
>>>>>>> upstream/android-13
		pending = 0;
	}
	spin_unlock_irqrestore(&signaler->wait.lock, flags);

	return pending;
}

int i915_sw_fence_await_sw_fence(struct i915_sw_fence *fence,
				 struct i915_sw_fence *signaler,
				 wait_queue_entry_t *wq)
{
	return __i915_sw_fence_await_sw_fence(fence, signaler, wq, 0);
}

int i915_sw_fence_await_sw_fence_gfp(struct i915_sw_fence *fence,
				     struct i915_sw_fence *signaler,
				     gfp_t gfp)
{
	return __i915_sw_fence_await_sw_fence(fence, signaler, NULL, gfp);
}

<<<<<<< HEAD
struct i915_sw_dma_fence_cb {
	struct dma_fence_cb base;
	struct i915_sw_fence *fence;
};

=======
>>>>>>> upstream/android-13
struct i915_sw_dma_fence_cb_timer {
	struct i915_sw_dma_fence_cb base;
	struct dma_fence *dma;
	struct timer_list timer;
	struct irq_work work;
	struct rcu_head rcu;
};

static void dma_i915_sw_fence_wake(struct dma_fence *dma,
				   struct dma_fence_cb *data)
{
	struct i915_sw_dma_fence_cb *cb = container_of(data, typeof(*cb), base);

<<<<<<< HEAD
=======
	i915_sw_fence_set_error_once(cb->fence, dma->error);
>>>>>>> upstream/android-13
	i915_sw_fence_complete(cb->fence);
	kfree(cb);
}

static void timer_i915_sw_fence_wake(struct timer_list *t)
{
	struct i915_sw_dma_fence_cb_timer *cb = from_timer(cb, t, timer);
	struct i915_sw_fence *fence;

	fence = xchg(&cb->base.fence, NULL);
	if (!fence)
		return;

<<<<<<< HEAD
	pr_warn("asynchronous wait on fence %s:%s:%x timed out\n",
		cb->dma->ops->get_driver_name(cb->dma),
		cb->dma->ops->get_timeline_name(cb->dma),
		cb->dma->seqno);

=======
	pr_notice("Asynchronous wait on fence %s:%s:%llx timed out (hint:%ps)\n",
		  cb->dma->ops->get_driver_name(cb->dma),
		  cb->dma->ops->get_timeline_name(cb->dma),
		  cb->dma->seqno,
		  i915_sw_fence_debug_hint(fence));

	i915_sw_fence_set_error_once(fence, -ETIMEDOUT);
>>>>>>> upstream/android-13
	i915_sw_fence_complete(fence);
}

static void dma_i915_sw_fence_wake_timer(struct dma_fence *dma,
					 struct dma_fence_cb *data)
{
	struct i915_sw_dma_fence_cb_timer *cb =
		container_of(data, typeof(*cb), base.base);
	struct i915_sw_fence *fence;

	fence = xchg(&cb->base.fence, NULL);
<<<<<<< HEAD
	if (fence)
		i915_sw_fence_complete(fence);
=======
	if (fence) {
		i915_sw_fence_set_error_once(fence, dma->error);
		i915_sw_fence_complete(fence);
	}
>>>>>>> upstream/android-13

	irq_work_queue(&cb->work);
}

static void irq_i915_sw_fence_work(struct irq_work *wrk)
{
	struct i915_sw_dma_fence_cb_timer *cb =
		container_of(wrk, typeof(*cb), work);

	del_timer_sync(&cb->timer);
	dma_fence_put(cb->dma);

	kfree_rcu(cb, rcu);
}

int i915_sw_fence_await_dma_fence(struct i915_sw_fence *fence,
				  struct dma_fence *dma,
				  unsigned long timeout,
				  gfp_t gfp)
{
	struct i915_sw_dma_fence_cb *cb;
	dma_fence_func_t func;
	int ret;

	debug_fence_assert(fence);
	might_sleep_if(gfpflags_allow_blocking(gfp));

<<<<<<< HEAD
	if (dma_fence_is_signaled(dma))
		return 0;
=======
	if (dma_fence_is_signaled(dma)) {
		i915_sw_fence_set_error_once(fence, dma->error);
		return 0;
	}
>>>>>>> upstream/android-13

	cb = kmalloc(timeout ?
		     sizeof(struct i915_sw_dma_fence_cb_timer) :
		     sizeof(struct i915_sw_dma_fence_cb),
		     gfp);
	if (!cb) {
		if (!gfpflags_allow_blocking(gfp))
			return -ENOMEM;

<<<<<<< HEAD
		return dma_fence_wait(dma, false);
=======
		ret = dma_fence_wait(dma, false);
		if (ret)
			return ret;

		i915_sw_fence_set_error_once(fence, dma->error);
		return 0;
>>>>>>> upstream/android-13
	}

	cb->fence = fence;
	i915_sw_fence_await(fence);

	func = dma_i915_sw_fence_wake;
	if (timeout) {
		struct i915_sw_dma_fence_cb_timer *timer =
			container_of(cb, typeof(*timer), base);

		timer->dma = dma_fence_get(dma);
		init_irq_work(&timer->work, irq_i915_sw_fence_work);

		timer_setup(&timer->timer,
			    timer_i915_sw_fence_wake, TIMER_IRQSAFE);
		mod_timer(&timer->timer, round_jiffies_up(jiffies + timeout));

		func = dma_i915_sw_fence_wake_timer;
	}

	ret = dma_fence_add_callback(dma, &cb->base, func);
	if (ret == 0) {
		ret = 1;
	} else {
		func(dma, &cb->base);
		if (ret == -ENOENT) /* fence already signaled */
			ret = 0;
	}

	return ret;
}

<<<<<<< HEAD
int i915_sw_fence_await_reservation(struct i915_sw_fence *fence,
				    struct reservation_object *resv,
=======
static void __dma_i915_sw_fence_wake(struct dma_fence *dma,
				     struct dma_fence_cb *data)
{
	struct i915_sw_dma_fence_cb *cb = container_of(data, typeof(*cb), base);

	i915_sw_fence_set_error_once(cb->fence, dma->error);
	i915_sw_fence_complete(cb->fence);
}

int __i915_sw_fence_await_dma_fence(struct i915_sw_fence *fence,
				    struct dma_fence *dma,
				    struct i915_sw_dma_fence_cb *cb)
{
	int ret;

	debug_fence_assert(fence);

	if (dma_fence_is_signaled(dma)) {
		i915_sw_fence_set_error_once(fence, dma->error);
		return 0;
	}

	cb->fence = fence;
	i915_sw_fence_await(fence);

	ret = 1;
	if (dma_fence_add_callback(dma, &cb->base, __dma_i915_sw_fence_wake)) {
		/* fence already signaled */
		__dma_i915_sw_fence_wake(dma, &cb->base);
		ret = 0;
	}

	return ret;
}

int i915_sw_fence_await_reservation(struct i915_sw_fence *fence,
				    struct dma_resv *resv,
>>>>>>> upstream/android-13
				    const struct dma_fence_ops *exclude,
				    bool write,
				    unsigned long timeout,
				    gfp_t gfp)
{
	struct dma_fence *excl;
	int ret = 0, pending;

	debug_fence_assert(fence);
	might_sleep_if(gfpflags_allow_blocking(gfp));

	if (write) {
		struct dma_fence **shared;
		unsigned int count, i;

<<<<<<< HEAD
		ret = reservation_object_get_fences_rcu(resv,
							&excl, &count, &shared);
=======
		ret = dma_resv_get_fences(resv, &excl, &count, &shared);
>>>>>>> upstream/android-13
		if (ret)
			return ret;

		for (i = 0; i < count; i++) {
			if (shared[i]->ops == exclude)
				continue;

			pending = i915_sw_fence_await_dma_fence(fence,
								shared[i],
								timeout,
								gfp);
			if (pending < 0) {
				ret = pending;
				break;
			}

			ret |= pending;
		}

		for (i = 0; i < count; i++)
			dma_fence_put(shared[i]);
		kfree(shared);
	} else {
<<<<<<< HEAD
		excl = reservation_object_get_excl_rcu(resv);
=======
		excl = dma_resv_get_excl_unlocked(resv);
>>>>>>> upstream/android-13
	}

	if (ret >= 0 && excl && excl->ops != exclude) {
		pending = i915_sw_fence_await_dma_fence(fence,
							excl,
							timeout,
							gfp);
		if (pending < 0)
			ret = pending;
		else
			ret |= pending;
	}

	dma_fence_put(excl);

	return ret;
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/lib_sw_fence.c"
#include "selftests/i915_sw_fence.c"
#endif
