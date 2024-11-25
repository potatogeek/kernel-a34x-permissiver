<<<<<<< HEAD
/*
 * Copyright (C) 2016 Linaro
 * Viresh Kumar <viresh.kumar@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016 Linaro
 * Viresh Kumar <viresh.kumar@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef __CPUFREQ_DT_H__
#define __CPUFREQ_DT_H__

#include <linux/types.h>

struct cpufreq_policy;

struct cpufreq_dt_platform_data {
	bool have_governor_per_policy;

<<<<<<< HEAD
=======
	unsigned int	(*get_intermediate)(struct cpufreq_policy *policy,
					    unsigned int index);
	int		(*target_intermediate)(struct cpufreq_policy *policy,
					       unsigned int index);
>>>>>>> upstream/android-13
	int (*suspend)(struct cpufreq_policy *policy);
	int (*resume)(struct cpufreq_policy *policy);
};

#endif /* __CPUFREQ_DT_H__ */
