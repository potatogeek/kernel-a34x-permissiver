<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * SMP support for R-Mobile / SH-Mobile - sh73a0 portion
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2010  Takashi Yoshii
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/delay.h>

#include <asm/smp_plat.h>
<<<<<<< HEAD
#include <asm/smp_twd.h>
=======
>>>>>>> upstream/android-13

#include "common.h"
#include "sh73a0.h"

<<<<<<< HEAD
#define WUPCR		IOMEM(0xe6151010)
#define SRESCR		IOMEM(0xe6151018)
#define PSTR		IOMEM(0xe6151040)
#define SBAR		IOMEM(0xe6180020)
#define APARMBAREA	IOMEM(0xe6f10020)
=======
#define CPG_BASE2	0xe6151000
#define WUPCR		0x10	/* System-CPU Wake Up Control Register */
#define SRESCR		0x18	/* System-CPU Software Reset Control Register */
#define PSTR		0x40	/* System-CPU Power Status Register */

#define SYSC_BASE	0xe6180000
#define SBAR		0x20	/* SYS Boot Address Register */

#define AP_BASE		0xe6f10000
#define APARMBAREA	0x20	/* Address Translation Area Register */
>>>>>>> upstream/android-13

#define SH73A0_SCU_BASE 0xf0000000

static int sh73a0_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned int lcpu = cpu_logical_map(cpu);
<<<<<<< HEAD

	if (((__raw_readl(PSTR) >> (4 * lcpu)) & 3) == 3)
		__raw_writel(1 << lcpu, WUPCR);	/* wake up */
	else
		__raw_writel(1 << lcpu, SRESCR);	/* reset */

=======
	void __iomem *cpg2 = ioremap(CPG_BASE2, PAGE_SIZE);

	if (((readl(cpg2 + PSTR) >> (4 * lcpu)) & 3) == 3)
		writel(1 << lcpu, cpg2 + WUPCR);	/* wake up */
	else
		writel(1 << lcpu, cpg2 + SRESCR);	/* reset */
	iounmap(cpg2);
>>>>>>> upstream/android-13
	return 0;
}

static void __init sh73a0_smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
	/* Map the reset vector (in headsmp.S) */
	__raw_writel(0, APARMBAREA);      /* 4k */
	__raw_writel(__pa(shmobile_boot_vector), SBAR);
=======
	void __iomem *ap = ioremap(AP_BASE, PAGE_SIZE);
	void __iomem *sysc = ioremap(SYSC_BASE, PAGE_SIZE);

	/* Map the reset vector (in headsmp.S) */
	writel(0, ap + APARMBAREA);      /* 4k */
	writel(__pa(shmobile_boot_vector), sysc + SBAR);
	iounmap(sysc);
	iounmap(ap);
>>>>>>> upstream/android-13

	/* setup sh73a0 specific SCU bits */
	shmobile_smp_scu_prepare_cpus(SH73A0_SCU_BASE, max_cpus);
}

const struct smp_operations sh73a0_smp_ops __initconst = {
	.smp_prepare_cpus	= sh73a0_smp_prepare_cpus,
	.smp_boot_secondary	= sh73a0_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_can_disable	= shmobile_smp_cpu_can_disable,
	.cpu_die		= shmobile_smp_scu_cpu_die,
	.cpu_kill		= shmobile_smp_scu_cpu_kill,
#endif
};
