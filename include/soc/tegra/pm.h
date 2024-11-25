<<<<<<< HEAD
/*
 * Copyright (C) 2014 NVIDIA Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2014 NVIDIA Corporation
>>>>>>> upstream/android-13
 */

#ifndef __SOC_TEGRA_PM_H__
#define __SOC_TEGRA_PM_H__

<<<<<<< HEAD
=======
#include <linux/errno.h>

>>>>>>> upstream/android-13
enum tegra_suspend_mode {
	TEGRA_SUSPEND_NONE = 0,
	TEGRA_SUSPEND_LP2, /* CPU voltage off */
	TEGRA_SUSPEND_LP1, /* CPU voltage off, DRAM self-refresh */
	TEGRA_SUSPEND_LP0, /* CPU + core voltage off, DRAM self-refresh */
	TEGRA_MAX_SUSPEND_MODE,
<<<<<<< HEAD
=======
	TEGRA_SUSPEND_NOT_READY,
>>>>>>> upstream/android-13
};

#if defined(CONFIG_PM_SLEEP) && defined(CONFIG_ARM)
enum tegra_suspend_mode
tegra_pm_validate_suspend_mode(enum tegra_suspend_mode mode);

/* low-level resume entry point */
void tegra_resume(void);
<<<<<<< HEAD
=======

int tegra30_pm_secondary_cpu_suspend(unsigned long arg);
void tegra_pm_clear_cpu_in_lp2(void);
void tegra_pm_set_cpu_in_lp2(void);
int tegra_pm_enter_lp2(void);
int tegra_pm_park_secondary_cpu(unsigned long cpu);
void tegra_pm_init_suspend(void);
>>>>>>> upstream/android-13
#else
static inline enum tegra_suspend_mode
tegra_pm_validate_suspend_mode(enum tegra_suspend_mode mode)
{
	return TEGRA_SUSPEND_NONE;
}

static inline void tegra_resume(void)
{
}
<<<<<<< HEAD
=======

static inline int tegra30_pm_secondary_cpu_suspend(unsigned long arg)
{
	return -ENOTSUPP;
}

static inline void tegra_pm_clear_cpu_in_lp2(void)
{
}

static inline void tegra_pm_set_cpu_in_lp2(void)
{
}

static inline int tegra_pm_enter_lp2(void)
{
	return -ENOTSUPP;
}

static inline int tegra_pm_park_secondary_cpu(unsigned long cpu)
{
	return -ENOTSUPP;
}

static inline void tegra_pm_init_suspend(void)
{
}
>>>>>>> upstream/android-13
#endif /* CONFIG_PM_SLEEP */

#endif /* __SOC_TEGRA_PM_H__ */
