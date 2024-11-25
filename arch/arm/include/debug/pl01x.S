<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/* arch/arm/include/debug/pl01x.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
*/
#include <linux/amba/serial.h>

#ifdef CONFIG_DEBUG_ZTE_ZX
#undef UART01x_DR
#undef UART01x_FR
#define UART01x_DR     0x04
#define UART01x_FR     0x14
#endif

#ifdef CONFIG_DEBUG_UART_PHYS
		.macro	addruart, rp, rv, tmp
		ldr	\rp, =CONFIG_DEBUG_UART_PHYS
		ldr	\rv, =CONFIG_DEBUG_UART_VIRT
		.endm
#endif

		.macro	senduart,rd,rx
		strb	\rd, [\rx, #UART01x_DR]
		.endm

<<<<<<< HEAD
		.macro	waituart,rd,rx
=======
		.macro	waituartcts,rd,rx
		.endm

		.macro	waituarttxrdy,rd,rx
>>>>>>> upstream/android-13
1001:		ldr	\rd, [\rx, #UART01x_FR]
 ARM_BE8(	rev	\rd, \rd )
		tst	\rd, #UART01x_FR_TXFF
		bne	1001b
		.endm

		.macro	busyuart,rd,rx
1001:		ldr	\rd, [\rx, #UART01x_FR]
 ARM_BE8(	rev	\rd, \rd )
		tst	\rd, #UART01x_FR_BUSY
		bne	1001b
		.endm
