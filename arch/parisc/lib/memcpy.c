<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *    Optimized memory copy routines.
 *
 *    Copyright (C) 2004 Randolph Chung <tausq@debian.org>
 *    Copyright (C) 2013-2017 Helge Deller <deller@gmx.de>
 *
<<<<<<< HEAD
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *    Portions derived from the GNU C Library
 *    Copyright (C) 1991, 1997, 2003 Free Software Foundation, Inc.
 *
=======
 *    Portions derived from the GNU C Library
 *    Copyright (C) 1991, 1997, 2003 Free Software Foundation, Inc.
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/compiler.h>
#include <linux/uaccess.h>

#define get_user_space() (uaccess_kernel() ? 0 : mfsp(3))
#define get_kernel_space() (0)

/* Returns 0 for success, otherwise, returns number of bytes not transferred. */
extern unsigned long pa_memcpy(void *dst, const void *src,
				unsigned long len);

unsigned long raw_copy_to_user(void __user *dst, const void *src,
			       unsigned long len)
{
	mtsp(get_kernel_space(), 1);
	mtsp(get_user_space(), 2);
	return pa_memcpy((void __force *)dst, src, len);
}
EXPORT_SYMBOL(raw_copy_to_user);

unsigned long raw_copy_from_user(void *dst, const void __user *src,
			       unsigned long len)
{
	mtsp(get_user_space(), 1);
	mtsp(get_kernel_space(), 2);
	return pa_memcpy(dst, (void __force *)src, len);
}
EXPORT_SYMBOL(raw_copy_from_user);

<<<<<<< HEAD
unsigned long raw_copy_in_user(void __user *dst, const void __user *src, unsigned long len)
{
	mtsp(get_user_space(), 1);
	mtsp(get_user_space(), 2);
	return pa_memcpy((void __force *)dst, (void __force *)src, len);
}


=======
>>>>>>> upstream/android-13
void * memcpy(void * dst,const void *src, size_t count)
{
	mtsp(get_kernel_space(), 1);
	mtsp(get_kernel_space(), 2);
	pa_memcpy(dst, src, count);
	return dst;
}

<<<<<<< HEAD
EXPORT_SYMBOL(raw_copy_in_user);
EXPORT_SYMBOL(memcpy);

long probe_kernel_read(void *dst, const void *src, size_t size)
{
	unsigned long addr = (unsigned long)src;

	if (addr < PAGE_SIZE)
		return -EFAULT;

	/* check for I/O space F_EXTEND(0xfff00000) access as well? */

	return __probe_kernel_read(dst, src, size);
=======
EXPORT_SYMBOL(memcpy);

bool copy_from_kernel_nofault_allowed(const void *unsafe_src, size_t size)
{
	if ((unsigned long)unsafe_src < PAGE_SIZE)
		return false;
	/* check for I/O space F_EXTEND(0xfff00000) access as well? */
	return true;
>>>>>>> upstream/android-13
}
