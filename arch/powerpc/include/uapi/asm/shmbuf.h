/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
#ifndef _ASM_POWERPC_SHMBUF_H
#define _ASM_POWERPC_SHMBUF_H

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

/*
 * The shmid64_ds structure for PPC architecture.
 *
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 2 miscellaneous 32-bit values
 */

struct shmid64_ds {
	struct ipc64_perm	shm_perm;	/* operation perms */
#ifdef __powerpc64__
<<<<<<< HEAD
	__kernel_time_t		shm_atime;	/* last attach time */
	__kernel_time_t		shm_dtime;	/* last detach time */
	__kernel_time_t		shm_ctime;	/* last change time */
=======
	long		shm_atime;	/* last attach time */
	long		shm_dtime;	/* last detach time */
	long		shm_ctime;	/* last change time */
>>>>>>> upstream/android-13
#else
	unsigned long		shm_atime_high;
	unsigned long		shm_atime;	/* last attach time */
	unsigned long		shm_dtime_high;
	unsigned long		shm_dtime;	/* last detach time */
	unsigned long		shm_ctime_high;
	unsigned long		shm_ctime;	/* last change time */
	unsigned long		__unused4;
#endif
	size_t			shm_segsz;	/* size of segment (bytes) */
	__kernel_pid_t		shm_cpid;	/* pid of creator */
	__kernel_pid_t		shm_lpid;	/* pid of last operator */
	unsigned long		shm_nattch;	/* no. of current attaches */
	unsigned long		__unused5;
	unsigned long		__unused6;
};

struct shminfo64 {
	unsigned long	shmmax;
	unsigned long	shmmin;
	unsigned long	shmmni;
	unsigned long	shmseg;
	unsigned long	shmall;
	unsigned long	__unused1;
	unsigned long	__unused2;
	unsigned long	__unused3;
	unsigned long	__unused4;
};

#endif	/* _ASM_POWERPC_SHMBUF_H */
