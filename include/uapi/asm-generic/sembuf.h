/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __ASM_GENERIC_SEMBUF_H
#define __ASM_GENERIC_SEMBUF_H

#include <asm/bitsperlong.h>
<<<<<<< HEAD

/*
 * The semid64_ds structure for x86 architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
=======
#include <asm/ipcbuf.h>

/*
 * The semid64_ds structure for most architectures (though it came from x86_32
 * originally). Note extra padding because this structure is passed back and
 * forth between kernel and user space.
>>>>>>> upstream/android-13
 *
 * semid64_ds was originally meant to be architecture specific, but
 * everyone just ended up making identical copies without specific
 * optimizations, so we may just as well all use the same one.
 *
<<<<<<< HEAD
 * 64 bit architectures use a 64-bit __kernel_time_t here, while
 * 32 bit architectures have a pair of unsigned long values.
 * so they do not need the first two padding words.
=======
 * 64 bit architectures use a 64-bit long time field here, while
 * 32 bit architectures have a pair of unsigned long values.
>>>>>>> upstream/android-13
 *
 * On big-endian systems, the padding is in the wrong place for
 * historic reasons, so user space has to reconstruct a time_t
 * value using
 *
 * user_semid_ds.sem_otime = kernel_semid64_ds.sem_otime +
 *		((long long)kernel_semid64_ds.sem_otime_high << 32)
 *
 * Pad space is left for 2 miscellaneous 32-bit values
 */
struct semid64_ds {
	struct ipc64_perm sem_perm;	/* permissions .. see ipc.h */
#if __BITS_PER_LONG == 64
<<<<<<< HEAD
	__kernel_time_t	sem_otime;	/* last semop time */
	__kernel_time_t	sem_ctime;	/* last change time */
=======
	long		sem_otime;	/* last semop time */
	long		sem_ctime;	/* last change time */
>>>>>>> upstream/android-13
#else
	unsigned long	sem_otime;	/* last semop time */
	unsigned long	sem_otime_high;
	unsigned long	sem_ctime;	/* last change time */
	unsigned long	sem_ctime_high;
#endif
	unsigned long	sem_nsems;	/* no. of semaphores in array */
	unsigned long	__unused3;
	unsigned long	__unused4;
};

#endif /* __ASM_GENERIC_SEMBUF_H */
