/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _XTENSA_UNISTD_H
#define _XTENSA_UNISTD_H

#define __ARCH_WANT_SYS_CLONE
<<<<<<< HEAD
#include <uapi/asm/unistd.h>

#define __ARCH_WANT_STAT64
#define __ARCH_WANT_SYS_UTIME
#define __ARCH_WANT_SYS_LLSEEK
#define __ARCH_WANT_SYS_GETPGRP

/* 
 * Ignore legacy system calls in the checksyscalls.sh script
 */

#define __IGNORE_fork				/* use clone */
#define __IGNORE_time
#define __IGNORE_alarm				/* use setitimer */
#define __IGNORE_pause
#define __IGNORE_mmap				/* use mmap2 */
#define __IGNORE_vfork				/* use clone */
#define __IGNORE_fadvise64			/* use fadvise64_64 */
=======
#define __ARCH_WANT_SYS_CLONE3
#include <uapi/asm/unistd.h>

#define __ARCH_WANT_NEW_STAT
#define __ARCH_WANT_STAT64
#define __ARCH_WANT_SYS_UTIME32
#define __ARCH_WANT_SYS_GETPGRP

#define NR_syscalls				__NR_syscalls
>>>>>>> upstream/android-13

#endif /* _XTENSA_UNISTD_H */
