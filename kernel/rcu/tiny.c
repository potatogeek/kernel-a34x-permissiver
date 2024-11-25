<<<<<<< HEAD
/*
 * Read-Copy Update mechanism for mutual exclusion, the Bloatwatch edition.
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
 * Copyright IBM Corporation, 2008
 *
 * Author: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * Read-Copy Update mechanism for mutual exclusion, the Bloatwatch edition.
 *
 * Copyright IBM Corporation, 2008
 *
 * Author: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *		Documentation/RCU
 */
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/rcupdate_wait.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/cpu.h>
#include <linux/prefetch.h>
<<<<<<< HEAD
=======
#include <linux/slab.h>
#include <linux/mm.h>
>>>>>>> upstream/android-13

#include "rcu.h"

/* Global control variables for rcupdate callback mechanism. */
struct rcu_ctrlblk {
	struct rcu_head *rcucblist;	/* List of pending callbacks (CBs). */
	struct rcu_head **donetail;	/* ->next pointer of last "done" CB. */
	struct rcu_head **curtail;	/* ->next pointer of last CB. */
<<<<<<< HEAD
};

/* Definition for rcupdate control block. */
static struct rcu_ctrlblk rcu_sched_ctrlblk = {
	.donetail	= &rcu_sched_ctrlblk.rcucblist,
	.curtail	= &rcu_sched_ctrlblk.rcucblist,
};

static struct rcu_ctrlblk rcu_bh_ctrlblk = {
	.donetail	= &rcu_bh_ctrlblk.rcucblist,
	.curtail	= &rcu_bh_ctrlblk.rcucblist,
};

void rcu_barrier_bh(void)
{
	wait_rcu_gp(call_rcu_bh);
}
EXPORT_SYMBOL(rcu_barrier_bh);

void rcu_barrier_sched(void)
{
	wait_rcu_gp(call_rcu_sched);
}
EXPORT_SYMBOL(rcu_barrier_sched);

/*
 * Helper function for rcu_sched_qs() and rcu_bh_qs().
 * Also irqs are disabled to avoid confusion due to interrupt handlers
 * invoking call_rcu().
 */
static int rcu_qsctr_help(struct rcu_ctrlblk *rcp)
{
	if (rcp->donetail != rcp->curtail) {
		rcp->donetail = rcp->curtail;
		return 1;
	}

	return 0;
}

/*
 * Record an rcu quiescent state.  And an rcu_bh quiescent state while we
 * are at it, given that any rcu quiescent state is also an rcu_bh
 * quiescent state.  Use "+" instead of "||" to defeat short circuiting.
 */
void rcu_sched_qs(void)
{
	unsigned long flags;

	local_irq_save(flags);
	if (rcu_qsctr_help(&rcu_sched_ctrlblk) +
	    rcu_qsctr_help(&rcu_bh_ctrlblk))
		raise_softirq(RCU_SOFTIRQ);
	local_irq_restore(flags);
}

/*
 * Record an rcu_bh quiescent state.
 */
void rcu_bh_qs(void)
{
	unsigned long flags;

	local_irq_save(flags);
	if (rcu_qsctr_help(&rcu_bh_ctrlblk))
		raise_softirq(RCU_SOFTIRQ);
=======
	unsigned long gp_seq;		/* Grace-period counter. */
};

/* Definition for rcupdate control block. */
static struct rcu_ctrlblk rcu_ctrlblk = {
	.donetail	= &rcu_ctrlblk.rcucblist,
	.curtail	= &rcu_ctrlblk.rcucblist,
	.gp_seq		= 0 - 300UL,
};

void rcu_barrier(void)
{
	wait_rcu_gp(call_rcu);
}
EXPORT_SYMBOL(rcu_barrier);

/* Record an rcu quiescent state.  */
void rcu_qs(void)
{
	unsigned long flags;

	local_irq_save(flags);
	if (rcu_ctrlblk.donetail != rcu_ctrlblk.curtail) {
		rcu_ctrlblk.donetail = rcu_ctrlblk.curtail;
		raise_softirq_irqoff(RCU_SOFTIRQ);
	}
	WRITE_ONCE(rcu_ctrlblk.gp_seq, rcu_ctrlblk.gp_seq + 1);
>>>>>>> upstream/android-13
	local_irq_restore(flags);
}

