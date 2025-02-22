<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * This contains hardware definitions that are common between i.MX21 and
 * i.MX27.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
=======
>>>>>>> upstream/android-13
 */

#ifndef __MACH_MX2x_H__
#define __MACH_MX2x_H__

/* The following addresses are common between i.MX21 and i.MX27 */

/* Register offsets */
#define MX2x_AIPI_BASE_ADDR		0x10000000
#define MX2x_AIPI_SIZE			SZ_1M
#define MX2x_DMA_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x01000)
#define MX2x_WDOG_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x02000)
#define MX2x_GPT1_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x03000)
#define MX2x_GPT2_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x04000)
#define MX2x_GPT3_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x05000)
#define MX2x_PWM_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x06000)
#define MX2x_RTC_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x07000)
#define MX2x_KPP_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x08000)
#define MX2x_OWIRE_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x09000)
#define MX2x_UART1_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0a000)
#define MX2x_UART2_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0b000)
#define MX2x_UART3_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0c000)
#define MX2x_UART4_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0d000)
#define MX2x_CSPI1_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0e000)
#define MX2x_CSPI2_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x0f000)
#define MX2x_SSI1_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x10000)
#define MX2x_SSI2_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x11000)
#define MX2x_I2C_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x12000)
#define MX2x_SDHC1_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x13000)
#define MX2x_SDHC2_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x14000)
#define MX2x_GPIO_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x15000)
#define MX2x_AUDMUX_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x16000)
#define MX2x_CSPI3_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x17000)
#define MX2x_LCDC_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x21000)
#define MX2x_SLCDC_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x22000)
#define MX2x_USBOTG_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x24000)
#define MX2x_EMMA_PP_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x26000)
#define MX2x_EMMA_PRP_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x26400)
#define MX2x_CCM_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x27000)
#define MX2x_SYSCTRL_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x27800)
#define MX2x_JAM_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x3e000)
#define MX2x_MAX_BASE_ADDR			(MX2x_AIPI_BASE_ADDR + 0x3f000)

#define MX2x_AVIC_BASE_ADDR		0x10040000

#define MX2x_SAHB1_BASE_ADDR		0x80000000
#define MX2x_SAHB1_SIZE			SZ_1M
#define MX2x_CSI_BASE_ADDR			(MX2x_SAHB1_BASE_ADDR + 0x0000)

