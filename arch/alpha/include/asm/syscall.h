/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ALPHA_SYSCALL_H
#define _ASM_ALPHA_SYSCALL_H

#include <uapi/linux/audit.h>

<<<<<<< HEAD
static inline int syscall_get_arch(void)
=======
static inline int syscall_get_arch(struct task_struct *task)
>>>>>>> upstream/android-13
{
	return AUDIT_ARCH_ALPHA;
}

<<<<<<< HEAD
=======
static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->r0;
}

>>>>>>> upstream/android-13
#endif	/* _ASM_ALPHA_SYSCALL_H */