/*
 * Check to see if the scheduling-clock interrupt came from an extended
 * quiescent state, and, if so, tell RCU about it.  This function must
 * be called from hardirq context.  It is normally called from the
 * scheduling-clock interrupt.
 */
<<<<<<< HEAD
void rcu_check_callbacks(int user)
{
	if (user)
		rcu_sched_qs();
	if (user || !in_softirq())
		rcu_bh_qs();
}

/*
 * Invoke the RCU callbacks on the specified rcu_ctrlkblk structure
 * whose grace period has elapsed.
 */
static void __rcu_process_callbacks(struct rcu_ctrlblk *rcp)
=======
void rcu_sched_clock_irq(int user)
{
	if (user) {
		rcu_qs();
	} else if (rcu_ctrlblk.donetail != rcu_ctrlblk.curtail) {
		set_tsk_need_resched(current);
		set_preempt_need_resched();
	}
}

/*
 * Reclaim the specified callback, either by invoking it for non-kfree cases or
 * freeing it directly (for kfree). Return true if kfreeing, false otherwise.
 */
static inline bool rcu_reclaim_tiny(struct rcu_head *head)
{
	rcu_callback_t f;
	unsigned long offset = (unsigned long)head->func;

	rcu_lock_acquire(&rcu_callback_map);
	if (__is_kvfree_rcu_offset(offset)) {
		trace_rcu_invoke_kvfree_callback("", head, offset);
		kvfree((void *)head - offset);
		rcu_lock_release(&rcu_callback_map);
		return true;
	}

	trace_rcu_invoke_callback("", head);
	f = head->func;
	WRITE_ONCE(head->func, (rcu_callback_t)0L);
	f(head);
	rcu_lock_release(&rcu_callback_map);
	return false;
}

/* Invoke the RCU callbacks whose grace period has elapsed.  */
static __latent_entropy void rcu_process_callbacks(struct softirq_action *unused)
>>>>>>> upstream/android-13
{
	struct rcu_head *next, *list;
	unsigned long flags;

	/* Move the ready-to-invoke callbacks to a local list. */
	local_irq_save(flags);
<<<<<<< HEAD
	if (rcp->donetail == &rcp->rcucblist) {
=======
	if (rcu_ctrlblk.donetail == &rcu_ctrlblk.rcucblist) {
>>>>>>> upstream/android-13
		/* No callbacks ready, so just leave. */
		local_irq_restore(flags);
		return;
	}
<<<<<<< HEAD
	list = rcp->rcucblist;
	rcp->rcucblist = *rcp->donetail;
	*rcp->donetail = NULL;
	if (rcp->curtail == rcp->donetail)
		rcp->curtail = &rcp->rcucblist;
	rcp->donetail = &rcp->rcucblist;
=======
	list = rcu_ctrlblk.rcucblist;
	rcu_ctrlblk.rcucblist = *rcu_ctrlblk.donetail;
	*rcu_ctrlblk.donetail = NULL;
	if (rcu_ctrlblk.curtail == rcu_ctrlblk.donetail)
		rcu_ctrlblk.curtail = &rcu_ctrlblk.rcucblist;
	rcu_ctrlblk.donetail = &rcu_ctrlblk.rcucblist;
>>>>>>> upstream/android-13
	local_irq_restore(flags);

	/* Invoke the callbacks on the local list. */
	while (list) {
		next = list->next;
		prefetch(next);
		debug_rcu_head_unqueue(list);
		local_bh_disable();
<<<<<<< HEAD
		__rcu_reclaim("", list);
=======
		rcu_reclaim_tiny(list);
>>>>>>> upstream/android-13
		local_bh_enable();
		list = next;
	}
}

<<<<<<< HEAD
static __latent_entropy void rcu_process_callbacks(struct softirq_action *unused)
{
	__rcu_process_callbacks(&rcu_sched_ctrlblk);
	__rcu_process_callbacks(&rcu_bh_ctrlblk);
}

/*
 * Wait for a grace period to elapse.  But it is illegal to invoke
 * synchronize_sched() from within an RCU read-side critical section.
 * Therefore, any legal call to synchronize_sched() is a quiescent
 * state, and so on a UP system, synchronize_sched() need do nothing.
 * Ditto for synchronize_rcu_bh().  (But Lai Jiangshan points out the
 * benefits of doing might_sleep() to reduce latency.)
 *
 * Cool, huh?  (Due to Josh Triplett.)
 */
void synchronize_sched(void)
=======
/*
 * Wait for a grace period to elapse.  But it is illegal to invoke
 * synchronize_rcu() from within an RCU read-side critical section.
 * Therefore, any legal call to synchronize_rcu() is a quiescent
 * state, and so on a UP system, synchronize_rcu() need do nothing.
 * (But Lai Jiangshan points out the benefits of doing might_sleep()
 * to reduce latency.)
 *
 * Cool, huh?  (Due to Josh Triplett.)
 */
void synchronize_rcu(void)
>>>>>>> upstream/android-13
{
	RCU_LOCKDEP_WARN(lock_is_held(&rcu_bh_lock_map) ||
			 lock_is_held(&rcu_lock_map) ||
			 lock_is_held(&rcu_sched_lock_map),
<<<<<<< HEAD
			 "Illegal synchronize_sched() in RCU read-side critical section");
}
EXPORT_SYMBOL_GPL(synchronize_sched);

/*
 * Helper function for call_rcu() and call_rcu_bh().
 */
static void __call_rcu(struct rcu_head *head,
		       rcu_callback_t func,
		       struct rcu_ctrlblk *rcp)
=======
			 "Illegal synchronize_rcu() in RCU read-side critical section");
}
EXPORT_SYMBOL_GPL(synchronize_rcu);

