/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_SYSCALL_H
#define __ASM_SH_SYSCALL_H

extern const unsigned long sys_call_table[];

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
# include <asm/syscall_32.h>
#else
# include <asm/syscall_64.h>
#endif
=======
#include <asm/syscall_32.h>
>>>>>>> upstream/android-13

#endif /* __ASM_SH_SYSCALL_H */
