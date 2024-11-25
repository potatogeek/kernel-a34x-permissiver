<<<<<<< HEAD
/*
 * Copyright (C) 2013 Regents of the University of California
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
 * Copyright (C) 2013 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_STRING_H
#define _ASM_RISCV_STRING_H

#include <linux/types.h>
#include <linux/linkage.h>

#define __HAVE_ARCH_MEMSET
extern asmlinkage void *memset(void *, int, size_t);
<<<<<<< HEAD

#define __HAVE_ARCH_MEMCPY
extern asmlinkage void *memcpy(void *, const void *, size_t);

=======
extern asmlinkage void *__memset(void *, int, size_t);
#define __HAVE_ARCH_MEMCPY
extern asmlinkage void *memcpy(void *, const void *, size_t);
extern asmlinkage void *__memcpy(void *, const void *, size_t);
#define __HAVE_ARCH_MEMMOVE
extern asmlinkage void *memmove(void *, const void *, size_t);
extern asmlinkage void *__memmove(void *, const void *, size_t);
/* For those files which don't want to check by kasan. */
#if defined(CONFIG_KASAN) && !defined(__SANITIZE_ADDRESS__)
#define memcpy(dst, src, len) __memcpy(dst, src, len)
#define memset(s, c, n) __memset(s, c, n)
#define memmove(dst, src, len) __memmove(dst, src, len)

#ifndef __NO_FORTIFY
#define __NO_FORTIFY /* FORTIFY_SOURCE uses __builtin_memcpy, etc. */
#endif

#endif
>>>>>>> upstream/android-13
#endif /* _ASM_RISCV_STRING_H */