/*
 * Post an RCU callback to be invoked after the end of an RCU grace
 * period.  But since we have but one CPU, that would be after any
 * quiescent state.
 */
void call_rcu(struct rcu_head *head, rcu_callback_t func)
>>>>>>> upstream/android-13
{
	unsigned long flags;

	debug_rcu_head_queue(head);
	head->func = func;
	head->next = NULL;

	local_irq_save(flags);
<<<<<<< HEAD
	*rcp->curtail = head;
	rcp->curtail = &head->next;
	local_irq_restore(flags);

	if (unlikely(is_idle_task(current))) {
		/* force scheduling for rcu_sched_qs() */
		resched_cpu(0);
	}
}

/*
 * Post an RCU callback to be invoked after the end of an RCU-sched grace
 * period.  But since we have but one CPU, that would be after any
 * quiescent state.
 */
void call_rcu_sched(struct rcu_head *head, rcu_callback_t func)
{
	__call_rcu(head, func, &rcu_sched_ctrlblk);
}
EXPORT_SYMBOL_GPL(call_rcu_sched);

/*
 * Post an RCU bottom-half callback to be invoked after any subsequent
 * quiescent state.
 */
void call_rcu_bh(struct rcu_head *head, rcu_callback_t func)
{
	__call_rcu(head, func, &rcu_bh_ctrlblk);
}
EXPORT_SYMBOL_GPL(call_rcu_bh);
=======
	*rcu_ctrlblk.curtail = head;
	rcu_ctrlblk.curtail = &head->next;
	local_irq_restore(flags);

	if (unlikely(is_idle_task(current))) {
		/* force scheduling for rcu_qs() */
		resched_cpu(0);
	}
}
EXPORT_SYMBOL_GPL(call_rcu);

/*
 * Return a grace-period-counter "cookie".  For more information,
 * see the Tree RCU header comment.
 */
unsigned long get_state_synchronize_rcu(void)
{
	return READ_ONCE(rcu_ctrlblk.gp_seq);
}
EXPORT_SYMBOL_GPL(get_state_synchronize_rcu);

/*
 * Return a grace-period-counter "cookie" and ensure that a future grace
 * period completes.  For more information, see the Tree RCU header comment.
 */
unsigned long start_poll_synchronize_rcu(void)
{
	unsigned long gp_seq = get_state_synchronize_rcu();

	if (unlikely(is_idle_task(current))) {
		/* force scheduling for rcu_qs() */
		resched_cpu(0);
	}
	return gp_seq;
}
EXPORT_SYMBOL_GPL(start_poll_synchronize_rcu);

/*
 * Return true if the grace period corresponding to oldstate has completed
 * and false otherwise.  For more information, see the Tree RCU header
 * comment.
 */
bool poll_state_synchronize_rcu(unsigned long oldstate)
{
	return READ_ONCE(rcu_ctrlblk.gp_seq) != oldstate;
}
EXPORT_SYMBOL_GPL(poll_state_synchronize_rcu);
>>>>>>> upstream/android-13

void __init rcu_init(void)
{
	open_softirq(RCU_SOFTIRQ, rcu_process_callbacks);
	rcu_early_boot_tests();
}
