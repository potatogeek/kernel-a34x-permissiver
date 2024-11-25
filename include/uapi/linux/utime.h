/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_UTIME_H
#define _LINUX_UTIME_H

#include <linux/types.h>

struct utimbuf {
<<<<<<< HEAD
	__kernel_time_t actime;
	__kernel_time_t modtime;
=======
	__kernel_old_time_t actime;
	__kernel_old_time_t modtime;
>>>>>>> upstream/android-13
};

#endif
