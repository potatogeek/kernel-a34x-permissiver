<<<<<<< HEAD
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2006 Atmark Techno, Inc.
>>>>>>> upstream/android-13
 */

#ifndef _ASM_MICROBLAZE_IRQ_H
#define _ASM_MICROBLAZE_IRQ_H

<<<<<<< HEAD
#define NR_IRQS		(32 + 1)
=======
>>>>>>> upstream/android-13
#include <asm-generic/irq.h>

struct pt_regs;
extern void do_IRQ(struct pt_regs *regs);

/* should be defined in each interrupt controller driver */
extern unsigned int xintc_get_irq(void);

#endif /* _ASM_MICROBLAZE_IRQ_H */
