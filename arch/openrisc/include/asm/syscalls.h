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

#ifndef __ASM_OPENRISC_SYSCALLS_H
#define __ASM_OPENRISC_SYSCALLS_H

asmlinkage long sys_or1k_atomic(unsigned long type, unsigned long *v1,
				unsigned long *v2);

#include <asm-generic/syscalls.h>

asmlinkage long __sys_clone(unsigned long clone_flags, unsigned long newsp,
			void __user *parent_tid, void __user *child_tid, int tls);
<<<<<<< HEAD
asmlinkage long __sys_fork(void);

#define sys_clone __sys_clone
=======
asmlinkage long __sys_clone3(struct clone_args __user *uargs, size_t size);
asmlinkage long __sys_fork(void);

#define sys_clone __sys_clone
#define sys_clone3 __sys_clone3
>>>>>>> upstream/android-13
#define sys_fork __sys_fork

#endif /* __ASM_OPENRISC_SYSCALLS_H */
