<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/include/linux/cpu_cooling.h
 *
 *  Copyright (C) 2012	Samsung Electronics Co., Ltd(http://www.samsung.com)
 *  Copyright (C) 2012  Amit Daniel <amit.kachhap@linaro.org>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<<<<<<< HEAD
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
=======
>>>>>>> upstream/android-13
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef __CPU_COOLING_H__
#define __CPU_COOLING_H__

#include <linux/of.h>
#include <linux/thermal.h>
#include <linux/cpumask.h>

struct cpufreq_policy;

<<<<<<< HEAD
typedef int (*plat_mitig_t)(int cpu, u32 clip_freq);

struct cpu_cooling_ops {
	plat_mitig_t ceil_limit, floor_limit;
};

#ifdef CONFIG_CPU_THERMAL
=======
#ifdef CONFIG_CPU_FREQ_THERMAL
>>>>>>> upstream/android-13
/**
 * cpufreq_cooling_register - function to create cpufreq cooling device.
 * @policy: cpufreq policy.
 */
struct thermal_cooling_device *
cpufreq_cooling_register(struct cpufreq_policy *policy);

<<<<<<< HEAD
struct thermal_cooling_device *
cpufreq_platform_cooling_register(struct cpufreq_policy *policy,
					struct cpu_cooling_ops *ops);

=======
>>>>>>> upstream/android-13
/**
 * cpufreq_cooling_unregister - function to remove cpufreq cooling device.
 * @cdev: thermal cooling device pointer.
 */
void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev);

<<<<<<< HEAD
#else /* !CONFIG_CPU_THERMAL */
=======
/**
 * of_cpufreq_cooling_register - create cpufreq cooling device based on DT.
 * @policy: cpufreq policy.
 */
struct thermal_cooling_device *
of_cpufreq_cooling_register(struct cpufreq_policy *policy);

#else /* !CONFIG_CPU_FREQ_THERMAL */
>>>>>>> upstream/android-13
static inline struct thermal_cooling_device *
cpufreq_cooling_register(struct cpufreq_policy *policy)
{
	return ERR_PTR(-ENOSYS);
}

<<<<<<< HEAD
static inline struct thermal_cooling_device *
cpufreq_platform_cooling_register(struct cpufreq_policy *policy,
					struct cpu_cooling_ops *ops)
{
	return NULL;
}

=======
>>>>>>> upstream/android-13
static inline
void cpufreq_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return;
}
<<<<<<< HEAD
#endif	/* CONFIG_CPU_THERMAL */

#if defined(CONFIG_THERMAL_OF) && defined(CONFIG_CPU_THERMAL)
/**
 * of_cpufreq_cooling_register - create cpufreq cooling device based on DT.
 * @policy: cpufreq policy.
 */
struct thermal_cooling_device *
of_cpufreq_cooling_register(struct cpufreq_policy *policy);
#else
=======

>>>>>>> upstream/android-13
static inline struct thermal_cooling_device *
of_cpufreq_cooling_register(struct cpufreq_policy *policy)
{
	return NULL;
}
<<<<<<< HEAD
#endif /* defined(CONFIG_THERMAL_OF) && defined(CONFIG_CPU_THERMAL) */
=======
#endif /* CONFIG_CPU_FREQ_THERMAL */

struct cpuidle_driver;

#ifdef CONFIG_CPU_IDLE_THERMAL
void cpuidle_cooling_register(struct cpuidle_driver *drv);
#else /* CONFIG_CPU_IDLE_THERMAL */
static inline void cpuidle_cooling_register(struct cpuidle_driver *drv)
{
}
#endif /* CONFIG_CPU_IDLE_THERMAL */
>>>>>>> upstream/android-13

#endif /* __CPU_COOLING_H__ */
