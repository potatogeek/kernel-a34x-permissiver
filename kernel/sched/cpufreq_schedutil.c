<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <trace/events/sched.h>

=======
>>>>>>> upstream/android-13
#include "sched.h"

#include <linux/sched/cpufreq.h>
#include <trace/events/power.h>
<<<<<<< HEAD
#include "cpufreq_schedutil.h"

void (*cpufreq_notifier_fp)(int cluster_id, unsigned long freq);
EXPORT_SYMBOL(cpufreq_notifier_fp);

struct sugov_tunables {
	struct gov_attr_set	attr_set;
	unsigned int		up_rate_limit_us;
	unsigned int		down_rate_limit_us;
=======
#include <trace/hooks/sched.h>

#define IOWAIT_BOOST_MIN	(SCHED_CAPACITY_SCALE / 8)

struct sugov_tunables {
	struct gov_attr_set	attr_set;
	unsigned int		rate_limit_us;
>>>>>>> upstream/android-13
};

struct sugov_policy {
	struct cpufreq_policy	*policy;

	struct sugov_tunables	*tunables;
	struct list_head	tunables_hook;

<<<<<<< HEAD
	raw_spinlock_t		update_lock;	/* For shared policies */
	u64			last_freq_update_time;
	s64			min_rate_limit_ns;
	s64			up_rate_delay_ns;
	s64			down_rate_delay_ns;
	unsigned int		next_freq;
	unsigned int		cached_raw_freq;
	unsigned int		prev_cached_raw_freq;
=======
	raw_spinlock_t		update_lock;
	u64			last_freq_update_time;
	s64			freq_update_delay_ns;
	unsigned int		next_freq;
	unsigned int		cached_raw_freq;
>>>>>>> upstream/android-13

	/* The next fields are only needed if fast switch cannot be used: */
	struct			irq_work irq_work;
	struct			kthread_work work;
	struct			mutex work_lock;
	struct			kthread_worker worker;
	struct task_struct	*thread;
	bool			work_in_progress;

	bool			limits_changed;
	bool			need_freq_update;
};

struct sugov_cpu {
	struct update_util_data	update_util;
	struct sugov_policy	*sg_policy;
	unsigned int		cpu;

	bool			iowait_boost_pending;
	unsigned int		iowait_boost;
	u64			last_update;

<<<<<<< HEAD
	unsigned long		bw_dl;
	unsigned long		min;
=======
	unsigned long		util;
	unsigned long		bw_dl;
>>>>>>> upstream/android-13
	unsigned long		max;

