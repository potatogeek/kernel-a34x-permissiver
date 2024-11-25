<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
// Copyright (C) 2005-2017 Andes Technology Corporation

#ifndef __ASM_NDS32_SYSCALLS_H
#define __ASM_NDS32_SYSCALLS_H

asmlinkage long sys_cacheflush(unsigned long addr, unsigned long len, unsigned int op);
asmlinkage long sys_fadvise64_64_wrapper(int fd, int advice, loff_t offset, loff_t len);
asmlinkage long sys_rt_sigreturn_wrapper(void);
<<<<<<< HEAD
=======
asmlinkage long sys_fp_udfiex_crtl(int cmd, int act);
>>>>>>> upstream/android-13

#include <asm-generic/syscalls.h>

#endif /* __ASM_NDS32_SYSCALLS_H */
