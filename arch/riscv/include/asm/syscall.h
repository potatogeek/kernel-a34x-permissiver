<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2008-2009 Red Hat, Inc.  All rights reserved.
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 * Copyright 2015 Regents of the University of California, Berkeley
 *
<<<<<<< HEAD
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 * See asm-generic/syscall.h for descriptions of what we must do here.
 */

#ifndef _ASM_RISCV_SYSCALL_H
#define _ASM_RISCV_SYSCALL_H

<<<<<<< HEAD
=======
#include <uapi/linux/audit.h>
>>>>>>> upstream/android-13
#include <linux/sched.h>
#include <linux/err.h>

/* The array of function pointers for syscalls. */
<<<<<<< HEAD
extern void *sys_call_table[];
=======
extern void * const sys_call_table[];
>>>>>>> upstream/android-13

/*
 * Only the low 32 bits of orig_r0 are meaningful, so we return int.
 * This importantly ignores the high bits on 64-bit, so comparisons
 * sign-extend the low 32 bits.
 */
static inline int syscall_get_nr(struct task_struct *task,
				 struct pt_regs *regs)
{
	return regs->a7;
}

<<<<<<< HEAD
static inline void syscall_set_nr(struct task_struct *task,
				  struct pt_regs *regs,
				  int sysno)
{
	regs->a7 = sysno;
}

=======
>>>>>>> upstream/android-13
static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
        regs->a0 = regs->orig_a0;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->a0;

	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->a0;
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->a0 = (long) error ?: val;
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	BUG_ON(i + n > 6);
	if (i == 0) {
		args[0] = regs->orig_a0;
		args++;
		n--;
	} else {
		i--;
	}
	memcpy(args, &regs->a1 + i, n * sizeof(args[0]));
=======
					 unsigned long *args)
{
	args[0] = regs->orig_a0;
	args++;
	memcpy(args, &regs->a1, 5 * sizeof(args[0]));
>>>>>>> upstream/android-13
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
        if (i == 0) {
                regs->orig_a0 = args[0];
                args++;
                n--;
	} else {
		i--;
	}
	memcpy(&regs->a1 + i, args, n * sizeof(regs->a1));
}

=======
					 const unsigned long *args)
{
	regs->orig_a0 = args[0];
	args++;
	memcpy(&regs->a1, args, 5 * sizeof(regs->a1));
}

static inline int syscall_get_arch(struct task_struct *task)
{
#ifdef CONFIG_64BIT
	return AUDIT_ARCH_RISCV64;
#else
	return AUDIT_ARCH_RISCV32;
#endif
}

asmlinkage long sys_riscv_flush_icache(uintptr_t, uintptr_t, uintptr_t);
>>>>>>> upstream/android-13
#endif	/* _ASM_RISCV_SYSCALL_H */
