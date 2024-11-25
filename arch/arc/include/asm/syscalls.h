<<<<<<< HEAD
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#ifndef _ASM_ARC_SYSCALLS_H
#define _ASM_ARC_SYSCALLS_H  1

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/types.h>

int sys_clone_wrapper(int, int, int, int, int);
<<<<<<< HEAD
=======
int sys_clone3_wrapper(void *, size_t);
>>>>>>> upstream/android-13
int sys_cacheflush(uint32_t, uint32_t uint32_t);
int sys_arc_settls(void *);
int sys_arc_gettls(void);
int sys_arc_usr_cmpxchg(int *, int, int);

#include <asm-generic/syscalls.h>

#endif
