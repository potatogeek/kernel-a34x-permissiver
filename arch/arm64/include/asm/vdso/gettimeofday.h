/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2018 ARM Limited
 */
#ifndef __ASM_VDSO_GETTIMEOFDAY_H
#define __ASM_VDSO_GETTIMEOFDAY_H

#ifndef __ASSEMBLY__

<<<<<<< HEAD
#include <asm/unistd.h>

#define __VDSO_USE_SYSCALL		ULLONG_MAX

#define VDSO_HAS_CLOCK_GETRES		1

#define VDSO_HAS_TIME			1

=======
#include <asm/barrier.h>
#include <asm/unistd.h>

#define VDSO_HAS_CLOCK_GETRES		1

>>>>>>> upstream/android-13
static __always_inline
int gettimeofday_fallback(struct __kernel_old_timeval *_tv,
			  struct timezone *_tz)
{
	register struct timezone *tz asm("x1") = _tz;
	register struct __kernel_old_timeval *tv asm("x0") = _tv;
	register long ret asm ("x0");
	register long nr asm("x8") = __NR_gettimeofday;

	asm volatile(
	"       svc #0\n"
	: "=r" (ret)
	: "r" (tv), "r" (tz), "r" (nr)
	: "memory");

	return ret;
}

static __always_inline
long clock_gettime_fallback(clockid_t _clkid, struct __kernel_timespec *_ts)
{
	register struct __kernel_timespec *ts asm("x1") = _ts;
	register clockid_t clkid asm("x0") = _clkid;
	register long ret asm ("x0");
	register long nr asm("x8") = __NR_clock_gettime;

	asm volatile(
	"       svc #0\n"
	: "=r" (ret)
	: "r" (clkid), "r" (ts), "r" (nr)
	: "memory");

	return ret;
}

static __always_inline
int clock_getres_fallback(clockid_t _clkid, struct __kernel_timespec *_ts)
{
	register struct __kernel_timespec *ts asm("x1") = _ts;
	register clockid_t clkid asm("x0") = _clkid;
	register long ret asm ("x0");
	register long nr asm("x8") = __NR_clock_getres;

	asm volatile(
	"       svc #0\n"
	: "=r" (ret)
	: "r" (clkid), "r" (ts), "r" (nr)
	: "memory");

	return ret;
}

<<<<<<< HEAD
static __always_inline u64 __arch_get_hw_counter(s32 clock_mode)
=======
static __always_inline u64 __arch_get_hw_counter(s32 clock_mode,
						 const struct vdso_data *vd)
>>>>>>> upstream/android-13
{
	u64 res;

	/*
<<<<<<< HEAD
	 * clock_mode == 0 implies that vDSO are enabled otherwise
	 * fallback on syscall.
	 */
	if (clock_mode)
		return __VDSO_USE_SYSCALL;
=======
	 * Core checks for mode already, so this raced against a concurrent
	 * update. Return something. Core will do another round and then
	 * see the mode change and fallback to the syscall.
	 */
	if (clock_mode == VDSO_CLOCKMODE_NONE)
		return 0;
>>>>>>> upstream/android-13

	/*
	 * This isb() is required to prevent that the counter value
	 * is speculated.
	 */
	isb();
	asm volatile("mrs %0, cntvct_el0" : "=r" (res) :: "memory");
<<<<<<< HEAD
	/*
	 * This isb() is required to prevent that the seq lock is
	 * speculated.#
	 */
	isb();
=======
	arch_counter_enforce_ordering(res);
>>>>>>> upstream/android-13

	return res;
}

static __always_inline
const struct vdso_data *__arch_get_vdso_data(void)
{
	return _vdso_data;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_TIME_NS
static __always_inline
const struct vdso_data *__arch_get_timens_vdso_data(const struct vdso_data *vd)
{
	return _timens_data;
}
#endif

>>>>>>> upstream/android-13
#endif /* !__ASSEMBLY__ */

#endif /* __ASM_VDSO_GETTIMEOFDAY_H */
