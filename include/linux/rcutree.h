<<<<<<< HEAD
/*
 * Read-Copy Update mechanism for mutual exclusion (tree-based version)
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
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 *	   Paul E. McKenney <paulmck@linux.vnet.ibm.com> Hierarchical algorithm
 *
 * Based on the original work by Paul McKenney <paulmck@us.ibm.com>
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Read-Copy Update mechanism for mutual exclusion (tree-based version)
 *
 * Copyright IBM Corporation, 2008
 *
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 *	   Paul E. McKenney <paulmck@linux.ibm.com> Hierarchical algorithm
 *
 * Based on the original work by Paul McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13
 * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *	Documentation/RCU
 */

#ifndef __LINUX_RCUTREE_H
#define __LINUX_RCUTREE_H

<<<<<<< HEAD
=======
void rcu_softirq_qs(void);
>>>>>>> upstream/android-13
void rcu_note_context_switch(bool preempt);
int rcu_needs_cpu(u64 basem, u64 *nextevt);
void rcu_cpu_stall_reset(void);

/*
 * Note a virtualization-based context switch.  This is simply a
 * wrapper around rcu_note_context_switch(), which allows TINY_RCU
 * to save a few bytes. The caller must have disabled interrupts.
 */
static inline void rcu_virt_note_context_switch(int cpu)
{
	rcu_note_context_switch(false);
}

<<<<<<< HEAD
void synchronize_rcu_bh(void);
void synchronize_sched_expedited(void);
void synchronize_rcu_expedited(void);

void kfree_call_rcu(struct rcu_head *head, rcu_callback_t func);

/**
 * synchronize_rcu_bh_expedited - Brute-force RCU-bh grace period
 *
 * Wait for an RCU-bh grace period to elapse, but use a "big hammer"
 * approach to force the grace period to end quickly.  This consumes
 * significant time on all CPUs and is unfriendly to real-time workloads,
 * so is thus not recommended for any sort of common-case code.  In fact,
 * if you are using synchronize_rcu_bh_expedited() in a loop, please
 * restructure your code to batch your updates, and then use a single
 * synchronize_rcu_bh() instead.
 *
 * Note that it is illegal to call this function while holding any lock
 * that is acquired by a CPU-hotplug notifier.  And yes, it is also illegal
 * to call this function from a CPU-hotplug notifier.  Failing to observe
 * these restriction will result in deadlock.
 */
static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched_expedited();
}

void rcu_barrier(void);
void rcu_barrier_bh(void);
void rcu_barrier_sched(void);
bool rcu_eqs_special_set(int cpu);
unsigned long get_state_synchronize_rcu(void);
void cond_synchronize_rcu(unsigned long oldstate);
unsigned long get_state_synchronize_sched(void);
void cond_synchronize_sched(unsigned long oldstate);
=======
void synchronize_rcu_expedited(void);
void kvfree_call_rcu(struct rcu_head *head, rcu_callback_t func);

void rcu_barrier(void);
bool rcu_eqs_special_set(int cpu);
void rcu_momentary_dyntick_idle(void);
void kfree_rcu_scheduler_running(void);
bool rcu_gp_might_be_stalled(void);
unsigned long get_state_synchronize_rcu(void);
unsigned long start_poll_synchronize_rcu(void);
bool poll_state_synchronize_rcu(unsigned long oldstate);
void cond_synchronize_rcu(unsigned long oldstate);
>>>>>>> upstream/android-13

void rcu_idle_enter(void);
void rcu_idle_exit(void);
void rcu_irq_enter(void);
void rcu_irq_exit(void);
void rcu_irq_enter_irqson(void);
void rcu_irq_exit_irqson(void);
<<<<<<< HEAD
=======
bool rcu_is_idle_cpu(int cpu);

#ifdef CONFIG_PROVE_RCU
void rcu_irq_exit_check_preempt(void);
#else
static inline void rcu_irq_exit_check_preempt(void) { }
#endif
>>>>>>> upstream/android-13

void exit_rcu(void);

void rcu_scheduler_starting(void);
extern int rcu_scheduler_active __read_mostly;
void rcu_end_inkernel_boot(void);
<<<<<<< HEAD
bool rcu_is_watching(void);
void rcu_all_qs(void);
=======
bool rcu_inkernel_boot_has_ended(void);
bool rcu_is_watching(void);
#ifndef CONFIG_PREEMPTION
void rcu_all_qs(void);
#endif
>>>>>>> upstream/android-13

/* RCUtree hotplug events */
int rcutree_prepare_cpu(unsigned int cpu);
int rcutree_online_cpu(unsigned int cpu);
int rcutree_offline_cpu(unsigned int cpu);
int rcutree_dead_cpu(unsigned int cpu);
int rcutree_dying_cpu(unsigned int cpu);
void rcu_cpu_starting(unsigned int cpu);

#endif /* __LINUX_RCUTREE_H */
