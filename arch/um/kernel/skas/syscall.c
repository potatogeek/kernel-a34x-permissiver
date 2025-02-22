<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/seccomp.h>
#include <kern_util.h>
#include <sysdep/ptrace.h>
#include <sysdep/ptrace_user.h>
#include <sysdep/syscalls.h>
<<<<<<< HEAD
=======
#include <linux/time-internal.h>
#include <asm/unistd.h>
>>>>>>> upstream/android-13

void handle_syscall(struct uml_pt_regs *r)
{
	struct pt_regs *regs = container_of(r, struct pt_regs, regs);
	int syscall;

<<<<<<< HEAD
=======
	/*
	 * If we have infinite CPU resources, then make every syscall also a
	 * preemption point, since we don't have any other preemption in this
	 * case, and kernel threads would basically never run until userspace
	 * went to sleep, even if said userspace interacts with the kernel in
	 * various ways.
	 */
	if (time_travel_mode == TT_MODE_INFCPU ||
	    time_travel_mode == TT_MODE_EXTERNAL)
		schedule();

>>>>>>> upstream/android-13
	/* Initialize the syscall number and default return value. */
	UPT_SYSCALL_NR(r) = PT_SYSCALL_NR(r->gp);
	PT_REGS_SET_SYSCALL_RETURN(regs, -ENOSYS);

	if (syscall_trace_enter(regs))
		goto out;

	/* Do the seccomp check after ptrace; failures should be fast. */
<<<<<<< HEAD
	if (secure_computing(NULL) == -1)
		goto out;

	syscall = UPT_SYSCALL_NR(r);
	if (syscall >= 0 && syscall <= __NR_syscall_max)
=======
	if (secure_computing() == -1)
		goto out;

	syscall = UPT_SYSCALL_NR(r);
	if (syscall >= 0 && syscall < __NR_syscalls)
>>>>>>> upstream/android-13
		PT_REGS_SET_SYSCALL_RETURN(regs,
				EXECUTE_SYSCALL(syscall, regs));

out:
	syscall_trace_leave(regs);
}
