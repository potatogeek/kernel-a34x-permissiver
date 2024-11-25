/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SPARC64_SYSCALLS_H
#define _SPARC64_SYSCALLS_H

struct pt_regs;

<<<<<<< HEAD
asmlinkage long sparc_do_fork(unsigned long clone_flags,
			      unsigned long stack_start,
			      struct pt_regs *regs,
			      unsigned long stack_size);
=======
asmlinkage long sparc_fork(struct pt_regs *regs);
asmlinkage long sparc_vfork(struct pt_regs *regs);
asmlinkage long sparc_clone(struct pt_regs *regs);
>>>>>>> upstream/android-13

#endif /* _SPARC64_SYSCALLS_H */
