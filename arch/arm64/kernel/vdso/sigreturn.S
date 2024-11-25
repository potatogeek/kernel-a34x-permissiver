<<<<<<< HEAD
/*
 * Sigreturn trampoline for returning from a signal when the SA_RESTORER
 * flag is not set.
 *
 * Copyright (C) 2012 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Sigreturn trampoline for returning from a signal when the SA_RESTORER
 * flag is not set. It serves primarily as a hall of shame for crappy
 * unwinders and features an exciting but mysterious NOP instruction.
 *
 * It's also fragile as hell, so please think twice before changing anything
 * in here.
 *
 * Copyright (C) 2012 ARM Limited
 *
>>>>>>> upstream/android-13
 * Author: Will Deacon <will.deacon@arm.com>
 */

#include <linux/linkage.h>
<<<<<<< HEAD
=======
#include <asm/assembler.h>
>>>>>>> upstream/android-13
#include <asm/unistd.h>

	.text

<<<<<<< HEAD
	nop
ENTRY(__kernel_rt_sigreturn)
	.cfi_startproc
	.cfi_signal_frame
	.cfi_def_cfa	x29, 0
	.cfi_offset	x29, 0 * 8
	.cfi_offset	x30, 1 * 8
	mov	x8, #__NR_rt_sigreturn
	svc	#0
	.cfi_endproc
ENDPROC(__kernel_rt_sigreturn)
=======
/*
 * NOTE!!!  You may notice that all of the .cfi directives in this file have
 * been commented out. This is because they have been shown to trigger segfaults
 * in libgcc when unwinding out of a SIGCANCEL handler to invoke pthread
 * cleanup handlers during the thread cancellation dance. By omitting the
 * directives, we trigger an arm64-specific fallback path in the unwinder which
 * recognises the signal frame and restores many of the registers directly from
 * the sigcontext. Re-enabling the cfi directives here therefore needs to be
 * much more comprehensive to reduce the risk of further regressions.
 */

/* Ensure that the mysterious NOP can be associated with a function. */
//	.cfi_startproc

/*
 * .cfi_signal_frame causes the corresponding Frame Description Entry (FDE) in
 * the .eh_frame section to be annotated as a signal frame. This allows DWARF
 * unwinders (e.g. libstdc++) to implement _Unwind_GetIPInfo() and identify
 * the next frame using the unmodified return address instead of subtracting 1,
 * which may yield the wrong FDE.
 */
//	.cfi_signal_frame

/*
 * Tell the unwinder where to locate the frame record linking back to the
 * interrupted context. We don't provide unwind info for registers other than
 * the frame pointer and the link register here; in practice, this is likely to
 * be insufficient for unwinding in C/C++ based runtimes, especially without a
 * means to restore the stack pointer. Thankfully, unwinders and debuggers
 * already have baked-in strategies for attempting to unwind out of signals.
 */
//	.cfi_def_cfa    x29, 0
//	.cfi_offset     x29, 0 * 8
//	.cfi_offset     x30, 1 * 8

/*
 * This mysterious NOP is required for some unwinders (e.g. libc++) that
 * unconditionally subtract one from the result of _Unwind_GetIP() in order to
 * identify the calling function.
 * Hack borrowed from arch/powerpc/kernel/vdso64/sigtramp.S.
 */
	nop	// Mysterious NOP

/*
 * GDB, libgcc and libunwind rely on being able to identify the sigreturn
 * instruction sequence to unwind from signal handlers. We cannot, therefore,
 * use SYM_FUNC_START() here, as it will emit a BTI C instruction and break the
 * unwinder. Thankfully, this function is only ever called from a RET and so
 * omitting the landing pad is perfectly fine.
 */
SYM_CODE_START(__kernel_rt_sigreturn)
//	PLEASE DO NOT MODIFY
	mov	x8, #__NR_rt_sigreturn
//	PLEASE DO NOT MODIFY
	svc	#0
//	PLEASE DO NOT MODIFY
//	.cfi_endproc
SYM_CODE_END(__kernel_rt_sigreturn)

emit_aarch64_feature_1_and
>>>>>>> upstream/android-13
