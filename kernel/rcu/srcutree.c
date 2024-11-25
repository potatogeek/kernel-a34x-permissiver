<<<<<<< HEAD
/*
 * Sleepable Read-Copy Update mechanism for mutual exclusion.
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
 * Copyright (C) IBM Corporation, 2006
 * Copyright (C) Fujitsu, 2012
 *
 * Author: Paul McKenney <paulmck@us.ibm.com>
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * Sleepable Read-Copy Update mechanism for mutual exclusion.
 *
 * Copyright (C) IBM Corporation, 2006
 * Copyright (C) Fujitsu, 2012
 *
 * Authors: Paul McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13
 *	   Lai Jiangshan <laijs@cn.fujitsu.com>
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *		Documentation/RCU/ *.txt
 *
 */

#define pr_fmt(fmt) "rcu: " fmt

#include <linux/export.h>
#include <linux/mutex.h>
#include <linux/percpu.h>
#include <linux/preempt.h>
#include <linux/rcupdate_wait.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/srcu.h>

#include "rcu.h"
#include "rcu_segcblist.h"

/* Holdoff in nanoseconds for auto-expediting. */
#define DEFAULT_SRCU_EXP_HOLDOFF (25 * 1000)
static ulong exp_holdoff = DEFAULT_SRCU_EXP_HOLDOFF;
module_param(exp_holdoff, ulong, 0444);

/* Overflow-check frequency.  N bits roughly says every 2**N grace periods. */
static ulong counter_wrap_check = (ULONG_MAX >> 2);
module_param(counter_wrap_check, ulong, 0444);

<<<<<<< HEAD
static void srcu_invoke_callbacks(struct work_struct *work);
static void srcu_reschedule(struct srcu_struct *sp, unsigned long delay);
static void process_srcu(struct work_struct *work);
=======
/* Early-boot callback-management, so early that no lock is required! */
static LIST_HEAD(srcu_boot_list);
static bool __read_mostly srcu_init_done;

static void srcu_invoke_callbacks(struct work_struct *work);
static void srcu_reschedule(struct srcu_struct *ssp, unsigned long delay);
static void process_srcu(struct work_struct *work);
static void srcu_delay_timer(struct timer_list *t);
>>>>>>> upstream/android-13

/* Wrappers for lock acquisition and release, see raw_spin_lock_rcu_node(). */
#define spin_lock_rcu_node(p)					\
do {									\
	spin_lock(&ACCESS_PRIVATE(p, lock));			\
	smp_mb__after_unlock_lock();					\
} while (0)

#define spin_unlock_rcu_node(p) spin_unlock(&ACCESS_PRIVATE(p, lock))

#define spin_lock_irq_rcu_node(p)					\
do {									\
	spin_lock_irq(&ACCESS_PRIVATE(p, lock));			\
	smp_mb__after_unlock_lock();					\
} while (0)

#define spin_unlock_irq_rcu_node(p)					\
	spin_unlock_irq(&ACCESS_PRIVATE(p, lock))

#define spin_lock_irqsave_rcu_node(p, flags)			\
do {									\
	spin_lock_irqsave(&ACCESS_PRIVATE(p, lock), flags);	\
	smp_mb__after_unlock_lock();					\
} while (0)

#define spin_unlock_irqrestore_rcu_node(p, flags)			\
	spin_unlock_irqrestore(&ACCESS_PRIVATE(p, lock), flags)	\

/*
 * Initialize SRCU combining tree.  Note that statically allocated
 * srcu_struct structures might already have srcu_read_lock() and
 * srcu_read_unlock() running against them.  So if the is_static parameter
 * is set, don't initialize ->srcu_lock_count[] and ->srcu_unlock_count[].
 */
