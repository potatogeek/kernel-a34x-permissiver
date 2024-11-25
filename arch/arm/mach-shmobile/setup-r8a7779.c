// SPDX-License-Identifier: GPL-2.0
/*
 * r8a7779 processor support
 *
 * Copyright (C) 2011, 2013  Renesas Solutions Corp.
 * Copyright (C) 2011  Magnus Damm
 * Copyright (C) 2013  Cogent Embedded, Inc.
 */
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/irqchip/arm-gic.h>
=======
#include <linux/irqchip.h>
>>>>>>> upstream/android-13

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "common.h"
#include "r8a7779.h"

<<<<<<< HEAD
static struct map_desc r8a7779_io_desc[] __initdata = {
	/* 2M identity mapping for 0xf0000000 (MPCORE) */
	{
		.virtual	= 0xf0000000,
		.pfn		= __phys_to_pfn(0xf0000000),
		.length		= SZ_2M,
		.type		= MT_DEVICE_NONSHARED
	},
	/* 16M identity mapping for 0xfexxxxxx (DMAC-S/HPBREG/INTC2/LRAM/DBSC) */
	{
		.virtual	= 0xfe000000,
		.pfn		= __phys_to_pfn(0xfe000000),
		.length		= SZ_16M,
		.type		= MT_DEVICE_NONSHARED
	},
};

static void __init r8a7779_map_io(void)
{
	debug_ll_io_init();
	iotable_init(r8a7779_io_desc, ARRAY_SIZE(r8a7779_io_desc));
}

/* IRQ */
#define INT2SMSKCR0 IOMEM(0xfe7822a0)
#define INT2SMSKCR1 IOMEM(0xfe7822a4)
#define INT2SMSKCR2 IOMEM(0xfe7822a8)
#define INT2SMSKCR3 IOMEM(0xfe7822ac)
#define INT2SMSKCR4 IOMEM(0xfe7822b0)

#define INT2NTSR0 IOMEM(0xfe700060)
#define INT2NTSR1 IOMEM(0xfe700064)

static void __init r8a7779_init_irq_dt(void)
{
	irqchip_init();

	/* route all interrupts to ARM */
	__raw_writel(0xffffffff, INT2NTSR0);
	__raw_writel(0x3fffffff, INT2NTSR1);

	/* unmask all known interrupts in INTCS2 */
	__raw_writel(0xfffffff0, INT2SMSKCR0);
	__raw_writel(0xfff7ffff, INT2SMSKCR1);
	__raw_writel(0xfffbffdf, INT2SMSKCR2);
	__raw_writel(0xbffffffc, INT2SMSKCR3);
	__raw_writel(0x003fee3f, INT2SMSKCR4);
=======
#define HPBREG_BASE	0xfe700000

/* IRQ */
#define INT2SMSKCR0	0x822a0	/* Interrupt Submask Clear Register 0 */
#define INT2SMSKCR1	0x822a4	/* Interrupt Submask Clear Register 1 */
#define INT2SMSKCR2	0x822a8	/* Interrupt Submask Clear Register 2 */
#define INT2SMSKCR3	0x822ac	/* Interrupt Submask Clear Register 3 */
#define INT2SMSKCR4	0x822b0	/* Interrupt Submask Clear Register 4 */

#define INT2NTSR0	0x00060	/* Interrupt Notification Select Register 0 */
#define INT2NTSR1	0x00064	/* Interrupt Notification Select Register 1 */

static void __init r8a7779_init_irq_dt(void)
{
	void __iomem *base = ioremap(HPBREG_BASE, 0x00100000);

	irqchip_init();

	/* route all interrupts to ARM */
	writel(0xffffffff, base + INT2NTSR0);
	writel(0x3fffffff, base + INT2NTSR1);

	/* unmask all known interrupts in INTCS2 */
	writel(0xfffffff0, base + INT2SMSKCR0);
	writel(0xfff7ffff, base + INT2SMSKCR1);
	writel(0xfffbffdf, base + INT2SMSKCR2);
	writel(0xbffffffc, base + INT2SMSKCR3);
	writel(0x003fee3f, base + INT2SMSKCR4);

	iounmap(base);
>>>>>>> upstream/android-13
}

static const char *const r8a7779_compat_dt[] __initconst = {
	"renesas,r8a7779",
	NULL,
};

DT_MACHINE_START(R8A7779_DT, "Generic R8A7779 (Flattened Device Tree)")
	.smp		= smp_ops(r8a7779_smp_ops),
<<<<<<< HEAD
	.map_io		= r8a7779_map_io,
	.init_early	= shmobile_init_delay,
=======
>>>>>>> upstream/android-13
	.init_irq	= r8a7779_init_irq_dt,
	.init_late	= shmobile_init_late,
	.dt_compat	= r8a7779_compat_dt,
MACHINE_END
