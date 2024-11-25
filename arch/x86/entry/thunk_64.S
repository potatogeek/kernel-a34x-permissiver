<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Save registers before calling assembly functions. This avoids
 * disturbance of register allocation in some inline assembly constructs.
 * Copyright 2001,2002 by Andi Kleen, SuSE Labs.
<<<<<<< HEAD
 * Added trace_hardirqs callers - Copyright 2007 Steven Rostedt, Red Hat, Inc.
 * Subject to the GNU public license, v.2. No warranty of any kind.
=======
>>>>>>> upstream/android-13
 */
#include <linux/linkage.h>
#include "calling.h"
#include <asm/asm.h>
#include <asm/export.h>

	/* rdi:	arg1 ... normal C conventions. rax is saved/restored. */
<<<<<<< HEAD
	.macro THUNK name, func, put_ret_addr_in_rdi=0
	.globl \name
	.type \name, @function
\name:
=======
	.macro THUNK name, func
SYM_FUNC_START_NOALIGN(\name)
>>>>>>> upstream/android-13
	pushq %rbp
	movq %rsp, %rbp

	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %rcx
	pushq %rax
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11

<<<<<<< HEAD
	.if \put_ret_addr_in_rdi
	/* 8(%rbp) is return addr on stack */
	movq 8(%rbp), %rdi
	.endif

	call \func
	jmp  .L_restore
	_ASM_NOKPROBE(\name)
	.endm

#ifdef CONFIG_TRACE_IRQFLAGS
	THUNK trace_hardirqs_on_thunk,trace_hardirqs_on_caller,1
	THUNK trace_hardirqs_off_thunk,trace_hardirqs_off_caller,1
#endif

#ifdef CONFIG_DEBUG_LOCK_ALLOC
	THUNK lockdep_sys_exit_thunk,lockdep_sys_exit
#endif

#ifdef CONFIG_PREEMPT
	THUNK ___preempt_schedule, preempt_schedule
	THUNK ___preempt_schedule_notrace, preempt_schedule_notrace
	EXPORT_SYMBOL(___preempt_schedule)
	EXPORT_SYMBOL(___preempt_schedule_notrace)
#endif

#if defined(CONFIG_TRACE_IRQFLAGS) \
 || defined(CONFIG_DEBUG_LOCK_ALLOC) \
 || defined(CONFIG_PREEMPT)
.L_restore:
=======
	call \func
	jmp  __thunk_restore
SYM_FUNC_END(\name)
	_ASM_NOKPROBE(\name)
	.endm

#ifdef CONFIG_PREEMPTION
	THUNK preempt_schedule_thunk, preempt_schedule
	THUNK preempt_schedule_notrace_thunk, preempt_schedule_notrace
	EXPORT_SYMBOL(preempt_schedule_thunk)
	EXPORT_SYMBOL(preempt_schedule_notrace_thunk)
#endif

#ifdef CONFIG_PREEMPTION
SYM_CODE_START_LOCAL_NOALIGN(__thunk_restore)
>>>>>>> upstream/android-13
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rax
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rbp
<<<<<<< HEAD
	ret
	_ASM_NOKPROBE(.L_restore)
=======
	RET
	_ASM_NOKPROBE(__thunk_restore)
SYM_CODE_END(__thunk_restore)
>>>>>>> upstream/android-13
#endif
