<<<<<<< HEAD
/*
 * RCU-based infrastructure for lightweight reader-writer locking
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * RCU-based infrastructure for lightweight reader-writer locking
 *
>>>>>>> upstream/android-13
 * Copyright (c) 2015, Red Hat, Inc.
 *
 * Author: Oleg Nesterov <oleg@redhat.com>
 */

#include <linux/rcu_sync.h>
#include <linux/sched.h>

<<<<<<< HEAD
#ifdef CONFIG_PROVE_RCU
#define __INIT_HELD(func)	.held = func,
#else
#define __INIT_HELD(func)
#endif

static const struct {
	void (*sync)(void);
	void (*call)(struct rcu_head *, void (*)(struct rcu_head *));
	void (*wait)(void);
#ifdef CONFIG_PROVE_RCU
	int  (*held)(void);
#endif
} gp_ops[] = {
	[RCU_SYNC] = {
		.sync = synchronize_rcu,
		.call = call_rcu,
		.wait = rcu_barrier,
		__INIT_HELD(rcu_read_lock_held)
	},
	[RCU_SCHED_SYNC] = {
		.sync = synchronize_sched,
		.call = call_rcu_sched,
		.wait = rcu_barrier_sched,
		__INIT_HELD(rcu_read_lock_sched_held)
	},
	[RCU_BH_SYNC] = {
		.sync = synchronize_rcu_bh,
		.call = call_rcu_bh,
		.wait = rcu_barrier_bh,
		__INIT_HELD(rcu_read_lock_bh_held)
	},
};

enum { GP_IDLE = 0, GP_PENDING, GP_PASSED };
enum { CB_IDLE = 0, CB_PENDING, CB_REPLAY };

#define	rss_lock	gp_wait.lock

#ifdef CONFIG_PROVE_RCU
void rcu_sync_lockdep_assert(struct rcu_sync *rsp)
{
	RCU_LOCKDEP_WARN(!gp_ops[rsp->gp_type].held(),
			 "suspicious rcu_sync_is_idle() usage");
}

EXPORT_SYMBOL_GPL(rcu_sync_lockdep_assert);
#endif

/**
 * rcu_sync_init() - Initialize an rcu_sync structure
 * @rsp: Pointer to rcu_sync structure to be initialized
 * @type: Flavor of RCU with which to synchronize rcu_sync structure
 */
