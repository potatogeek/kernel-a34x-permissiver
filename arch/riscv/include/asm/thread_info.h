<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2009 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
<<<<<<< HEAD
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_THREAD_INFO_H
#define _ASM_RISCV_THREAD_INFO_H

#include <asm/page.h>
#include <linux/const.h>

<<<<<<< HEAD
/* thread information allocation */
#ifdef CONFIG_64BIT
#define THREAD_SIZE_ORDER	(2)
#else
#define THREAD_SIZE_ORDER	(1)
#endif
#define THREAD_SIZE		(PAGE_SIZE << THREAD_SIZE_ORDER)

=======
#ifdef CONFIG_KASAN
#define KASAN_STACK_ORDER 1
#else
#define KASAN_STACK_ORDER 0
#endif

/* thread information allocation */
#ifdef CONFIG_64BIT
#define THREAD_SIZE_ORDER	(2 + KASAN_STACK_ORDER)
#else
#define THREAD_SIZE_ORDER	(1 + KASAN_STACK_ORDER)
#endif
#define THREAD_SIZE		(PAGE_SIZE << THREAD_SIZE_ORDER)

/*
 * By aligning VMAP'd stacks to 2 * THREAD_SIZE, we can detect overflow by
 * checking sp & (1 << THREAD_SHIFT), which we can do cheaply in the entry
 * assembly.
 */
#ifdef CONFIG_VMAP_STACK
#define THREAD_ALIGN            (2 * THREAD_SIZE)
#else
#define THREAD_ALIGN            THREAD_SIZE
#endif

#define THREAD_SHIFT            (PAGE_SHIFT + THREAD_SIZE_ORDER)
#define OVERFLOW_STACK_SIZE     SZ_4K
#define SHADOW_OVERFLOW_STACK_SIZE (1024)

>>>>>>> upstream/android-13
#ifndef __ASSEMBLY__

#include <asm/processor.h>
#include <asm/csr.h>

<<<<<<< HEAD
typedef unsigned long mm_segment_t;

=======
>>>>>>> upstream/android-13
/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - if the members of this struct changes, the assembly constants
 *   in asm-offsets.c must be updated accordingly
 * - thread_info is included in task_struct at an offset of 0.  This means that
 *   tp points to both thread_info and task_struct.
 */
struct thread_info {
	unsigned long		flags;		/* low level flags */
	int                     preempt_count;  /* 0=>preemptible, <0=>BUG */
<<<<<<< HEAD
	mm_segment_t		addr_limit;
=======
>>>>>>> upstream/android-13
	/*
	 * These stack pointers are overwritten on every system call or
	 * exception.  SP is also saved to the stack it can be recovered when
	 * overwritten.
	 */
	long			kernel_sp;	/* Kernel stack pointer */
	long			user_sp;	/* User stack pointer */
	int			cpu;
};

/*
 * macros/functions for gaining access to the thread information structure
 *
 * preempt_count needs to be 1 initially, until the scheduler is functional.
 */
#define INIT_THREAD_INFO(tsk)			\
{						\
	.flags		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
<<<<<<< HEAD
	.addr_limit	= KERNEL_DS,		\
=======
>>>>>>> upstream/android-13
}

#endif /* !__ASSEMBLY__ */

/*
 * thread information flags
 * - these are process state flags that various assembly files may need to
 *   access
 * - pending work-to-be-done flags are in lowest half-word
 * - other flags in upper half-word(s)
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_NOTIFY_RESUME	1	/* callback before returning to user */
#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */
#define TIF_RESTORE_SIGMASK	4	/* restore signal mask in do_signal() */
#define TIF_MEMDIE		5	/* is terminating due to OOM killer */
#define TIF_SYSCALL_TRACEPOINT  6       /* syscall tracepoint instrumentation */
<<<<<<< HEAD
=======
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing */
#define TIF_SECCOMP		8	/* syscall secure computing */
#define TIF_NOTIFY_SIGNAL	9	/* signal notifications exist */
#define TIF_UPROBE		10	/* uprobe breakpoint or singlestep */
>>>>>>> upstream/android-13

#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
<<<<<<< HEAD

#define _TIF_WORK_MASK \
	(_TIF_NOTIFY_RESUME | _TIF_SIGPENDING | _TIF_NEED_RESCHED)
=======
#define _TIF_SYSCALL_TRACEPOINT	(1 << TIF_SYSCALL_TRACEPOINT)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1 << TIF_SECCOMP)
#define _TIF_NOTIFY_SIGNAL	(1 << TIF_NOTIFY_SIGNAL)
#define _TIF_UPROBE		(1 << TIF_UPROBE)

#define _TIF_WORK_MASK \
	(_TIF_NOTIFY_RESUME | _TIF_SIGPENDING | _TIF_NEED_RESCHED | \
	 _TIF_NOTIFY_SIGNAL | _TIF_UPROBE)

#define _TIF_SYSCALL_WORK \
	(_TIF_SYSCALL_TRACE | _TIF_SYSCALL_TRACEPOINT | _TIF_SYSCALL_AUDIT | \
	 _TIF_SECCOMP)
>>>>>>> upstream/android-13

#endif /* _ASM_RISCV_THREAD_INFO_H */
