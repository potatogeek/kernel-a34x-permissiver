<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * OpenRISC Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * OpenRISC implementation:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 * et al.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_OPENRISC_MMU_CONTEXT_H
#define __ASM_OPENRISC_MMU_CONTEXT_H

#include <asm-generic/mm_hooks.h>

<<<<<<< HEAD
extern int init_new_context(struct task_struct *tsk, struct mm_struct *mm);
=======
#define init_new_context init_new_context
extern int init_new_context(struct task_struct *tsk, struct mm_struct *mm);
#define destroy_context destroy_context
>>>>>>> upstream/android-13
extern void destroy_context(struct mm_struct *mm);
extern void switch_mm(struct mm_struct *prev, struct mm_struct *next,
		      struct task_struct *tsk);

<<<<<<< HEAD
#define deactivate_mm(tsk, mm)	do { } while (0)

=======
>>>>>>> upstream/android-13
#define activate_mm(prev, next) switch_mm((prev), (next), NULL)

/* current active pgd - this is similar to other processors pgd
 * registers like cr3 on the i386
 */

extern volatile pgd_t *current_pgd[]; /* defined in arch/openrisc/mm/fault.c */

<<<<<<< HEAD
static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}
=======
#include <asm-generic/mmu_context.h>
>>>>>>> upstream/android-13

#endif
