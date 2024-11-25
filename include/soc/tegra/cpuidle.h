<<<<<<< HEAD
/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __SOC_TEGRA_CPUIDLE_H__
#define __SOC_TEGRA_CPUIDLE_H__

<<<<<<< HEAD
#if defined(CONFIG_ARM) && defined(CONFIG_ARCH_TEGRA) && defined(CONFIG_CPU_IDLE)
=======
#ifdef CONFIG_ARM_TEGRA_CPUIDLE
>>>>>>> upstream/android-13
void tegra_cpuidle_pcie_irqs_in_use(void);
#else
static inline void tegra_cpuidle_pcie_irqs_in_use(void)
{
}
#endif

#endif /* __SOC_TEGRA_CPUIDLE_H__ */
