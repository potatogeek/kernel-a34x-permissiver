<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Shared support for SH-X3 interrupt controllers.
 *
 *  Copyright (C) 2009 - 2010  Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/init.h>

#define INTACK		0xfe4100b8
#define INTACKCLR	0xfe4100bc
#define INTC_USERIMASK	0xfe411000

#ifdef CONFIG_INTC_BALANCING
unsigned int irq_lookup(unsigned int irq)
{
	return __raw_readl(INTACK) & 1 ? irq : NO_IRQ_IGNORE;
}

void irq_finish(unsigned int irq)
{
	__raw_writel(irq2evt(irq), INTACKCLR);
}
#endif

static int __init shx3_irq_setup(void)
{
	return register_intc_userimask(INTC_USERIMASK);
}
arch_initcall(shx3_irq_setup);
