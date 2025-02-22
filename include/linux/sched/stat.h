/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_STAT_H
#define _LINUX_SCHED_STAT_H

#include <linux/percpu.h>
<<<<<<< HEAD
=======
#include <linux/kconfig.h>
>>>>>>> upstream/android-13

/*
 * Various counters maintained by the scheduler and fork(),
 * exposed via /proc, sys.c or used by drivers via these APIs.
 *
<<<<<<< HEAD
 * ( Note that all these values are aquired without locking,
=======
 * ( Note that all these values are acquired without locking,
>>>>>>> upstream/android-13
 *   so they can only be relied on in narrow circumstances. )
 */

extern unsigned long total_forks;
extern int nr_threads;
DECLARE_PER_CPU(unsigned long, process_counts);
extern int nr_processes(void);
<<<<<<< HEAD
extern unsigned long nr_running(void);
extern bool single_task_running(void);
extern unsigned long nr_iowait(void);
extern unsigned long nr_iowait_cpu(int cpu);
extern void get_iowait_load(unsigned long *nr_waiters, unsigned long *load);

static inline int sched_info_on(void)
{
#ifdef CONFIG_SCHEDSTATS
	return 1;
#elif defined(CONFIG_TASK_DELAY_ACCT)
	extern int delayacct_on;
	return delayacct_on;
#else
	return 0;
#endif
=======
extern unsigned int nr_running(void);
extern bool single_task_running(void);
extern unsigned int nr_iowait(void);
extern unsigned int nr_iowait_cpu(int cpu);

static inline int sched_info_on(void)
{
	return IS_ENABLED(CONFIG_SCHED_INFO);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_SCHEDSTATS
void force_schedstat_enabled(void);
#endif

#endif /* _LINUX_SCHED_STAT_H */
