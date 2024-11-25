<<<<<<< HEAD
/*
 * RCU expedited grace periods
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
 * Copyright IBM Corporation, 2016
 *
 * Authors: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
 */

#include <linux/lockdep.h>
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * RCU expedited grace periods
 *
 * Copyright IBM Corporation, 2016
 *
 * Authors: Paul E. McKenney <paulmck@linux.ibm.com>
 */

#include <linux/lockdep.h>
#include <linux/sec_debug.h>

static void rcu_exp_handler(void *unused);
static int rcu_print_task_exp_stall(struct rcu_node *rnp);
>>>>>>> upstream/android-13

/*
 * Record the start of an expedited grace period.
 */
<<<<<<< HEAD
static void rcu_exp_gp_seq_start(struct rcu_state *rsp)
{
	rcu_seq_start(&rsp->expedited_sequence);
}

/*
 * Return then value that expedited-grace-period counter will have
 * at the end of the current grace period.
 */
static __maybe_unused unsigned long rcu_exp_gp_seq_endval(struct rcu_state *rsp)
{
	return rcu_seq_endval(&rsp->expedited_sequence);
=======
static void rcu_exp_gp_seq_start(void)
{
	rcu_seq_start(&rcu_state.expedited_sequence);
}

/*
 * Return the value that the expedited-grace-period counter will have
 * at the end of the current grace period.
 */
static __maybe_unused unsigned long rcu_exp_gp_seq_endval(void)
{
	return rcu_seq_endval(&rcu_state.expedited_sequence);
>>>>>>> upstream/android-13
}

/*
 * Record the end of an expedited grace period.
 */
<<<<<<< HEAD
static void rcu_exp_gp_seq_end(struct rcu_state *rsp)
{
	rcu_seq_end(&rsp->expedited_sequence);
=======
static void rcu_exp_gp_seq_end(void)
{
	rcu_seq_end(&rcu_state.expedited_sequence);
>>>>>>> upstream/android-13
	smp_mb(); /* Ensure that consecutive grace periods serialize. */
}

/*
<<<<<<< HEAD
 * Take a snapshot of the expedited-grace-period counter.
 */
static unsigned long rcu_exp_gp_seq_snap(struct rcu_state *rsp)
=======
 * Take a snapshot of the expedited-grace-period counter, which is the
 * earliest value that will indicate that a full grace period has
 * elapsed since the current time.
 */
static unsigned long rcu_exp_gp_seq_snap(void)
>>>>>>> upstream/android-13
{
	unsigned long s;

	smp_mb(); /* Caller's modifications seen first by other CPUs. */
<<<<<<< HEAD
	s = rcu_seq_snap(&rsp->expedited_sequence);
	trace_rcu_exp_grace_period(rsp->name, s, TPS("snap"));
=======
	s = rcu_seq_snap(&rcu_state.expedited_sequence);
	trace_rcu_exp_grace_period(rcu_state.name, s, TPS("snap"));
>>>>>>> upstream/android-13
	return s;
}

/*
 * Given a counter snapshot from rcu_exp_gp_seq_snap(), return true
 * if a full expedited grace period has elapsed since that snapshot
 * was taken.
 */
<<<<<<< HEAD
static bool rcu_exp_gp_seq_done(struct rcu_state *rsp, unsigned long s)
{
	return rcu_seq_done(&rsp->expedited_sequence, s);
=======
static bool rcu_exp_gp_seq_done(unsigned long s)
{
	return rcu_seq_done(&rcu_state.expedited_sequence, s);
>>>>>>> upstream/android-13
}

/*
 * Reset the ->expmaskinit values in the rcu_node tree to reflect any
 * recent CPU-online activity.  Note that these masks are not cleared
 * when CPUs go offline, so they reflect the union of all CPUs that have
 * ever been online.  This means that this function normally takes its
 * no-work-to-do fastpath.
 */
<<<<<<< HEAD
static void sync_exp_reset_tree_hotplug(struct rcu_state *rsp)
=======
static void sync_exp_reset_tree_hotplug(void)
>>>>>>> upstream/android-13
{
	bool done;
	unsigned long flags;
	unsigned long mask;
	unsigned long oldmask;
<<<<<<< HEAD
	int ncpus = smp_load_acquire(&rsp->ncpus); /* Order against locking. */
=======
	int ncpus = smp_load_acquire(&rcu_state.ncpus); /* Order vs. locking. */
>>>>>>> upstream/android-13
	struct rcu_node *rnp;
	struct rcu_node *rnp_up;

	/* If no new CPUs onlined since last time, nothing to do. */
<<<<<<< HEAD
	if (likely(ncpus == rsp->ncpus_snap))
		return;
	rsp->ncpus_snap = ncpus;
=======
	if (likely(ncpus == rcu_state.ncpus_snap))
		return;
	rcu_state.ncpus_snap = ncpus;
>>>>>>> upstream/android-13

	/*
	 * Each pass through the following loop propagates newly onlined
	 * CPUs for the current rcu_node structure up the rcu_node tree.
	 */
<<<<<<< HEAD
	rcu_for_each_leaf_node(rsp, rnp) {
=======
	rcu_for_each_leaf_node(rnp) {
>>>>>>> upstream/android-13
		raw_spin_lock_irqsave_rcu_node(rnp, flags);
		if (rnp->expmaskinit == rnp->expmaskinitnext) {
			raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
			continue;  /* No new CPUs, nothing to do. */
		}

		/* Update this node's mask, track old value for propagation. */
		oldmask = rnp->expmaskinit;
		rnp->expmaskinit = rnp->expmaskinitnext;
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);

		/* If was already nonzero, nothing to propagate. */
		if (oldmask)
			continue;

		/* Propagate the new CPU up the tree. */
		mask = rnp->grpmask;
		rnp_up = rnp->parent;
		done = false;
		while (rnp_up) {
			raw_spin_lock_irqsave_rcu_node(rnp_up, flags);
			if (rnp_up->expmaskinit)
				done = true;
			rnp_up->expmaskinit |= mask;
			raw_spin_unlock_irqrestore_rcu_node(rnp_up, flags);
			if (done)
				break;
			mask = rnp_up->grpmask;
			rnp_up = rnp_up->parent;
		}
	}
}

/*
 * Reset the ->expmask values in the rcu_node tree in preparation for
 * a new expedited grace period.
 */
<<<<<<< HEAD
static void __maybe_unused sync_exp_reset_tree(struct rcu_state *rsp)
=======
static void __maybe_unused sync_exp_reset_tree(void)
>>>>>>> upstream/android-13
{
	unsigned long flags;
	struct rcu_node *rnp;

<<<<<<< HEAD
	sync_exp_reset_tree_hotplug(rsp);
	rcu_for_each_node_breadth_first(rsp, rnp) {
		raw_spin_lock_irqsave_rcu_node(rnp, flags);
		WARN_ON_ONCE(rnp->expmask);
		rnp->expmask = rnp->expmaskinit;
=======
	sync_exp_reset_tree_hotplug();
	rcu_for_each_node_breadth_first(rnp) {
		raw_spin_lock_irqsave_rcu_node(rnp, flags);
		WARN_ON_ONCE(rnp->expmask);
		WRITE_ONCE(rnp->expmask, rnp->expmaskinit);
>>>>>>> upstream/android-13
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
	}
}

/*
 * Return non-zero if there is no RCU expedited grace period in progress
 * for the specified rcu_node structure, in other words, if all CPUs and
 * tasks covered by the specified rcu_node structure have done their bit
<<<<<<< HEAD
 * for the current expedited grace period.  Works only for preemptible
 * RCU -- other RCU implementation use other means.
 *
 * Caller must hold the specificed rcu_node structure's ->lock
 */
static bool sync_rcu_preempt_exp_done(struct rcu_node *rnp)
{
	raw_lockdep_assert_held_rcu_node(rnp);

	return rnp->exp_tasks == NULL &&
=======
 * for the current expedited grace period.
 */
static bool sync_rcu_exp_done(struct rcu_node *rnp)
{
	raw_lockdep_assert_held_rcu_node(rnp);
	return READ_ONCE(rnp->exp_tasks) == NULL &&
>>>>>>> upstream/android-13
	       READ_ONCE(rnp->expmask) == 0;
}

/*
<<<<<<< HEAD
 * Like sync_rcu_preempt_exp_done(), but this function assumes the caller
 * doesn't hold the rcu_node's ->lock, and will acquire and release the lock
 * itself
 */
static bool sync_rcu_preempt_exp_done_unlocked(struct rcu_node *rnp)
=======
 * Like sync_rcu_exp_done(), but where the caller does not hold the
 * rcu_node's ->lock.
 */
static bool sync_rcu_exp_done_unlocked(struct rcu_node *rnp)
>>>>>>> upstream/android-13
{
	unsigned long flags;
	bool ret;

	raw_spin_lock_irqsave_rcu_node(rnp, flags);
<<<<<<< HEAD
	ret = sync_rcu_preempt_exp_done(rnp);
=======
	ret = sync_rcu_exp_done(rnp);
>>>>>>> upstream/android-13
	raw_spin_unlock_irqrestore_rcu_node(rnp, flags);

	return ret;
}


/*
 * Report the exit from RCU read-side critical section for the last task
 * that queued itself during or before the current expedited preemptible-RCU
 * grace period.  This event is reported either to the rcu_node structure on
 * which the task was queued or to one of that rcu_node structure's ancestors,
 * recursively up the tree.  (Calm down, calm down, we do the recursion
 * iteratively!)
<<<<<<< HEAD
 *
 * Caller must hold the specified rcu_node structure's ->lock.
 */
static void __rcu_report_exp_rnp(struct rcu_state *rsp, struct rcu_node *rnp,
=======
 */
static void __rcu_report_exp_rnp(struct rcu_node *rnp,
>>>>>>> upstream/android-13
				 bool wake, unsigned long flags)
	__releases(rnp->lock)
{
	unsigned long mask;

<<<<<<< HEAD
	for (;;) {
		if (!sync_rcu_preempt_exp_done(rnp)) {
=======
	raw_lockdep_assert_held_rcu_node(rnp);
	for (;;) {
		if (!sync_rcu_exp_done(rnp)) {
>>>>>>> upstream/android-13
			if (!rnp->expmask)
				rcu_initiate_boost(rnp, flags);
			else
				raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
			break;
		}
		if (rnp->parent == NULL) {
			raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
			if (wake) {
				smp_mb(); /* EGP done before wake_up(). */
<<<<<<< HEAD
				swake_up_one(&rsp->expedited_wq);
=======
				swake_up_one(&rcu_state.expedited_wq);
>>>>>>> upstream/android-13
			}
			break;
		}
		mask = rnp->grpmask;
		raw_spin_unlock_rcu_node(rnp); /* irqs remain disabled */
		rnp = rnp->parent;
		raw_spin_lock_rcu_node(rnp); /* irqs already disabled */
		WARN_ON_ONCE(!(rnp->expmask & mask));
<<<<<<< HEAD
		rnp->expmask &= ~mask;
=======
		WRITE_ONCE(rnp->expmask, rnp->expmask & ~mask);
>>>>>>> upstream/android-13
	}
}

/*
 * Report expedited quiescent state for specified node.  This is a
 * lock-acquisition wrapper function for __rcu_report_exp_rnp().
 */
<<<<<<< HEAD
static void __maybe_unused rcu_report_exp_rnp(struct rcu_state *rsp,
					      struct rcu_node *rnp, bool wake)
=======
static void __maybe_unused rcu_report_exp_rnp(struct rcu_node *rnp, bool wake)
>>>>>>> upstream/android-13
{
	unsigned long flags;

	raw_spin_lock_irqsave_rcu_node(rnp, flags);
<<<<<<< HEAD
	__rcu_report_exp_rnp(rsp, rnp, wake, flags);
=======
	__rcu_report_exp_rnp(rnp, wake, flags);
>>>>>>> upstream/android-13
}

/*
 * Report expedited quiescent state for multiple CPUs, all covered by the
 * specified leaf rcu_node structure.
 */
<<<<<<< HEAD
static void rcu_report_exp_cpu_mult(struct rcu_state *rsp, struct rcu_node *rnp,
				    unsigned long mask, bool wake)
{
	unsigned long flags;
=======
static void rcu_report_exp_cpu_mult(struct rcu_node *rnp,
				    unsigned long mask, bool wake)
{
	int cpu;
	unsigned long flags;
	struct rcu_data *rdp;
>>>>>>> upstream/android-13

	raw_spin_lock_irqsave_rcu_node(rnp, flags);
	if (!(rnp->expmask & mask)) {
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
		return;
	}
<<<<<<< HEAD
	rnp->expmask &= ~mask;
	__rcu_report_exp_rnp(rsp, rnp, wake, flags); /* Releases rnp->lock. */
=======
	WRITE_ONCE(rnp->expmask, rnp->expmask & ~mask);
	for_each_leaf_node_cpu_mask(rnp, cpu, mask) {
		rdp = per_cpu_ptr(&rcu_data, cpu);
		if (!IS_ENABLED(CONFIG_NO_HZ_FULL) || !rdp->rcu_forced_tick_exp)
			continue;
		rdp->rcu_forced_tick_exp = false;
		tick_dep_clear_cpu(cpu, TICK_DEP_BIT_RCU_EXP);
	}
	__rcu_report_exp_rnp(rnp, wake, flags); /* Releases rnp->lock. */
>>>>>>> upstream/android-13
}

/*
 * Report expedited quiescent state for specified rcu_data (CPU).
 */
<<<<<<< HEAD
static void rcu_report_exp_rdp(struct rcu_state *rsp, struct rcu_data *rdp,
			       bool wake)
{
	rcu_report_exp_cpu_mult(rsp, rdp->mynode, rdp->grpmask, wake);
}

/* Common code for synchronize_{rcu,sched}_expedited() work-done checking. */
static bool sync_exp_work_done(struct rcu_state *rsp, unsigned long s)
{
	if (rcu_exp_gp_seq_done(rsp, s)) {
		trace_rcu_exp_grace_period(rsp->name, s, TPS("done"));
		/* Ensure test happens before caller kfree(). */
		smp_mb__before_atomic(); /* ^^^ */
=======
static void rcu_report_exp_rdp(struct rcu_data *rdp)
{
	WRITE_ONCE(rdp->exp_deferred_qs, false);
	rcu_report_exp_cpu_mult(rdp->mynode, rdp->grpmask, true);
}

/* Common code for work-done checking. */
static bool sync_exp_work_done(unsigned long s)
{
	if (rcu_exp_gp_seq_done(s)) {
		trace_rcu_exp_grace_period(rcu_state.name, s, TPS("done"));
		smp_mb(); /* Ensure test happens before caller kfree(). */
>>>>>>> upstream/android-13
		return true;
	}
	return false;
}

/*
 * Funnel-lock acquisition for expedited grace periods.  Returns true
 * if some other task completed an expedited grace period that this task
 * can piggy-back on, and with no mutex held.  Otherwise, returns false
 * with the mutex held, indicating that the caller must actually do the
 * expedited grace period.
 */
<<<<<<< HEAD
static bool exp_funnel_lock(struct rcu_state *rsp, unsigned long s)
{
	struct rcu_data *rdp = per_cpu_ptr(rsp->rda, raw_smp_processor_id());
	struct rcu_node *rnp = rdp->mynode;
	struct rcu_node *rnp_root = rcu_get_root(rsp);
=======
static bool exp_funnel_lock(unsigned long s)
{
	struct rcu_data *rdp = per_cpu_ptr(&rcu_data, raw_smp_processor_id());
	struct rcu_node *rnp = rdp->mynode;
	struct rcu_node *rnp_root = rcu_get_root();
>>>>>>> upstream/android-13

	/* Low-contention fastpath. */
	if (ULONG_CMP_LT(READ_ONCE(rnp->exp_seq_rq), s) &&
	    (rnp == rnp_root ||
	     ULONG_CMP_LT(READ_ONCE(rnp_root->exp_seq_rq), s)) &&
<<<<<<< HEAD
	    mutex_trylock(&rsp->exp_mutex))
=======
	    mutex_trylock(&rcu_state.exp_mutex))
>>>>>>> upstream/android-13
		goto fastpath;

	/*
	 * Each pass through the following loop works its way up
	 * the rcu_node tree, returning if others have done the work or
<<<<<<< HEAD
	 * otherwise falls through to acquire rsp->exp_mutex.  The mapping
=======
	 * otherwise falls through to acquire ->exp_mutex.  The mapping
>>>>>>> upstream/android-13
	 * from CPU to rcu_node structure can be inexact, as it is just
	 * promoting locality and is not strictly needed for correctness.
	 */
	for (; rnp != NULL; rnp = rnp->parent) {
<<<<<<< HEAD
		if (sync_exp_work_done(rsp, s))
=======
		if (sync_exp_work_done(s))
>>>>>>> upstream/android-13
			return true;

		/* Work not done, either wait here or go up. */
		spin_lock(&rnp->exp_lock);
		if (ULONG_CMP_GE(rnp->exp_seq_rq, s)) {

			/* Someone else doing GP, so wait for them. */
			spin_unlock(&rnp->exp_lock);
<<<<<<< HEAD
			trace_rcu_exp_funnel_lock(rsp->name, rnp->level,
						  rnp->grplo, rnp->grphi,
						  TPS("wait"));
			wait_event(rnp->exp_wq[rcu_seq_ctr(s) & 0x3],
				   sync_exp_work_done(rsp, s));
			return true;
		}
		rnp->exp_seq_rq = s; /* Followers can wait on us. */
		spin_unlock(&rnp->exp_lock);
		trace_rcu_exp_funnel_lock(rsp->name, rnp->level, rnp->grplo,
					  rnp->grphi, TPS("nxtlvl"));
	}
	mutex_lock(&rsp->exp_mutex);
fastpath:
	if (sync_exp_work_done(rsp, s)) {
		mutex_unlock(&rsp->exp_mutex);
		return true;
	}
	rcu_exp_gp_seq_start(rsp);
	trace_rcu_exp_grace_period(rsp->name, s, TPS("start"));
	return false;
}

/* Invoked on each online non-idle CPU for expedited quiescent state. */
static void sync_sched_exp_handler(void *data)
{
	struct rcu_data *rdp;
	struct rcu_node *rnp;
	struct rcu_state *rsp = data;

	rdp = this_cpu_ptr(rsp->rda);
	rnp = rdp->mynode;
	if (!(READ_ONCE(rnp->expmask) & rdp->grpmask) ||
	    __this_cpu_read(rcu_sched_data.cpu_no_qs.b.exp))
		return;
	if (rcu_is_cpu_rrupt_from_idle()) {
		rcu_report_exp_rdp(&rcu_sched_state,
				   this_cpu_ptr(&rcu_sched_data), true);
		return;
	}
	__this_cpu_write(rcu_sched_data.cpu_no_qs.b.exp, true);
	/* Store .exp before .rcu_urgent_qs. */
	smp_store_release(this_cpu_ptr(&rcu_dynticks.rcu_urgent_qs), true);
	resched_cpu(smp_processor_id());
}

/* Send IPI for expedited cleanup if needed at end of CPU-hotplug operation. */
static void sync_sched_exp_online_cleanup(int cpu)
{
	struct rcu_data *rdp;
	int ret;
	struct rcu_node *rnp;
	struct rcu_state *rsp = &rcu_sched_state;

	rdp = per_cpu_ptr(rsp->rda, cpu);
	rnp = rdp->mynode;
	if (!(READ_ONCE(rnp->expmask) & rdp->grpmask))
		return;
	ret = smp_call_function_single(cpu, sync_sched_exp_handler, rsp, 0);
	WARN_ON_ONCE(ret);
}

=======
			trace_rcu_exp_funnel_lock(rcu_state.name, rnp->level,
						  rnp->grplo, rnp->grphi,
						  TPS("wait"));
			wait_event(rnp->exp_wq[rcu_seq_ctr(s) & 0x3],
				   sync_exp_work_done(s));
			return true;
		}
		WRITE_ONCE(rnp->exp_seq_rq, s); /* Followers can wait on us. */
		spin_unlock(&rnp->exp_lock);
		trace_rcu_exp_funnel_lock(rcu_state.name, rnp->level,
					  rnp->grplo, rnp->grphi, TPS("nxtlvl"));
	}
	mutex_lock(&rcu_state.exp_mutex);
fastpath:
	if (sync_exp_work_done(s)) {
		mutex_unlock(&rcu_state.exp_mutex);
		return true;
	}
	rcu_exp_gp_seq_start();
	trace_rcu_exp_grace_period(rcu_state.name, s, TPS("start"));
	return false;
}

>>>>>>> upstream/android-13
/*
 * Select the CPUs within the specified rcu_node that the upcoming
 * expedited grace period needs to wait for.
 */
<<<<<<< HEAD
static void sync_rcu_exp_select_node_cpus(struct work_struct *wp)
{
	int cpu;
	unsigned long flags;
	smp_call_func_t func;
	unsigned long mask_ofl_test;
	unsigned long mask_ofl_ipi;
	int ret;
	struct rcu_exp_work *rewp =
		container_of(wp, struct rcu_exp_work, rew_work);
	struct rcu_node *rnp = container_of(rewp, struct rcu_node, rew);
	struct rcu_state *rsp = rewp->rew_rsp;

	func = rewp->rew_func;
=======
static void __sync_rcu_exp_select_node_cpus(struct rcu_exp_work *rewp)
{
	int cpu;
	unsigned long flags;
	unsigned long mask_ofl_test;
	unsigned long mask_ofl_ipi;
	int ret;
	struct rcu_node *rnp = container_of(rewp, struct rcu_node, rew);

>>>>>>> upstream/android-13
	raw_spin_lock_irqsave_rcu_node(rnp, flags);

	/* Each pass checks a CPU for identity, offline, and idle. */
	mask_ofl_test = 0;
	for_each_leaf_node_cpu_mask(rnp, cpu, rnp->expmask) {
<<<<<<< HEAD
		unsigned long mask = leaf_node_cpu_bit(rnp, cpu);
		struct rcu_data *rdp = per_cpu_ptr(rsp->rda, cpu);
		struct rcu_dynticks *rdtp = per_cpu_ptr(&rcu_dynticks, cpu);
=======
		struct rcu_data *rdp = per_cpu_ptr(&rcu_data, cpu);
		unsigned long mask = rdp->grpmask;
>>>>>>> upstream/android-13
		int snap;

		if (raw_smp_processor_id() == cpu ||
		    !(rnp->qsmaskinitnext & mask)) {
			mask_ofl_test |= mask;
		} else {
<<<<<<< HEAD
			snap = rcu_dynticks_snap(rdtp);
=======
			snap = rcu_dynticks_snap(rdp);
>>>>>>> upstream/android-13
			if (rcu_dynticks_in_eqs(snap))
				mask_ofl_test |= mask;
			else
				rdp->exp_dynticks_snap = snap;
		}
	}
	mask_ofl_ipi = rnp->expmask & ~mask_ofl_test;

	/*
	 * Need to wait for any blocked tasks as well.	Note that
	 * additional blocking tasks will also block the expedited GP
	 * until such time as the ->expmask bits are cleared.
	 */
	if (rcu_preempt_has_tasks(rnp))
<<<<<<< HEAD
		rnp->exp_tasks = rnp->blkd_tasks.next;
	raw_spin_unlock_irqrestore_rcu_node(rnp, flags);

	/* IPI the remaining CPUs for expedited quiescent state. */
	for_each_leaf_node_cpu_mask(rnp, cpu, rnp->expmask) {
		unsigned long mask = leaf_node_cpu_bit(rnp, cpu);
		struct rcu_data *rdp = per_cpu_ptr(rsp->rda, cpu);

		if (!(mask_ofl_ipi & mask))
			continue;
retry_ipi:
		if (rcu_dynticks_in_eqs_since(rdp->dynticks,
					      rdp->exp_dynticks_snap)) {
			mask_ofl_test |= mask;
			continue;
		}
		ret = smp_call_function_single(cpu, func, rsp, 0);
		if (!ret) {
			mask_ofl_ipi &= ~mask;
			continue;
		}
=======
		WRITE_ONCE(rnp->exp_tasks, rnp->blkd_tasks.next);
	raw_spin_unlock_irqrestore_rcu_node(rnp, flags);

	/* IPI the remaining CPUs for expedited quiescent state. */
	for_each_leaf_node_cpu_mask(rnp, cpu, mask_ofl_ipi) {
		struct rcu_data *rdp = per_cpu_ptr(&rcu_data, cpu);
		unsigned long mask = rdp->grpmask;

retry_ipi:
		if (rcu_dynticks_in_eqs_since(rdp, rdp->exp_dynticks_snap)) {
			mask_ofl_test |= mask;
			continue;
		}
		if (get_cpu() == cpu) {
			mask_ofl_test |= mask;
			put_cpu();
			continue;
		}
		ret = smp_call_function_single(cpu, rcu_exp_handler, NULL, 0);
		put_cpu();
		/* The CPU will report the QS in response to the IPI. */
		if (!ret)
			continue;

>>>>>>> upstream/android-13
		/* Failed, raced with CPU hotplug operation. */
		raw_spin_lock_irqsave_rcu_node(rnp, flags);
		if ((rnp->qsmaskinitnext & mask) &&
		    (rnp->expmask & mask)) {
			/* Online, so delay for a bit and try again. */
			raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
<<<<<<< HEAD
			trace_rcu_exp_grace_period(rsp->name, rcu_exp_gp_seq_endval(rsp), TPS("selectofl"));
			schedule_timeout_uninterruptible(1);
			goto retry_ipi;
		}
		/* CPU really is offline, so we can ignore it. */
		if (!(rnp->expmask & mask))
			mask_ofl_ipi &= ~mask;
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
	}
	/* Report quiescent states for those that went offline. */
	mask_ofl_test |= mask_ofl_ipi;
	if (mask_ofl_test)
		rcu_report_exp_cpu_mult(rsp, rnp, mask_ofl_test, false);
}

/*
 * Select the nodes that the upcoming expedited grace period needs
 * to wait for.
 */
static void sync_rcu_exp_select_cpus(struct rcu_state *rsp,
				     smp_call_func_t func)
{
	int cpu;
	struct rcu_node *rnp;

	trace_rcu_exp_grace_period(rsp->name, rcu_exp_gp_seq_endval(rsp), TPS("reset"));
	sync_exp_reset_tree(rsp);
	trace_rcu_exp_grace_period(rsp->name, rcu_exp_gp_seq_endval(rsp), TPS("select"));

	/* Schedule work for each leaf rcu_node structure. */
	rcu_for_each_leaf_node(rsp, rnp) {
		rnp->exp_need_flush = false;
		if (!READ_ONCE(rnp->expmask))
			continue; /* Avoid early boot non-existent wq. */
		rnp->rew.rew_func = func;
		rnp->rew.rew_rsp = rsp;
		if (!READ_ONCE(rcu_par_gp_wq) ||
		    rcu_scheduler_active != RCU_SCHEDULER_RUNNING ||
		    rcu_is_last_leaf_node(rsp, rnp)) {
			/* No workqueues yet or last leaf, do direct call. */
			sync_rcu_exp_select_node_cpus(&rnp->rew.rew_work);
			continue;
		}
		INIT_WORK(&rnp->rew.rew_work, sync_rcu_exp_select_node_cpus);
		preempt_disable();
		cpu = cpumask_next(rnp->grplo - 1, cpu_online_mask);
		/* If all offline, queue the work on an unbound CPU. */
		if (unlikely(cpu > rnp->grphi))
			cpu = WORK_CPU_UNBOUND;
		queue_work_on(cpu, rcu_par_gp_wq, &rnp->rew.rew_work);
		preempt_enable();
		rnp->exp_need_flush = true;
	}

	/* Wait for workqueue jobs (if any) to complete. */
	rcu_for_each_leaf_node(rsp, rnp)
		if (rnp->exp_need_flush)
			flush_work(&rnp->rew.rew_work);
}

static void synchronize_sched_expedited_wait(struct rcu_state *rsp)
{
	int cpu;
	unsigned long jiffies_stall;
	unsigned long jiffies_start;
	unsigned long mask;
	int ndetected;
	struct rcu_node *rnp;
	struct rcu_node *rnp_root = rcu_get_root(rsp);
	int ret;

	trace_rcu_exp_grace_period(rsp->name, rcu_exp_gp_seq_endval(rsp), TPS("startwait"));
	jiffies_stall = rcu_jiffies_till_stall_check();
	jiffies_start = jiffies;

	for (;;) {
		ret = swait_event_timeout_exclusive(
				rsp->expedited_wq,
				sync_rcu_preempt_exp_done_unlocked(rnp_root),
				jiffies_stall);
		if (ret > 0 || sync_rcu_preempt_exp_done_unlocked(rnp_root))
			return;
		WARN_ON(ret < 0);  /* workqueues should not be signaled. */
		if (rcu_cpu_stall_suppress)
			continue;
		panic_on_rcu_stall();
		pr_err("INFO: %s detected expedited stalls on CPUs/tasks: {",
		       rsp->name);
		ndetected = 0;
		rcu_for_each_leaf_node(rsp, rnp) {
			ndetected += rcu_print_task_exp_stall(rnp);
			for_each_leaf_node_possible_cpu(rnp, cpu) {
				struct rcu_data *rdp;

				mask = leaf_node_cpu_bit(rnp, cpu);
				if (!(rnp->expmask & mask))
					continue;
				ndetected++;
				rdp = per_cpu_ptr(rsp->rda, cpu);
				pr_cont(" %d-%c%c%c", cpu,
					"O."[!!cpu_online(cpu)],
					"o."[!!(rdp->grpmask & rnp->expmaskinit)],
					"N."[!!(rdp->grpmask & rnp->expmaskinitnext)]);
			}
		}
		pr_cont(" } %lu jiffies s: %lu root: %#lx/%c\n",
			jiffies - jiffies_start, rsp->expedited_sequence,
			rnp_root->expmask, ".T"[!!rnp_root->exp_tasks]);
		if (ndetected) {
			pr_err("blocking rcu_node structures:");
			rcu_for_each_node_breadth_first(rsp, rnp) {
				if (rnp == rnp_root)
					continue; /* printed unconditionally */
				if (sync_rcu_preempt_exp_done_unlocked(rnp))
					continue;
				pr_cont(" l=%u:%d-%d:%#lx/%c",
					rnp->level, rnp->grplo, rnp->grphi,
					rnp->expmask,
					".T"[!!rnp->exp_tasks]);
			}
			pr_cont("\n");
		}
		rcu_for_each_leaf_node(rsp, rnp) {
			for_each_leaf_node_possible_cpu(rnp, cpu) {
				mask = leaf_node_cpu_bit(rnp, cpu);
				if (!(rnp->expmask & mask))
					continue;
				dump_cpu_task(cpu);
			}
		}
		jiffies_stall = 3 * rcu_jiffies_till_stall_check() + 3;
	}
}

/*
 * Wait for the current expedited grace period to complete, and then
 * wake up everyone who piggybacked on the just-completed expedited
 * grace period.  Also update all the ->exp_seq_rq counters as needed
 * in order to avoid counter-wrap problems.
 */
static void rcu_exp_wait_wake(struct rcu_state *rsp, unsigned long s)
{
	struct rcu_node *rnp;

	synchronize_sched_expedited_wait(rsp);

	// Switch over to wakeup mode, allowing the next GP to proceed.
	// End the previous grace period only after acquiring the mutex
	// to ensure that only one GP runs concurrently with wakeups.
	mutex_lock(&rsp->exp_wake_mutex);
	rcu_exp_gp_seq_end(rsp);
	trace_rcu_exp_grace_period(rsp->name, s, TPS("end"));

	rcu_for_each_node_breadth_first(rsp, rnp) {
		if (ULONG_CMP_LT(READ_ONCE(rnp->exp_seq_rq), s)) {
			spin_lock(&rnp->exp_lock);
			/* Recheck, avoid hang in case someone just arrived. */
			if (ULONG_CMP_LT(rnp->exp_seq_rq, s))
				rnp->exp_seq_rq = s;
			spin_unlock(&rnp->exp_lock);
		}
		smp_mb(); /* All above changes before wakeup. */
		wake_up_all(&rnp->exp_wq[rcu_seq_ctr(s) & 0x3]);
	}
	trace_rcu_exp_grace_period(rsp->name, s, TPS("endwake"));
	mutex_unlock(&rsp->exp_wake_mutex);
}

/*
 * Common code to drive an expedited grace period forward, used by
 * workqueues and mid-boot-time tasks.
 */
static void rcu_exp_sel_wait_wake(struct rcu_state *rsp,
				  smp_call_func_t func, unsigned long s)
{
	/* Initialize the rcu_node tree in preparation for the wait. */
	sync_rcu_exp_select_cpus(rsp, func);

	/* Wait and clean up, including waking everyone. */
	rcu_exp_wait_wake(rsp, s);
=======
			trace_rcu_exp_grace_period(rcu_state.name, rcu_exp_gp_seq_endval(), TPS("selectofl"));
			schedule_timeout_idle(1);
			goto retry_ipi;
		}
		/* CPU really is offline, so we must report its QS. */
		if (rnp->expmask & mask)
			mask_ofl_test |= mask;
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
	}
	/* Report quiescent states for those that went offline. */
	if (mask_ofl_test)
		rcu_report_exp_cpu_mult(rnp, mask_ofl_test, false);
}

static void rcu_exp_sel_wait_wake(unsigned long s);

#ifdef CONFIG_RCU_EXP_KTHREAD
static void sync_rcu_exp_select_node_cpus(struct kthread_work *wp)
{
	struct rcu_exp_work *rewp =
		container_of(wp, struct rcu_exp_work, rew_work);

	__sync_rcu_exp_select_node_cpus(rewp);
}

static inline bool rcu_gp_par_worker_started(void)
{
	return !!READ_ONCE(rcu_exp_par_gp_kworker);
}

static inline void sync_rcu_exp_select_cpus_queue_work(struct rcu_node *rnp)
{
	kthread_init_work(&rnp->rew.rew_work, sync_rcu_exp_select_node_cpus);
	/*
	 * Use rcu_exp_par_gp_kworker, because flushing a work item from
	 * another work item on the same kthread worker can result in
	 * deadlock.
	 */
	kthread_queue_work(rcu_exp_par_gp_kworker, &rnp->rew.rew_work);
}

static inline void sync_rcu_exp_select_cpus_flush_work(struct rcu_node *rnp)
{
	kthread_flush_work(&rnp->rew.rew_work);
}

/*
 * Work-queue handler to drive an expedited grace period forward.
 */
static void wait_rcu_exp_gp(struct kthread_work *wp)
{
	struct rcu_exp_work *rewp;

	rewp = container_of(wp, struct rcu_exp_work, rew_work);
	rcu_exp_sel_wait_wake(rewp->rew_s);
}

static inline void synchronize_rcu_expedited_queue_work(struct rcu_exp_work *rew)
{
	kthread_init_work(&rew->rew_work, wait_rcu_exp_gp);
	kthread_queue_work(rcu_exp_gp_kworker, &rew->rew_work);
}

static inline void synchronize_rcu_expedited_destroy_work(struct rcu_exp_work *rew)
{
}
#else /* !CONFIG_RCU_EXP_KTHREAD */
static void sync_rcu_exp_select_node_cpus(struct work_struct *wp)
{
	struct rcu_exp_work *rewp =
		container_of(wp, struct rcu_exp_work, rew_work);

	__sync_rcu_exp_select_node_cpus(rewp);
}

static inline bool rcu_gp_par_worker_started(void)
{
	return !!READ_ONCE(rcu_par_gp_wq);
}

static inline void sync_rcu_exp_select_cpus_queue_work(struct rcu_node *rnp)
{
	int cpu = find_next_bit(&rnp->ffmask, BITS_PER_LONG, -1);

	INIT_WORK(&rnp->rew.rew_work, sync_rcu_exp_select_node_cpus);
	/* If all offline, queue the work on an unbound CPU. */
	if (unlikely(cpu > rnp->grphi - rnp->grplo))
		cpu = WORK_CPU_UNBOUND;
	else
		cpu += rnp->grplo;
	queue_work_on(cpu, rcu_par_gp_wq, &rnp->rew.rew_work);
}

static inline void sync_rcu_exp_select_cpus_flush_work(struct rcu_node *rnp)
{
	flush_work(&rnp->rew.rew_work);
>>>>>>> upstream/android-13
}

/*
 * Work-queue handler to drive an expedited grace period forward.
 */
static void wait_rcu_exp_gp(struct work_struct *wp)
{
	struct rcu_exp_work *rewp;

	rewp = container_of(wp, struct rcu_exp_work, rew_work);
<<<<<<< HEAD
	rcu_exp_sel_wait_wake(rewp->rew_rsp, rewp->rew_func, rewp->rew_s);
}

/*
 * Given an rcu_state pointer and a smp_call_function() handler, kick
 * off the specified flavor of expedited grace period.
 */
static void _synchronize_rcu_expedited(struct rcu_state *rsp,
				       smp_call_func_t func)
{
	struct rcu_data *rdp;
	struct rcu_exp_work rew;
	struct rcu_node *rnp;
	unsigned long s;

	/* If expedited grace periods are prohibited, fall back to normal. */
	if (rcu_gp_is_normal()) {
		wait_rcu_gp(rsp->call);
		return;
	}

	/* Take a snapshot of the sequence number.  */
	s = rcu_exp_gp_seq_snap(rsp);
	if (exp_funnel_lock(rsp, s))
		return;  /* Someone else did our work for us. */

	/* Ensure that load happens before action based on it. */
	if (unlikely(rcu_scheduler_active == RCU_SCHEDULER_INIT)) {
		/* Direct call during scheduler init and early_initcalls(). */
		rcu_exp_sel_wait_wake(rsp, func, s);
	} else {
		/* Marshall arguments & schedule the expedited grace period. */
		rew.rew_func = func;
		rew.rew_rsp = rsp;
		rew.rew_s = s;
		INIT_WORK_ONSTACK(&rew.rew_work, wait_rcu_exp_gp);
		queue_work(rcu_gp_wq, &rew.rew_work);
	}

	/* Wait for expedited grace period to complete. */
	rdp = per_cpu_ptr(rsp->rda, raw_smp_processor_id());
	rnp = rcu_get_root(rsp);
	wait_event(rnp->exp_wq[rcu_seq_ctr(s) & 0x3],
		   sync_exp_work_done(rsp, s));
	smp_mb(); /* Workqueue actions happen before return. */

	/* Let the next expedited grace period start. */
	mutex_unlock(&rsp->exp_mutex);
}

/**
 * synchronize_sched_expedited - Brute-force RCU-sched grace period
 *
 * Wait for an RCU-sched grace period to elapse, but use a "big hammer"
 * approach to force the grace period to end quickly.  This consumes
 * significant time on all CPUs and is unfriendly to real-time workloads,
 * so is thus not recommended for any sort of common-case code.  In fact,
 * if you are using synchronize_sched_expedited() in a loop, please
 * restructure your code to batch your updates, and then use a single
 * synchronize_sched() instead.
 *
 * This implementation can be thought of as an application of sequence
 * locking to expedited grace periods, but using the sequence counter to
 * determine when someone else has already done the work instead of for
 * retrying readers.
 */
void synchronize_sched_expedited(void)
{
	struct rcu_state *rsp = &rcu_sched_state;

	RCU_LOCKDEP_WARN(lock_is_held(&rcu_bh_lock_map) ||
			 lock_is_held(&rcu_lock_map) ||
			 lock_is_held(&rcu_sched_lock_map),
			 "Illegal synchronize_sched_expedited() in RCU read-side critical section");

	/* If only one CPU, this is automatically a grace period. */
	if (rcu_blocking_is_gp())
		return;

	_synchronize_rcu_expedited(rsp, sync_sched_exp_handler);
}
EXPORT_SYMBOL_GPL(synchronize_sched_expedited);
=======
	rcu_exp_sel_wait_wake(rewp->rew_s);
}

static inline void synchronize_rcu_expedited_queue_work(struct rcu_exp_work *rew)
{
	INIT_WORK_ONSTACK(&rew->rew_work, wait_rcu_exp_gp);
	queue_work(rcu_gp_wq, &rew->rew_work);
	secdbg_dtsk_built_set_data(DTYPE_WORK, &rew.rew_work);
}

static inline void synchronize_rcu_expedited_destroy_work(struct rcu_exp_work *rew)
{
	destroy_work_on_stack(&rew->rew_work);
}
#endif /* CONFIG_RCU_EXP_KTHREAD */

/*
 * Select the nodes that the upcoming expedited grace period needs
 * to wait for.
 */
static void sync_rcu_exp_select_cpus(void)
{
	struct rcu_node *rnp;

	trace_rcu_exp_grace_period(rcu_state.name, rcu_exp_gp_seq_endval(), TPS("reset"));
	sync_exp_reset_tree();
	trace_rcu_exp_grace_period(rcu_state.name, rcu_exp_gp_seq_endval(), TPS("select"));

	/* Schedule work for each leaf rcu_node structure. */
	rcu_for_each_leaf_node(rnp) {
		rnp->exp_need_flush = false;
		if (!READ_ONCE(rnp->expmask))
			continue; /* Avoid early boot non-existent wq. */
		if (!rcu_gp_par_worker_started() ||
		    rcu_scheduler_active != RCU_SCHEDULER_RUNNING ||
		    rcu_is_last_leaf_node(rnp)) {
			/* No worker started yet or last leaf, do direct call. */
			sync_rcu_exp_select_node_cpus(&rnp->rew.rew_work);
			continue;
		}
		sync_rcu_exp_select_cpus_queue_work(rnp);
		rnp->exp_need_flush = true;
	}

	/* Wait for jobs (if any) to complete. */
	rcu_for_each_leaf_node(rnp)
		if (rnp->exp_need_flush)
			sync_rcu_exp_select_cpus_flush_work(rnp);
}

/*
 * Wait for the expedited grace period to elapse, within time limit.
 * If the time limit is exceeded without the grace period elapsing,
 * return false, otherwise return true.
 */
static bool synchronize_rcu_expedited_wait_once(long tlimit)
{
	int t;
	struct rcu_node *rnp_root = rcu_get_root();

	t = swait_event_timeout_exclusive(rcu_state.expedited_wq,
					  sync_rcu_exp_done_unlocked(rnp_root),
					  tlimit);
	// Workqueues should not be signaled.
	if (t > 0 || sync_rcu_exp_done_unlocked(rnp_root))
		return true;
	WARN_ON(t < 0);  /* workqueues should not be signaled. */
	return false;
}

/*
 * Wait for the expedited grace period to elapse, issuing any needed
 * RCU CPU stall warnings along the way.
 */
static void synchronize_rcu_expedited_wait(void)
{
	int cpu;
	unsigned long j;
	unsigned long jiffies_stall;
	unsigned long jiffies_start;
	unsigned long mask;
	int ndetected;
	struct rcu_data *rdp;
	struct rcu_node *rnp;
	struct rcu_node *rnp_root = rcu_get_root();

	trace_rcu_exp_grace_period(rcu_state.name, rcu_exp_gp_seq_endval(), TPS("startwait"));
	jiffies_stall = rcu_jiffies_till_stall_check();
	jiffies_start = jiffies;
	if (tick_nohz_full_enabled() && rcu_inkernel_boot_has_ended()) {
		if (synchronize_rcu_expedited_wait_once(1))
			return;
		rcu_for_each_leaf_node(rnp) {
			for_each_leaf_node_cpu_mask(rnp, cpu, rnp->expmask) {
				rdp = per_cpu_ptr(&rcu_data, cpu);
				if (rdp->rcu_forced_tick_exp)
					continue;
				rdp->rcu_forced_tick_exp = true;
				tick_dep_set_cpu(cpu, TICK_DEP_BIT_RCU_EXP);
			}
		}
		j = READ_ONCE(jiffies_till_first_fqs);
		if (synchronize_rcu_expedited_wait_once(j + HZ))
			return;
		WARN_ON_ONCE(IS_ENABLED(CONFIG_PREEMPT_RT));
	}

	for (;;) {
		if (synchronize_rcu_expedited_wait_once(jiffies_stall))
			return;
		if (rcu_stall_is_suppressed())
			continue;
		panic_on_rcu_stall();
		trace_rcu_stall_warning(rcu_state.name, TPS("ExpeditedStall"));
		pr_auto(ASL1, "INFO: %s detected expedited stalls on CPUs/tasks: {",
		       rcu_state.name);
		ndetected = 0;
		rcu_for_each_leaf_node(rnp) {
			ndetected += rcu_print_task_exp_stall(rnp);
			for_each_leaf_node_possible_cpu(rnp, cpu) {
				struct rcu_data *rdp;

				mask = leaf_node_cpu_bit(rnp, cpu);
				if (!(READ_ONCE(rnp->expmask) & mask))
					continue;
				ndetected++;
				rdp = per_cpu_ptr(&rcu_data, cpu);
				pr_cont(" %d-%c%c%c", cpu,
					"O."[!!cpu_online(cpu)],
					"o."[!!(rdp->grpmask & rnp->expmaskinit)],
					"N."[!!(rdp->grpmask & rnp->expmaskinitnext)]);
			}
		}
		pr_cont(" } %lu jiffies s: %lu root: %#lx/%c\n",
			jiffies - jiffies_start, rcu_state.expedited_sequence,
			data_race(rnp_root->expmask),
			".T"[!!data_race(rnp_root->exp_tasks)]);
		if (ndetected) {
			pr_err("blocking rcu_node structures (internal RCU debug):");
			rcu_for_each_node_breadth_first(rnp) {
				if (rnp == rnp_root)
					continue; /* printed unconditionally */
				if (sync_rcu_exp_done_unlocked(rnp))
					continue;
				pr_cont(" l=%u:%d-%d:%#lx/%c",
					rnp->level, rnp->grplo, rnp->grphi,
					data_race(rnp->expmask),
					".T"[!!data_race(rnp->exp_tasks)]);
			}
			pr_cont("\n");
		}
		rcu_for_each_leaf_node(rnp) {
			for_each_leaf_node_possible_cpu(rnp, cpu) {
				mask = leaf_node_cpu_bit(rnp, cpu);
				if (!(READ_ONCE(rnp->expmask) & mask))
					continue;
				dump_cpu_task(cpu);
			}
		}
		if (IS_ENABLED(CONFIG_SEC_DEBUG_PANIC_ON_RCU_STALL))
			panic("RCU Stall\n");

		jiffies_stall = 3 * rcu_jiffies_till_stall_check() + 3;
	}
}

/*
 * Wait for the current expedited grace period to complete, and then
 * wake up everyone who piggybacked on the just-completed expedited
 * grace period.  Also update all the ->exp_seq_rq counters as needed
 * in order to avoid counter-wrap problems.
 */
static void rcu_exp_wait_wake(unsigned long s)
{
	struct rcu_node *rnp;

	synchronize_rcu_expedited_wait();

	// Switch over to wakeup mode, allowing the next GP to proceed.
	// End the previous grace period only after acquiring the mutex
	// to ensure that only one GP runs concurrently with wakeups.
	mutex_lock(&rcu_state.exp_wake_mutex);
	rcu_exp_gp_seq_end();
	trace_rcu_exp_grace_period(rcu_state.name, s, TPS("end"));

	rcu_for_each_node_breadth_first(rnp) {
		if (ULONG_CMP_LT(READ_ONCE(rnp->exp_seq_rq), s)) {
			spin_lock(&rnp->exp_lock);
			/* Recheck, avoid hang in case someone just arrived. */
			if (ULONG_CMP_LT(rnp->exp_seq_rq, s))
				WRITE_ONCE(rnp->exp_seq_rq, s);
			spin_unlock(&rnp->exp_lock);
		}
		smp_mb(); /* All above changes before wakeup. */
		wake_up_all(&rnp->exp_wq[rcu_seq_ctr(s) & 0x3]);
	}
	trace_rcu_exp_grace_period(rcu_state.name, s, TPS("endwake"));
	mutex_unlock(&rcu_state.exp_wake_mutex);
}

/*
 * Common code to drive an expedited grace period forward, used by
 * workqueues and mid-boot-time tasks.
 */
static void rcu_exp_sel_wait_wake(unsigned long s)
{
	/* Initialize the rcu_node tree in preparation for the wait. */
	sync_rcu_exp_select_cpus();

	/* Wait and clean up, including waking everyone. */
	rcu_exp_wait_wake(s);
}
>>>>>>> upstream/android-13

#ifdef CONFIG_PREEMPT_RCU

/*
 * Remote handler for smp_call_function_single().  If there is an
 * RCU read-side critical section in effect, request that the
 * next rcu_read_unlock() record the quiescent state up the
 * ->expmask fields in the rcu_node tree.  Otherwise, immediately
 * report the quiescent state.
 */
<<<<<<< HEAD
static void sync_rcu_exp_handler(void *info)
{
	struct rcu_data *rdp;
	struct rcu_state *rsp = info;
	struct task_struct *t = current;

	/*
	 * Within an RCU read-side critical section, request that the next
	 * rcu_read_unlock() report.  Unless this RCU read-side critical
	 * section has already blocked, in which case it is already set
	 * up for the expedited grace period to wait on it.
	 */
	if (t->rcu_read_lock_nesting > 0 &&
	    !t->rcu_read_unlock_special.b.blocked) {
		t->rcu_read_unlock_special.b.exp_need_qs = true;
=======
static void rcu_exp_handler(void *unused)
{
	int depth = rcu_preempt_depth();
	unsigned long flags;
	struct rcu_data *rdp = this_cpu_ptr(&rcu_data);
	struct rcu_node *rnp = rdp->mynode;
	struct task_struct *t = current;

	/*
	 * First, the common case of not being in an RCU read-side
	 * critical section.  If also enabled or idle, immediately
	 * report the quiescent state, otherwise defer.
	 */
	if (!depth) {
		if (!(preempt_count() & (PREEMPT_MASK | SOFTIRQ_MASK)) ||
		    rcu_dynticks_curr_cpu_in_eqs()) {
			rcu_report_exp_rdp(rdp);
		} else {
			rdp->exp_deferred_qs = true;
			set_tsk_need_resched(t);
			set_preempt_need_resched();
		}
>>>>>>> upstream/android-13
		return;
	}

	/*
<<<<<<< HEAD
	 * We are either exiting an RCU read-side critical section (negative
	 * values of t->rcu_read_lock_nesting) or are not in one at all
	 * (zero value of t->rcu_read_lock_nesting).  Or we are in an RCU
	 * read-side critical section that blocked before this expedited
	 * grace period started.  Either way, we can immediately report
	 * the quiescent state.
	 */
	rdp = this_cpu_ptr(rsp->rda);
	rcu_report_exp_rdp(rsp, rdp, true);
}

/**
 * synchronize_rcu_expedited - Brute-force RCU grace period
 *
 * Wait for an RCU-preempt grace period, but expedite it.  The basic
 * idea is to IPI all non-idle non-nohz online CPUs.  The IPI handler
 * checks whether the CPU is in an RCU-preempt critical section, and
 * if so, it sets a flag that causes the outermost rcu_read_unlock()
 * to report the quiescent state.  On the other hand, if the CPU is
 * not in an RCU read-side critical section, the IPI handler reports
 * the quiescent state immediately.
 *
 * Although this is a greate improvement over previous expedited
 * implementations, it is still unfriendly to real-time workloads, so is
 * thus not recommended for any sort of common-case code.  In fact, if
 * you are using synchronize_rcu_expedited() in a loop, please restructure
 * your code to batch your updates, and then Use a single synchronize_rcu()
 * instead.
 */
void synchronize_rcu_expedited(void)
{
	struct rcu_state *rsp = rcu_state_p;
=======
	 * Second, the less-common case of being in an RCU read-side
	 * critical section.  In this case we can count on a future
	 * rcu_read_unlock().  However, this rcu_read_unlock() might
	 * execute on some other CPU, but in that case there will be
	 * a future context switch.  Either way, if the expedited
	 * grace period is still waiting on this CPU, set ->deferred_qs
	 * so that the eventual quiescent state will be reported.
	 * Note that there is a large group of race conditions that
	 * can have caused this quiescent state to already have been
	 * reported, so we really do need to check ->expmask.
	 */
	if (depth > 0) {
		raw_spin_lock_irqsave_rcu_node(rnp, flags);
		if (rnp->expmask & rdp->grpmask) {
			rdp->exp_deferred_qs = true;
			t->rcu_read_unlock_special.b.exp_hint = true;
		}
		raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
		return;
	}

	// Finally, negative nesting depth should not happen.
	WARN_ON_ONCE(1);
}

/* PREEMPTION=y, so no PREEMPTION=n expedited grace period to clean up after. */
static void sync_sched_exp_online_cleanup(int cpu)
{
}

/*
 * Scan the current list of tasks blocked within RCU read-side critical
 * sections, printing out the tid of each that is blocking the current
 * expedited grace period.
 */
static int rcu_print_task_exp_stall(struct rcu_node *rnp)
{
	unsigned long flags;
	int ndetected = 0;
	struct task_struct *t;

	if (!READ_ONCE(rnp->exp_tasks))
		return 0;
	raw_spin_lock_irqsave_rcu_node(rnp, flags);
	t = list_entry(rnp->exp_tasks->prev,
		       struct task_struct, rcu_node_entry);
	list_for_each_entry_continue(t, &rnp->blkd_tasks, rcu_node_entry) {
		pr_cont(" P%d", t->pid);
		ndetected++;
	}
	raw_spin_unlock_irqrestore_rcu_node(rnp, flags);
	return ndetected;
}

#else /* #ifdef CONFIG_PREEMPT_RCU */

/* Request an expedited quiescent state. */
static void rcu_exp_need_qs(void)
{
	__this_cpu_write(rcu_data.cpu_no_qs.b.exp, true);
	/* Store .exp before .rcu_urgent_qs. */
	smp_store_release(this_cpu_ptr(&rcu_data.rcu_urgent_qs), true);
	set_tsk_need_resched(current);
	set_preempt_need_resched();
}

/* Invoked on each online non-idle CPU for expedited quiescent state. */
static void rcu_exp_handler(void *unused)
{
	struct rcu_data *rdp = this_cpu_ptr(&rcu_data);
	struct rcu_node *rnp = rdp->mynode;

	if (!(READ_ONCE(rnp->expmask) & rdp->grpmask) ||
	    __this_cpu_read(rcu_data.cpu_no_qs.b.exp))
		return;
	if (rcu_is_cpu_rrupt_from_idle()) {
		rcu_report_exp_rdp(this_cpu_ptr(&rcu_data));
		return;
	}
	rcu_exp_need_qs();
}

/* Send IPI for expedited cleanup if needed at end of CPU-hotplug operation. */
static void sync_sched_exp_online_cleanup(int cpu)
{
	unsigned long flags;
	int my_cpu;
	struct rcu_data *rdp;
	int ret;
	struct rcu_node *rnp;

	rdp = per_cpu_ptr(&rcu_data, cpu);
	rnp = rdp->mynode;
	my_cpu = get_cpu();
	/* Quiescent state either not needed or already requested, leave. */
	if (!(READ_ONCE(rnp->expmask) & rdp->grpmask) ||
	    rdp->cpu_no_qs.b.exp) {
		put_cpu();
		return;
	}
	/* Quiescent state needed on current CPU, so set it up locally. */
	if (my_cpu == cpu) {
		local_irq_save(flags);
		rcu_exp_need_qs();
		local_irq_restore(flags);
		put_cpu();
		return;
	}
	/* Quiescent state needed on some other CPU, send IPI. */
	ret = smp_call_function_single(cpu, rcu_exp_handler, NULL, 0);
	put_cpu();
	WARN_ON_ONCE(ret);
}

/*
 * Because preemptible RCU does not exist, we never have to check for
 * tasks blocked within RCU read-side critical sections that are
 * blocking the current expedited grace period.
 */
static int rcu_print_task_exp_stall(struct rcu_node *rnp)
{
	return 0;
}

#endif /* #else #ifdef CONFIG_PREEMPT_RCU */

/**
 * synchronize_rcu_expedited - Brute-force RCU grace period
 *
 * Wait for an RCU grace period, but expedite it.  The basic idea is to
 * IPI all non-idle non-nohz online CPUs.  The IPI handler checks whether
 * the CPU is in an RCU critical section, and if so, it sets a flag that
 * causes the outermost rcu_read_unlock() to report the quiescent state
 * for RCU-preempt or asks the scheduler for help for RCU-sched.  On the
 * other hand, if the CPU is not in an RCU read-side critical section,
 * the IPI handler reports the quiescent state immediately.
 *
 * Although this is a great improvement over previous expedited
 * implementations, it is still unfriendly to real-time workloads, so is
 * thus not recommended for any sort of common-case code.  In fact, if
 * you are using synchronize_rcu_expedited() in a loop, please restructure
 * your code to batch your updates, and then use a single synchronize_rcu()
 * instead.
 *
 * This has the same semantics as (but is more brutal than) synchronize_rcu().
 */
void synchronize_rcu_expedited(void)
{
	bool boottime = (rcu_scheduler_active == RCU_SCHEDULER_INIT);
	struct rcu_exp_work rew;
	struct rcu_node *rnp;
	unsigned long s;
>>>>>>> upstream/android-13

	RCU_LOCKDEP_WARN(lock_is_held(&rcu_bh_lock_map) ||
			 lock_is_held(&rcu_lock_map) ||
			 lock_is_held(&rcu_sched_lock_map),
			 "Illegal synchronize_rcu_expedited() in RCU read-side critical section");

<<<<<<< HEAD
	if (rcu_scheduler_active == RCU_SCHEDULER_INACTIVE)
		return;
	_synchronize_rcu_expedited(rsp, sync_rcu_exp_handler);
}
EXPORT_SYMBOL_GPL(synchronize_rcu_expedited);

#else /* #ifdef CONFIG_PREEMPT_RCU */

/*
 * Wait for an rcu-preempt grace period, but make it happen quickly.
 * But because preemptible RCU does not exist, map to rcu-sched.
 */
void synchronize_rcu_expedited(void)
{
	synchronize_sched_expedited();
}
EXPORT_SYMBOL_GPL(synchronize_rcu_expedited);

#endif /* #else #ifdef CONFIG_PREEMPT_RCU */
=======
	/* Is the state is such that the call is a grace period? */
	if (rcu_blocking_is_gp())
		return;

	/* If expedited grace periods are prohibited, fall back to normal. */
	if (rcu_gp_is_normal()) {
		wait_rcu_gp(call_rcu);
		return;
	}

	/* Take a snapshot of the sequence number.  */
	s = rcu_exp_gp_seq_snap();
	if (exp_funnel_lock(s))
		return;  /* Someone else did our work for us. */

	/* Ensure that load happens before action based on it. */
	if (unlikely(boottime)) {
		/* Direct call during scheduler init and early_initcalls(). */
		rcu_exp_sel_wait_wake(s);
	} else {
		/* Marshall arguments & schedule the expedited grace period. */
		rew.rew_s = s;
		synchronize_rcu_expedited_queue_work(&rew);
	}

	/* Wait for expedited grace period to complete. */
	rnp = rcu_get_root();
	wait_event(rnp->exp_wq[rcu_seq_ctr(s) & 0x3],
		   sync_exp_work_done(s));
	smp_mb(); /* Work actions happen before return. */

	secdbg_dtsk_built_clear_data();

	/* Let the next expedited grace period start. */
	mutex_unlock(&rcu_state.exp_mutex);

	if (likely(!boottime))
		synchronize_rcu_expedited_destroy_work(&rew);
}
EXPORT_SYMBOL_GPL(synchronize_rcu_expedited);
>>>>>>> upstream/android-13
