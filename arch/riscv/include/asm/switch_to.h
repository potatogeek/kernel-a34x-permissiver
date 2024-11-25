<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
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
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_SWITCH_TO_H
#define _ASM_RISCV_SWITCH_TO_H

<<<<<<< HEAD
=======
#include <linux/jump_label.h>
#include <linux/sched/task_stack.h>
>>>>>>> upstream/android-13
#include <asm/processor.h>
#include <asm/ptrace.h>
#include <asm/csr.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_FPU
>>>>>>> upstream/android-13
extern void __fstate_save(struct task_struct *save_to);
extern void __fstate_restore(struct task_struct *restore_from);

static inline void __fstate_clean(struct pt_regs *regs)
{
<<<<<<< HEAD
	regs->sstatus = (regs->sstatus & ~SR_FS) | SR_FS_CLEAN;
=======
	regs->status = (regs->status & ~SR_FS) | SR_FS_CLEAN;
}

static inline void fstate_off(struct task_struct *task,
			      struct pt_regs *regs)
{
	regs->status = (regs->status & ~SR_FS) | SR_FS_OFF;
>>>>>>> upstream/android-13
}

static inline void fstate_save(struct task_struct *task,
			       struct pt_regs *regs)
{
<<<<<<< HEAD
	if ((regs->sstatus & SR_FS) == SR_FS_DIRTY) {
=======
	if ((regs->status & SR_FS) == SR_FS_DIRTY) {
>>>>>>> upstream/android-13
		__fstate_save(task);
		__fstate_clean(regs);
	}
}

static inline void fstate_restore(struct task_struct *task,
				  struct pt_regs *regs)
{
<<<<<<< HEAD
	if ((regs->sstatus & SR_FS) != SR_FS_OFF) {
=======
	if ((regs->status & SR_FS) != SR_FS_OFF) {
>>>>>>> upstream/android-13
		__fstate_restore(task);
		__fstate_clean(regs);
	}
}

static inline void __switch_to_aux(struct task_struct *prev,
				   struct task_struct *next)
{
	struct pt_regs *regs;

	regs = task_pt_regs(prev);
<<<<<<< HEAD
	if (unlikely(regs->sstatus & SR_SD))
=======
	if (unlikely(regs->status & SR_SD))
>>>>>>> upstream/android-13
		fstate_save(prev, regs);
	fstate_restore(next, task_pt_regs(next));
}

<<<<<<< HEAD
=======
extern struct static_key_false cpu_hwcap_fpu;
static __always_inline bool has_fpu(void)
{
	return static_branch_likely(&cpu_hwcap_fpu);
}
#else
static __always_inline bool has_fpu(void) { return false; }
#define fstate_save(task, regs) do { } while (0)
#define fstate_restore(task, regs) do { } while (0)
#define __switch_to_aux(__prev, __next) do { } while (0)
#endif

>>>>>>> upstream/android-13
extern struct task_struct *__switch_to(struct task_struct *,
				       struct task_struct *);

#define switch_to(prev, next, last)			\
do {							\
	struct task_struct *__prev = (prev);		\
	struct task_struct *__next = (next);		\
<<<<<<< HEAD
	__switch_to_aux(__prev, __next);		\
=======
	if (has_fpu())					\
		__switch_to_aux(__prev, __next);	\
>>>>>>> upstream/android-13
	((last) = __switch_to(__prev, __next));		\
} while (0)

#endif /* _ASM_RISCV_SWITCH_TO_H */