void rcu_sync_init(struct rcu_sync *rsp, enum rcu_sync_type type)
{
	memset(rsp, 0, sizeof(*rsp));
	init_waitqueue_head(&rsp->gp_wait);
	rsp->gp_type = type;
=======
enum { GP_IDLE = 0, GP_ENTER, GP_PASSED, GP_EXIT, GP_REPLAY };

#define	rss_lock	gp_wait.lock

/**
 * rcu_sync_init() - Initialize an rcu_sync structure
 * @rsp: Pointer to rcu_sync structure to be initialized
 */
void rcu_sync_init(struct rcu_sync *rsp)
{
	memset(rsp, 0, sizeof(*rsp));
	init_waitqueue_head(&rsp->gp_wait);
>>>>>>> upstream/android-13
}

/**
 * rcu_sync_enter_start - Force readers onto slow path for multiple updates
 * @rsp: Pointer to rcu_sync structure to use for synchronization
 *
 * Must be called after rcu_sync_init() and before first use.
 *
 * Ensures rcu_sync_is_idle() returns false and rcu_sync_{enter,exit}()
 * pairs turn into NO-OPs.
 */
void rcu_sync_enter_start(struct rcu_sync *rsp)
{
	rsp->gp_count++;
	rsp->gp_state = GP_PASSED;
}

<<<<<<< HEAD
=======

static void rcu_sync_func(struct rcu_head *rhp);

static void rcu_sync_call(struct rcu_sync *rsp)
{
	call_rcu(&rsp->cb_head, rcu_sync_func);
}

/**
 * rcu_sync_func() - Callback function managing reader access to fastpath
 * @rhp: Pointer to rcu_head in rcu_sync structure to use for synchronization
 *
 * This function is passed to call_rcu() function by rcu_sync_enter() and
 * rcu_sync_exit(), so that it is invoked after a grace period following the
 * that invocation of enter/exit.
 *
 * If it is called by rcu_sync_enter() it signals that all the readers were
 * switched onto slow path.
 *
 * If it is called by rcu_sync_exit() it takes action based on events that
 * have taken place in the meantime, so that closely spaced rcu_sync_enter()
 * and rcu_sync_exit() pairs need not wait for a grace period.
 *
 * If another rcu_sync_enter() is invoked before the grace period
 * ended, reset state to allow the next rcu_sync_exit() to let the
 * readers back onto their fastpaths (after a grace period).  If both
 * another rcu_sync_enter() and its matching rcu_sync_exit() are invoked
 * before the grace period ended, re-invoke call_rcu() on behalf of that
 * rcu_sync_exit().  Otherwise, set all state back to idle so that readers
 * can again use their fastpaths.
 */
static void rcu_sync_func(struct rcu_head *rhp)
{
	struct rcu_sync *rsp = container_of(rhp, struct rcu_sync, cb_head);
	unsigned long flags;

	WARN_ON_ONCE(READ_ONCE(rsp->gp_state) == GP_IDLE);
	WARN_ON_ONCE(READ_ONCE(rsp->gp_state) == GP_PASSED);

	spin_lock_irqsave(&rsp->rss_lock, flags);
	if (rsp->gp_count) {
		/*
		 * We're at least a GP after the GP_IDLE->GP_ENTER transition.
		 */
		WRITE_ONCE(rsp->gp_state, GP_PASSED);
		wake_up_locked(&rsp->gp_wait);
	} else if (rsp->gp_state == GP_REPLAY) {
		/*
		 * A new rcu_sync_exit() has happened; requeue the callback to
		 * catch a later GP.
		 */
		WRITE_ONCE(rsp->gp_state, GP_EXIT);
		rcu_sync_call(rsp);
	} else {
		/*
		 * We're at least a GP after the last rcu_sync_exit(); everybody
		 * will now have observed the write side critical section.
		 * Let 'em rip!
		 */
		WRITE_ONCE(rsp->gp_state, GP_IDLE);
	}
	spin_unlock_irqrestore(&rsp->rss_lock, flags);
}

>>>>>>> upstream/android-13
/**
 * rcu_sync_enter() - Force readers onto slowpath
 * @rsp: Pointer to rcu_sync structure to use for synchronization
 *
 * This function is used by updaters who need readers to make use of
 * a slowpath during the update.  After this function returns, all
 * subsequent calls to rcu_sync_is_idle() will return false, which
 * tells readers to stay off their fastpaths.  A later call to
 * rcu_sync_exit() re-enables reader slowpaths.
 *
 * When called in isolation, rcu_sync_enter() must wait for a grace
 * period, however, closely spaced calls to rcu_sync_enter() can
 * optimize away the grace-period wait via a state machine implemented
 * by rcu_sync_enter(), rcu_sync_exit(), and rcu_sync_func().
 */
void rcu_sync_enter(struct rcu_sync *rsp)
{
<<<<<<< HEAD
	bool need_wait, need_sync;

	spin_lock_irq(&rsp->rss_lock);
	need_wait = rsp->gp_count++;
	need_sync = rsp->gp_state == GP_IDLE;
	if (need_sync)
		rsp->gp_state = GP_PENDING;
	spin_unlock_irq(&rsp->rss_lock);

	BUG_ON(need_wait && need_sync);

	if (need_sync) {
		gp_ops[rsp->gp_type].sync();
		rsp->gp_state = GP_PASSED;
		wake_up_all(&rsp->gp_wait);
	} else if (need_wait) {
		wait_event(rsp->gp_wait, rsp->gp_state == GP_PASSED);
	} else {
		/*
		 * Possible when there's a pending CB from a rcu_sync_exit().
		 * Nobody has yet been allowed the 'fast' path and thus we can
		 * avoid doing any sync(). The callback will get 'dropped'.
		 */
		BUG_ON(rsp->gp_state != GP_PASSED);
	}
}

/**
 * rcu_sync_func() - Callback function managing reader access to fastpath
 * @rhp: Pointer to rcu_head in rcu_sync structure to use for synchronization
 *
 * This function is passed to one of the call_rcu() functions by
 * rcu_sync_exit(), so that it is invoked after a grace period following the
 * that invocation of rcu_sync_exit().  It takes action based on events that
 * have taken place in the meantime, so that closely spaced rcu_sync_enter()
 * and rcu_sync_exit() pairs need not wait for a grace period.
 *
 * If another rcu_sync_enter() is invoked before the grace period
 * ended, reset state to allow the next rcu_sync_exit() to let the
 * readers back onto their fastpaths (after a grace period).  If both
 * another rcu_sync_enter() and its matching rcu_sync_exit() are invoked
 * before the grace period ended, re-invoke call_rcu() on behalf of that
 * rcu_sync_exit().  Otherwise, set all state back to idle so that readers
 * can again use their fastpaths.
 */
static void rcu_sync_func(struct rcu_head *rhp)
{
	struct rcu_sync *rsp = container_of(rhp, struct rcu_sync, cb_head);
	unsigned long flags;

	BUG_ON(rsp->gp_state != GP_PASSED);
	BUG_ON(rsp->cb_state == CB_IDLE);

	spin_lock_irqsave(&rsp->rss_lock, flags);
	if (rsp->gp_count) {
		/*
		 * A new rcu_sync_begin() has happened; drop the callback.
		 */
		rsp->cb_state = CB_IDLE;
	} else if (rsp->cb_state == CB_REPLAY) {
		/*
		 * A new rcu_sync_exit() has happened; requeue the callback
		 * to catch a later GP.
		 */
		rsp->cb_state = CB_PENDING;
		gp_ops[rsp->gp_type].call(&rsp->cb_head, rcu_sync_func);
	} else {
		/*
		 * We're at least a GP after rcu_sync_exit(); eveybody will now
		 * have observed the write side critical section. Let 'em rip!.
		 */
		rsp->cb_state = CB_IDLE;
		rsp->gp_state = GP_IDLE;
	}
	spin_unlock_irqrestore(&rsp->rss_lock, flags);
}

/**
 * rcu_sync_exit() - Allow readers back onto fast patch after grace period
=======
	int gp_state;

	spin_lock_irq(&rsp->rss_lock);
	gp_state = rsp->gp_state;
	if (gp_state == GP_IDLE) {
		WRITE_ONCE(rsp->gp_state, GP_ENTER);
		WARN_ON_ONCE(rsp->gp_count);
		/*
		 * Note that we could simply do rcu_sync_call(rsp) here and
		 * avoid the "if (gp_state == GP_IDLE)" block below.
		 *
		 * However, synchronize_rcu() can be faster if rcu_expedited
		 * or rcu_blocking_is_gp() is true.
		 *
		 * Another reason is that we can't wait for rcu callback if
		 * we are called at early boot time but this shouldn't happen.
		 */
	}
	rsp->gp_count++;
	spin_unlock_irq(&rsp->rss_lock);

	if (gp_state == GP_IDLE) {
		/*
		 * See the comment above, this simply does the "synchronous"
		 * call_rcu(rcu_sync_func) which does GP_ENTER -> GP_PASSED.
		 */
		synchronize_rcu();
		rcu_sync_func(&rsp->cb_head);
		/* Not really needed, wait_event() would see GP_PASSED. */
		return;
	}

	wait_event(rsp->gp_wait, READ_ONCE(rsp->gp_state) >= GP_PASSED);
}

