<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Dummy stubs used when CONFIG_POSIX_TIMERS=n
 *
 * Created by:  Nicolas Pitre, July 2016
 * Copyright:   (C) 2016 Linaro Limited
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/posix-timers.h>
<<<<<<< HEAD
=======
#include <linux/time_namespace.h>
>>>>>>> upstream/android-13
#include <linux/compat.h>

#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
/* Architectures may override SYS_NI and COMPAT_SYS_NI */
#include <asm/syscall_wrapper.h>
#endif

asmlinkage long sys_ni_posix_timers(void)
{
	pr_err_once("process %d (%s) attempted a POSIX timer syscall "
		    "while CONFIG_POSIX_TIMERS is not set\n",
		    current->pid, current->comm);
	return -ENOSYS;
}

#ifndef SYS_NI
#define SYS_NI(name)  SYSCALL_ALIAS(sys_##name, sys_ni_posix_timers)
#endif

#ifndef COMPAT_SYS_NI
#define COMPAT_SYS_NI(name)  SYSCALL_ALIAS(compat_sys_##name, sys_ni_posix_timers)
#endif

SYS_NI(timer_create);
SYS_NI(timer_gettime);
SYS_NI(timer_getoverrun);
SYS_NI(timer_settime);
SYS_NI(timer_delete);
SYS_NI(clock_adjtime);
SYS_NI(getitimer);
SYS_NI(setitimer);
<<<<<<< HEAD
=======
SYS_NI(clock_adjtime32);
>>>>>>> upstream/android-13
#ifdef __ARCH_WANT_SYS_ALARM
SYS_NI(alarm);
#endif

/*
 * We preserve minimal support for CLOCK_REALTIME and CLOCK_MONOTONIC
 * as it is easy to remain compatible with little code. CLOCK_BOOTTIME
 * is also included for convenience as at least systemd uses it.
 */

SYSCALL_DEFINE2(clock_settime, const clockid_t, which_clock,
		const struct __kernel_timespec __user *, tp)
{
	struct timespec64 new_tp;

	if (which_clock != CLOCK_REALTIME)
		return -EINVAL;
	if (get_timespec64(&new_tp, tp))
		return -EFAULT;

	return do_sys_settimeofday64(&new_tp, NULL);
}

