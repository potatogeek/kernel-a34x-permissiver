<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/backtrace.S
 *
 *  Copyright (C) 1995, 1996 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 27/03/03 Ian Molton Clean up CONFIG_CPU
 *
=======
 * 27/03/03 Ian Molton Clean up CONFIG_CPU
>>>>>>> upstream/android-13
 */
#include <linux/kern_levels.h>
#include <linux/linkage.h>
#include <asm/assembler.h>
		.text

@ fp is 0 or stack frame

#define frame	r4
#define sv_fp	r5
#define sv_pc	r6
#define mask	r7
#define offset	r8
<<<<<<< HEAD
=======
#define loglvl	r9
>>>>>>> upstream/android-13

ENTRY(c_backtrace)

#if !defined(CONFIG_FRAME_POINTER) || !defined(CONFIG_PRINTK)
		ret	lr
ENDPROC(c_backtrace)
#else
<<<<<<< HEAD
		stmfd	sp!, {r4 - r8, lr}	@ Save an extra register so we have a location...
		movs	frame, r0		@ if frame pointer is zero
		beq	no_frame		@ we have no stack frames
=======
		stmfd	sp!, {r4 - r9, lr}	@ Save an extra register so we have a location...
		movs	frame, r0		@ if frame pointer is zero
		beq	no_frame		@ we have no stack frames
		mov	loglvl, r2
>>>>>>> upstream/android-13

		tst	r1, #0x10		@ 26 or 32-bit mode?
 ARM(		moveq	mask, #0xfc000003	)
 THUMB(		moveq	mask, #0xfc000000	)
 THUMB(		orreq	mask, #0x03		)
		movne	mask, #0		@ mask for 32-bit

1:		stmfd	sp!, {pc}		@ calculate offset of PC stored
		ldr	r0, [sp], #4		@ by stmfd for this CPU
		adr	r1, 1b
		sub	offset, r0, r1

/*
 * Stack frame layout:
 *             optionally saved caller registers (r4 - r10)
 *             saved fp
 *             saved sp
 *             saved lr
 *    frame => saved pc
 *             optionally saved arguments (r0 - r3)
 * saved sp => <next word>
 *
 * Functions start with the following code sequence:
 *                  mov   ip, sp
 *                  stmfd sp!, {r0 - r3} (optional)
 * corrected pc =>  stmfd sp!, {..., fp, ip, lr, pc}
 */
for_each_frame:	tst	frame, mask		@ Check for address exceptions
		bne	no_frame

1001:		ldr	sv_pc, [frame, #0]	@ get saved pc
1002:		ldr	sv_fp, [frame, #-12]	@ get saved fp

		sub	sv_pc, sv_pc, offset	@ Correct PC for prefetching
		bic	sv_pc, sv_pc, mask	@ mask PC/LR for the mode

1003:		ldr	r2, [sv_pc, #-4]	@ if stmfd sp!, {args} exists,
		ldr	r3, .Ldsi+4		@ adjust saved 'pc' back one
<<<<<<< HEAD
		teq	r3, r2, lsr #10		@ instruction
=======
		teq	r3, r2, lsr #11		@ instruction
>>>>>>> upstream/android-13
		subne	r0, sv_pc, #4		@ allow for mov
		subeq	r0, sv_pc, #8		@ allow for mov + stmia

		ldr	r1, [frame, #-4]	@ get saved lr
		mov	r2, frame
		bic	r1, r1, mask		@ mask PC/LR for the mode
<<<<<<< HEAD
=======
		mov	r3, loglvl
>>>>>>> upstream/android-13
		bl	dump_backtrace_entry

		ldr	r1, [sv_pc, #-4]	@ if stmfd sp!, {args} exists,
		ldr	r3, .Ldsi+4
		teq	r3, r1, lsr #11
		ldreq	r0, [frame, #-8]	@ get sp
		subeq	r0, r0, #4		@ point at the last arg
<<<<<<< HEAD
=======
		mov	r2, loglvl
>>>>>>> upstream/android-13
		bleq	dump_backtrace_stm	@ dump saved registers

1004:		ldr	r1, [sv_pc, #0]		@ if stmfd sp!, {..., fp, ip, lr, pc}
		ldr	r3, .Ldsi		@ instruction exists,
		teq	r3, r1, lsr #11
		subeq	r0, frame, #16
<<<<<<< HEAD
=======
		mov	r2, loglvl
>>>>>>> upstream/android-13
		bleq	dump_backtrace_stm	@ dump saved registers

		teq	sv_fp, #0		@ zero saved fp means
		beq	no_frame		@ no further frames

		cmp	sv_fp, frame		@ next frame must be
		mov	frame, sv_fp		@ above the current frame
		bhi	for_each_frame

1006:		adr	r0, .Lbad
<<<<<<< HEAD
		mov	r1, frame
		bl	printk
no_frame:	ldmfd	sp!, {r4 - r8, pc}
=======
		mov	r1, loglvl
		mov	r2, frame
		bl	_printk
no_frame:	ldmfd	sp!, {r4 - r9, pc}
>>>>>>> upstream/android-13
ENDPROC(c_backtrace)
		
		.pushsection __ex_table,"a"
		.align	3
		.long	1001b, 1006b
		.long	1002b, 1006b
		.long	1003b, 1006b
		.long	1004b, 1006b
		.popsection

<<<<<<< HEAD
.Lbad:		.asciz	"Backtrace aborted due to bad frame pointer <%p>\n"
=======
.Lbad:		.asciz	"%sBacktrace aborted due to bad frame pointer <%p>\n"
>>>>>>> upstream/android-13
		.align
.Ldsi:		.word	0xe92dd800 >> 11	@ stmfd sp!, {... fp, ip, lr, pc}
		.word	0xe92d0000 >> 11	@ stmfd sp!, {}

#endif
