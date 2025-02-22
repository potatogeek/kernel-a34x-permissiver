<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * arch/arm/mach-iop32x/irq.c
 *
 * Generic IOP32X IRQ handling functionality
 *
 * Author: Rory Bolt <rorybolt@pacbell.net>
 * Copyright (C) 2002 Rory Bolt
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <asm/mach/irq.h>
#include <asm/irq.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <asm/mach-types.h>

=======
#include <asm/mach-types.h>

#include "hardware.h"

>>>>>>> upstream/android-13
static u32 iop32x_mask;

static void intctl_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c0, c0, 0" : : "r" (val));
}

static void intstr_write(u32 val)
{
	asm volatile("mcr p6, 0, %0, c4, c0, 0" : : "r" (val));
}

static void
iop32x_irq_mask(struct irq_data *d)
{
<<<<<<< HEAD
	iop32x_mask &= ~(1 << d->irq);
=======
	iop32x_mask &= ~(1 << (d->irq - 1));
>>>>>>> upstream/android-13
	intctl_write(iop32x_mask);
}

static void
iop32x_irq_unmask(struct irq_data *d)
{
<<<<<<< HEAD
	iop32x_mask |= 1 << d->irq;
=======
	iop32x_mask |= 1 << (d->irq - 1);
>>>>>>> upstream/android-13
	intctl_write(iop32x_mask);
}

struct irq_chip ext_chip = {
	.name		= "IOP32x",
	.irq_ack	= iop32x_irq_mask,
	.irq_mask	= iop32x_irq_mask,
	.irq_unmask	= iop32x_irq_unmask,
};

void __init iop32x_init_irq(void)
{
	int i;

	iop_init_cp6_handler();

	intctl_write(0);
	intstr_write(0);
	if (machine_is_glantank() ||
	    machine_is_iq80321() ||
	    machine_is_iq31244() ||
	    machine_is_n2100() ||
	    machine_is_em7210())
		*IOP3XX_PCIIRSR = 0x0f;

<<<<<<< HEAD
	for (i = 0; i < NR_IRQS; i++) {
=======
	for (i = 1; i < NR_IRQS; i++) {
>>>>>>> upstream/android-13
		irq_set_chip_and_handler(i, &ext_chip, handle_level_irq);
		irq_clear_status_flags(i, IRQ_NOREQUEST | IRQ_NOPROBE);
	}
}
