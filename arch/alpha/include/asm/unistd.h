/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ALPHA_UNISTD_H
#define _ALPHA_UNISTD_H

#include <uapi/asm/unistd.h>

<<<<<<< HEAD
#define NR_SYSCALLS			523

=======
#define NR_SYSCALLS	__NR_syscalls

#define __ARCH_WANT_NEW_STAT
>>>>>>> upstream/android-13
#define __ARCH_WANT_OLD_READDIR
#define __ARCH_WANT_STAT64
#define __ARCH_WANT_SYS_GETHOSTNAME
#define __ARCH_WANT_SYS_FADVISE64
#define __ARCH_WANT_SYS_GETPGRP
#define __ARCH_WANT_SYS_OLDUMOUNT
#define __ARCH_WANT_SYS_SIGPENDING
<<<<<<< HEAD
=======
#define __ARCH_WANT_SYS_UTIME
>>>>>>> upstream/android-13
#define __ARCH_WANT_SYS_FORK
#define __ARCH_WANT_SYS_VFORK
#define __ARCH_WANT_SYS_CLONE

#endif /* _ALPHA_UNISTD_H */
