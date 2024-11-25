<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_IRQ_H
#define _ASM_RISCV_IRQ_H

<<<<<<< HEAD
#define NR_IRQS         0

void riscv_timer_interrupt(void);
void riscv_software_interrupt(void);

#include <asm-generic/irq.h>

=======
#include <linux/interrupt.h>
#include <linux/linkage.h>

#include <asm-generic/irq.h>

extern void __init init_IRQ(void);

>>>>>>> upstream/android-13
#endif /* _ASM_RISCV_IRQ_H */
