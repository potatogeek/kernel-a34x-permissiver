

#ifndef __LINUX_CPUFREQ_LIMIT_H__
#define __LINUX_CPUFREQ_LIMIT_H__

enum {
	DVFS_USER_ID = 0,
	DVFS_TOUCH_ID,
	DVFS_FINGER_ID,
	DVFS_OVERLIMIT_ID,
	DVFS_MAX_ID
};

extern void cpufreq_limit_set_table(int cpu, struct cpufreq_frequency_table *ftbl);
#ifdef CONFIG_CPU_FREQ_LIMIT
extern int set_freq_limit(unsigned int id, int freq);
#else
static inline int set_freq_limit(unsigned int id, int freq) { return 0; }
#endif

#endif 
