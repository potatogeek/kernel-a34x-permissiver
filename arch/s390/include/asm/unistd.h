/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *
 *  Derived from "include/asm-i386/unistd.h"
 */
#ifndef _ASM_S390_UNISTD_H_
#define _ASM_S390_UNISTD_H_

#include <uapi/asm/unistd.h>
#include <asm/unistd_nr.h>

<<<<<<< HEAD
#define __IGNORE_time
#define __IGNORE_pkey_mprotect
#define __IGNORE_pkey_alloc
#define __IGNORE_pkey_free

=======
#define __ARCH_WANT_NEW_STAT
>>>>>>> upstream/android-13
#define __ARCH_WANT_OLD_READDIR
#define __ARCH_WANT_SYS_ALARM
#define __ARCH_WANT_SYS_GETHOSTNAME
#define __ARCH_WANT_SYS_PAUSE
#define __ARCH_WANT_SYS_SIGNAL
#define __ARCH_WANT_SYS_UTIME
#define __ARCH_WANT_SYS_SOCKETCALL
#define __ARCH_WANT_SYS_IPC
#define __ARCH_WANT_SYS_FADVISE64
#define __ARCH_WANT_SYS_GETPGRP
<<<<<<< HEAD
#define __ARCH_WANT_SYS_LLSEEK
=======
>>>>>>> upstream/android-13
#define __ARCH_WANT_SYS_NICE
#define __ARCH_WANT_SYS_OLD_GETRLIMIT
#define __ARCH_WANT_SYS_OLD_MMAP
#define __ARCH_WANT_SYS_OLDUMOUNT
#define __ARCH_WANT_SYS_SIGPENDING
#define __ARCH_WANT_SYS_SIGPROCMASK
# ifdef CONFIG_COMPAT
<<<<<<< HEAD
#   define __ARCH_WANT_COMPAT_SYS_TIME
=======
#   define __ARCH_WANT_SYS_TIME32
#   define __ARCH_WANT_SYS_UTIME32
>>>>>>> upstream/android-13
# endif
#define __ARCH_WANT_SYS_FORK
#define __ARCH_WANT_SYS_VFORK
#define __ARCH_WANT_SYS_CLONE
<<<<<<< HEAD
=======
#define __ARCH_WANT_SYS_CLONE3
>>>>>>> upstream/android-13

#endif /* _ASM_S390_UNISTD_H_ */
