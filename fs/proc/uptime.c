// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/time.h>
<<<<<<< HEAD
=======
#include <linux/time_namespace.h>
>>>>>>> upstream/android-13
#include <linux/kernel_stat.h>

static int uptime_proc_show(struct seq_file *m, void *v)
{
	struct timespec64 uptime;
	struct timespec64 idle;
<<<<<<< HEAD
	u64 nsec;
	u32 rem;
	int i;

	nsec = 0;
	for_each_possible_cpu(i)
		nsec += (__force u64) kcpustat_cpu(i).cpustat[CPUTIME_IDLE];

	ktime_get_boottime_ts64(&uptime);
	idle.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
=======
	u64 idle_nsec;
	u32 rem;
	int i;

	idle_nsec = 0;
	for_each_possible_cpu(i) {
		struct kernel_cpustat kcs;

		kcpustat_cpu_fetch(&kcs, i);
		idle_nsec += get_idle_time(&kcs, i);
	}

	ktime_get_boottime_ts64(&uptime);
	timens_add_boottime(&uptime);

	idle.tv_sec = div_u64_rem(idle_nsec, NSEC_PER_SEC, &rem);
>>>>>>> upstream/android-13
	idle.tv_nsec = rem;
	seq_printf(m, "%lu.%02lu %lu.%02lu\n",
			(unsigned long) uptime.tv_sec,
			(uptime.tv_nsec / (NSEC_PER_SEC / 100)),
			(unsigned long) idle.tv_sec,
			(idle.tv_nsec / (NSEC_PER_SEC / 100)));
	return 0;
}

static int __init proc_uptime_init(void)
{
	proc_create_single("uptime", 0, NULL, uptime_proc_show);
	return 0;
}
fs_initcall(proc_uptime_init);
