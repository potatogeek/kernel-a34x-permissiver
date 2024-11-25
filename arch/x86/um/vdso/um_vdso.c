<<<<<<< HEAD
/*
 * Copyright (C) 2011 Richard Weinberger <richrd@nod.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Richard Weinberger <richrd@nod.at>
 *
>>>>>>> upstream/android-13
 * This vDSO turns all calls into a syscall so that UML can trap them.
 */


/* Disable profiling for userspace code */
#define DISABLE_BRANCH_PROFILING

#include <linux/time.h>
#include <linux/getcpu.h>
#include <asm/unistd.h>

<<<<<<< HEAD
int __vdso_clock_gettime(clockid_t clock, struct timespec *ts)
=======
int __vdso_clock_gettime(clockid_t clock, struct __kernel_old_timespec *ts)
>>>>>>> upstream/android-13
{
	long ret;

	asm("syscall" : "=a" (ret) :
		"0" (__NR_clock_gettime), "D" (clock), "S" (ts) : "memory");

	return ret;
}
<<<<<<< HEAD
int clock_gettime(clockid_t, struct timespec *)
	__attribute__((weak, alias("__vdso_clock_gettime")));

int __vdso_gettimeofday(struct timeval *tv, struct timezone *tz)
=======
int clock_gettime(clockid_t, struct __kernel_old_timespec *)
	__attribute__((weak, alias("__vdso_clock_gettime")));

int __vdso_gettimeofday(struct __kernel_old_timeval *tv, struct timezone *tz)
>>>>>>> upstream/android-13
{
	long ret;

	asm("syscall" : "=a" (ret) :
		"0" (__NR_gettimeofday), "D" (tv), "S" (tz) : "memory");

	return ret;
}
<<<<<<< HEAD
int gettimeofday(struct timeval *, struct timezone *)
	__attribute__((weak, alias("__vdso_gettimeofday")));

time_t __vdso_time(time_t *t)
=======
int gettimeofday(struct __kernel_old_timeval *, struct timezone *)
	__attribute__((weak, alias("__vdso_gettimeofday")));

__kernel_old_time_t __vdso_time(__kernel_old_time_t *t)
>>>>>>> upstream/android-13
{
	long secs;

	asm volatile("syscall"
		: "=a" (secs)
		: "0" (__NR_time), "D" (t) : "cc", "r11", "cx", "memory");

	return secs;
}
<<<<<<< HEAD
int time(time_t *t) __attribute__((weak, alias("__vdso_time")));
=======
__kernel_old_time_t time(__kernel_old_time_t *t) __attribute__((weak, alias("__vdso_time")));
>>>>>>> upstream/android-13

long
__vdso_getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *unused)
{
	/*
	 * UML does not support SMP, we can cheat here. :)
	 */

	if (cpu)
		*cpu = 0;
	if (node)
		*node = 0;

	return 0;
}

long getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache)
	__attribute__((weak, alias("__vdso_getcpu")));