	/* The field below is for single-CPU policies only: */
#ifdef CONFIG_NO_HZ_COMMON
	unsigned long		saved_idle_calls;
#endif
};

static DEFINE_PER_CPU(struct sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

static bool sugov_should_update_freq(struct sugov_policy *sg_policy, u64 time)
{
	s64 delta_ns;

	/*
	 * Since cpufreq_update_util() is called with rq->lock held for
	 * the @target_cpu, our per-CPU data is fully serialized.
	 *
	 * However, drivers cannot in general deal with cross-CPU
	 * requests, so while get_next_freq() will work, our
	 * sugov_update_commit() call may not for the fast switching platforms.
	 *
	 * Hence stop here for remote requests if they aren't supported
	 * by the hardware, as calculating the frequency is pointless if
	 * we cannot in fact act on it.
	 *
	 * This is needed on the slow switching platforms too to prevent CPUs
	 * going offline from leaving stale IRQ work items behind.
	 */
	if (!cpufreq_this_cpu_can_update(sg_policy->policy))
		return false;

	if (unlikely(sg_policy->limits_changed)) {
		sg_policy->limits_changed = false;
		sg_policy->need_freq_update = true;
		return true;
	}

<<<<<<< HEAD
	/* No need to recalculate next freq for min_rate_limit_us
	 * at least. However we might still decide to further rate
	 * limit once frequency change direction is decided, according
	 * to the separate rate limits.
	 */

	delta_ns = time - sg_policy->last_freq_update_time;
	return delta_ns >= sg_policy->min_rate_limit_ns;
}

static bool sugov_up_down_rate_limit(struct sugov_policy *sg_policy, u64 time,
				     unsigned int next_freq)
{
	s64 delta_ns;

	delta_ns = time - sg_policy->last_freq_update_time;

	if (next_freq > sg_policy->next_freq &&
	    delta_ns < sg_policy->up_rate_delay_ns)
			return true;

	if (next_freq < sg_policy->next_freq &&
	    delta_ns < sg_policy->down_rate_delay_ns)
			return true;

	return false;
=======
	delta_ns = time - sg_policy->last_freq_update_time;

	return delta_ns >= sg_policy->freq_update_delay_ns;
>>>>>>> upstream/android-13
}

static bool sugov_update_next_freq(struct sugov_policy *sg_policy, u64 time,
				   unsigned int next_freq)
{
<<<<<<< HEAD
	if (sg_policy->next_freq == next_freq)
		return false;

	if (sugov_up_down_rate_limit(sg_policy, time, next_freq)) {
		/* Restore cached freq as next_freq is not changed */
		sg_policy->cached_raw_freq = sg_policy->prev_cached_raw_freq;
		return false;
	}
=======
	bool should_update = true;

	if (sg_policy->need_freq_update)
		sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);
	else if (sg_policy->next_freq == next_freq)
		return false;

	trace_android_rvh_set_sugov_update(sg_policy, next_freq, &should_update);
	if (!should_update)
		return false;
>>>>>>> upstream/android-13

	sg_policy->next_freq = next_freq;
	sg_policy->last_freq_update_time = time;

	return true;
}

<<<<<<< HEAD
#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
#else
static void sugov_fast_switch(struct sugov_policy *sg_policy, u64 time,
			      unsigned int next_freq)
{
	struct cpufreq_policy *policy = sg_policy->policy;
	int cpu;

	if (!sugov_update_next_freq(sg_policy, time, next_freq))
		return;

	next_freq = cpufreq_driver_fast_switch(policy, next_freq);
	if (!next_freq)
		return;

	policy->cur = next_freq;

	if (trace_cpu_frequency_enabled()) {
		for_each_cpu(cpu, policy->cpus)
			trace_cpu_frequency(next_freq, cpu);
	}
}

static void sugov_deferred_update(struct sugov_policy *sg_policy, u64 time,
				  unsigned int next_freq)
{
	if (!sugov_update_next_freq(sg_policy, time, next_freq))
		return;

=======
static void sugov_deferred_update(struct sugov_policy *sg_policy)
{
>>>>>>> upstream/android-13
	if (!sg_policy->work_in_progress) {
		sg_policy->work_in_progress = true;
		irq_work_queue(&sg_policy->irq_work);
	}
}
<<<<<<< HEAD
#endif

#ifdef CONFIG_NONLINEAR_FREQ_CTL

#include "cpufreq_schedutil_plus.c"
#else
=======

>>>>>>> upstream/android-13
/**
 * get_next_freq - Compute a new frequency for a given cpufreq policy.
 * @sg_policy: schedutil policy object to compute the new frequency for.
 * @util: Current CPU utilization.
 * @max: CPU capacity.
 *
 * If the utilization is frequency-invariant, choose the new frequency to be
 * proportional to it, that is
 *
 * next_freq = C * max_freq * util / max
 *
 * Otherwise, approximate the would-be frequency-invariant utilization by
 * util_raw * (curr_freq / max_freq) which leads to
 *
 * next_freq = C * curr_freq * util_raw / max
 *
 * Take C = 1.25 for the frequency tipping point at (util / max) = 0.8.
 *
 * The lowest driver-supported frequency which is equal or greater than the raw
 * next_freq (as calculated above) is returned, subject to policy min/max and
 * cpufreq driver limitations.
 */
static unsigned int get_next_freq(struct sugov_policy *sg_policy,
				  unsigned long util, unsigned long max)
{
	struct cpufreq_policy *policy = sg_policy->policy;
	unsigned int freq = arch_scale_freq_invariant() ?
				policy->cpuinfo.max_freq : policy->cur;
<<<<<<< HEAD

#ifdef CONFIG_MTK_SCHED_EXTENSION
	freq = map_util_freq_with_margin(util, freq, max);
#else
	freq = map_util_freq(util, freq, max);
#endif

	freq = clamp_val(freq, policy->min, policy->max);
=======
	unsigned long next_freq = 0;

	util = map_util_perf(util);
	trace_android_vh_map_util_freq(util, freq, max, &next_freq);
	trace_android_vh_map_util_freq_new(util, freq, max, &next_freq, policy,
			&sg_policy->need_freq_update);
	if (next_freq)
		freq = next_freq;
	else
		freq = map_util_freq(util, freq, max);
>>>>>>> upstream/android-13

	if (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		return sg_policy->next_freq;

<<<<<<< HEAD
	sg_policy->need_freq_update = false;
	sg_policy->prev_cached_raw_freq = sg_policy->cached_raw_freq;
	sg_policy->cached_raw_freq = freq;

#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
	return freq;
#else
	return cpufreq_driver_resolve_freq(policy, freq);
#endif
}
#endif

/*
 * This function computes an effective utilization for the given CPU, to be
 * used for frequency selection given the linear relation: f = u * f_max.
 *
 * The scheduler tracks the following metrics:
 *
 *   cpu_util_{cfs,rt,dl,irq}()
 *   cpu_bw_dl()
 *
 * Where the cfs,rt and dl util numbers are tracked with the same metric and
 * synchronized windows and are thus directly comparable.
 *
 * The @util parameter passed to this function is assumed to be the aggregation
 * of RT and CFS util numbers. The cases of DL and IRQ are managed here.
 *
 * The cfs,rt,dl utilization are the running times measured with rq->clock_task
 * which excludes things like IRQ and steal-time. These latter are then accrued
 * in the irq utilization.
 *
 * The DL bandwidth number otoh is not a measured metric but a value computed
 * based on the task model parameters and gives the minimal utilization
 * required to meet deadlines.
 */
unsigned long schedutil_cpu_util(int cpu, unsigned long util_cfs,
				 unsigned long max, enum schedutil_type type,
				 struct task_struct *p)
{
	unsigned long dl_util, util, irq;
	struct rq *rq = cpu_rq(cpu);

	if (sched_feat(SUGOV_RT_MAX_FREQ) && !IS_BUILTIN(CONFIG_UCLAMP_TASK) &&
	    type == FREQUENCY_UTIL && rt_rq_is_runnable(&rq->rt)) {
		return max;
	}

	if (type == FREQUENCY_UTIL && idle_cpu(cpu))
		return 0;

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	if (unlikely(irq >= max))
		return max;

	/*
	 * Because the time spend on RT/DL tasks is visible as 'lost' time to
	 * CFS tasks and we use the same metric to track the effective
	 * utilization (PELT windows are synchronized) we can directly add them
	 * to obtain the CPU's actual utilization.
	 *
	 * CFS and RT utilization can be boosted or capped, depending on
	 * utilization clamp constraints requested by currently RUNNABLE
	 * tasks.
	 * When there are no CFS RUNNABLE tasks, clamps are released and
	 * frequency will be gracefully reduced with the utilization decay.
	 */
	util = util_cfs + cpu_util_rt(rq);
	if (type == FREQUENCY_UTIL)
		util = uclamp_rq_util_with(rq, util, p);

	dl_util = cpu_util_dl(rq);

	/*
	 * For frequency selection we do not make cpu_util_dl() a permanent part
	 * of this sum because we want to use cpu_bw_dl() later on, but we need
	 * to check if the CFS+RT+DL sum is saturated (ie. no idle time) such
	 * that we select f_max when there is no idle time.
	 *
	 * NOTE: numerical errors or stop class might cause us to not quite hit
	 * saturation when we should -- something for later.
	 */
	if (util + dl_util >= max)
		return max;

	/*
	 * OTOH, for energy computation we need the estimated running time, so
	 * include util_dl and ignore dl_bw.
	 */
	if (type == ENERGY_UTIL)
		util += dl_util;

	/*
	 * There is still idle time; further improve the number by using the
	 * irq metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              1 - irq
	 *   U' = irq + ------- * U
	 *                max
	 */
	util = scale_irq_capacity(util, irq, max);
	util += irq;

	/*
	 * Bandwidth required by DEADLINE must always be granted while, for
	 * FAIR and RT, we use blocked utilization of IDLE CPUs as a mechanism
	 * to gracefully reduce the frequency when no tasks show up for longer
	 * periods of time.
	 *
	 * Ideally we would like to set bw_dl as min/guaranteed freq and util +
	 * bw_dl as requested freq. However, cpufreq is not yet ready for such
	 * an interface. So, we only do the latter for now.
	 */
	if (type == FREQUENCY_UTIL)
		util += cpu_bw_dl(rq);

	return min(max, util);
}

static unsigned long sugov_get_util(struct sugov_cpu *sg_cpu)
{
	struct rq *rq = cpu_rq(sg_cpu->cpu);
#ifdef CONFIG_SCHED_TUNE
	unsigned long util = stune_util(sg_cpu->cpu, cpu_util_rt(rq));
#else
	unsigned long util = cpu_util_freq(sg_cpu->cpu);
#endif
	unsigned long util_cfs = util - cpu_util_rt(rq);
	unsigned long max = arch_scale_cpu_capacity(NULL, sg_cpu->cpu);

	sg_cpu->max = max;
	sg_cpu->bw_dl = cpu_bw_dl(rq);

	return schedutil_cpu_util(sg_cpu->cpu, util_cfs, max,
				  FREQUENCY_UTIL, NULL);
=======
	sg_policy->cached_raw_freq = freq;
	return cpufreq_driver_resolve_freq(policy, freq);
}

static void sugov_get_util(struct sugov_cpu *sg_cpu)
{
	struct rq *rq = cpu_rq(sg_cpu->cpu);
	unsigned long max = arch_scale_cpu_capacity(sg_cpu->cpu);

	sg_cpu->max = max;
	sg_cpu->bw_dl = cpu_bw_dl(rq);
	sg_cpu->util = effective_cpu_util(sg_cpu->cpu, cpu_util_cfs(rq), max,
					  FREQUENCY_UTIL, NULL);
>>>>>>> upstream/android-13
}

/**
 * sugov_iowait_reset() - Reset the IO boost status of a CPU.
 * @sg_cpu: the sugov data for the CPU to boost
 * @time: the update time from the caller
 * @set_iowait_boost: true if an IO boost has been requested
 *
 * The IO wait boost of a task is disabled after a tick since the last update
 * of a CPU. If a new IO wait boost is requested after more then a tick, then
<<<<<<< HEAD
 * we enable the boost starting from the minimum frequency, which improves
 * energy efficiency by ignoring sporadic wakeups from IO.
=======
 * we enable the boost starting from IOWAIT_BOOST_MIN, which improves energy
 * efficiency by ignoring sporadic wakeups from IO.
>>>>>>> upstream/android-13
 */
static bool sugov_iowait_reset(struct sugov_cpu *sg_cpu, u64 time,
			       bool set_iowait_boost)
{
	s64 delta_ns = time - sg_cpu->last_update;

	/* Reset boost only if a tick has elapsed since last request */
	if (delta_ns <= TICK_NSEC)
		return false;

<<<<<<< HEAD
	sg_cpu->iowait_boost = set_iowait_boost ? sg_cpu->min : 0;
=======
	sg_cpu->iowait_boost = set_iowait_boost ? IOWAIT_BOOST_MIN : 0;
>>>>>>> upstream/android-13
	sg_cpu->iowait_boost_pending = set_iowait_boost;

	return true;
}

/**
 * sugov_iowait_boost() - Updates the IO boost status of a CPU.
 * @sg_cpu: the sugov data for the CPU to boost
 * @time: the update time from the caller
 * @flags: SCHED_CPUFREQ_IOWAIT if the task is waking up after an IO wait
 *
 * Each time a task wakes up after an IO operation, the CPU utilization can be
 * boosted to a certain utilization which doubles at each "frequent and
<<<<<<< HEAD
 * successive" wakeup from IO, ranging from the utilization of the minimum
 * OPP to the utilization of the maximum OPP.
=======
 * successive" wakeup from IO, ranging from IOWAIT_BOOST_MIN to the utilization
 * of the maximum OPP.
 *
>>>>>>> upstream/android-13
 * To keep doubling, an IO boost has to be requested at least once per tick,
 * otherwise we restart from the utilization of the minimum OPP.
 */
static void sugov_iowait_boost(struct sugov_cpu *sg_cpu, u64 time,
			       unsigned int flags)
{
	bool set_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	if (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, time, set_iowait_boost))
		return;

	/* Boost only tasks waking up after IO */
	if (!set_iowait_boost)
		return;

	/* Ensure boost doubles only one time at each request */
	if (sg_cpu->iowait_boost_pending)
		return;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	if (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(unsigned int, sg_cpu->iowait_boost << 1, SCHED_CAPACITY_SCALE);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
		return;
	}

	/* First wakeup after IO: start with minimum boost */
<<<<<<< HEAD
	sg_cpu->iowait_boost = sg_cpu->min;
=======
	sg_cpu->iowait_boost = IOWAIT_BOOST_MIN;
>>>>>>> upstream/android-13
}

/**
 * sugov_iowait_apply() - Apply the IO boost to a CPU.
 * @sg_cpu: the sugov data for the cpu to boost
 * @time: the update time from the caller
<<<<<<< HEAD
 * @util: the utilization to (eventually) boost
 * @max: the maximum value the utilization can be boosted to
=======
>>>>>>> upstream/android-13
 *
 * A CPU running a task which woken up after an IO operation can have its
 * utilization boosted to speed up the completion of those IO operations.
 * The IO boost value is increased each time a task wakes up from IO, in
 * sugov_iowait_apply(), and it's instead decreased by this function,
 * each time an increase has not been requested (!iowait_boost_pending).
 *
 * A CPU which also appears to have been idle for at least one tick has also
 * its IO boost utilization reset.
 *
 * This mechanism is designed to boost high frequently IO waiting tasks, while
 * being more conservative on tasks which does sporadic IO operations.
 */
<<<<<<< HEAD
static unsigned long sugov_iowait_apply(struct sugov_cpu *sg_cpu, u64 time,
					unsigned long util, unsigned long max)
=======
static void sugov_iowait_apply(struct sugov_cpu *sg_cpu, u64 time)
>>>>>>> upstream/android-13
{
	unsigned long boost;

	/* No boost currently required */
	if (!sg_cpu->iowait_boost)
<<<<<<< HEAD
		return util;

	/* Reset boost if the CPU appears to have been idle enough */
	if (sugov_iowait_reset(sg_cpu, time, false))
		return util;
=======
		return;

	/* Reset boost if the CPU appears to have been idle enough */
	if (sugov_iowait_reset(sg_cpu, time, false))
		return;
>>>>>>> upstream/android-13

	if (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
<<<<<<< HEAD
		if (sg_cpu->iowait_boost < sg_cpu->min) {
			sg_cpu->iowait_boost = 0;
			return util;
=======
		if (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			return;
>>>>>>> upstream/android-13
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
<<<<<<< HEAD
	 * @util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	boost = (sg_cpu->iowait_boost * max) >> SCHED_CAPACITY_SHIFT;
	return max(boost, util);
=======
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	boost = (sg_cpu->iowait_boost * sg_cpu->max) >> SCHED_CAPACITY_SHIFT;
	boost = uclamp_rq_util_with(cpu_rq(sg_cpu->cpu), boost, NULL);
	if (sg_cpu->util < boost)
		sg_cpu->util = boost;
>>>>>>> upstream/android-13
}

#ifdef CONFIG_NO_HZ_COMMON
static bool sugov_cpu_is_busy(struct sugov_cpu *sg_cpu)
{
	unsigned long idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	bool ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	return ret;
}
#else
static inline bool sugov_cpu_is_busy(struct sugov_cpu *sg_cpu) { return false; }
#endif /* CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
<<<<<<< HEAD
static inline void ignore_dl_rate_limit(struct sugov_cpu *sg_cpu, struct sugov_policy *sg_policy)
{
	if (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_dl)
		sg_policy->limits_changed = true;
}

static inline void __cpufreq_notifier_fp(int cid, unsigned int next_f)
{
	if (cpufreq_notifier_fp)
		cpufreq_notifier_fp(cid, next_f);
}

static void sugov_update_single(struct update_util_data *hook, u64 time,
				unsigned int flags)
{
	struct sugov_cpu *sg_cpu = container_of(hook, struct sugov_cpu, update_util);
	struct sugov_policy *sg_policy = sg_cpu->sg_policy;
/*#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT */
	struct cpufreq_policy *policy = sg_policy->policy;
/*#endif */
	int cid = arch_cpu_cluster_id(policy->cpu);
	unsigned long util, max;
	unsigned int next_f;
	bool busy;

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu, sg_policy);

	if (!sugov_should_update_freq(sg_policy, time))
		return;

	/* Limits may have changed, don't skip frequency update */
	busy = !sg_policy->need_freq_update && sugov_cpu_is_busy(sg_cpu);

	util = sugov_get_util(sg_cpu);
	max = sg_cpu->max;
	util = sugov_iowait_apply(sg_cpu, time, util, max);
	next_f = get_next_freq(sg_policy, util, max);
=======
static inline void ignore_dl_rate_limit(struct sugov_cpu *sg_cpu)
{
	if (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_dl)
		sg_cpu->sg_policy->limits_changed = true;
}

static inline bool sugov_update_single_common(struct sugov_cpu *sg_cpu,
					      u64 time, unsigned int flags)
{
	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	if (!sugov_should_update_freq(sg_cpu->sg_policy, time))
		return false;

	sugov_get_util(sg_cpu);
	sugov_iowait_apply(sg_cpu, time);

	return true;
}

static void sugov_update_single_freq(struct update_util_data *hook, u64 time,
				     unsigned int flags)
{
	struct sugov_cpu *sg_cpu = container_of(hook, struct sugov_cpu, update_util);
	struct sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned int cached_freq = sg_policy->cached_raw_freq;
	unsigned int next_f;

	if (!sugov_update_single_common(sg_cpu, time, flags))
		return;

	next_f = get_next_freq(sg_policy, sg_cpu->util, sg_cpu->max);
>>>>>>> upstream/android-13
	/*
	 * Do not reduce the frequency if the CPU has not been idle
	 * recently, as the reduction is likely to be premature then.
	 */
<<<<<<< HEAD
	if (busy && next_f < sg_policy->next_freq) {
		next_f = sg_policy->next_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = sg_policy->prev_cached_raw_freq;
	}

#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
	if (sugov_update_next_freq(sg_policy, time, next_f)) {
		mt_cpufreq_set_by_wfi_load_cluster(cid, next_f);
		policy->cur = next_f;
		trace_sched_util(cid, next_f, time);
	}
#else
=======
	if (sugov_cpu_is_busy(sg_cpu) && next_f < sg_policy->next_freq) {
		next_f = sg_policy->next_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	if (!sugov_update_next_freq(sg_policy, time, next_f))
		return;
>>>>>>> upstream/android-13

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	if (sg_policy->policy->fast_switch_enabled) {
<<<<<<< HEAD
		sugov_fast_switch(sg_policy, time, next_f);
	} else {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy, time, next_f);
		raw_spin_unlock(&sg_policy->update_lock);
	}
#endif

	__cpufreq_notifier_fp(cid, next_f);

=======
		cpufreq_driver_fast_switch(sg_policy->policy, next_f);
	} else {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_lock);
	}
}

static void sugov_update_single_perf(struct update_util_data *hook, u64 time,
				     unsigned int flags)
{
	struct sugov_cpu *sg_cpu = container_of(hook, struct sugov_cpu, update_util);
	unsigned long prev_util = sg_cpu->util;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	if (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, time, flags);
		return;
	}

	if (!sugov_update_single_common(sg_cpu, time, flags))
		return;

	/*
	 * Do not reduce the target performance level if the CPU has not been
	 * idle recently, as the reduction is likely to be premature then.
	 */
	if (sugov_cpu_is_busy(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_cpu->cpu, map_util_perf(sg_cpu->bw_dl),
				   map_util_perf(sg_cpu->util), sg_cpu->max);

	sg_cpu->sg_policy->last_freq_update_time = time;
>>>>>>> upstream/android-13
}

static unsigned int sugov_next_freq_shared(struct sugov_cpu *sg_cpu, u64 time)
{
	struct sugov_policy *sg_policy = sg_cpu->sg_policy;
	struct cpufreq_policy *policy = sg_policy->policy;
	unsigned long util = 0, max = 1;
	unsigned int j;
<<<<<<< HEAD
	unsigned int next_f;
#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
	int cid;
#endif
=======
>>>>>>> upstream/android-13

	for_each_cpu(j, policy->cpus) {
		struct sugov_cpu *j_sg_cpu = &per_cpu(sugov_cpu, j);
		unsigned long j_util, j_max;

<<<<<<< HEAD
		j_util = sugov_get_util(j_sg_cpu);
		j_max = j_sg_cpu->max;
		j_util = sugov_iowait_apply(j_sg_cpu, time, j_util, j_max);

#ifdef CONFIG_UCLAMP_TASK
		trace_schedutil_uclamp_util(j, j_util);
#endif
=======
		sugov_get_util(j_sg_cpu);
		sugov_iowait_apply(j_sg_cpu, time);
		j_util = j_sg_cpu->util;
		j_max = j_sg_cpu->max;
>>>>>>> upstream/android-13

		if (j_util * max > j_max * util) {
			util = j_util;
			max = j_max;
		}
	}

<<<<<<< HEAD
	next_f = get_next_freq(sg_policy, util, max);

#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
	cid = arch_cpu_cluster_id(policy->cpu);
	next_f = mt_cpufreq_find_close_freq(cid, next_f);
#endif

	return next_f;
=======
	return get_next_freq(sg_policy, util, max);
>>>>>>> upstream/android-13
}

static void
sugov_update_shared(struct update_util_data *hook, u64 time, unsigned int flags)
{
	struct sugov_cpu *sg_cpu = container_of(hook, struct sugov_cpu, update_util);
	struct sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned int next_f;
<<<<<<< HEAD
	int cid;
=======
>>>>>>> upstream/android-13

	raw_spin_lock(&sg_policy->update_lock);

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

<<<<<<< HEAD
	ignore_dl_rate_limit(sg_cpu, sg_policy);

	cid = arch_cpu_cluster_id(sg_policy->policy->cpu);
=======
	ignore_dl_rate_limit(sg_cpu);
>>>>>>> upstream/android-13

	if (sugov_should_update_freq(sg_policy, time)) {
		next_f = sugov_next_freq_shared(sg_cpu, time);

<<<<<<< HEAD
#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
		if (sugov_update_next_freq(sg_policy, time, next_f)) {
			next_f = mt_cpufreq_find_close_freq(cid, next_f);
			mt_cpufreq_set_by_wfi_load_cluster(cid, next_f);
			__cpufreq_notifier_fp(cid, next_f);
			trace_sched_util(cid, next_f, time);
		}
#else
		if (sg_policy->policy->fast_switch_enabled)
			sugov_fast_switch(sg_policy, time, next_f);
		else
			sugov_deferred_update(sg_policy, time, next_f);
		__cpufreq_notifier_fp(cid, next_f);
#endif
	}

=======
		if (!sugov_update_next_freq(sg_policy, time, next_f))
			goto unlock;

		if (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, next_f);
		else
			sugov_deferred_update(sg_policy);
	}
unlock:
>>>>>>> upstream/android-13
	raw_spin_unlock(&sg_policy->update_lock);
}

static void sugov_work(struct kthread_work *work)
{
	struct sugov_policy *sg_policy = container_of(work, struct sugov_policy, work);
	unsigned int freq;
	unsigned long flags;

	/*
	 * Hold sg_policy->update_lock shortly to handle the case where:
<<<<<<< HEAD
	 * incase sg_policy->next_freq is read here, and then updated by
=======
	 * in case sg_policy->next_freq is read here, and then updated by
>>>>>>> upstream/android-13
	 * sugov_deferred_update() just before work_in_progress is set to false
	 * here, we may miss queueing the new update.
	 *
	 * Note: If a work was queued after the update_lock is released,
	 * sugov_work() will just be called again by kthread_work code; and the
	 * request will be proceed before the sugov thread sleeps.
	 */
	raw_spin_lock_irqsave(&sg_policy->update_lock, flags);
	freq = sg_policy->next_freq;
	sg_policy->work_in_progress = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_lock, flags);

	mutex_lock(&sg_policy->work_lock);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_RELATION_L);
	mutex_unlock(&sg_policy->work_lock);
}

