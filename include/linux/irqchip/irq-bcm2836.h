<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Root interrupt controller for the BCM2836 (Raspberry Pi 2).
 *
 * Copyright 2015 Broadcom
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#define LOCAL_CONTROL			0x000
#define LOCAL_PRESCALER			0x008

/*
 * The low 2 bits identify the CPU that the GPU IRQ goes to, and the
 * next 2 bits identify the CPU that the GPU FIQ goes to.
 */
#define LOCAL_GPU_ROUTING		0x00c
/* When setting bits 0-3, enables PMU interrupts on that CPU. */
#define LOCAL_PM_ROUTING_SET		0x010
/* When setting bits 0-3, disables PMU interrupts on that CPU. */
#define LOCAL_PM_ROUTING_CLR		0x014
/*
 * The low 4 bits of this are the CPU's timer IRQ enables, and the
 * next 4 bits are the CPU's timer FIQ enables (which override the IRQ
 * bits).
 */
#define LOCAL_TIMER_INT_CONTROL0	0x040
/*
 * The low 4 bits of this are the CPU's per-mailbox IRQ enables, and
 * the next 4 bits are the CPU's per-mailbox FIQ enables (which
 * override the IRQ bits).
 */
#define LOCAL_MAILBOX_INT_CONTROL0	0x050
/*
<<<<<<< HEAD
 * The CPU's interrupt status register.  Bits are defined by the the
=======
 * The CPU's interrupt status register.  Bits are defined by the
>>>>>>> upstream/android-13
 * LOCAL_IRQ_* bits below.
 */
#define LOCAL_IRQ_PENDING0		0x060
/* Same status bits as above, but for FIQ. */
#define LOCAL_FIQ_PENDING0		0x070
/*
 * Mailbox write-to-set bits.  There are 16 mailboxes, 4 per CPU, and
 * these bits are organized by mailbox number and then CPU number.  We
 * use mailbox 0 for IPIs.  The mailbox's interrupt is raised while
 * any bit is set.
 */
#define LOCAL_MAILBOX0_SET0		0x080
#define LOCAL_MAILBOX3_SET0		0x08c
/* Mailbox write-to-clear bits. */
#define LOCAL_MAILBOX0_CLR0		0x0c0
#define LOCAL_MAILBOX3_CLR0		0x0cc

#define LOCAL_IRQ_CNTPSIRQ	0
#define LOCAL_IRQ_CNTPNSIRQ	1
#define LOCAL_IRQ_CNTHPIRQ	2
#define LOCAL_IRQ_CNTVIRQ	3
#define LOCAL_IRQ_MAILBOX0	4
#define LOCAL_IRQ_MAILBOX1	5
#define LOCAL_IRQ_MAILBOX2	6
#define LOCAL_IRQ_MAILBOX3	7
#define LOCAL_IRQ_GPU_FAST	8
#define LOCAL_IRQ_PMU_FAST	9
#define LAST_IRQ		LOCAL_IRQ_PMU_FAST
