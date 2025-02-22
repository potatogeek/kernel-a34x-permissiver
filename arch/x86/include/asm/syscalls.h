<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * syscalls.h - Linux syscall interfaces (arch-specific)
 *
 * Copyright (c) 2008 Jaswinder Singh Rajput
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_X86_SYSCALLS_H
#define _ASM_X86_SYSCALLS_H

<<<<<<< HEAD
#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/types.h>

=======
>>>>>>> upstream/android-13
/* Common in X86_32 and X86_64 */
/* kernel/ioport.c */
long ksys_ioperm(unsigned long from, unsigned long num, int turn_on);

<<<<<<< HEAD
#ifdef CONFIG_X86_32
/*
 * These definitions are only valid on pure 32-bit systems; x86-64 uses a
 * different syscall calling convention
 */
asmlinkage long sys_ioperm(unsigned long, unsigned long, int);
asmlinkage long sys_iopl(unsigned int);

/* kernel/ldt.c */
asmlinkage long sys_modify_ldt(int, void __user *, unsigned long);

/* kernel/signal.c */
asmlinkage long sys_rt_sigreturn(void);

/* kernel/tls.c */
asmlinkage long sys_set_thread_area(struct user_desc __user *);
asmlinkage long sys_get_thread_area(struct user_desc __user *);

/* X86_32 only */

/* kernel/signal.c */
asmlinkage long sys_sigreturn(void);

/* kernel/vm86_32.c */
struct vm86_struct;
asmlinkage long sys_vm86old(struct vm86_struct __user *);
asmlinkage long sys_vm86(unsigned long, unsigned long);

#endif /* CONFIG_X86_32 */
=======
>>>>>>> upstream/android-13
#endif /* _ASM_X86_SYSCALLS_H */