static void sugov_irq_work(struct irq_work *irq_work)
{
	struct sugov_policy *sg_policy;

	sg_policy = container_of(irq_work, struct sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->worker, &sg_policy->work);
}

/************************** sysfs interface ************************/

static struct sugov_tunables *global_tunables;
static DEFINE_MUTEX(global_tunables_lock);

static inline struct sugov_tunables *to_sugov_tunables(struct gov_attr_set *attr_set)
{
	return container_of(attr_set, struct sugov_tunables, attr_set);
}

<<<<<<< HEAD
static DEFINE_MUTEX(min_rate_lock);

static void update_min_rate_limit_ns(struct sugov_policy *sg_policy)
{
	mutex_lock(&min_rate_lock);
	sg_policy->min_rate_limit_ns = min(sg_policy->up_rate_delay_ns,
					   sg_policy->down_rate_delay_ns);
	mutex_unlock(&min_rate_lock);
}

static ssize_t up_rate_limit_us_show(struct gov_attr_set *attr_set, char *buf)
{
	struct sugov_tunables *tunables = to_sugov_tunables(attr_set);

	return sprintf(buf, "%u\n", tunables->up_rate_limit_us);
}

static ssize_t down_rate_limit_us_show(struct gov_attr_set *attr_set, char *buf)
{
	struct sugov_tunables *tunables = to_sugov_tunables(attr_set);

	return sprintf(buf, "%u\n", tunables->down_rate_limit_us);
}

