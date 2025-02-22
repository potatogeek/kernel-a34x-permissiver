<<<<<<< HEAD
/*
 * Copyright (C) 2018 Linaro Ltd <ard.biesheuvel@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018 Linaro Ltd <ard.biesheuvel@linaro.org>
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>

<<<<<<< HEAD
ENTRY(__efi_rt_asm_wrapper)
=======
SYM_FUNC_START(__efi_rt_asm_wrapper)
>>>>>>> upstream/android-13
	stp	x29, x30, [sp, #-32]!
	mov	x29, sp

	/*
	 * Register x18 is designated as the 'platform' register by the AAPCS,
	 * which means firmware running at the same exception level as the OS
	 * (such as UEFI) should never touch it.
	 */
	stp	x1, x18, [sp, #16]

	/*
	 * We are lucky enough that no EFI runtime services take more than
	 * 5 arguments, so all are passed in registers rather than via the
	 * stack.
	 */
	mov	x8, x0
	mov	x0, x2
	mov	x1, x3
	mov	x2, x4
	mov	x3, x5
	mov	x4, x6
	blr	x8

	ldp	x1, x2, [sp, #16]
	cmp	x2, x18
	ldp	x29, x30, [sp], #32
	b.ne	0f
	ret
0:
<<<<<<< HEAD
#ifdef CONFIG_SHADOW_CALL_STACK
	/*
	 * Restore x18 before returning to instrumented code. This is
	 * safe because the wrapper is called with preemption disabled and
	 * a separate shadow stack is used for interrupts.
	 */
	mov	x18, x2
#endif
	b	efi_handle_corrupted_x18	// tail call
ENDPROC(__efi_rt_asm_wrapper)
=======
	/*
	 * With CONFIG_SHADOW_CALL_STACK, the kernel uses x18 to store a
	 * shadow stack pointer, which we need to restore before returning to
	 * potentially instrumented code. This is safe because the wrapper is
	 * called with preemption disabled and a separate shadow stack is used
	 * for interrupts.
	 */
	mov	x18, x2
	b	efi_handle_corrupted_x18	// tail call
SYM_FUNC_END(__efi_rt_asm_wrapper)
>>>>>>> upstream/android-13