/* fixed interrupt numbers */
#include <asm/irq.h>
#define MX2x_INT_CSPI3		(NR_IRQS_LEGACY + 6)
#define MX2x_INT_GPIO		(NR_IRQS_LEGACY + 8)
#define MX2x_INT_SDHC2		(NR_IRQS_LEGACY + 10)
#define MX2x_INT_SDHC1		(NR_IRQS_LEGACY + 11)
#define MX2x_INT_I2C		(NR_IRQS_LEGACY + 12)
#define MX2x_INT_SSI2		(NR_IRQS_LEGACY + 13)
#define MX2x_INT_SSI1		(NR_IRQS_LEGACY + 14)
#define MX2x_INT_CSPI2		(NR_IRQS_LEGACY + 15)
#define MX2x_INT_CSPI1		(NR_IRQS_LEGACY + 16)
#define MX2x_INT_UART4		(NR_IRQS_LEGACY + 17)
#define MX2x_INT_UART3		(NR_IRQS_LEGACY + 18)
#define MX2x_INT_UART2		(NR_IRQS_LEGACY + 19)
#define MX2x_INT_UART1		(NR_IRQS_LEGACY + 20)
#define MX2x_INT_KPP		(NR_IRQS_LEGACY + 21)
#define MX2x_INT_RTC		(NR_IRQS_LEGACY + 22)
#define MX2x_INT_PWM		(NR_IRQS_LEGACY + 23)
#define MX2x_INT_GPT3		(NR_IRQS_LEGACY + 24)
#define MX2x_INT_GPT2		(NR_IRQS_LEGACY + 25)
#define MX2x_INT_GPT1		(NR_IRQS_LEGACY + 26)
#define MX2x_INT_WDOG		(NR_IRQS_LEGACY + 27)
#define MX2x_INT_PCMCIA		(NR_IRQS_LEGACY + 28)
#define MX2x_INT_NANDFC		(NR_IRQS_LEGACY + 29)
#define MX2x_INT_CSI		(NR_IRQS_LEGACY + 31)
#define MX2x_INT_DMACH0		(NR_IRQS_LEGACY + 32)
#define MX2x_INT_DMACH1		(NR_IRQS_LEGACY + 33)
#define MX2x_INT_DMACH2		(NR_IRQS_LEGACY + 34)
#define MX2x_INT_DMACH3		(NR_IRQS_LEGACY + 35)
#define MX2x_INT_DMACH4		(NR_IRQS_LEGACY + 36)
#define MX2x_INT_DMACH5		(NR_IRQS_LEGACY + 37)
#define MX2x_INT_DMACH6		(NR_IRQS_LEGACY + 38)
#define MX2x_INT_DMACH7		(NR_IRQS_LEGACY + 39)
#define MX2x_INT_DMACH8		(NR_IRQS_LEGACY + 40)
#define MX2x_INT_DMACH9		(NR_IRQS_LEGACY + 41)
#define MX2x_INT_DMACH10	(NR_IRQS_LEGACY + 42)
#define MX2x_INT_DMACH11	(NR_IRQS_LEGACY + 43)
#define MX2x_INT_DMACH12	(NR_IRQS_LEGACY + 44)
#define MX2x_INT_DMACH13	(NR_IRQS_LEGACY + 45)
#define MX2x_INT_DMACH14	(NR_IRQS_LEGACY + 46)
#define MX2x_INT_DMACH15	(NR_IRQS_LEGACY + 47)
#define MX2x_INT_EMMAPRP	(NR_IRQS_LEGACY + 51)
#define MX2x_INT_EMMAPP		(NR_IRQS_LEGACY + 52)
#define MX2x_INT_SLCDC		(NR_IRQS_LEGACY + 60)
#define MX2x_INT_LCDC		(NR_IRQS_LEGACY + 61)

/* fixed DMA request numbers */
#define MX2x_DMA_REQ_CSPI3_RX	1
#define MX2x_DMA_REQ_CSPI3_TX	2
#define MX2x_DMA_REQ_EXT	3
#define MX2x_DMA_REQ_SDHC2	6
#define MX2x_DMA_REQ_SDHC1	7
#define MX2x_DMA_REQ_SSI2_RX0	8
#define MX2x_DMA_REQ_SSI2_TX0	9
#define MX2x_DMA_REQ_SSI2_RX1	10
#define MX2x_DMA_REQ_SSI2_TX1	11
#define MX2x_DMA_REQ_SSI1_RX0	12
#define MX2x_DMA_REQ_SSI1_TX0	13
#define MX2x_DMA_REQ_SSI1_RX1	14
#define MX2x_DMA_REQ_SSI1_TX1	15
#define MX2x_DMA_REQ_CSPI2_RX	16
#define MX2x_DMA_REQ_CSPI2_TX	17
#define MX2x_DMA_REQ_CSPI1_RX	18
#define MX2x_DMA_REQ_CSPI1_TX	19
#define MX2x_DMA_REQ_UART4_RX	20
#define MX2x_DMA_REQ_UART4_TX	21
#define MX2x_DMA_REQ_UART3_RX	22
#define MX2x_DMA_REQ_UART3_TX	23
#define MX2x_DMA_REQ_UART2_RX	24
#define MX2x_DMA_REQ_UART2_TX	25
#define MX2x_DMA_REQ_UART1_RX	26
#define MX2x_DMA_REQ_UART1_TX	27
#define MX2x_DMA_REQ_CSI_STAT	30
#define MX2x_DMA_REQ_CSI_RX	31

#endif /* ifndef __MACH_MX2x_H__ */