static ssize_t up_rate_limit_us_store(struct gov_attr_set *attr_set,
				      const char *buf, size_t count)
=======
static ssize_t rate_limit_us_show(struct gov_attr_set *attr_set, char *buf)
{
	struct sugov_tunables *tunables = to_sugov_tunables(attr_set);

	return sprintf(buf, "%u\n", tunables->rate_limit_us);
}

static ssize_t
rate_limit_us_store(struct gov_attr_set *attr_set, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	struct sugov_tunables *tunables = to_sugov_tunables(attr_set);
	struct sugov_policy *sg_policy;
	unsigned int rate_limit_us;

	if (kstrtouint(buf, 10, &rate_limit_us))
		return -EINVAL;

<<<<<<< HEAD
	tunables->up_rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook) {
		sg_policy->up_rate_delay_ns = rate_limit_us * NSEC_PER_USEC;
		update_min_rate_limit_ns(sg_policy);
	}
=======
	tunables->rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook)
		sg_policy->freq_update_delay_ns = rate_limit_us * NSEC_PER_USEC;
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t down_rate_limit_us_store(struct gov_attr_set *attr_set,
					const char *buf, size_t count)
{
	struct sugov_tunables *tunables = to_sugov_tunables(attr_set);
	struct sugov_policy *sg_policy;
	unsigned int rate_limit_us;

	if (kstrtouint(buf, 10, &rate_limit_us))
		return -EINVAL;

	tunables->down_rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook) {
		sg_policy->down_rate_delay_ns = rate_limit_us * NSEC_PER_USEC;
		update_min_rate_limit_ns(sg_policy);
	}

	return count;
}