<<<<<<< HEAD
static void init_srcu_struct_nodes(struct srcu_struct *sp, bool is_static)
=======
static void init_srcu_struct_nodes(struct srcu_struct *ssp)
>>>>>>> upstream/android-13
{
	int cpu;
	int i;
	int level = 0;
	int levelspread[RCU_NUM_LVLS];
	struct srcu_data *sdp;
	struct srcu_node *snp;
	struct srcu_node *snp_first;

<<<<<<< HEAD
	/* Work out the overall tree geometry. */
	sp->level[0] = &sp->node[0];
	for (i = 1; i < rcu_num_lvls; i++)
		sp->level[i] = sp->level[i - 1] + num_rcu_lvl[i - 1];
	rcu_init_levelspread(levelspread, num_rcu_lvl);

	/* Each pass through this loop initializes one srcu_node structure. */
	rcu_for_each_node_breadth_first(sp, snp) {
=======
	/* Initialize geometry if it has not already been initialized. */
	rcu_init_geometry();

	/* Work out the overall tree geometry. */
	ssp->level[0] = &ssp->node[0];
	for (i = 1; i < rcu_num_lvls; i++)
		ssp->level[i] = ssp->level[i - 1] + num_rcu_lvl[i - 1];
	rcu_init_levelspread(levelspread, num_rcu_lvl);

	/* Each pass through this loop initializes one srcu_node structure. */
	srcu_for_each_node_breadth_first(ssp, snp) {
>>>>>>> upstream/android-13
		spin_lock_init(&ACCESS_PRIVATE(snp, lock));
		WARN_ON_ONCE(ARRAY_SIZE(snp->srcu_have_cbs) !=
			     ARRAY_SIZE(snp->srcu_data_have_cbs));
		for (i = 0; i < ARRAY_SIZE(snp->srcu_have_cbs); i++) {
			snp->srcu_have_cbs[i] = 0;
			snp->srcu_data_have_cbs[i] = 0;
		}
		snp->srcu_gp_seq_needed_exp = 0;
		snp->grplo = -1;
		snp->grphi = -1;
<<<<<<< HEAD
		if (snp == &sp->node[0]) {
=======
		if (snp == &ssp->node[0]) {
>>>>>>> upstream/android-13
			/* Root node, special case. */
			snp->srcu_parent = NULL;
			continue;
		}

		/* Non-root node. */
<<<<<<< HEAD
		if (snp == sp->level[level + 1])
			level++;
		snp->srcu_parent = sp->level[level - 1] +
				   (snp - sp->level[level]) /
=======
		if (snp == ssp->level[level + 1])
			level++;
		snp->srcu_parent = ssp->level[level - 1] +
				   (snp - ssp->level[level]) /
>>>>>>> upstream/android-13
				   levelspread[level - 1];
	}

	/*
	 * Initialize the per-CPU srcu_data array, which feeds into the
	 * leaves of the srcu_node tree.
	 */
	WARN_ON_ONCE(ARRAY_SIZE(sdp->srcu_lock_count) !=
		     ARRAY_SIZE(sdp->srcu_unlock_count));
	level = rcu_num_lvls - 1;
<<<<<<< HEAD
	snp_first = sp->level[level];
	for_each_possible_cpu(cpu) {
		sdp = per_cpu_ptr(sp->sda, cpu);
		spin_lock_init(&ACCESS_PRIVATE(sdp, lock));
		rcu_segcblist_init(&sdp->srcu_cblist);
		sdp->srcu_cblist_invoking = false;
		sdp->srcu_gp_seq_needed = sp->srcu_gp_seq;
		sdp->srcu_gp_seq_needed_exp = sp->srcu_gp_seq;
=======
	snp_first = ssp->level[level];
	for_each_possible_cpu(cpu) {
		sdp = per_cpu_ptr(ssp->sda, cpu);
		spin_lock_init(&ACCESS_PRIVATE(sdp, lock));
		rcu_segcblist_init(&sdp->srcu_cblist);
		sdp->srcu_cblist_invoking = false;
		sdp->srcu_gp_seq_needed = ssp->srcu_gp_seq;
		sdp->srcu_gp_seq_needed_exp = ssp->srcu_gp_seq;
>>>>>>> upstream/android-13
		sdp->mynode = &snp_first[cpu / levelspread[level]];
		for (snp = sdp->mynode; snp != NULL; snp = snp->srcu_parent) {
			if (snp->grplo < 0)
				snp->grplo = cpu;
			snp->grphi = cpu;
		}
		sdp->cpu = cpu;
<<<<<<< HEAD
		INIT_DELAYED_WORK(&sdp->work, srcu_invoke_callbacks);
		sdp->sp = sp;
		sdp->grpmask = 1 << (cpu - sdp->mynode->grplo);
		if (is_static)
			continue;

		/* Dynamically allocated, better be no srcu_read_locks()! */
		for (i = 0; i < ARRAY_SIZE(sdp->srcu_lock_count); i++) {
			sdp->srcu_lock_count[i] = 0;
			sdp->srcu_unlock_count[i] = 0;
		}
=======
		INIT_WORK(&sdp->work, srcu_invoke_callbacks);
		timer_setup(&sdp->delay_work, srcu_delay_timer, 0);
		sdp->ssp = ssp;
		sdp->grpmask = 1 << (cpu - sdp->mynode->grplo);
>>>>>>> upstream/android-13
	}
}

/*
 * Initialize non-compile-time initialized fields, including the
 * associated srcu_node and srcu_data structures.  The is_static
 * parameter is passed through to init_srcu_struct_nodes(), and
 * also tells us that ->sda has already been wired up to srcu_data.
 */
<<<<<<< HEAD
static int init_srcu_struct_fields(struct srcu_struct *sp, bool is_static)
{
	mutex_init(&sp->srcu_cb_mutex);
	mutex_init(&sp->srcu_gp_mutex);
	sp->srcu_idx = 0;
	sp->srcu_gp_seq = 0;
	sp->srcu_barrier_seq = 0;
	mutex_init(&sp->srcu_barrier_mutex);
	atomic_set(&sp->srcu_barrier_cpu_cnt, 0);
	INIT_DELAYED_WORK(&sp->work, process_srcu);
	if (!is_static)
		sp->sda = alloc_percpu(struct srcu_data);
	init_srcu_struct_nodes(sp, is_static);
	sp->srcu_gp_seq_needed_exp = 0;
	sp->srcu_last_gp_end = ktime_get_mono_fast_ns();
	smp_store_release(&sp->srcu_gp_seq_needed, 0); /* Init done. */
	return sp->sda ? 0 : -ENOMEM;
=======
static int init_srcu_struct_fields(struct srcu_struct *ssp, bool is_static)
{
	mutex_init(&ssp->srcu_cb_mutex);
	mutex_init(&ssp->srcu_gp_mutex);
	ssp->srcu_idx = 0;
	ssp->srcu_gp_seq = 0;
	ssp->srcu_barrier_seq = 0;
	mutex_init(&ssp->srcu_barrier_mutex);
	atomic_set(&ssp->srcu_barrier_cpu_cnt, 0);
	INIT_DELAYED_WORK(&ssp->work, process_srcu);
	if (!is_static)
		ssp->sda = alloc_percpu(struct srcu_data);
	if (!ssp->sda)
		return -ENOMEM;
	init_srcu_struct_nodes(ssp);
	ssp->srcu_gp_seq_needed_exp = 0;
	ssp->srcu_last_gp_end = ktime_get_mono_fast_ns();
	smp_store_release(&ssp->srcu_gp_seq_needed, 0); /* Init done. */
	return 0;
>>>>>>> upstream/android-13
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC

<<<<<<< HEAD
int __init_srcu_struct(struct srcu_struct *sp, const char *name,
		       struct lock_class_key *key)
{
	/* Don't re-initialize a lock while it is held. */
	debug_check_no_locks_freed((void *)sp, sizeof(*sp));
	lockdep_init_map(&sp->dep_map, name, key, 0);
	spin_lock_init(&ACCESS_PRIVATE(sp, lock));
	return init_srcu_struct_fields(sp, false);
=======
int __init_srcu_struct(struct srcu_struct *ssp, const char *name,
		       struct lock_class_key *key)
{
	/* Don't re-initialize a lock while it is held. */
	debug_check_no_locks_freed((void *)ssp, sizeof(*ssp));
	lockdep_init_map(&ssp->dep_map, name, key, 0);
	spin_lock_init(&ACCESS_PRIVATE(ssp, lock));
	return init_srcu_struct_fields(ssp, false);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(__init_srcu_struct);

#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */

/**
 * init_srcu_struct - initialize a sleep-RCU structure
<<<<<<< HEAD
 * @sp: structure to initialize.
=======
 * @ssp: structure to initialize.
>>>>>>> upstream/android-13
 *
 * Must invoke this on a given srcu_struct before passing that srcu_struct
 * to any other function.  Each srcu_struct represents a separate domain
 * of SRCU protection.
 */
<<<<<<< HEAD
int init_srcu_struct(struct srcu_struct *sp)
{
	spin_lock_init(&ACCESS_PRIVATE(sp, lock));
	return init_srcu_struct_fields(sp, false);
=======
int init_srcu_struct(struct srcu_struct *ssp)
{
	spin_lock_init(&ACCESS_PRIVATE(ssp, lock));
	return init_srcu_struct_fields(ssp, false);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(init_srcu_struct);

#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

/*
 * First-use initialization of statically allocated srcu_struct
 * structure.  Wiring up the combining tree is more than can be
 * done with compile-time initialization, so this check is added
<<<<<<< HEAD
 * to each update-side SRCU primitive.  Use sp->lock, which -is-
 * compile-time initialized, to resolve races involving multiple
 * CPUs trying to garner first-use privileges.
 */
static void check_init_srcu_struct(struct srcu_struct *sp)
{
	unsigned long flags;

	WARN_ON_ONCE(rcu_scheduler_active == RCU_SCHEDULER_INIT);
	/* The smp_load_acquire() pairs with the smp_store_release(). */
	if (!rcu_seq_state(smp_load_acquire(&sp->srcu_gp_seq_needed))) /*^^^*/
		return; /* Already initialized. */
	spin_lock_irqsave_rcu_node(sp, flags);
	if (!rcu_seq_state(sp->srcu_gp_seq_needed)) {
		spin_unlock_irqrestore_rcu_node(sp, flags);
		return;
	}
	init_srcu_struct_fields(sp, true);
	spin_unlock_irqrestore_rcu_node(sp, flags);
=======
 * to each update-side SRCU primitive.  Use ssp->lock, which -is-
 * compile-time initialized, to resolve races involving multiple
 * CPUs trying to garner first-use privileges.
 */
static void check_init_srcu_struct(struct srcu_struct *ssp)
{
	unsigned long flags;

	/* The smp_load_acquire() pairs with the smp_store_release(). */
	if (!rcu_seq_state(smp_load_acquire(&ssp->srcu_gp_seq_needed))) /*^^^*/
		return; /* Already initialized. */
	spin_lock_irqsave_rcu_node(ssp, flags);
	if (!rcu_seq_state(ssp->srcu_gp_seq_needed)) {
		spin_unlock_irqrestore_rcu_node(ssp, flags);
		return;
	}
	init_srcu_struct_fields(ssp, true);
	spin_unlock_irqrestore_rcu_node(ssp, flags);
>>>>>>> upstream/android-13
}

/*
 * Returns approximate total of the readers' ->srcu_lock_count[] values
 * for the rank of per-CPU counters specified by idx.
 */
<<<<<<< HEAD
static unsigned long srcu_readers_lock_idx(struct srcu_struct *sp, int idx)
=======
static unsigned long srcu_readers_lock_idx(struct srcu_struct *ssp, int idx)
>>>>>>> upstream/android-13
{
	int cpu;
	unsigned long sum = 0;

	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		struct srcu_data *cpuc = per_cpu_ptr(sp->sda, cpu);
=======
		struct srcu_data *cpuc = per_cpu_ptr(ssp->sda, cpu);
>>>>>>> upstream/android-13

		sum += READ_ONCE(cpuc->srcu_lock_count[idx]);
	}
	return sum;
}

/*
 * Returns approximate total of the readers' ->srcu_unlock_count[] values
 * for the rank of per-CPU counters specified by idx.
 */
<<<<<<< HEAD
static unsigned long srcu_readers_unlock_idx(struct srcu_struct *sp, int idx)
=======
static unsigned long srcu_readers_unlock_idx(struct srcu_struct *ssp, int idx)
>>>>>>> upstream/android-13
{
	int cpu;
	unsigned long sum = 0;

	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		struct srcu_data *cpuc = per_cpu_ptr(sp->sda, cpu);
=======
		struct srcu_data *cpuc = per_cpu_ptr(ssp->sda, cpu);
>>>>>>> upstream/android-13

		sum += READ_ONCE(cpuc->srcu_unlock_count[idx]);
	}
	return sum;
}

/*
 * Return true if the number of pre-existing readers is determined to
 * be zero.
 */
<<<<<<< HEAD
static bool srcu_readers_active_idx_check(struct srcu_struct *sp, int idx)
{
	unsigned long unlocks;

	unlocks = srcu_readers_unlock_idx(sp, idx);
=======
static bool srcu_readers_active_idx_check(struct srcu_struct *ssp, int idx)
{
	unsigned long unlocks;

	unlocks = srcu_readers_unlock_idx(ssp, idx);
>>>>>>> upstream/android-13

	/*
	 * Make sure that a lock is always counted if the corresponding
	 * unlock is counted. Needs to be a smp_mb() as the read side may
	 * contain a read from a variable that is written to before the
	 * synchronize_srcu() in the write side. In this case smp_mb()s
	 * A and B act like the store buffering pattern.
	 *
	 * This smp_mb() also pairs with smp_mb() C to prevent accesses
	 * after the synchronize_srcu() from being executed before the
	 * grace period ends.
	 */
	smp_mb(); /* A */

	/*
	 * If the locks are the same as the unlocks, then there must have
	 * been no readers on this index at some time in between. This does
	 * not mean that there are no more readers, as one could have read
	 * the current index but not have incremented the lock counter yet.
	 *
	 * So suppose that the updater is preempted here for so long
	 * that more than ULONG_MAX non-nested readers come and go in
	 * the meantime.  It turns out that this cannot result in overflow
	 * because if a reader modifies its unlock count after we read it
	 * above, then that reader's next load of ->srcu_idx is guaranteed
	 * to get the new value, which will cause it to operate on the
	 * other bank of counters, where it cannot contribute to the
	 * overflow of these counters.  This means that there is a maximum
	 * of 2*NR_CPUS increments, which cannot overflow given current
	 * systems, especially not on 64-bit systems.
	 *
	 * OK, how about nesting?  This does impose a limit on nesting
	 * of floor(ULONG_MAX/NR_CPUS/2), which should be sufficient,
	 * especially on 64-bit systems.
	 */
<<<<<<< HEAD
	return srcu_readers_lock_idx(sp, idx) == unlocks;
=======
	return srcu_readers_lock_idx(ssp, idx) == unlocks;
>>>>>>> upstream/android-13
}

/**
 * srcu_readers_active - returns true if there are readers. and false
 *                       otherwise
<<<<<<< HEAD
 * @sp: which srcu_struct to count active readers (holding srcu_read_lock).
=======
 * @ssp: which srcu_struct to count active readers (holding srcu_read_lock).
>>>>>>> upstream/android-13
 *
 * Note that this is not an atomic primitive, and can therefore suffer
 * severe errors when invoked on an active srcu_struct.  That said, it
 * can be useful as an error check at cleanup time.
 */
<<<<<<< HEAD
static bool srcu_readers_active(struct srcu_struct *sp)
=======
static bool srcu_readers_active(struct srcu_struct *ssp)
>>>>>>> upstream/android-13
{
	int cpu;
	unsigned long sum = 0;

	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		struct srcu_data *cpuc = per_cpu_ptr(sp->sda, cpu);
=======
		struct srcu_data *cpuc = per_cpu_ptr(ssp->sda, cpu);
>>>>>>> upstream/android-13

		sum += READ_ONCE(cpuc->srcu_lock_count[0]);
		sum += READ_ONCE(cpuc->srcu_lock_count[1]);
		sum -= READ_ONCE(cpuc->srcu_unlock_count[0]);
		sum -= READ_ONCE(cpuc->srcu_unlock_count[1]);
	}
	return sum;
}

#define SRCU_INTERVAL		1

/*
 * Return grace-period delay, zero if there are expedited grace
 * periods pending, SRCU_INTERVAL otherwise.
 */
<<<<<<< HEAD
static unsigned long srcu_get_delay(struct srcu_struct *sp)
{
	if (ULONG_CMP_LT(READ_ONCE(sp->srcu_gp_seq),
			 READ_ONCE(sp->srcu_gp_seq_needed_exp)))
=======
static unsigned long srcu_get_delay(struct srcu_struct *ssp)
{
	if (ULONG_CMP_LT(READ_ONCE(ssp->srcu_gp_seq),
			 READ_ONCE(ssp->srcu_gp_seq_needed_exp)))
>>>>>>> upstream/android-13
		return 0;
	return SRCU_INTERVAL;
}

<<<<<<< HEAD
/* Helper for cleanup_srcu_struct() and cleanup_srcu_struct_quiesced(). */
void _cleanup_srcu_struct(struct srcu_struct *sp, bool quiesced)
{
	int cpu;

	if (WARN_ON(!srcu_get_delay(sp)))
		return; /* Just leak it! */
	if (WARN_ON(srcu_readers_active(sp)))
		return; /* Just leak it! */
	if (quiesced) {
		if (WARN_ON(delayed_work_pending(&sp->work)))
			return; /* Just leak it! */
	} else {
		flush_delayed_work(&sp->work);
	}
	for_each_possible_cpu(cpu)
		if (quiesced) {
			if (WARN_ON(delayed_work_pending(&per_cpu_ptr(sp->sda, cpu)->work)))
				return; /* Just leak it! */
		} else {
			flush_delayed_work(&per_cpu_ptr(sp->sda, cpu)->work);
		}
	if (WARN_ON(rcu_seq_state(READ_ONCE(sp->srcu_gp_seq)) != SRCU_STATE_IDLE) ||
	    WARN_ON(srcu_readers_active(sp))) {
		pr_info("%s: Active srcu_struct %p state: %d\n",
			__func__, sp, rcu_seq_state(READ_ONCE(sp->srcu_gp_seq)));
		return; /* Caller forgot to stop doing call_srcu()? */
	}
	free_percpu(sp->sda);
	sp->sda = NULL;
}
EXPORT_SYMBOL_GPL(_cleanup_srcu_struct);
=======
/**
 * cleanup_srcu_struct - deconstruct a sleep-RCU structure
 * @ssp: structure to clean up.
 *
 * Must invoke this after you are finished using a given srcu_struct that
 * was initialized via init_srcu_struct(), else you leak memory.
 */
void cleanup_srcu_struct(struct srcu_struct *ssp)
{
	int cpu;

	if (WARN_ON(!srcu_get_delay(ssp)))
		return; /* Just leak it! */
	if (WARN_ON(srcu_readers_active(ssp)))
		return; /* Just leak it! */
	flush_delayed_work(&ssp->work);
	for_each_possible_cpu(cpu) {
		struct srcu_data *sdp = per_cpu_ptr(ssp->sda, cpu);

		del_timer_sync(&sdp->delay_work);
		flush_work(&sdp->work);
		if (WARN_ON(rcu_segcblist_n_cbs(&sdp->srcu_cblist)))
			return; /* Forgot srcu_barrier(), so just leak it! */
	}
	if (WARN_ON(rcu_seq_state(READ_ONCE(ssp->srcu_gp_seq)) != SRCU_STATE_IDLE) ||
	    WARN_ON(srcu_readers_active(ssp))) {
		pr_info("%s: Active srcu_struct %p state: %d\n",
			__func__, ssp, rcu_seq_state(READ_ONCE(ssp->srcu_gp_seq)));
		return; /* Caller forgot to stop doing call_srcu()? */
	}
	free_percpu(ssp->sda);
	ssp->sda = NULL;
}
EXPORT_SYMBOL_GPL(cleanup_srcu_struct);
>>>>>>> upstream/android-13

/*
 * Counts the new reader in the appropriate per-CPU element of the
 * srcu_struct.
 * Returns an index that must be passed to the matching srcu_read_unlock().
 */
<<<<<<< HEAD
int __srcu_read_lock(struct srcu_struct *sp)
{
	int idx;

	idx = READ_ONCE(sp->srcu_idx) & 0x1;
	this_cpu_inc(sp->sda->srcu_lock_count[idx]);
=======
int __srcu_read_lock(struct srcu_struct *ssp)
{
	int idx;

	idx = READ_ONCE(ssp->srcu_idx) & 0x1;
	this_cpu_inc(ssp->sda->srcu_lock_count[idx]);
>>>>>>> upstream/android-13
	smp_mb(); /* B */  /* Avoid leaking the critical section. */
	return idx;
}
EXPORT_SYMBOL_GPL(__srcu_read_lock);

/*
 * Removes the count for the old reader from the appropriate per-CPU
 * element of the srcu_struct.  Note that this may well be a different
 * CPU than that which was incremented by the corresponding srcu_read_lock().
 */
<<<<<<< HEAD
void __srcu_read_unlock(struct srcu_struct *sp, int idx)
{
	smp_mb(); /* C */  /* Avoid leaking the critical section. */
	this_cpu_inc(sp->sda->srcu_unlock_count[idx]);
=======
void __srcu_read_unlock(struct srcu_struct *ssp, int idx)
{
	smp_mb(); /* C */  /* Avoid leaking the critical section. */
	this_cpu_inc(ssp->sda->srcu_unlock_count[idx]);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(__srcu_read_unlock);

/*
 * We use an adaptive strategy for synchronize_srcu() and especially for
 * synchronize_srcu_expedited().  We spin for a fixed time period
 * (defined below) to allow SRCU readers to exit their read-side critical
 * sections.  If there are still some readers after a few microseconds,
 * we repeatedly block for 1-millisecond time periods.
 */
#define SRCU_RETRY_CHECK_DELAY		5

/*
 * Start an SRCU grace period.
 */
<<<<<<< HEAD
static void srcu_gp_start(struct srcu_struct *sp)
{
	struct srcu_data *sdp = this_cpu_ptr(sp->sda);
	int state;

	lockdep_assert_held(&ACCESS_PRIVATE(sp, lock));
	WARN_ON_ONCE(ULONG_CMP_GE(sp->srcu_gp_seq, sp->srcu_gp_seq_needed));
	spin_lock_rcu_node(sdp);  /* Interrupts already disabled. */
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&sp->srcu_gp_seq));
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist,
				       rcu_seq_snap(&sp->srcu_gp_seq));
	spin_unlock_rcu_node(sdp);  /* Interrupts remain disabled. */
	smp_mb(); /* Order prior store to ->srcu_gp_seq_needed vs. GP start. */
	rcu_seq_start(&sp->srcu_gp_seq);
	state = rcu_seq_state(READ_ONCE(sp->srcu_gp_seq));
	WARN_ON_ONCE(state != SRCU_STATE_SCAN1);
}

/*
 * Track online CPUs to guide callback workqueue placement.
 */
DEFINE_PER_CPU(bool, srcu_online);

void srcu_online_cpu(unsigned int cpu)
{
	WRITE_ONCE(per_cpu(srcu_online, cpu), true);
}

void srcu_offline_cpu(unsigned int cpu)
{
	WRITE_ONCE(per_cpu(srcu_online, cpu), false);
}

/*
 * Place the workqueue handler on the specified CPU if online, otherwise
 * just run it whereever.  This is useful for placing workqueue handlers
 * that are to invoke the specified CPU's callbacks.
 */
static bool srcu_queue_delayed_work_on(int cpu, struct workqueue_struct *wq,
				       struct delayed_work *dwork,
				       unsigned long delay)
{
	bool ret;

	preempt_disable();
	if (READ_ONCE(per_cpu(srcu_online, cpu)))
		ret = queue_delayed_work_on(cpu, wq, dwork, delay);
	else
		ret = queue_delayed_work(wq, dwork, delay);
	preempt_enable();
	return ret;
=======
static void srcu_gp_start(struct srcu_struct *ssp)
{
	struct srcu_data *sdp = this_cpu_ptr(ssp->sda);
	int state;

	lockdep_assert_held(&ACCESS_PRIVATE(ssp, lock));
	WARN_ON_ONCE(ULONG_CMP_GE(ssp->srcu_gp_seq, ssp->srcu_gp_seq_needed));
	spin_lock_rcu_node(sdp);  /* Interrupts already disabled. */
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&ssp->srcu_gp_seq));
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist,
				       rcu_seq_snap(&ssp->srcu_gp_seq));
	spin_unlock_rcu_node(sdp);  /* Interrupts remain disabled. */
	smp_mb(); /* Order prior store to ->srcu_gp_seq_needed vs. GP start. */
	rcu_seq_start(&ssp->srcu_gp_seq);
	state = rcu_seq_state(ssp->srcu_gp_seq);
	WARN_ON_ONCE(state != SRCU_STATE_SCAN1);
}


static void srcu_delay_timer(struct timer_list *t)
{
	struct srcu_data *sdp = container_of(t, struct srcu_data, delay_work);

	queue_work_on(sdp->cpu, rcu_gp_wq, &sdp->work);
}

static void srcu_queue_delayed_work_on(struct srcu_data *sdp,
				       unsigned long delay)
{
	if (!delay) {
		queue_work_on(sdp->cpu, rcu_gp_wq, &sdp->work);
		return;
	}

	timer_reduce(&sdp->delay_work, jiffies + delay);
>>>>>>> upstream/android-13
}

/*
 * Schedule callback invocation for the specified srcu_data structure,
 * if possible, on the corresponding CPU.
 */
static void srcu_schedule_cbs_sdp(struct srcu_data *sdp, unsigned long delay)
{
<<<<<<< HEAD
	srcu_queue_delayed_work_on(sdp->cpu, rcu_gp_wq, &sdp->work, delay);
=======
	srcu_queue_delayed_work_on(sdp, delay);
>>>>>>> upstream/android-13
}

/*
 * Schedule callback invocation for all srcu_data structures associated
 * with the specified srcu_node structure that have callbacks for the
 * just-completed grace period, the one corresponding to idx.  If possible,
 * schedule this invocation on the corresponding CPUs.
 */
<<<<<<< HEAD
static void srcu_schedule_cbs_snp(struct srcu_struct *sp, struct srcu_node *snp,
=======
static void srcu_schedule_cbs_snp(struct srcu_struct *ssp, struct srcu_node *snp,
>>>>>>> upstream/android-13
				  unsigned long mask, unsigned long delay)
{
	int cpu;

	for (cpu = snp->grplo; cpu <= snp->grphi; cpu++) {
		if (!(mask & (1 << (cpu - snp->grplo))))
			continue;
<<<<<<< HEAD
		srcu_schedule_cbs_sdp(per_cpu_ptr(sp->sda, cpu), delay);
=======
		srcu_schedule_cbs_sdp(per_cpu_ptr(ssp->sda, cpu), delay);
>>>>>>> upstream/android-13
	}
}

/*
 * Note the end of an SRCU grace period.  Initiates callback invocation
 * and starts a new grace period if needed.
 *
 * The ->srcu_cb_mutex acquisition does not protect any data, but
 * instead prevents more than one grace period from starting while we
 * are initiating callback invocation.  This allows the ->srcu_have_cbs[]
 * array to have a finite number of elements.
 */
<<<<<<< HEAD
static void srcu_gp_end(struct srcu_struct *sp)
=======
static void srcu_gp_end(struct srcu_struct *ssp)
>>>>>>> upstream/android-13
{
	unsigned long cbdelay;
	bool cbs;
	bool last_lvl;
	int cpu;
	unsigned long flags;
	unsigned long gpseq;
	int idx;
	unsigned long mask;
	struct srcu_data *sdp;
	struct srcu_node *snp;

	/* Prevent more than one additional grace period. */
<<<<<<< HEAD
	mutex_lock(&sp->srcu_cb_mutex);

	/* End the current grace period. */
	spin_lock_irq_rcu_node(sp);
	idx = rcu_seq_state(sp->srcu_gp_seq);
	WARN_ON_ONCE(idx != SRCU_STATE_SCAN2);
	cbdelay = srcu_get_delay(sp);
	sp->srcu_last_gp_end = ktime_get_mono_fast_ns();
	rcu_seq_end(&sp->srcu_gp_seq);
	gpseq = rcu_seq_current(&sp->srcu_gp_seq);
	if (ULONG_CMP_LT(sp->srcu_gp_seq_needed_exp, gpseq))
		sp->srcu_gp_seq_needed_exp = gpseq;
	spin_unlock_irq_rcu_node(sp);
	mutex_unlock(&sp->srcu_gp_mutex);
=======
	mutex_lock(&ssp->srcu_cb_mutex);

	/* End the current grace period. */
	spin_lock_irq_rcu_node(ssp);
	idx = rcu_seq_state(ssp->srcu_gp_seq);
	WARN_ON_ONCE(idx != SRCU_STATE_SCAN2);
	cbdelay = srcu_get_delay(ssp);
	WRITE_ONCE(ssp->srcu_last_gp_end, ktime_get_mono_fast_ns());
	rcu_seq_end(&ssp->srcu_gp_seq);
	gpseq = rcu_seq_current(&ssp->srcu_gp_seq);
	if (ULONG_CMP_LT(ssp->srcu_gp_seq_needed_exp, gpseq))
		WRITE_ONCE(ssp->srcu_gp_seq_needed_exp, gpseq);
	spin_unlock_irq_rcu_node(ssp);
	mutex_unlock(&ssp->srcu_gp_mutex);
>>>>>>> upstream/android-13
	/* A new grace period can start at this point.  But only one. */

	/* Initiate callback invocation as needed. */
	idx = rcu_seq_ctr(gpseq) % ARRAY_SIZE(snp->srcu_have_cbs);
<<<<<<< HEAD
	rcu_for_each_node_breadth_first(sp, snp) {
		spin_lock_irq_rcu_node(snp);
		cbs = false;
		last_lvl = snp >= sp->level[rcu_num_lvls - 1];
=======
	srcu_for_each_node_breadth_first(ssp, snp) {
		spin_lock_irq_rcu_node(snp);
		cbs = false;
		last_lvl = snp >= ssp->level[rcu_num_lvls - 1];
>>>>>>> upstream/android-13
		if (last_lvl)
			cbs = snp->srcu_have_cbs[idx] == gpseq;
		snp->srcu_have_cbs[idx] = gpseq;
		rcu_seq_set_state(&snp->srcu_have_cbs[idx], 1);
		if (ULONG_CMP_LT(snp->srcu_gp_seq_needed_exp, gpseq))
<<<<<<< HEAD
			snp->srcu_gp_seq_needed_exp = gpseq;
=======
			WRITE_ONCE(snp->srcu_gp_seq_needed_exp, gpseq);
>>>>>>> upstream/android-13
		mask = snp->srcu_data_have_cbs[idx];
		snp->srcu_data_have_cbs[idx] = 0;
		spin_unlock_irq_rcu_node(snp);
		if (cbs)
<<<<<<< HEAD
			srcu_schedule_cbs_snp(sp, snp, mask, cbdelay);
=======
			srcu_schedule_cbs_snp(ssp, snp, mask, cbdelay);
>>>>>>> upstream/android-13

		/* Occasionally prevent srcu_data counter wrap. */
		if (!(gpseq & counter_wrap_check) && last_lvl)
			for (cpu = snp->grplo; cpu <= snp->grphi; cpu++) {
<<<<<<< HEAD
				sdp = per_cpu_ptr(sp->sda, cpu);
=======
				sdp = per_cpu_ptr(ssp->sda, cpu);
>>>>>>> upstream/android-13
				spin_lock_irqsave_rcu_node(sdp, flags);
				if (ULONG_CMP_GE(gpseq,
						 sdp->srcu_gp_seq_needed + 100))
					sdp->srcu_gp_seq_needed = gpseq;
				if (ULONG_CMP_GE(gpseq,
						 sdp->srcu_gp_seq_needed_exp + 100))
					sdp->srcu_gp_seq_needed_exp = gpseq;
				spin_unlock_irqrestore_rcu_node(sdp, flags);
			}
	}

	/* Callback initiation done, allow grace periods after next. */
<<<<<<< HEAD
	mutex_unlock(&sp->srcu_cb_mutex);

	/* Start a new grace period if needed. */
	spin_lock_irq_rcu_node(sp);
	gpseq = rcu_seq_current(&sp->srcu_gp_seq);
	if (!rcu_seq_state(gpseq) &&
	    ULONG_CMP_LT(gpseq, sp->srcu_gp_seq_needed)) {
		srcu_gp_start(sp);
		spin_unlock_irq_rcu_node(sp);
		srcu_reschedule(sp, 0);
	} else {
		spin_unlock_irq_rcu_node(sp);
=======
	mutex_unlock(&ssp->srcu_cb_mutex);

	/* Start a new grace period if needed. */
	spin_lock_irq_rcu_node(ssp);
	gpseq = rcu_seq_current(&ssp->srcu_gp_seq);
	if (!rcu_seq_state(gpseq) &&
	    ULONG_CMP_LT(gpseq, ssp->srcu_gp_seq_needed)) {
		srcu_gp_start(ssp);
		spin_unlock_irq_rcu_node(ssp);
		srcu_reschedule(ssp, 0);
	} else {
		spin_unlock_irq_rcu_node(ssp);
>>>>>>> upstream/android-13
	}
}

/*
 * Funnel-locking scheme to scalably mediate many concurrent expedited
 * grace-period requests.  This function is invoked for the first known
 * expedited request for a grace period that has already been requested,
 * but without expediting.  To start a completely new grace period,
 * whether expedited or not, use srcu_funnel_gp_start() instead.
 */
<<<<<<< HEAD
static void srcu_funnel_exp_start(struct srcu_struct *sp, struct srcu_node *snp,
=======
static void srcu_funnel_exp_start(struct srcu_struct *ssp, struct srcu_node *snp,
>>>>>>> upstream/android-13
				  unsigned long s)
{
	unsigned long flags;

	for (; snp != NULL; snp = snp->srcu_parent) {
<<<<<<< HEAD
		if (rcu_seq_done(&sp->srcu_gp_seq, s) ||
=======
		if (rcu_seq_done(&ssp->srcu_gp_seq, s) ||
>>>>>>> upstream/android-13
		    ULONG_CMP_GE(READ_ONCE(snp->srcu_gp_seq_needed_exp), s))
			return;
		spin_lock_irqsave_rcu_node(snp, flags);
		if (ULONG_CMP_GE(snp->srcu_gp_seq_needed_exp, s)) {
			spin_unlock_irqrestore_rcu_node(snp, flags);
			return;
		}
		WRITE_ONCE(snp->srcu_gp_seq_needed_exp, s);
		spin_unlock_irqrestore_rcu_node(snp, flags);
	}
<<<<<<< HEAD
	spin_lock_irqsave_rcu_node(sp, flags);
	if (ULONG_CMP_LT(sp->srcu_gp_seq_needed_exp, s))
		sp->srcu_gp_seq_needed_exp = s;
	spin_unlock_irqrestore_rcu_node(sp, flags);
=======
	spin_lock_irqsave_rcu_node(ssp, flags);
	if (ULONG_CMP_LT(ssp->srcu_gp_seq_needed_exp, s))
		WRITE_ONCE(ssp->srcu_gp_seq_needed_exp, s);
	spin_unlock_irqrestore_rcu_node(ssp, flags);
>>>>>>> upstream/android-13
}

/*
 * Funnel-locking scheme to scalably mediate many concurrent grace-period
 * requests.  The winner has to do the work of actually starting grace
 * period s.  Losers must either ensure that their desired grace-period
 * number is recorded on at least their leaf srcu_node structure, or they
 * must take steps to invoke their own callbacks.
 *
 * Note that this function also does the work of srcu_funnel_exp_start(),
 * in some cases by directly invoking it.
 */
<<<<<<< HEAD
static void srcu_funnel_gp_start(struct srcu_struct *sp, struct srcu_data *sdp,
=======
static void srcu_funnel_gp_start(struct srcu_struct *ssp, struct srcu_data *sdp,
>>>>>>> upstream/android-13
				 unsigned long s, bool do_norm)
{
	unsigned long flags;
	int idx = rcu_seq_ctr(s) % ARRAY_SIZE(sdp->mynode->srcu_have_cbs);
	struct srcu_node *snp = sdp->mynode;
	unsigned long snp_seq;

	/* Each pass through the loop does one level of the srcu_node tree. */
	for (; snp != NULL; snp = snp->srcu_parent) {
<<<<<<< HEAD
		if (rcu_seq_done(&sp->srcu_gp_seq, s) && snp != sdp->mynode)
=======
		if (rcu_seq_done(&ssp->srcu_gp_seq, s) && snp != sdp->mynode)
>>>>>>> upstream/android-13
			return; /* GP already done and CBs recorded. */
		spin_lock_irqsave_rcu_node(snp, flags);
		if (ULONG_CMP_GE(snp->srcu_have_cbs[idx], s)) {
			snp_seq = snp->srcu_have_cbs[idx];
			if (snp == sdp->mynode && snp_seq == s)
				snp->srcu_data_have_cbs[idx] |= sdp->grpmask;
			spin_unlock_irqrestore_rcu_node(snp, flags);
			if (snp == sdp->mynode && snp_seq != s) {
				srcu_schedule_cbs_sdp(sdp, do_norm
							   ? SRCU_INTERVAL
							   : 0);
				return;
			}
			if (!do_norm)
<<<<<<< HEAD
				srcu_funnel_exp_start(sp, snp, s);
=======
				srcu_funnel_exp_start(ssp, snp, s);
>>>>>>> upstream/android-13
			return;
		}
		snp->srcu_have_cbs[idx] = s;
		if (snp == sdp->mynode)
			snp->srcu_data_have_cbs[idx] |= sdp->grpmask;
		if (!do_norm && ULONG_CMP_LT(snp->srcu_gp_seq_needed_exp, s))
<<<<<<< HEAD
			snp->srcu_gp_seq_needed_exp = s;
=======
			WRITE_ONCE(snp->srcu_gp_seq_needed_exp, s);
>>>>>>> upstream/android-13
		spin_unlock_irqrestore_rcu_node(snp, flags);
	}

	/* Top of tree, must ensure the grace period will be started. */
<<<<<<< HEAD
	spin_lock_irqsave_rcu_node(sp, flags);
	if (ULONG_CMP_LT(sp->srcu_gp_seq_needed, s)) {
=======
	spin_lock_irqsave_rcu_node(ssp, flags);
	if (ULONG_CMP_LT(ssp->srcu_gp_seq_needed, s)) {
>>>>>>> upstream/android-13
		/*
		 * Record need for grace period s.  Pair with load
		 * acquire setting up for initialization.
		 */
<<<<<<< HEAD
		smp_store_release(&sp->srcu_gp_seq_needed, s); /*^^^*/
	}
	if (!do_norm && ULONG_CMP_LT(sp->srcu_gp_seq_needed_exp, s))
		sp->srcu_gp_seq_needed_exp = s;

	/* If grace period not already done and none in progress, start it. */
	if (!rcu_seq_done(&sp->srcu_gp_seq, s) &&
	    rcu_seq_state(sp->srcu_gp_seq) == SRCU_STATE_IDLE) {
		WARN_ON_ONCE(ULONG_CMP_GE(sp->srcu_gp_seq, sp->srcu_gp_seq_needed));
		srcu_gp_start(sp);
		queue_delayed_work(rcu_gp_wq, &sp->work, srcu_get_delay(sp));
	}
	spin_unlock_irqrestore_rcu_node(sp, flags);
=======
		smp_store_release(&ssp->srcu_gp_seq_needed, s); /*^^^*/
	}
	if (!do_norm && ULONG_CMP_LT(ssp->srcu_gp_seq_needed_exp, s))
		WRITE_ONCE(ssp->srcu_gp_seq_needed_exp, s);

	/* If grace period not already done and none in progress, start it. */
	if (!rcu_seq_done(&ssp->srcu_gp_seq, s) &&
	    rcu_seq_state(ssp->srcu_gp_seq) == SRCU_STATE_IDLE) {
		WARN_ON_ONCE(ULONG_CMP_GE(ssp->srcu_gp_seq, ssp->srcu_gp_seq_needed));
		srcu_gp_start(ssp);
		if (likely(srcu_init_done))
			queue_delayed_work(rcu_gp_wq, &ssp->work,
					   srcu_get_delay(ssp));
		else if (list_empty(&ssp->work.work.entry))
			list_add(&ssp->work.work.entry, &srcu_boot_list);
	}
	spin_unlock_irqrestore_rcu_node(ssp, flags);
>>>>>>> upstream/android-13
}

/*
 * Wait until all readers counted by array index idx complete, but
 * loop an additional time if there is an expedited grace period pending.
 * The caller must ensure that ->srcu_idx is not changed while checking.
 */
<<<<<<< HEAD
static bool try_check_zero(struct srcu_struct *sp, int idx, int trycount)
{
	for (;;) {
		if (srcu_readers_active_idx_check(sp, idx))
			return true;
		if (--trycount + !srcu_get_delay(sp) <= 0)
=======
static bool try_check_zero(struct srcu_struct *ssp, int idx, int trycount)
{
	for (;;) {
		if (srcu_readers_active_idx_check(ssp, idx))
			return true;
		if (--trycount + !srcu_get_delay(ssp) <= 0)
>>>>>>> upstream/android-13
			return false;
		udelay(SRCU_RETRY_CHECK_DELAY);
	}
}

/*
 * Increment the ->srcu_idx counter so that future SRCU readers will
 * use the other rank of the ->srcu_(un)lock_count[] arrays.  This allows
 * us to wait for pre-existing readers in a starvation-free manner.
 */
<<<<<<< HEAD
static void srcu_flip(struct srcu_struct *sp)
=======
static void srcu_flip(struct srcu_struct *ssp)
>>>>>>> upstream/android-13
{
	/*
	 * Ensure that if this updater saw a given reader's increment
	 * from __srcu_read_lock(), that reader was using an old value
	 * of ->srcu_idx.  Also ensure that if a given reader sees the
	 * new value of ->srcu_idx, this updater's earlier scans cannot
	 * have seen that reader's increments (which is OK, because this
	 * grace period need not wait on that reader).
	 */
	smp_mb(); /* E */  /* Pairs with B and C. */

<<<<<<< HEAD
	WRITE_ONCE(sp->srcu_idx, sp->srcu_idx + 1);
=======
	WRITE_ONCE(ssp->srcu_idx, ssp->srcu_idx + 1);
>>>>>>> upstream/android-13

	/*
	 * Ensure that if the updater misses an __srcu_read_unlock()
	 * increment, that task's next __srcu_read_lock() will see the
	 * above counter update.  Note that both this memory barrier
	 * and the one in srcu_readers_active_idx_check() provide the
	 * guarantee for __srcu_read_lock().
	 */
	smp_mb(); /* D */  /* Pairs with C. */
}

/*
 * If SRCU is likely idle, return true, otherwise return false.
 *
 * Note that it is OK for several current from-idle requests for a new
 * grace period from idle to specify expediting because they will all end
 * up requesting the same grace period anyhow.  So no loss.
 *
 * Note also that if any CPU (including the current one) is still invoking
 * callbacks, this function will nevertheless say "idle".  This is not
 * ideal, but the overhead of checking all CPUs' callback lists is even
 * less ideal, especially on large systems.  Furthermore, the wakeup
 * can happen before the callback is fully removed, so we have no choice
 * but to accept this type of error.
 *
 * This function is also subject to counter-wrap errors, but let's face
 * it, if this function was preempted for enough time for the counters
 * to wrap, it really doesn't matter whether or not we expedite the grace
 * period.  The extra overhead of a needlessly expedited grace period is
<<<<<<< HEAD
 * negligible when amoritized over that time period, and the extra latency
 * of a needlessly non-expedited grace period is similarly negligible.
 */
static bool srcu_might_be_idle(struct srcu_struct *sp)
=======
 * negligible when amortized over that time period, and the extra latency
 * of a needlessly non-expedited grace period is similarly negligible.
 */
static bool srcu_might_be_idle(struct srcu_struct *ssp)
>>>>>>> upstream/android-13
{
	unsigned long curseq;
	unsigned long flags;
	struct srcu_data *sdp;
	unsigned long t;
<<<<<<< HEAD

	/* If the local srcu_data structure has callbacks, not idle.  */
	local_irq_save(flags);
	sdp = this_cpu_ptr(sp->sda);
	if (rcu_segcblist_pend_cbs(&sdp->srcu_cblist)) {
		local_irq_restore(flags);
		return false; /* Callbacks already present, so not idle. */
	}
	local_irq_restore(flags);

	/*
	 * No local callbacks, so probabalistically probe global state.
	 * Exact information would require acquiring locks, which would
	 * kill scalability, hence the probabalistic nature of the probe.
=======
	unsigned long tlast;

	check_init_srcu_struct(ssp);
	/* If the local srcu_data structure has callbacks, not idle.  */
	sdp = raw_cpu_ptr(ssp->sda);
	spin_lock_irqsave_rcu_node(sdp, flags);
	if (rcu_segcblist_pend_cbs(&sdp->srcu_cblist)) {
		spin_unlock_irqrestore_rcu_node(sdp, flags);
		return false; /* Callbacks already present, so not idle. */
	}
	spin_unlock_irqrestore_rcu_node(sdp, flags);

	/*
	 * No local callbacks, so probabilistically probe global state.
	 * Exact information would require acquiring locks, which would
	 * kill scalability, hence the probabilistic nature of the probe.
>>>>>>> upstream/android-13
	 */

	/* First, see if enough time has passed since the last GP. */
	t = ktime_get_mono_fast_ns();
<<<<<<< HEAD
	if (exp_holdoff == 0 ||
	    time_in_range_open(t, sp->srcu_last_gp_end,
			       sp->srcu_last_gp_end + exp_holdoff))
		return false; /* Too soon after last GP. */

	/* Next, check for probable idleness. */
	curseq = rcu_seq_current(&sp->srcu_gp_seq);
	smp_mb(); /* Order ->srcu_gp_seq with ->srcu_gp_seq_needed. */
	if (ULONG_CMP_LT(curseq, READ_ONCE(sp->srcu_gp_seq_needed)))
		return false; /* Grace period in progress, so not idle. */
	smp_mb(); /* Order ->srcu_gp_seq with prior access. */
	if (curseq != rcu_seq_current(&sp->srcu_gp_seq))
=======
	tlast = READ_ONCE(ssp->srcu_last_gp_end);
	if (exp_holdoff == 0 ||
	    time_in_range_open(t, tlast, tlast + exp_holdoff))
		return false; /* Too soon after last GP. */

	/* Next, check for probable idleness. */
	curseq = rcu_seq_current(&ssp->srcu_gp_seq);
	smp_mb(); /* Order ->srcu_gp_seq with ->srcu_gp_seq_needed. */
	if (ULONG_CMP_LT(curseq, READ_ONCE(ssp->srcu_gp_seq_needed)))
		return false; /* Grace period in progress, so not idle. */
	smp_mb(); /* Order ->srcu_gp_seq with prior access. */
	if (curseq != rcu_seq_current(&ssp->srcu_gp_seq))
>>>>>>> upstream/android-13
		return false; /* GP # changed, so not idle. */
	return true; /* With reasonable probability, idle! */
}

/*
 * SRCU callback function to leak a callback.
 */
static void srcu_leak_callback(struct rcu_head *rhp)
{
}

/*
<<<<<<< HEAD
=======
 * Start an SRCU grace period, and also queue the callback if non-NULL.
 */
static unsigned long srcu_gp_start_if_needed(struct srcu_struct *ssp,
					     struct rcu_head *rhp, bool do_norm)
{
	unsigned long flags;
	int idx;
	bool needexp = false;
	bool needgp = false;
	unsigned long s;
	struct srcu_data *sdp;

	check_init_srcu_struct(ssp);
	idx = srcu_read_lock(ssp);
	sdp = raw_cpu_ptr(ssp->sda);
	spin_lock_irqsave_rcu_node(sdp, flags);
	if (rhp)
		rcu_segcblist_enqueue(&sdp->srcu_cblist, rhp);
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&ssp->srcu_gp_seq));
	s = rcu_seq_snap(&ssp->srcu_gp_seq);
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist, s);
	if (ULONG_CMP_LT(sdp->srcu_gp_seq_needed, s)) {
		sdp->srcu_gp_seq_needed = s;
		needgp = true;
	}
	if (!do_norm && ULONG_CMP_LT(sdp->srcu_gp_seq_needed_exp, s)) {
		sdp->srcu_gp_seq_needed_exp = s;
		needexp = true;
	}
	spin_unlock_irqrestore_rcu_node(sdp, flags);
	if (needgp)
		srcu_funnel_gp_start(ssp, sdp, s, do_norm);
	else if (needexp)
		srcu_funnel_exp_start(ssp, sdp->mynode, s);
	srcu_read_unlock(ssp, idx);
	return s;
}

/*
>>>>>>> upstream/android-13
 * Enqueue an SRCU callback on the srcu_data structure associated with
 * the current CPU and the specified srcu_struct structure, initiating
 * grace-period processing if it is not already running.
 *
 * Note that all CPUs must agree that the grace period extended beyond
 * all pre-existing SRCU read-side critical section.  On systems with
 * more than one CPU, this means that when "func()" is invoked, each CPU
 * is guaranteed to have executed a full memory barrier since the end of
 * its last corresponding SRCU read-side critical section whose beginning
 * preceded the call to call_srcu().  It also means that each CPU executing
 * an SRCU read-side critical section that continues beyond the start of
 * "func()" must have executed a memory barrier after the call_srcu()
 * but before the beginning of that SRCU read-side critical section.
 * Note that these guarantees include CPUs that are offline, idle, or
 * executing in user mode, as well as CPUs that are executing in the kernel.
 *
 * Furthermore, if CPU A invoked call_srcu() and CPU B invoked the
 * resulting SRCU callback function "func()", then both CPU A and CPU
 * B are guaranteed to execute a full memory barrier during the time
 * interval between the call to call_srcu() and the invocation of "func()".
 * This guarantee applies even if CPU A and CPU B are the same CPU (but
 * again only if the system has more than one CPU).
 *
 * Of course, these guarantees apply only for invocations of call_srcu(),
 * srcu_read_lock(), and srcu_read_unlock() that are all passed the same
 * srcu_struct structure.
 */
<<<<<<< HEAD
void __call_srcu(struct srcu_struct *sp, struct rcu_head *rhp,
		 rcu_callback_t func, bool do_norm)
{
	unsigned long flags;
	bool needexp = false;
	bool needgp = false;
	unsigned long s;
	struct srcu_data *sdp;

	check_init_srcu_struct(sp);
=======
static void __call_srcu(struct srcu_struct *ssp, struct rcu_head *rhp,
			rcu_callback_t func, bool do_norm)
{
>>>>>>> upstream/android-13
	if (debug_rcu_head_queue(rhp)) {
		/* Probable double call_srcu(), so leak the callback. */
		WRITE_ONCE(rhp->func, srcu_leak_callback);
		WARN_ONCE(1, "call_srcu(): Leaked duplicate callback\n");
		return;
	}
	rhp->func = func;
<<<<<<< HEAD
	local_irq_save(flags);
	sdp = this_cpu_ptr(sp->sda);
	spin_lock_rcu_node(sdp);
	rcu_segcblist_enqueue(&sdp->srcu_cblist, rhp, false);
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&sp->srcu_gp_seq));
	s = rcu_seq_snap(&sp->srcu_gp_seq);
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist, s);
	if (ULONG_CMP_LT(sdp->srcu_gp_seq_needed, s)) {
		sdp->srcu_gp_seq_needed = s;
		needgp = true;
	}
	if (!do_norm && ULONG_CMP_LT(sdp->srcu_gp_seq_needed_exp, s)) {
		sdp->srcu_gp_seq_needed_exp = s;
		needexp = true;
	}
	spin_unlock_irqrestore_rcu_node(sdp, flags);
	if (needgp)
		srcu_funnel_gp_start(sp, sdp, s, do_norm);
	else if (needexp)
		srcu_funnel_exp_start(sp, sdp->mynode, s);
=======
	(void)srcu_gp_start_if_needed(ssp, rhp, do_norm);
>>>>>>> upstream/android-13
}

/**
 * call_srcu() - Queue a callback for invocation after an SRCU grace period
<<<<<<< HEAD
 * @sp: srcu_struct in queue the callback
=======
 * @ssp: srcu_struct in queue the callback
>>>>>>> upstream/android-13
 * @rhp: structure to be used for queueing the SRCU callback.
 * @func: function to be invoked after the SRCU grace period
 *
 * The callback function will be invoked some time after a full SRCU
 * grace period elapses, in other words after all pre-existing SRCU
 * read-side critical sections have completed.  However, the callback
 * function might well execute concurrently with other SRCU read-side
 * critical sections that started after call_srcu() was invoked.  SRCU
 * read-side critical sections are delimited by srcu_read_lock() and
 * srcu_read_unlock(), and may be nested.
 *
 * The callback will be invoked from process context, but must nevertheless
 * be fast and must not block.
 */
<<<<<<< HEAD
void call_srcu(struct srcu_struct *sp, struct rcu_head *rhp,
	       rcu_callback_t func)
{
	__call_srcu(sp, rhp, func, true);
=======
void call_srcu(struct srcu_struct *ssp, struct rcu_head *rhp,
	       rcu_callback_t func)
{
	__call_srcu(ssp, rhp, func, true);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(call_srcu);

/*
 * Helper function for synchronize_srcu() and synchronize_srcu_expedited().
 */
<<<<<<< HEAD
static void __synchronize_srcu(struct srcu_struct *sp, bool do_norm)
{
	struct rcu_synchronize rcu;

	RCU_LOCKDEP_WARN(lock_is_held(&sp->dep_map) ||
=======
static void __synchronize_srcu(struct srcu_struct *ssp, bool do_norm)
{
	struct rcu_synchronize rcu;

	RCU_LOCKDEP_WARN(lockdep_is_held(ssp) ||
>>>>>>> upstream/android-13
			 lock_is_held(&rcu_bh_lock_map) ||
			 lock_is_held(&rcu_lock_map) ||
			 lock_is_held(&rcu_sched_lock_map),
			 "Illegal synchronize_srcu() in same-type SRCU (or in RCU) read-side critical section");

	if (rcu_scheduler_active == RCU_SCHEDULER_INACTIVE)
		return;
	might_sleep();
<<<<<<< HEAD
	check_init_srcu_struct(sp);
	init_completion(&rcu.completion);
	init_rcu_head_on_stack(&rcu.head);
	__call_srcu(sp, &rcu.head, wakeme_after_rcu, do_norm);
=======
	check_init_srcu_struct(ssp);
	init_completion(&rcu.completion);
	init_rcu_head_on_stack(&rcu.head);
	__call_srcu(ssp, &rcu.head, wakeme_after_rcu, do_norm);
>>>>>>> upstream/android-13
	wait_for_completion(&rcu.completion);
	destroy_rcu_head_on_stack(&rcu.head);

	/*
	 * Make sure that later code is ordered after the SRCU grace
	 * period.  This pairs with the spin_lock_irq_rcu_node()
	 * in srcu_invoke_callbacks().  Unlike Tree RCU, this is needed
	 * because the current CPU might have been totally uninvolved with
	 * (and thus unordered against) that grace period.
	 */
	smp_mb();
}

/**
 * synchronize_srcu_expedited - Brute-force SRCU grace period
<<<<<<< HEAD
 * @sp: srcu_struct with which to synchronize.
=======
 * @ssp: srcu_struct with which to synchronize.
>>>>>>> upstream/android-13
 *
 * Wait for an SRCU grace period to elapse, but be more aggressive about
 * spinning rather than blocking when waiting.
 *
 * Note that synchronize_srcu_expedited() has the same deadlock and
 * memory-ordering properties as does synchronize_srcu().
 */
<<<<<<< HEAD
void synchronize_srcu_expedited(struct srcu_struct *sp)
{
	__synchronize_srcu(sp, rcu_gp_is_normal());
=======
void synchronize_srcu_expedited(struct srcu_struct *ssp)
{
	__synchronize_srcu(ssp, rcu_gp_is_normal());
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(synchronize_srcu_expedited);

/**
 * synchronize_srcu - wait for prior SRCU read-side critical-section completion
<<<<<<< HEAD
 * @sp: srcu_struct with which to synchronize.
=======
 * @ssp: srcu_struct with which to synchronize.
>>>>>>> upstream/android-13
 *
 * Wait for the count to drain to zero of both indexes. To avoid the
 * possible starvation of synchronize_srcu(), it waits for the count of
 * the index=((->srcu_idx & 1) ^ 1) to drain to zero at first,
 * and then flip the srcu_idx and wait for the count of the other index.
 *
 * Can block; must be called from process context.
 *
 * Note that it is illegal to call synchronize_srcu() from the corresponding
 * SRCU read-side critical section; doing so will result in deadlock.
 * However, it is perfectly legal to call synchronize_srcu() on one
 * srcu_struct from some other srcu_struct's read-side critical section,
 * as long as the resulting graph of srcu_structs is acyclic.
 *
 * There are memory-ordering constraints implied by synchronize_srcu().
 * On systems with more than one CPU, when synchronize_srcu() returns,
 * each CPU is guaranteed to have executed a full memory barrier since
<<<<<<< HEAD
 * the end of its last corresponding SRCU-sched read-side critical section
=======
 * the end of its last corresponding SRCU read-side critical section
>>>>>>> upstream/android-13
 * whose beginning preceded the call to synchronize_srcu().  In addition,
 * each CPU having an SRCU read-side critical section that extends beyond
 * the return from synchronize_srcu() is guaranteed to have executed a
 * full memory barrier after the beginning of synchronize_srcu() and before
 * the beginning of that SRCU read-side critical section.  Note that these
 * guarantees include CPUs that are offline, idle, or executing in user mode,
 * as well as CPUs that are executing in the kernel.
 *
 * Furthermore, if CPU A invoked synchronize_srcu(), which returned
 * to its caller on CPU B, then both CPU A and CPU B are guaranteed
 * to have executed a full memory barrier during the execution of
 * synchronize_srcu().  This guarantee applies even if CPU A and CPU B
 * are the same CPU, but again only if the system has more than one CPU.
 *
 * Of course, these memory-ordering guarantees apply only when
 * synchronize_srcu(), srcu_read_lock(), and srcu_read_unlock() are
 * passed the same srcu_struct structure.
 *
<<<<<<< HEAD
=======
 * Implementation of these memory-ordering guarantees is similar to
 * that of synchronize_rcu().
 *
>>>>>>> upstream/android-13
 * If SRCU is likely idle, expedite the first request.  This semantic
 * was provided by Classic SRCU, and is relied upon by its users, so TREE
 * SRCU must also provide it.  Note that detecting idleness is heuristic
 * and subject to both false positives and negatives.
 */
<<<<<<< HEAD
void synchronize_srcu(struct srcu_struct *sp)
{
	if (srcu_might_be_idle(sp) || rcu_gp_is_expedited())
		synchronize_srcu_expedited(sp);
	else
		__synchronize_srcu(sp, true);
}
EXPORT_SYMBOL_GPL(synchronize_srcu);

=======
void synchronize_srcu(struct srcu_struct *ssp)
{
	if (srcu_might_be_idle(ssp) || rcu_gp_is_expedited())
		synchronize_srcu_expedited(ssp);
	else
		__synchronize_srcu(ssp, true);
}
EXPORT_SYMBOL_GPL(synchronize_srcu);

/**
 * get_state_synchronize_srcu - Provide an end-of-grace-period cookie
 * @ssp: srcu_struct to provide cookie for.
 *
 * This function returns a cookie that can be passed to
 * poll_state_synchronize_srcu(), which will return true if a full grace
 * period has elapsed in the meantime.  It is the caller's responsibility
 * to make sure that grace period happens, for example, by invoking
 * call_srcu() after return from get_state_synchronize_srcu().
 */
unsigned long get_state_synchronize_srcu(struct srcu_struct *ssp)
{
	// Any prior manipulation of SRCU-protected data must happen
	// before the load from ->srcu_gp_seq.
	smp_mb();
	return rcu_seq_snap(&ssp->srcu_gp_seq);
}
EXPORT_SYMBOL_GPL(get_state_synchronize_srcu);

/**
 * start_poll_synchronize_srcu - Provide cookie and start grace period
 * @ssp: srcu_struct to provide cookie for.
 *
 * This function returns a cookie that can be passed to
 * poll_state_synchronize_srcu(), which will return true if a full grace
 * period has elapsed in the meantime.  Unlike get_state_synchronize_srcu(),
 * this function also ensures that any needed SRCU grace period will be
 * started.  This convenience does come at a cost in terms of CPU overhead.
 */
unsigned long start_poll_synchronize_srcu(struct srcu_struct *ssp)
{
	return srcu_gp_start_if_needed(ssp, NULL, true);
}
EXPORT_SYMBOL_GPL(start_poll_synchronize_srcu);

/**
 * poll_state_synchronize_srcu - Has cookie's grace period ended?
 * @ssp: srcu_struct to provide cookie for.
 * @cookie: Return value from get_state_synchronize_srcu() or start_poll_synchronize_srcu().
 *
 * This function takes the cookie that was returned from either
 * get_state_synchronize_srcu() or start_poll_synchronize_srcu(), and
 * returns @true if an SRCU grace period elapsed since the time that the
 * cookie was created.
 *
 * Because cookies are finite in size, wrapping/overflow is possible.
 * This is more pronounced on 32-bit systems where cookies are 32 bits,
 * where in theory wrapping could happen in about 14 hours assuming
 * 25-microsecond expedited SRCU grace periods.  However, a more likely
 * overflow lower bound is on the order of 24 days in the case of
 * one-millisecond SRCU grace periods.  Of course, wrapping in a 64-bit
 * system requires geologic timespans, as in more than seven million years
 * even for expedited SRCU grace periods.
 *
 * Wrapping/overflow is much more of an issue for CONFIG_SMP=n systems
 * that also have CONFIG_PREEMPTION=n, which selects Tiny SRCU.  This uses
 * a 16-bit cookie, which rcutorture routinely wraps in a matter of a
 * few minutes.  If this proves to be a problem, this counter will be
 * expanded to the same size as for Tree SRCU.
 */
bool poll_state_synchronize_srcu(struct srcu_struct *ssp, unsigned long cookie)
{
	if (!rcu_seq_done(&ssp->srcu_gp_seq, cookie))
		return false;
	// Ensure that the end of the SRCU grace period happens before
	// any subsequent code that the caller might execute.
	smp_mb(); // ^^^
	return true;
}
EXPORT_SYMBOL_GPL(poll_state_synchronize_srcu);

>>>>>>> upstream/android-13
/*
 * Callback function for srcu_barrier() use.
 */
static void srcu_barrier_cb(struct rcu_head *rhp)
{
	struct srcu_data *sdp;
<<<<<<< HEAD
	struct srcu_struct *sp;

	sdp = container_of(rhp, struct srcu_data, srcu_barrier_head);
	sp = sdp->sp;
	if (atomic_dec_and_test(&sp->srcu_barrier_cpu_cnt))
		complete(&sp->srcu_barrier_completion);
=======
	struct srcu_struct *ssp;

	sdp = container_of(rhp, struct srcu_data, srcu_barrier_head);
	ssp = sdp->ssp;
	if (atomic_dec_and_test(&ssp->srcu_barrier_cpu_cnt))
		complete(&ssp->srcu_barrier_completion);
>>>>>>> upstream/android-13
}

/**
 * srcu_barrier - Wait until all in-flight call_srcu() callbacks complete.
<<<<<<< HEAD
 * @sp: srcu_struct on which to wait for in-flight callbacks.
 */
void srcu_barrier(struct srcu_struct *sp)
{
	int cpu;
	struct srcu_data *sdp;
	unsigned long s = rcu_seq_snap(&sp->srcu_barrier_seq);

	check_init_srcu_struct(sp);
	mutex_lock(&sp->srcu_barrier_mutex);
	if (rcu_seq_done(&sp->srcu_barrier_seq, s)) {
		smp_mb(); /* Force ordering following return. */
		mutex_unlock(&sp->srcu_barrier_mutex);
		return; /* Someone else did our work for us. */
	}
	rcu_seq_start(&sp->srcu_barrier_seq);
	init_completion(&sp->srcu_barrier_completion);

	/* Initial count prevents reaching zero until all CBs are posted. */
	atomic_set(&sp->srcu_barrier_cpu_cnt, 1);
=======
 * @ssp: srcu_struct on which to wait for in-flight callbacks.
 */
void srcu_barrier(struct srcu_struct *ssp)
{
	int cpu;
	struct srcu_data *sdp;
	unsigned long s = rcu_seq_snap(&ssp->srcu_barrier_seq);

	check_init_srcu_struct(ssp);
	mutex_lock(&ssp->srcu_barrier_mutex);
	if (rcu_seq_done(&ssp->srcu_barrier_seq, s)) {
		smp_mb(); /* Force ordering following return. */
		mutex_unlock(&ssp->srcu_barrier_mutex);
		return; /* Someone else did our work for us. */
	}
	rcu_seq_start(&ssp->srcu_barrier_seq);
	init_completion(&ssp->srcu_barrier_completion);

	/* Initial count prevents reaching zero until all CBs are posted. */
	atomic_set(&ssp->srcu_barrier_cpu_cnt, 1);
>>>>>>> upstream/android-13

	/*
	 * Each pass through this loop enqueues a callback, but only
	 * on CPUs already having callbacks enqueued.  Note that if
	 * a CPU already has callbacks enqueue, it must have already
	 * registered the need for a future grace period, so all we
	 * need do is enqueue a callback that will use the same
	 * grace period as the last callback already in the queue.
	 */
	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		sdp = per_cpu_ptr(sp->sda, cpu);
		spin_lock_irq_rcu_node(sdp);
		atomic_inc(&sp->srcu_barrier_cpu_cnt);
		sdp->srcu_barrier_head.func = srcu_barrier_cb;
		debug_rcu_head_queue(&sdp->srcu_barrier_head);
		if (!rcu_segcblist_entrain(&sdp->srcu_cblist,
					   &sdp->srcu_barrier_head, 0)) {
			debug_rcu_head_unqueue(&sdp->srcu_barrier_head);
			atomic_dec(&sp->srcu_barrier_cpu_cnt);
=======
		sdp = per_cpu_ptr(ssp->sda, cpu);
		spin_lock_irq_rcu_node(sdp);
		atomic_inc(&ssp->srcu_barrier_cpu_cnt);
		sdp->srcu_barrier_head.func = srcu_barrier_cb;
		debug_rcu_head_queue(&sdp->srcu_barrier_head);
		if (!rcu_segcblist_entrain(&sdp->srcu_cblist,
					   &sdp->srcu_barrier_head)) {
			debug_rcu_head_unqueue(&sdp->srcu_barrier_head);
			atomic_dec(&ssp->srcu_barrier_cpu_cnt);
>>>>>>> upstream/android-13
		}
		spin_unlock_irq_rcu_node(sdp);
	}

	/* Remove the initial count, at which point reaching zero can happen. */
<<<<<<< HEAD
	if (atomic_dec_and_test(&sp->srcu_barrier_cpu_cnt))
		complete(&sp->srcu_barrier_completion);
	wait_for_completion(&sp->srcu_barrier_completion);

	rcu_seq_end(&sp->srcu_barrier_seq);
	mutex_unlock(&sp->srcu_barrier_mutex);
=======
	if (atomic_dec_and_test(&ssp->srcu_barrier_cpu_cnt))
		complete(&ssp->srcu_barrier_completion);
	wait_for_completion(&ssp->srcu_barrier_completion);

	rcu_seq_end(&ssp->srcu_barrier_seq);
	mutex_unlock(&ssp->srcu_barrier_mutex);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(srcu_barrier);

/**
 * srcu_batches_completed - return batches completed.
<<<<<<< HEAD
 * @sp: srcu_struct on which to report batch completion.
=======
 * @ssp: srcu_struct on which to report batch completion.
>>>>>>> upstream/android-13
 *
 * Report the number of batches, correlated with, but not necessarily
 * precisely the same as, the number of grace periods that have elapsed.
 */
<<<<<<< HEAD
unsigned long srcu_batches_completed(struct srcu_struct *sp)
{
	return sp->srcu_idx;
=======
unsigned long srcu_batches_completed(struct srcu_struct *ssp)
{
	return READ_ONCE(ssp->srcu_idx);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(srcu_batches_completed);

/*
 * Core SRCU state machine.  Push state bits of ->srcu_gp_seq
 * to SRCU_STATE_SCAN2, and invoke srcu_gp_end() when scan has
 * completed in that state.
 */
<<<<<<< HEAD
static void srcu_advance_state(struct srcu_struct *sp)
{
	int idx;

	mutex_lock(&sp->srcu_gp_mutex);
=======
static void srcu_advance_state(struct srcu_struct *ssp)
{
	int idx;

	mutex_lock(&ssp->srcu_gp_mutex);
>>>>>>> upstream/android-13

	/*
	 * Because readers might be delayed for an extended period after
	 * fetching ->srcu_idx for their index, at any point in time there
	 * might well be readers using both idx=0 and idx=1.  We therefore
	 * need to wait for readers to clear from both index values before
	 * invoking a callback.
	 *
	 * The load-acquire ensures that we see the accesses performed
	 * by the prior grace period.
	 */
<<<<<<< HEAD
	idx = rcu_seq_state(smp_load_acquire(&sp->srcu_gp_seq)); /* ^^^ */
	if (idx == SRCU_STATE_IDLE) {
		spin_lock_irq_rcu_node(sp);
		if (ULONG_CMP_GE(sp->srcu_gp_seq, sp->srcu_gp_seq_needed)) {
			WARN_ON_ONCE(rcu_seq_state(sp->srcu_gp_seq));
			spin_unlock_irq_rcu_node(sp);
			mutex_unlock(&sp->srcu_gp_mutex);
			return;
		}
		idx = rcu_seq_state(READ_ONCE(sp->srcu_gp_seq));
		if (idx == SRCU_STATE_IDLE)
			srcu_gp_start(sp);
		spin_unlock_irq_rcu_node(sp);
		if (idx != SRCU_STATE_IDLE) {
			mutex_unlock(&sp->srcu_gp_mutex);
=======
	idx = rcu_seq_state(smp_load_acquire(&ssp->srcu_gp_seq)); /* ^^^ */
	if (idx == SRCU_STATE_IDLE) {
		spin_lock_irq_rcu_node(ssp);
		if (ULONG_CMP_GE(ssp->srcu_gp_seq, ssp->srcu_gp_seq_needed)) {
			WARN_ON_ONCE(rcu_seq_state(ssp->srcu_gp_seq));
			spin_unlock_irq_rcu_node(ssp);
			mutex_unlock(&ssp->srcu_gp_mutex);
			return;
		}
		idx = rcu_seq_state(READ_ONCE(ssp->srcu_gp_seq));
		if (idx == SRCU_STATE_IDLE)
			srcu_gp_start(ssp);
		spin_unlock_irq_rcu_node(ssp);
		if (idx != SRCU_STATE_IDLE) {
			mutex_unlock(&ssp->srcu_gp_mutex);
>>>>>>> upstream/android-13
			return; /* Someone else started the grace period. */
		}
	}

<<<<<<< HEAD
	if (rcu_seq_state(READ_ONCE(sp->srcu_gp_seq)) == SRCU_STATE_SCAN1) {
		idx = 1 ^ (sp->srcu_idx & 1);
		if (!try_check_zero(sp, idx, 1)) {
			mutex_unlock(&sp->srcu_gp_mutex);
			return; /* readers present, retry later. */
		}
		srcu_flip(sp);
		rcu_seq_set_state(&sp->srcu_gp_seq, SRCU_STATE_SCAN2);
	}

	if (rcu_seq_state(READ_ONCE(sp->srcu_gp_seq)) == SRCU_STATE_SCAN2) {
=======
	if (rcu_seq_state(READ_ONCE(ssp->srcu_gp_seq)) == SRCU_STATE_SCAN1) {
		idx = 1 ^ (ssp->srcu_idx & 1);
		if (!try_check_zero(ssp, idx, 1)) {
			mutex_unlock(&ssp->srcu_gp_mutex);
			return; /* readers present, retry later. */
		}
		srcu_flip(ssp);
		spin_lock_irq_rcu_node(ssp);
		rcu_seq_set_state(&ssp->srcu_gp_seq, SRCU_STATE_SCAN2);
		spin_unlock_irq_rcu_node(ssp);
	}

	if (rcu_seq_state(READ_ONCE(ssp->srcu_gp_seq)) == SRCU_STATE_SCAN2) {
>>>>>>> upstream/android-13

		/*
		 * SRCU read-side critical sections are normally short,
		 * so check at least twice in quick succession after a flip.
		 */
<<<<<<< HEAD
		idx = 1 ^ (sp->srcu_idx & 1);
		if (!try_check_zero(sp, idx, 2)) {
			mutex_unlock(&sp->srcu_gp_mutex);
			return; /* readers present, retry later. */
		}
		srcu_gp_end(sp);  /* Releases ->srcu_gp_mutex. */
=======
		idx = 1 ^ (ssp->srcu_idx & 1);
		if (!try_check_zero(ssp, idx, 2)) {
			mutex_unlock(&ssp->srcu_gp_mutex);
			return; /* readers present, retry later. */
		}
		srcu_gp_end(ssp);  /* Releases ->srcu_gp_mutex. */
>>>>>>> upstream/android-13
	}
}

/*
 * Invoke a limited number of SRCU callbacks that have passed through
 * their grace period.  If there are more to do, SRCU will reschedule
 * the workqueue.  Note that needed memory barriers have been executed
 * in this task's context by srcu_readers_active_idx_check().
 */
static void srcu_invoke_callbacks(struct work_struct *work)
{
<<<<<<< HEAD
=======
	long len;
>>>>>>> upstream/android-13
	bool more;
	struct rcu_cblist ready_cbs;
	struct rcu_head *rhp;
	struct srcu_data *sdp;
<<<<<<< HEAD
	struct srcu_struct *sp;

	sdp = container_of(work, struct srcu_data, work.work);
	sp = sdp->sp;
	rcu_cblist_init(&ready_cbs);
	spin_lock_irq_rcu_node(sdp);
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&sp->srcu_gp_seq));
=======
	struct srcu_struct *ssp;

	sdp = container_of(work, struct srcu_data, work);

	ssp = sdp->ssp;
	rcu_cblist_init(&ready_cbs);
	spin_lock_irq_rcu_node(sdp);
	rcu_segcblist_advance(&sdp->srcu_cblist,
			      rcu_seq_current(&ssp->srcu_gp_seq));
>>>>>>> upstream/android-13
	if (sdp->srcu_cblist_invoking ||
	    !rcu_segcblist_ready_cbs(&sdp->srcu_cblist)) {
		spin_unlock_irq_rcu_node(sdp);
		return;  /* Someone else on the job or nothing to do. */
	}

	/* We are on the job!  Extract and invoke ready callbacks. */
	sdp->srcu_cblist_invoking = true;
	rcu_segcblist_extract_done_cbs(&sdp->srcu_cblist, &ready_cbs);
<<<<<<< HEAD
=======
	len = ready_cbs.len;
>>>>>>> upstream/android-13
	spin_unlock_irq_rcu_node(sdp);
	rhp = rcu_cblist_dequeue(&ready_cbs);
	for (; rhp != NULL; rhp = rcu_cblist_dequeue(&ready_cbs)) {
		debug_rcu_head_unqueue(rhp);
		local_bh_disable();
		rhp->func(rhp);
		local_bh_enable();
	}
<<<<<<< HEAD
=======
	WARN_ON_ONCE(ready_cbs.len);
>>>>>>> upstream/android-13

	/*
	 * Update counts, accelerate new callbacks, and if needed,
	 * schedule another round of callback invocation.
	 */
	spin_lock_irq_rcu_node(sdp);
<<<<<<< HEAD
	rcu_segcblist_insert_count(&sdp->srcu_cblist, &ready_cbs);
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist,
				       rcu_seq_snap(&sp->srcu_gp_seq));
=======
	rcu_segcblist_add_len(&sdp->srcu_cblist, -len);
	(void)rcu_segcblist_accelerate(&sdp->srcu_cblist,
				       rcu_seq_snap(&ssp->srcu_gp_seq));
>>>>>>> upstream/android-13
	sdp->srcu_cblist_invoking = false;
	more = rcu_segcblist_ready_cbs(&sdp->srcu_cblist);
	spin_unlock_irq_rcu_node(sdp);
	if (more)
		srcu_schedule_cbs_sdp(sdp, 0);
}

/*
 * Finished one round of SRCU grace period.  Start another if there are
 * more SRCU callbacks queued, otherwise put SRCU into not-running state.
 */
<<<<<<< HEAD
static void srcu_reschedule(struct srcu_struct *sp, unsigned long delay)
{
	bool pushgp = true;

	spin_lock_irq_rcu_node(sp);
	if (ULONG_CMP_GE(sp->srcu_gp_seq, sp->srcu_gp_seq_needed)) {
		if (!WARN_ON_ONCE(rcu_seq_state(sp->srcu_gp_seq))) {
			/* All requests fulfilled, time to go idle. */
			pushgp = false;
		}
	} else if (!rcu_seq_state(sp->srcu_gp_seq)) {
		/* Outstanding request and no GP.  Start one. */
		srcu_gp_start(sp);
	}
	spin_unlock_irq_rcu_node(sp);

	if (pushgp)
		queue_delayed_work(rcu_gp_wq, &sp->work, delay);
=======
static void srcu_reschedule(struct srcu_struct *ssp, unsigned long delay)
{
	bool pushgp = true;

	spin_lock_irq_rcu_node(ssp);
	if (ULONG_CMP_GE(ssp->srcu_gp_seq, ssp->srcu_gp_seq_needed)) {
		if (!WARN_ON_ONCE(rcu_seq_state(ssp->srcu_gp_seq))) {
			/* All requests fulfilled, time to go idle. */
			pushgp = false;
		}
	} else if (!rcu_seq_state(ssp->srcu_gp_seq)) {
		/* Outstanding request and no GP.  Start one. */
		srcu_gp_start(ssp);
	}
	spin_unlock_irq_rcu_node(ssp);

	if (pushgp)
		queue_delayed_work(rcu_gp_wq, &ssp->work, delay);
>>>>>>> upstream/android-13
}

/*
 * This is the work-queue function that handles SRCU grace periods.
 */
static void process_srcu(struct work_struct *work)
{
<<<<<<< HEAD
	struct srcu_struct *sp;

	sp = container_of(work, struct srcu_struct, work.work);

	srcu_advance_state(sp);
	srcu_reschedule(sp, srcu_get_delay(sp));
}

void srcutorture_get_gp_data(enum rcutorture_type test_type,
			     struct srcu_struct *sp, int *flags,
=======
	struct srcu_struct *ssp;

	ssp = container_of(work, struct srcu_struct, work.work);

	srcu_advance_state(ssp);
	srcu_reschedule(ssp, srcu_get_delay(ssp));
}

void srcutorture_get_gp_data(enum rcutorture_type test_type,
			     struct srcu_struct *ssp, int *flags,
>>>>>>> upstream/android-13
			     unsigned long *gp_seq)
{
	if (test_type != SRCU_FLAVOR)
		return;
	*flags = 0;
<<<<<<< HEAD
	*gp_seq = rcu_seq_current(&sp->srcu_gp_seq);
}
EXPORT_SYMBOL_GPL(srcutorture_get_gp_data);

void srcu_torture_stats_print(struct srcu_struct *sp, char *tt, char *tf)
=======
	*gp_seq = rcu_seq_current(&ssp->srcu_gp_seq);
}
EXPORT_SYMBOL_GPL(srcutorture_get_gp_data);

void srcu_torture_stats_print(struct srcu_struct *ssp, char *tt, char *tf)
>>>>>>> upstream/android-13
{
	int cpu;
	int idx;
	unsigned long s0 = 0, s1 = 0;

<<<<<<< HEAD
	idx = sp->srcu_idx & 0x1;
	pr_alert("%s%s Tree SRCU g%ld per-CPU(idx=%d):",
		 tt, tf, rcu_seq_current(&sp->srcu_gp_seq), idx);
=======
	idx = ssp->srcu_idx & 0x1;
	pr_alert("%s%s Tree SRCU g%ld per-CPU(idx=%d):",
		 tt, tf, rcu_seq_current(&ssp->srcu_gp_seq), idx);
>>>>>>> upstream/android-13
	for_each_possible_cpu(cpu) {
		unsigned long l0, l1;
		unsigned long u0, u1;
		long c0, c1;
		struct srcu_data *sdp;

<<<<<<< HEAD
		sdp = per_cpu_ptr(sp->sda, cpu);
		u0 = sdp->srcu_unlock_count[!idx];
		u1 = sdp->srcu_unlock_count[idx];
=======
		sdp = per_cpu_ptr(ssp->sda, cpu);
		u0 = data_race(sdp->srcu_unlock_count[!idx]);
		u1 = data_race(sdp->srcu_unlock_count[idx]);
>>>>>>> upstream/android-13

		/*
		 * Make sure that a lock is always counted if the corresponding
		 * unlock is counted.
		 */
		smp_rmb();

<<<<<<< HEAD
		l0 = sdp->srcu_lock_count[!idx];
		l1 = sdp->srcu_lock_count[idx];

		c0 = l0 - u0;
		c1 = l1 - u1;
		pr_cont(" %d(%ld,%ld %1p)",
			cpu, c0, c1, rcu_segcblist_head(&sdp->srcu_cblist));
=======
		l0 = data_race(sdp->srcu_lock_count[!idx]);
		l1 = data_race(sdp->srcu_lock_count[idx]);

		c0 = l0 - u0;
		c1 = l1 - u1;
		pr_cont(" %d(%ld,%ld %c)",
			cpu, c0, c1,
			"C."[rcu_segcblist_empty(&sdp->srcu_cblist)]);
>>>>>>> upstream/android-13
		s0 += c0;
		s1 += c1;
	}
	pr_cont(" T(%ld,%ld)\n", s0, s1);
}
EXPORT_SYMBOL_GPL(srcu_torture_stats_print);

static int __init srcu_bootup_announce(void)
{
	pr_info("Hierarchical SRCU implementation.\n");
	if (exp_holdoff != DEFAULT_SRCU_EXP_HOLDOFF)
		pr_info("\tNon-default auto-expedite holdoff of %lu ns.\n", exp_holdoff);
	return 0;
}
early_initcall(srcu_bootup_announce);
<<<<<<< HEAD
=======

void __init srcu_init(void)
{
	struct srcu_struct *ssp;

	/*
	 * Once that is set, call_srcu() can follow the normal path and
	 * queue delayed work. This must follow RCU workqueues creation
	 * and timers initialization.
	 */
	srcu_init_done = true;
	while (!list_empty(&srcu_boot_list)) {
		ssp = list_first_entry(&srcu_boot_list, struct srcu_struct,
				      work.work.entry);
		list_del_init(&ssp->work.work.entry);
		queue_work(rcu_gp_wq, &ssp->work.work);
	}
}

#ifdef CONFIG_MODULES

/* Initialize any global-scope srcu_struct structures used by this module. */
static int srcu_module_coming(struct module *mod)
{
	int i;
	struct srcu_struct **sspp = mod->srcu_struct_ptrs;
	int ret;

	for (i = 0; i < mod->num_srcu_structs; i++) {
		ret = init_srcu_struct(*(sspp++));
		if (WARN_ON_ONCE(ret))
			return ret;
	}
	return 0;
}

/* Clean up any global-scope srcu_struct structures used by this module. */
static void srcu_module_going(struct module *mod)
{
	int i;
	struct srcu_struct **sspp = mod->srcu_struct_ptrs;

	for (i = 0; i < mod->num_srcu_structs; i++)
		cleanup_srcu_struct(*(sspp++));
}

/* Handle one module, either coming or going. */
static int srcu_module_notify(struct notifier_block *self,
			      unsigned long val, void *data)
{
	struct module *mod = data;
	int ret = 0;

	switch (val) {
	case MODULE_STATE_COMING:
		ret = srcu_module_coming(mod);
		break;
	case MODULE_STATE_GOING:
		srcu_module_going(mod);
		break;
	default:
		break;
	}
	return ret;
}

static struct notifier_block srcu_module_nb = {
	.notifier_call = srcu_module_notify,
	.priority = 0,
};

static __init int init_srcu_module_notifier(void)
{
	int ret;

	ret = register_module_notifier(&srcu_module_nb);
	if (ret)
		pr_warn("Failed to register srcu module notifier\n");
	return ret;
}
late_initcall(init_srcu_module_notifier);

#endif /* #ifdef CONFIG_MODULES */
>>>>>>> upstream/android-13