/**
 * rcu_sync_exit() - Allow readers back onto fast path after grace period
>>>>>>> upstream/android-13
 * @rsp: Pointer to rcu_sync structure to use for synchronization
 *
 * This function is used by updaters who have completed, and can therefore
 * now allow readers to make use of their fastpaths after a grace period
 * has elapsed.  After this grace period has completed, all subsequent
 * calls to rcu_sync_is_idle() will return true, which tells readers that
 * they can once again use their fastpaths.
 */
void rcu_sync_exit(struct rcu_sync *rsp)
{
<<<<<<< HEAD
	spin_lock_irq(&rsp->rss_lock);
	if (!--rsp->gp_count) {
		if (rsp->cb_state == CB_IDLE) {
			rsp->cb_state = CB_PENDING;
			gp_ops[rsp->gp_type].call(&rsp->cb_head, rcu_sync_func);
		} else if (rsp->cb_state == CB_PENDING) {
			rsp->cb_state = CB_REPLAY;
=======
	WARN_ON_ONCE(READ_ONCE(rsp->gp_state) == GP_IDLE);
	WARN_ON_ONCE(READ_ONCE(rsp->gp_count) == 0);

	spin_lock_irq(&rsp->rss_lock);
	if (!--rsp->gp_count) {
		if (rsp->gp_state == GP_PASSED) {
			WRITE_ONCE(rsp->gp_state, GP_EXIT);
			rcu_sync_call(rsp);
		} else if (rsp->gp_state == GP_EXIT) {
			WRITE_ONCE(rsp->gp_state, GP_REPLAY);
>>>>>>> upstream/android-13
		}
	}
	spin_unlock_irq(&rsp->rss_lock);
}

/**
 * rcu_sync_dtor() - Clean up an rcu_sync structure
 * @rsp: Pointer to rcu_sync structure to be cleaned up
 */
void rcu_sync_dtor(struct rcu_sync *rsp)
{
<<<<<<< HEAD
	int cb_state;

	BUG_ON(rsp->gp_count);

	spin_lock_irq(&rsp->rss_lock);
	if (rsp->cb_state == CB_REPLAY)
		rsp->cb_state = CB_PENDING;
	cb_state = rsp->cb_state;
	spin_unlock_irq(&rsp->rss_lock);

	if (cb_state != CB_IDLE) {
		gp_ops[rsp->gp_type].wait();
		BUG_ON(rsp->cb_state != CB_IDLE);
=======
	int gp_state;

	WARN_ON_ONCE(READ_ONCE(rsp->gp_count));
	WARN_ON_ONCE(READ_ONCE(rsp->gp_state) == GP_PASSED);

	spin_lock_irq(&rsp->rss_lock);
	if (rsp->gp_state == GP_REPLAY)
		WRITE_ONCE(rsp->gp_state, GP_EXIT);
	gp_state = rsp->gp_state;
	spin_unlock_irq(&rsp->rss_lock);

	if (gp_state != GP_IDLE) {
		rcu_barrier();
		WARN_ON_ONCE(rsp->gp_state != GP_IDLE);
>>>>>>> upstream/android-13
	}
}