static struct governor_attr up_rate_limit_us = __ATTR_RW(up_rate_limit_us);
static struct governor_attr down_rate_limit_us = __ATTR_RW(down_rate_limit_us);

static struct attribute *sugov_attributes[] = {
	&up_rate_limit_us.attr,
	&down_rate_limit_us.attr,
	NULL
};

static struct kobj_type sugov_tunables_ktype = {
	.default_attrs = sugov_attributes,
	.sysfs_ops = &governor_sysfs_ops,
=======
static struct governor_attr rate_limit_us = __ATTR_RW(rate_limit_us);

static struct attribute *sugov_attrs[] = {
	&rate_limit_us.attr,
	NULL
};
ATTRIBUTE_GROUPS(sugov);

static void sugov_tunables_free(struct kobject *kobj)
{
	struct gov_attr_set *attr_set = container_of(kobj, struct gov_attr_set, kobj);

	kfree(to_sugov_tunables(attr_set));
}

static struct kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
>>>>>>> upstream/android-13
};

/********************** cpufreq governor interface *********************/

struct cpufreq_governor schedutil_gov;

<<<<<<< HEAD
int schedutil_set_down_rate_limit_us(int cpu, unsigned int rate_limit_us)
{
	struct cpufreq_policy *policy;
	struct sugov_policy *sg_policy;
	struct sugov_tunables *tunables;
	struct gov_attr_set *attr_set;

	policy = cpufreq_cpu_get(cpu);
	if (!policy)
		return -EINVAL;

	if (policy->governor != &schedutil_gov)
		return -ENOENT;

	mutex_lock(&global_tunables_lock);
	sg_policy = policy->governor_data;
	if (!sg_policy) {
		mutex_unlock(&global_tunables_lock);
		cpufreq_cpu_put(policy);
		return -EINVAL;
	}

	tunables = sg_policy->tunables;
	tunables->down_rate_limit_us = rate_limit_us;
	attr_set = &tunables->attr_set;

	mutex_lock(&attr_set->update_lock);
	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook) {
		sg_policy->down_rate_delay_ns = rate_limit_us * NSEC_PER_USEC;
		update_min_rate_limit_ns(sg_policy);
	}
	mutex_unlock(&attr_set->update_lock);
	mutex_unlock(&global_tunables_lock);

	if (policy)
		cpufreq_cpu_put(policy);
	return 0;
}
EXPORT_SYMBOL(schedutil_set_down_rate_limit_us);

