<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/memchr.S
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
ENTRY(memchr)
1:	subs	r2, r2, #1
	bmi	2f
	ldrb	r3, [r0], #1
	teq	r3, r1
	bne	1b
	sub	r0, r0, #1
2:	movne	r0, #0
	ret	lr
ENDPROC(memchr)
