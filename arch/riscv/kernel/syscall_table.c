<<<<<<< HEAD
/*
 * Copyright (C) 2009 Arnd Bergmann <arnd@arndb.de>
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
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2009 Arnd Bergmann <arnd@arndb.de>
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <asm-generic/syscalls.h>
<<<<<<< HEAD
#include <asm/vdso.h>
=======
#include <asm/syscall.h>
>>>>>>> upstream/android-13

#undef __SYSCALL
#define __SYSCALL(nr, call)	[nr] = (call),

<<<<<<< HEAD
void *sys_call_table[__NR_syscalls] = {
=======
void * const sys_call_table[__NR_syscalls] = {
>>>>>>> upstream/android-13
	[0 ... __NR_syscalls - 1] = sys_ni_syscall,
#include <asm/unistd.h>
};