int schedutil_set_up_rate_limit_us(int cpu, unsigned int rate_limit_us)
{
	struct cpufreq_policy *policy;
	struct sugov_policy *sg_policy;
	struct sugov_tunables *tunables;
	struct gov_attr_set *attr_set;

	policy = cpufreq_cpu_get(cpu);
	if (!policy)
		return -EINVAL;

	if (policy->governor != &schedutil_gov)
		return -ENOENT;

	mutex_lock(&global_tunables_lock);
	sg_policy = policy->governor_data;
	if (!sg_policy) {
		mutex_unlock(&global_tunables_lock);
		cpufreq_cpu_put(policy);
		return -EINVAL;
	}

	tunables = sg_policy->tunables;
	tunables->up_rate_limit_us = rate_limit_us;
	attr_set = &tunables->attr_set;

	mutex_lock(&attr_set->update_lock);
	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook) {
		sg_policy->up_rate_delay_ns = rate_limit_us * NSEC_PER_USEC;
		update_min_rate_limit_ns(sg_policy);
	}
	mutex_unlock(&attr_set->update_lock);
	mutex_unlock(&global_tunables_lock);

	if (policy)
		cpufreq_cpu_put(policy);
	return 0;
}
EXPORT_SYMBOL(schedutil_set_up_rate_limit_us);

