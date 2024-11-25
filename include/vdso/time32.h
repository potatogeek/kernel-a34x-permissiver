/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __VDSO_TIME32_H
#define __VDSO_TIME32_H

<<<<<<< HEAD
/*
 * New aliases for compat time functions. These will be used to replace
 * the compat code so it can be shared between 32-bit and 64-bit builds
 * both of which provide compatibility with old 32-bit tasks.
 */
#define old_time32_t		compat_time_t
#define old_timeval32		compat_timeval
#define old_timespec32		compat_timespec
=======
typedef s32		old_time32_t;

struct old_timespec32 {
	old_time32_t	tv_sec;
	s32		tv_nsec;
};

struct old_timeval32 {
	old_time32_t	tv_sec;
	s32		tv_usec;
};
>>>>>>> upstream/android-13

#endif /* __VDSO_TIME32_H */
