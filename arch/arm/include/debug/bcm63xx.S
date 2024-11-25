<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Broadcom BCM63xx low-level UART debug
 *
 * Copyright (C) 2014 Broadcom Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/serial_bcm63xx.h>

	.macro	addruart, rp, rv, tmp
	ldr	\rp, =CONFIG_DEBUG_UART_PHYS
	ldr	\rv, =CONFIG_DEBUG_UART_VIRT
	.endm

	.macro	senduart, rd, rx
	/* word access do not work */
	strb	\rd, [\rx, #UART_FIFO_REG]
	.endm

<<<<<<< HEAD
	.macro	waituart, rd, rx
=======
	.macro	waituarttxrdy, rd, rx
>>>>>>> upstream/android-13
1001:	ldr	\rd, [\rx, #UART_IR_REG]
	tst	\rd, #(1 << UART_IR_TXEMPTY)
	beq	1001b
	.endm

<<<<<<< HEAD
=======
	.macro	waituartcts, rd, rx
	.endm

>>>>>>> upstream/android-13
	.macro	busyuart, rd, rx
1002:	ldr	\rd, [\rx, #UART_IR_REG]
	tst	\rd, #(1 << UART_IR_TXTRESH)
	beq	1002b
	.endm
