<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm64/include/asm/seccomp.h
 *
 * Copyright (C) 2014 Linaro Limited
 * Author: AKASHI Takahiro <takahiro.akashi@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASM_SECCOMP_H
#define _ASM_SECCOMP_H

#include <asm/unistd.h>

#ifdef CONFIG_COMPAT
#define __NR_seccomp_read_32		__NR_compat_read
#define __NR_seccomp_write_32		__NR_compat_write
#define __NR_seccomp_exit_32		__NR_compat_exit
#define __NR_seccomp_sigreturn_32	__NR_compat_rt_sigreturn
#endif /* CONFIG_COMPAT */

#include <asm-generic/seccomp.h>

<<<<<<< HEAD
=======
#define SECCOMP_ARCH_NATIVE		AUDIT_ARCH_AARCH64
#define SECCOMP_ARCH_NATIVE_NR		NR_syscalls
#define SECCOMP_ARCH_NATIVE_NAME	"aarch64"
#ifdef CONFIG_COMPAT
# define SECCOMP_ARCH_COMPAT		AUDIT_ARCH_ARM
# define SECCOMP_ARCH_COMPAT_NR	__NR_compat_syscalls
# define SECCOMP_ARCH_COMPAT_NAME	"arm"
#endif

>>>>>>> upstream/android-13
#endif /* _ASM_SECCOMP_H */
