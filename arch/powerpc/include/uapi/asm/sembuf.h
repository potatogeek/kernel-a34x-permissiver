/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
#ifndef _ASM_POWERPC_SEMBUF_H
#define _ASM_POWERPC_SEMBUF_H

<<<<<<< HEAD
=======
#include <asm/ipcbuf.h>

>>>>>>> upstream/android-13
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

/*
 * The semid64_ds structure for PPC architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 2 miscellaneous 32/64-bit values
 */

struct semid64_ds {
	struct ipc64_perm sem_perm;	/* permissions .. see ipc.h */
#ifndef __powerpc64__
	unsigned long	sem_otime_high;
	unsigned long	sem_otime;	/* last semop time */
	unsigned long	sem_ctime_high;
	unsigned long	sem_ctime;	/* last change time */
#else
<<<<<<< HEAD
	__kernel_time_t	sem_otime;	/* last semop time */
	__kernel_time_t	sem_ctime;	/* last change time */
=======
	long		sem_otime;	/* last semop time */
	long		sem_ctime;	/* last change time */
>>>>>>> upstream/android-13
#endif
	unsigned long	sem_nsems;	/* no. of semaphores in array */
	unsigned long	__unused3;
	unsigned long	__unused4;
};

#endif	/* _ASM_POWERPC_SEMBUF_H */
