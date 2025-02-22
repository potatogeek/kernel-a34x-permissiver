<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/io-readsw-armv4.S
 *
 *  Copyright (C) 1995-2000 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.macro	pack, rd, hw1, hw2
#ifndef __ARMEB__
		orr	\rd, \hw1, \hw2, lsl #16
#else
		orr	\rd, \hw2, \hw1, lsl #16
#endif
		.endm

.Linsw_align:	movs	ip, r1, lsl #31
		bne	.Linsw_noalign
		ldrh	ip, [r0]
		sub	r2, r2, #1
		strh	ip, [r1], #2

ENTRY(__raw_readsw)
		teq	r2, #0
		reteq	lr
		tst	r1, #3
		bne	.Linsw_align

		stmfd	sp!, {r4, r5, lr}

		subs	r2, r2, #8
		bmi	.Lno_insw_8

.Linsw_8_lp:	ldrh	r3, [r0]
		ldrh	r4, [r0]
		pack	r3, r3, r4

		ldrh	r4, [r0]
		ldrh	r5, [r0]
		pack	r4, r4, r5

		ldrh	r5, [r0]
		ldrh	ip, [r0]
		pack	r5, r5, ip

		ldrh	ip, [r0]
		ldrh	lr, [r0]
		pack	ip, ip, lr

		subs	r2, r2, #8
		stmia	r1!, {r3 - r5, ip}
		bpl	.Linsw_8_lp

.Lno_insw_8:	tst	r2, #4
		beq	.Lno_insw_4

		ldrh	r3, [r0]
		ldrh	r4, [r0]
		pack	r3, r3, r4

		ldrh	r4, [r0]
		ldrh	ip, [r0]
		pack	r4, r4, ip

		stmia	r1!, {r3, r4}

.Lno_insw_4:	movs	r2, r2, lsl #31
		bcc	.Lno_insw_2

		ldrh	r3, [r0]
		ldrh	ip, [r0]
		pack	r3, r3, ip
		str	r3, [r1], #4

<<<<<<< HEAD
.Lno_insw_2:	ldrneh	r3, [r0]
		strneh	r3, [r1]
=======
.Lno_insw_2:	ldrhne	r3, [r0]
		strhne	r3, [r1]
>>>>>>> upstream/android-13

		ldmfd	sp!, {r4, r5, pc}

#ifdef __ARMEB__
#define _BE_ONLY_(code...)	code
#define _LE_ONLY_(code...)
#define push_hbyte0		lsr #8
#define pull_hbyte1		lsl #24
#else
#define _BE_ONLY_(code...)
#define _LE_ONLY_(code...) code
#define push_hbyte0		lsl #24
#define pull_hbyte1		lsr #8
#endif

.Linsw_noalign:	stmfd	sp!, {r4, lr}
<<<<<<< HEAD
		ldrccb	ip, [r1, #-1]!
=======
		ldrbcc	ip, [r1, #-1]!
>>>>>>> upstream/android-13
		bcc	1f

		ldrh	ip, [r0]
		sub	r2, r2, #1
   _BE_ONLY_(	mov	ip, ip, ror #8		)
		strb	ip, [r1], #1
   _LE_ONLY_(	mov	ip, ip, lsr #8		)
   _BE_ONLY_(	mov	ip, ip, lsr #24		)

1:		subs	r2, r2, #2
		bmi	3f
   _BE_ONLY_(	mov	ip, ip, lsl #24		)

2:		ldrh	r3, [r0]
		ldrh	r4, [r0]
		subs	r2, r2, #2
		orr	ip, ip, r3, lsl #8
		orr	ip, ip, r4, push_hbyte0
		str	ip, [r1], #4
		mov	ip, r4, pull_hbyte1
		bpl	2b

   _BE_ONLY_(	mov	ip, ip, lsr #24		)

3:		tst	r2, #1
		strb	ip, [r1], #1
<<<<<<< HEAD
		ldrneh	ip, [r0]
   _BE_ONLY_(	movne	ip, ip, ror #8		)
		strneb	ip, [r1], #1
   _LE_ONLY_(	movne	ip, ip, lsr #8		)
   _BE_ONLY_(	movne	ip, ip, lsr #24		)
		strneb	ip, [r1]
=======
		ldrhne	ip, [r0]
   _BE_ONLY_(	movne	ip, ip, ror #8		)
		strbne	ip, [r1], #1
   _LE_ONLY_(	movne	ip, ip, lsr #8		)
   _BE_ONLY_(	movne	ip, ip, lsr #24		)
		strbne	ip, [r1]
>>>>>>> upstream/android-13
		ldmfd	sp!, {r4, pc}
ENDPROC(__raw_readsw)