=======
>>>>>>> upstream/android-13
static struct sugov_policy *sugov_policy_alloc(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy;

	sg_policy = kzalloc(sizeof(*sg_policy), GFP_KERNEL);
	if (!sg_policy)
		return NULL;

	sg_policy->policy = policy;
	raw_spin_lock_init(&sg_policy->update_lock);
	return sg_policy;
}

static void sugov_policy_free(struct sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

static int sugov_kthread_create(struct sugov_policy *sg_policy)
{
	struct task_struct *thread;
	struct sched_attr attr = {
		.size		= sizeof(struct sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_priority	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	=  1000000,
		.sched_deadline = 10000000,
		.sched_period	= 10000000,
	};
	struct cpufreq_policy *policy = sg_policy->policy;
	int ret;

	/* kthread only required for slow path */
	if (policy->fast_switch_enabled)
		return 0;

<<<<<<< HEAD
=======
	trace_android_vh_set_sugov_sched_attr(&attr);
>>>>>>> upstream/android-13
	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_worker(&sg_policy->worker);
	thread = kthread_create(kthread_worker_fn, &sg_policy->worker,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	if (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %ld\n", PTR_ERR(thread));
		return PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	if (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		return ret;
	}

	sg_policy->thread = thread;
	kthread_bind_mask(thread, policy->related_cpus);
	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_lock);

	wake_up_process(thread);

	return 0;
}

static void sugov_kthread_stop(struct sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	if (sg_policy->policy->fast_switch_enabled)
		return;

	kthread_flush_worker(&sg_policy->worker);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_lock);
}

static struct sugov_tunables *sugov_tunables_alloc(struct sugov_policy *sg_policy)
{
	struct sugov_tunables *tunables;

	tunables = kzalloc(sizeof(*tunables), GFP_KERNEL);
	if (tunables) {
		gov_attr_set_init(&tunables->attr_set, &sg_policy->tunables_hook);
		if (!have_governor_per_policy())
			global_tunables = tunables;
	}
	return tunables;
}

<<<<<<< HEAD
static void sugov_tunables_free(struct sugov_tunables *tunables)
{
	if (!have_governor_per_policy())
		global_tunables = NULL;

	kfree(tunables);
=======
static void sugov_clear_global_tunables(void)
{
	if (!have_governor_per_policy())
		global_tunables = NULL;
>>>>>>> upstream/android-13
}

static int sugov_init(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy;
	struct sugov_tunables *tunables;
	int ret = 0;

	/* State should be equivalent to EXIT */
	if (policy->governor_data)
		return -EBUSY;

<<<<<<< HEAD
	policy->dvfs_possible_from_any_cpu = true;

=======
>>>>>>> upstream/android-13
	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	if (!sg_policy) {
		ret = -ENOMEM;
		goto disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	if (ret)
		goto free_sg_policy;

	mutex_lock(&global_tunables_lock);

	if (global_tunables) {
		if (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			goto stop_kthread;
		}
		policy->governor_data = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set, &sg_policy->tunables_hook);
		goto out;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	if (!tunables) {
		ret = -ENOMEM;
		goto stop_kthread;
	}

<<<<<<< HEAD
	tunables->up_rate_limit_us = cpufreq_policy_transition_delay_us(policy);
	tunables->down_rate_limit_us = cpufreq_policy_transition_delay_us(policy);
=======
	tunables->rate_limit_us = cpufreq_policy_transition_delay_us(policy);
>>>>>>> upstream/android-13

	policy->governor_data = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_add(&tunables->attr_set.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	if (ret)
		goto fail;

out:
	mutex_unlock(&global_tunables_lock);
	return 0;

fail:
	kobject_put(&tunables->attr_set.kobj);
	policy->governor_data = NULL;
<<<<<<< HEAD
	sugov_tunables_free(tunables);
=======
	sugov_clear_global_tunables();
>>>>>>> upstream/android-13

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_lock);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	return ret;
}

static void sugov_exit(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy = policy->governor_data;
	struct sugov_tunables *tunables = sg_policy->tunables;
	unsigned int count;

	mutex_lock(&global_tunables_lock);

	count = gov_attr_set_put(&tunables->attr_set, &sg_policy->tunables_hook);
	policy->governor_data = NULL;
	if (!count)
<<<<<<< HEAD
		sugov_tunables_free(tunables);
=======
		sugov_clear_global_tunables();
>>>>>>> upstream/android-13

	mutex_unlock(&global_tunables_lock);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);
}

static int sugov_start(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy = policy->governor_data;
<<<<<<< HEAD
	unsigned int cpu;

	sg_policy->up_rate_delay_ns =
		sg_policy->tunables->up_rate_limit_us * NSEC_PER_USEC;
	sg_policy->down_rate_delay_ns =
		sg_policy->tunables->down_rate_limit_us * NSEC_PER_USEC;
	update_min_rate_limit_ns(sg_policy);
=======
	void (*uu)(struct update_util_data *data, u64 time, unsigned int flags);
	unsigned int cpu;

	sg_policy->freq_update_delay_ns	= sg_policy->tunables->rate_limit_us * NSEC_PER_USEC;
>>>>>>> upstream/android-13
	sg_policy->last_freq_update_time	= 0;
	sg_policy->next_freq			= 0;
	sg_policy->work_in_progress		= false;
	sg_policy->limits_changed		= false;
<<<<<<< HEAD
	sg_policy->need_freq_update		= false;
	sg_policy->cached_raw_freq		= 0;
	sg_policy->prev_cached_raw_freq		= 0;
=======
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);
>>>>>>> upstream/android-13

	for_each_cpu(cpu, policy->cpus) {
		struct sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, sizeof(*sg_cpu));
		sg_cpu->cpu			= cpu;
		sg_cpu->sg_policy		= sg_policy;
<<<<<<< HEAD
		sg_cpu->min			=
			(SCHED_CAPACITY_SCALE * policy->cpuinfo.min_freq) /
			policy->cpuinfo.max_freq;
	}

	for_each_cpu(cpu, policy->cpus) {
		struct sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util,
					     policy_is_shared(policy) ?
							sugov_update_shared :
							sugov_update_single);
=======
	}

	if (policy_is_shared(policy))
		uu = sugov_update_shared;
	else if (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	else
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		struct sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util, uu);
>>>>>>> upstream/android-13
	}
	return 0;
}

