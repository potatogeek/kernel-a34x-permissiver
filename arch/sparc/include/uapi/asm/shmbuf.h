/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _SPARC_SHMBUF_H
#define _SPARC_SHMBUF_H

/* 
 * The shmid64_ds structure for sparc architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 2 miscellaneous 32-bit values
 */

struct shmid64_ds {
	struct ipc64_perm	shm_perm;	/* operation perms */
#if defined(__sparc__) && defined(__arch64__)
<<<<<<< HEAD
	__kernel_time_t		shm_atime;	/* last attach time */
	__kernel_time_t		shm_dtime;	/* last detach time */
	__kernel_time_t		shm_ctime;	/* last change time */
=======
	long			shm_atime;	/* last attach time */
	long			shm_dtime;	/* last detach time */
	long			shm_ctime;	/* last change time */
>>>>>>> upstream/android-13
#else
	unsigned long		shm_atime_high;
	unsigned long		shm_atime;	/* last attach time */
	unsigned long		shm_dtime_high;
	unsigned long		shm_dtime;	/* last detach time */
	unsigned long		shm_ctime_high;
	unsigned long		shm_ctime;	/* last change time */
#endif
	size_t			shm_segsz;	/* size of segment (bytes) */
	__kernel_pid_t		shm_cpid;	/* pid of creator */
	__kernel_pid_t		shm_lpid;	/* pid of last operator */
	unsigned long		shm_nattch;	/* no. of current attaches */
	unsigned long		__unused1;
	unsigned long		__unused2;
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

#endif /* _SPARC_SHMBUF_H */
