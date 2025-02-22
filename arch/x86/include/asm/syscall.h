<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Access to user system call parameters and results
 *
 * Copyright (C) 2008-2009 Red Hat, Inc.  All rights reserved.
 *
<<<<<<< HEAD
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
=======
>>>>>>> upstream/android-13
 * See asm-generic/syscall.h for descriptions of what we must do here.
 */

#ifndef _ASM_X86_SYSCALL_H
#define _ASM_X86_SYSCALL_H

#include <uapi/linux/audit.h>
#include <linux/sched.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <asm/asm-offsets.h>	/* For NR_syscalls */
#include <asm/thread_info.h>	/* for TS_COMPAT */
#include <asm/unistd.h>

#ifdef CONFIG_X86_64
typedef asmlinkage long (*sys_call_ptr_t)(const struct pt_regs *);
#else
typedef asmlinkage long (*sys_call_ptr_t)(unsigned long, unsigned long,
					  unsigned long, unsigned long,
					  unsigned long, unsigned long);
#endif /* CONFIG_X86_64 */
=======
#include <asm/thread_info.h>	/* for TS_COMPAT */
#include <asm/unistd.h>

typedef long (*sys_call_ptr_t)(const struct pt_regs *);
>>>>>>> upstream/android-13
extern const sys_call_ptr_t sys_call_table[];

#if defined(CONFIG_X86_32)
#define ia32_sys_call_table sys_call_table
<<<<<<< HEAD
#define __NR_syscall_compat_max __NR_syscall_max
#define IA32_NR_syscalls NR_syscalls
#endif

#if defined(CONFIG_IA32_EMULATION)
extern const sys_call_ptr_t ia32_sys_call_table[];
=======
#else
/*
 * These may not exist, but still put the prototypes in so we
 * can use IS_ENABLED().
 */
extern const sys_call_ptr_t ia32_sys_call_table[];
extern const sys_call_ptr_t x32_sys_call_table[];
>>>>>>> upstream/android-13
#endif

/*
 * Only the low 32 bits of orig_ax are meaningful, so we return int.
 * This importantly ignores the high bits on 64-bit, so comparisons
 * sign-extend the low 32 bits.
 */
static inline int syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
{
	return regs->orig_ax;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->ax = regs->orig_ax;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->ax;
#ifdef CONFIG_IA32_EMULATION
	/*
	 * TS_COMPAT is set for 32-bit syscall entries and then
	 * remains set until we return to user mode.
	 */
	if (task->thread_info.status & (TS_COMPAT|TS_I386_REGS_POKED))
		/*
		 * Sign-extend the value so (int)-EFOO becomes (long)-EFOO
		 * and will match correctly in comparisons.
		 */
		error = (long) (int) error;
#endif
	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->ax;
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->ax = (long) error ?: val;
}

#ifdef CONFIG_X86_32

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(args, &regs->bx + i, n * sizeof(args[0]));
=======
					 unsigned long *args)
{
	memcpy(args, &regs->bx, 6 * sizeof(args[0]));
>>>>>>> upstream/android-13
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(&regs->bx + i, args, n * sizeof(args[0]));
}

<<<<<<< HEAD
static inline int syscall_get_arch(void)
=======
static inline int syscall_get_arch(struct task_struct *task)
>>>>>>> upstream/android-13
{
	return AUDIT_ARCH_I386;
}

#else	 /* CONFIG_X86_64 */

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT)
		switch (i) {
		case 0:
			if (!n--) break;
			*args++ = regs->bx;
		case 1:
			if (!n--) break;
			*args++ = regs->cx;
		case 2:
			if (!n--) break;
			*args++ = regs->dx;
		case 3:
			if (!n--) break;
			*args++ = regs->si;
		case 4:
			if (!n--) break;
			*args++ = regs->di;
		case 5:
			if (!n--) break;
			*args++ = regs->bp;
		case 6:
			if (!n--) break;
		default:
			BUG();
			break;
		}
	else
# endif
		switch (i) {
		case 0:
			if (!n--) break;
			*args++ = regs->di;
		case 1:
			if (!n--) break;
			*args++ = regs->si;
		case 2:
			if (!n--) break;
			*args++ = regs->dx;
		case 3:
			if (!n--) break;
			*args++ = regs->r10;
		case 4:
			if (!n--) break;
			*args++ = regs->r8;
		case 5:
			if (!n--) break;
			*args++ = regs->r9;
		case 6:
			if (!n--) break;
		default:
			BUG();
			break;
		}
=======
					 unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT) {
		*args++ = regs->bx;
		*args++ = regs->cx;
		*args++ = regs->dx;
		*args++ = regs->si;
		*args++ = regs->di;
		*args   = regs->bp;
	} else
# endif
	{
		*args++ = regs->di;
		*args++ = regs->si;
		*args++ = regs->dx;
		*args++ = regs->r10;
		*args++ = regs->r8;
		*args   = regs->r9;
	}
>>>>>>> upstream/android-13
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT)
		switch (i) {
		case 0:
			if (!n--) break;
			regs->bx = *args++;
		case 1:
			if (!n--) break;
			regs->cx = *args++;
		case 2:
			if (!n--) break;
			regs->dx = *args++;
		case 3:
			if (!n--) break;
			regs->si = *args++;
		case 4:
			if (!n--) break;
			regs->di = *args++;
		case 5:
			if (!n--) break;
			regs->bp = *args++;
		case 6:
			if (!n--) break;
		default:
			BUG();
			break;
		}
	else
# endif
		switch (i) {
		case 0:
			if (!n--) break;
			regs->di = *args++;
		case 1:
			if (!n--) break;
			regs->si = *args++;
		case 2:
			if (!n--) break;
			regs->dx = *args++;
		case 3:
			if (!n--) break;
			regs->r10 = *args++;
		case 4:
			if (!n--) break;
			regs->r8 = *args++;
		case 5:
			if (!n--) break;
			regs->r9 = *args++;
		case 6:
			if (!n--) break;
		default:
			BUG();
			break;
		}
}

static inline int syscall_get_arch(void)
{
	/* x32 tasks should be considered AUDIT_ARCH_X86_64. */
	return in_ia32_syscall() ? AUDIT_ARCH_I386 : AUDIT_ARCH_X86_64;
}
=======
					 const unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT) {
		regs->bx = *args++;
		regs->cx = *args++;
		regs->dx = *args++;
		regs->si = *args++;
		regs->di = *args++;
		regs->bp = *args;
	} else
# endif
	{
		regs->di = *args++;
		regs->si = *args++;
		regs->dx = *args++;
		regs->r10 = *args++;
		regs->r8 = *args++;
		regs->r9 = *args;
	}
}

static inline int syscall_get_arch(struct task_struct *task)
{
	/* x32 tasks should be considered AUDIT_ARCH_X86_64. */
	return (IS_ENABLED(CONFIG_IA32_EMULATION) &&
		task->thread_info.status & TS_COMPAT)
		? AUDIT_ARCH_I386 : AUDIT_ARCH_X86_64;
}

void do_syscall_64(struct pt_regs *regs, int nr);
void do_int80_syscall_32(struct pt_regs *regs);
long do_fast_syscall_32(struct pt_regs *regs);

>>>>>>> upstream/android-13
#endif	/* CONFIG_X86_32 */

#endif	/* _ASM_X86_SYSCALL_H */
