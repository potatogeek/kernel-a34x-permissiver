<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Fallback per-CPU frame pointer holder
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_GENERIC_IRQ_REGS_H
#define _ASM_GENERIC_IRQ_REGS_H

#include <linux/percpu.h>

/*
 * Per-cpu current frame pointer - the location of the last exception frame on
 * the stack
 */
DECLARE_PER_CPU(struct pt_regs *, __irq_regs);

static inline struct pt_regs *get_irq_regs(void)
{
	return __this_cpu_read(__irq_regs);
}

static inline struct pt_regs *set_irq_regs(struct pt_regs *new_regs)
{
	struct pt_regs *old_regs;

	old_regs = __this_cpu_read(__irq_regs);
	__this_cpu_write(__irq_regs, new_regs);
	return old_regs;
}

#endif /* _ASM_GENERIC_IRQ_REGS_H */