int do_clock_gettime(clockid_t which_clock, struct timespec64 *tp)
{
	switch (which_clock) {
	case CLOCK_REALTIME:
		ktime_get_real_ts64(tp);
		break;
	case CLOCK_MONOTONIC:
		ktime_get_ts64(tp);
<<<<<<< HEAD
		break;
	case CLOCK_BOOTTIME:
		ktime_get_boottime_ts64(tp);
=======
		timens_add_monotonic(tp);
		break;
	case CLOCK_BOOTTIME:
		ktime_get_boottime_ts64(tp);
		timens_add_boottime(tp);
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
SYSCALL_DEFINE2(clock_gettime, const clockid_t, which_clock,
		struct __kernel_timespec __user *, tp)
{
	int ret;
	struct timespec64 kernel_tp;

	ret = do_clock_gettime(which_clock, &kernel_tp);
	if (ret)
		return ret;

	if (put_timespec64(&kernel_tp, tp))
		return -EFAULT;
	return 0;
}

SYSCALL_DEFINE2(clock_getres, const clockid_t, which_clock, struct __kernel_timespec __user *, tp)
{
	struct timespec64 rtn_tp = {
		.tv_sec = 0,
		.tv_nsec = hrtimer_resolution,
	};

	switch (which_clock) {
	case CLOCK_REALTIME:
	case CLOCK_MONOTONIC:
	case CLOCK_BOOTTIME:
		if (put_timespec64(&rtn_tp, tp))
			return -EFAULT;
		return 0;
	default:
		return -EINVAL;
	}
}

SYSCALL_DEFINE4(clock_nanosleep, const clockid_t, which_clock, int, flags,
		const struct __kernel_timespec __user *, rqtp,
		struct __kernel_timespec __user *, rmtp)
{
	struct timespec64 t;
<<<<<<< HEAD
=======
	ktime_t texp;
>>>>>>> upstream/android-13

	switch (which_clock) {
	case CLOCK_REALTIME:
	case CLOCK_MONOTONIC:
	case CLOCK_BOOTTIME:
		break;
	default:
		return -EINVAL;
	}

	if (get_timespec64(&t, rqtp))
		return -EFAULT;
	if (!timespec64_valid(&t))
		return -EINVAL;
	if (flags & TIMER_ABSTIME)
		rmtp = NULL;
	current->restart_block.nanosleep.type = rmtp ? TT_NATIVE : TT_NONE;
	current->restart_block.nanosleep.rmtp = rmtp;
<<<<<<< HEAD
	return hrtimer_nanosleep(&t, flags & TIMER_ABSTIME ?
=======
	texp = timespec64_to_ktime(t);
	if (flags & TIMER_ABSTIME)
		texp = timens_ktime_to_host(which_clock, texp);
	return hrtimer_nanosleep(texp, flags & TIMER_ABSTIME ?
>>>>>>> upstream/android-13
				 HRTIMER_MODE_ABS : HRTIMER_MODE_REL,
				 which_clock);
}

#ifdef CONFIG_COMPAT
COMPAT_SYS_NI(timer_create);
<<<<<<< HEAD
COMPAT_SYS_NI(clock_adjtime);
COMPAT_SYS_NI(timer_settime);
COMPAT_SYS_NI(timer_gettime);
=======
#endif

#if defined(CONFIG_COMPAT) || defined(CONFIG_ALPHA)
>>>>>>> upstream/android-13
COMPAT_SYS_NI(getitimer);
COMPAT_SYS_NI(setitimer);
#endif

#ifdef CONFIG_COMPAT_32BIT_TIME
<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE2(clock_settime, const clockid_t, which_clock,
		       struct compat_timespec __user *, tp)
=======
SYS_NI(timer_settime32);
SYS_NI(timer_gettime32);

SYSCALL_DEFINE2(clock_settime32, const clockid_t, which_clock,
		struct old_timespec32 __user *, tp)
>>>>>>> upstream/android-13
{
	struct timespec64 new_tp;

	if (which_clock != CLOCK_REALTIME)
		return -EINVAL;
<<<<<<< HEAD
	if (compat_get_timespec64(&new_tp, tp))
=======
	if (get_old_timespec32(&new_tp, tp))
>>>>>>> upstream/android-13
		return -EFAULT;

	return do_sys_settimeofday64(&new_tp, NULL);
}

<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE2(clock_gettime, clockid_t, which_clock,
		       struct compat_timespec __user *, tp)
=======
SYSCALL_DEFINE2(clock_gettime32, clockid_t, which_clock,
		struct old_timespec32 __user *, tp)
>>>>>>> upstream/android-13
{
	int ret;
	struct timespec64 kernel_tp;

	ret = do_clock_gettime(which_clock, &kernel_tp);
	if (ret)
		return ret;

<<<<<<< HEAD
	if (compat_put_timespec64(&kernel_tp, tp))
=======
	if (put_old_timespec32(&kernel_tp, tp))
>>>>>>> upstream/android-13
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE2(clock_getres, clockid_t, which_clock,
		       struct compat_timespec __user *, tp)
=======
SYSCALL_DEFINE2(clock_getres_time32, clockid_t, which_clock,
		struct old_timespec32 __user *, tp)
>>>>>>> upstream/android-13
{
	struct timespec64 rtn_tp = {
		.tv_sec = 0,
		.tv_nsec = hrtimer_resolution,
	};

	switch (which_clock) {
	case CLOCK_REALTIME:
	case CLOCK_MONOTONIC:
	case CLOCK_BOOTTIME:
<<<<<<< HEAD
		if (compat_put_timespec64(&rtn_tp, tp))
=======
		if (put_old_timespec32(&rtn_tp, tp))
>>>>>>> upstream/android-13
			return -EFAULT;
		return 0;
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
COMPAT_SYSCALL_DEFINE4(clock_nanosleep, clockid_t, which_clock, int, flags,
		       struct compat_timespec __user *, rqtp,
		       struct compat_timespec __user *, rmtp)
{
	struct timespec64 t;
=======
SYSCALL_DEFINE4(clock_nanosleep_time32, clockid_t, which_clock, int, flags,
		struct old_timespec32 __user *, rqtp,
		struct old_timespec32 __user *, rmtp)
{
	struct timespec64 t;
	ktime_t texp;
>>>>>>> upstream/android-13

	switch (which_clock) {
	case CLOCK_REALTIME:
	case CLOCK_MONOTONIC:
	case CLOCK_BOOTTIME:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (compat_get_timespec64(&t, rqtp))
=======
	if (get_old_timespec32(&t, rqtp))
>>>>>>> upstream/android-13
		return -EFAULT;
	if (!timespec64_valid(&t))
		return -EINVAL;
	if (flags & TIMER_ABSTIME)
		rmtp = NULL;
	current->restart_block.nanosleep.type = rmtp ? TT_COMPAT : TT_NONE;
	current->restart_block.nanosleep.compat_rmtp = rmtp;
<<<<<<< HEAD
	return hrtimer_nanosleep(&t, flags & TIMER_ABSTIME ?
=======
	texp = timespec64_to_ktime(t);
	if (flags & TIMER_ABSTIME)
		texp = timens_ktime_to_host(which_clock, texp);
	return hrtimer_nanosleep(texp, flags & TIMER_ABSTIME ?
>>>>>>> upstream/android-13
				 HRTIMER_MODE_ABS : HRTIMER_MODE_REL,
				 which_clock);
}
#endif
