<<<<<<< HEAD
/*
 * Copyright (C) 2013 Richard Weinberger <richrd@nod.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Richard Weinberger <richrd@nod.at>
>>>>>>> upstream/android-13
 */

#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <os.h>

<<<<<<< HEAD
long probe_kernel_read(void *dst, const void *src, size_t size)
=======
bool copy_from_kernel_nofault_allowed(const void *src, size_t size)
>>>>>>> upstream/android-13
{
	void *psrc = (void *)rounddown((unsigned long)src, PAGE_SIZE);

	if ((unsigned long)src < PAGE_SIZE || size <= 0)
<<<<<<< HEAD
		return -EFAULT;

	if (os_mincore(psrc, size + src - psrc) <= 0)
		return -EFAULT;

	return __probe_kernel_read(dst, src, size);
=======
		return false;
	if (os_mincore(psrc, size + src - psrc) <= 0)
		return false;
	return true;
>>>>>>> upstream/android-13
}
