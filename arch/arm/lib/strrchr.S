<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/strrchr.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 *  ASM optimised string functions
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.text
		.align	5
ENTRY(strrchr)
		mov	r3, #0
1:		ldrb	r2, [r0], #1
		teq	r2, r1
		subeq	r3, r0, #1
		teq	r2, #0
		bne	1b
		mov	r0, r3
		ret	lr
ENDPROC(strrchr)
