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

/*
 * There is explicitly no include guard here because this file is expected to
<<<<<<< HEAD
 * be included multiple times.  See uapi/asm/syscalls.h for more info.
 */

#define __ARCH_WANT_SYS_CLONE
#include <uapi/asm/unistd.h>
#include <uapi/asm/syscalls.h>
=======
 * be included multiple times.
 */

#define __ARCH_WANT_SYS_CLONE
#define __ARCH_WANT_MEMFD_SECRET

#include <uapi/asm/unistd.h>

#define NR_syscalls (__NR_syscalls)
>>>>>>> upstream/android-13
