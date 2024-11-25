/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_SYSCALLS_H
#define __ASM_SH_SYSCALLS_H

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> upstream/android-13
asmlinkage int old_mmap(unsigned long addr, unsigned long len,
			unsigned long prot, unsigned long flags,
			int fd, unsigned long off);
asmlinkage long sys_mmap2(unsigned long addr, unsigned long len,
			  unsigned long prot, unsigned long flags,
			  unsigned long fd, unsigned long pgoff);

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
# include <asm/syscalls_32.h>
#else
# include <asm/syscalls_64.h>
#endif

#endif /* __KERNEL__ */
=======
#include <asm/syscalls_32.h>

>>>>>>> upstream/android-13
#endif /* __ASM_SH_SYSCALLS_H */