static void sugov_stop(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy = policy->governor_data;
	unsigned int cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

<<<<<<< HEAD
	synchronize_sched();
=======
	synchronize_rcu();
>>>>>>> upstream/android-13

	if (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

static void sugov_limits(struct cpufreq_policy *policy)
{
	struct sugov_policy *sg_policy = policy->governor_data;

	if (!policy->fast_switch_enabled) {
		mutex_lock(&sg_policy->work_lock);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_lock);
	}

	sg_policy->limits_changed = true;
}

struct cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
<<<<<<< HEAD
	.dynamic_switching	= true,
=======
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
>>>>>>> upstream/android-13
	.init			= sugov_init,
	.exit			= sugov_exit,
	.start			= sugov_start,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
struct cpufreq_governor *cpufreq_default_governor(void)
{
	return &schedutil_gov;
}
#endif

<<<<<<< HEAD
static int __init sugov_register(void)
{
	return cpufreq_register_governor(&schedutil_gov);
}
fs_initcall(sugov_register);

#ifdef CONFIG_ENERGY_MODEL
extern bool sched_energy_update;
extern struct mutex sched_energy_mutex;

static void rebuild_sd_workfn(struct work_struct *work)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}
static DECLARE_WORK(rebuild_sd_work, rebuild_sd_workfn);

/*
 * EAS shouldn't be attempted without sugov, so rebuild the sched_domains
 * on governor changes to make sure the scheduler knows about it.
 */
void sched_cpufreq_governor_change(struct cpufreq_policy *policy,
				  struct cpufreq_governor *old_gov)
{
	if (old_gov == &schedutil_gov || policy->governor == &schedutil_gov) {
		/*
		 * When called from the cpufreq_register_driver() path, the
		 * cpu_hotplug_lock is already held, so use a work item to
		 * avoid nested locking in rebuild_sched_domains().
		 */
		schedule_work(&rebuild_sd_work);
	}

}
#endif
=======
cpufreq_governor_init(schedutil_gov);
>>>>>>> upstream/android-13
