/* SPDX-License-Identifier: GPL-2.0 */
/* syscall.h */

#ifndef _ASM_PARISC_SYSCALL_H_
#define _ASM_PARISC_SYSCALL_H_

#include <uapi/linux/audit.h>
#include <linux/compat.h>
#include <linux/err.h>
#include <asm/ptrace.h>

#define NR_syscalls (__NR_Linux_syscalls)

static inline long syscall_get_nr(struct task_struct *tsk,
				  struct pt_regs *regs)
{
	return regs->gr[20];
}

static inline void syscall_get_arguments(struct task_struct *tsk,
<<<<<<< HEAD
					 struct pt_regs *regs, unsigned int i,
					 unsigned int n, unsigned long *args)
{
	BUG_ON(i);

	switch (n) {
	case 6:
		args[5] = regs->gr[21];
	case 5:
		args[4] = regs->gr[22];
	case 4:
		args[3] = regs->gr[23];
	case 3:
		args[2] = regs->gr[24];
	case 2:
		args[1] = regs->gr[25];
	case 1:
		args[0] = regs->gr[26];
	case 0:
		break;
	default:
		BUG();
	}
=======
					 struct pt_regs *regs,
					 unsigned long *args)
{
	args[5] = regs->gr[21];
	args[4] = regs->gr[22];
	args[3] = regs->gr[23];
	args[2] = regs->gr[24];
	args[1] = regs->gr[25];
	args[0] = regs->gr[26];
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->gr[28];
	return IS_ERR_VALUE(error) ? error : 0;
>>>>>>> upstream/android-13
}

static inline long syscall_get_return_value(struct task_struct *task,
						struct pt_regs *regs)
{
	return regs->gr[28];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->gr[28] = error ? error : val;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	/* do nothing */
}

<<<<<<< HEAD
static inline int syscall_get_arch(void)
{
	int arch = AUDIT_ARCH_PARISC;
#ifdef CONFIG_64BIT
	if (!is_compat_task())
=======
static inline int syscall_get_arch(struct task_struct *task)
{
	int arch = AUDIT_ARCH_PARISC;
#ifdef CONFIG_64BIT
	if (!__is_compat_task(task))
>>>>>>> upstream/android-13
		arch = AUDIT_ARCH_PARISC64;
#endif
	return arch;
}
#endif /*_ASM_PARISC_SYSCALL_H_*/
