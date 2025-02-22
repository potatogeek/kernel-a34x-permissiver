<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  Atheros AR933X UART defines
 *
 *  Copyright (C) 2011 Gabor Juhos <juhosg@openwrt.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __AR933X_UART_H
#define __AR933X_UART_H

#define AR933X_UART_REGS_SIZE		20
#define AR933X_UART_FIFO_SIZE		16

#define AR933X_UART_DATA_REG		0x00
#define AR933X_UART_CS_REG		0x04
#define AR933X_UART_CLOCK_REG		0x08
#define AR933X_UART_INT_REG		0x0c
#define AR933X_UART_INT_EN_REG		0x10

#define AR933X_UART_DATA_TX_RX_MASK	0xff
#define AR933X_UART_DATA_RX_CSR		BIT(8)
#define AR933X_UART_DATA_TX_CSR		BIT(9)

#define AR933X_UART_CS_PARITY_S		0
#define AR933X_UART_CS_PARITY_M		0x3
#define	  AR933X_UART_CS_PARITY_NONE	0
#define	  AR933X_UART_CS_PARITY_ODD	2
#define	  AR933X_UART_CS_PARITY_EVEN	3
#define AR933X_UART_CS_IF_MODE_S	2
#define AR933X_UART_CS_IF_MODE_M	0x3
#define	  AR933X_UART_CS_IF_MODE_NONE	0
#define	  AR933X_UART_CS_IF_MODE_DTE	1
#define	  AR933X_UART_CS_IF_MODE_DCE	2
#define AR933X_UART_CS_FLOW_CTRL_S	4
#define AR933X_UART_CS_FLOW_CTRL_M	0x3
#define AR933X_UART_CS_DMA_EN		BIT(6)
#define AR933X_UART_CS_TX_READY_ORIDE	BIT(7)
#define AR933X_UART_CS_RX_READY_ORIDE	BIT(8)
#define AR933X_UART_CS_TX_READY		BIT(9)
#define AR933X_UART_CS_RX_BREAK		BIT(10)
#define AR933X_UART_CS_TX_BREAK		BIT(11)
#define AR933X_UART_CS_HOST_INT		BIT(12)
#define AR933X_UART_CS_HOST_INT_EN	BIT(13)
#define AR933X_UART_CS_TX_BUSY		BIT(14)
#define AR933X_UART_CS_RX_BUSY		BIT(15)

#define AR933X_UART_CLOCK_STEP_M	0xffff
#define AR933X_UART_CLOCK_SCALE_M	0xfff
#define AR933X_UART_CLOCK_SCALE_S	16
#define AR933X_UART_CLOCK_STEP_M	0xffff

#define AR933X_UART_INT_RX_VALID	BIT(0)
#define AR933X_UART_INT_TX_READY	BIT(1)
#define AR933X_UART_INT_RX_FRAMING_ERR	BIT(2)
#define AR933X_UART_INT_RX_OFLOW_ERR	BIT(3)
#define AR933X_UART_INT_TX_OFLOW_ERR	BIT(4)
#define AR933X_UART_INT_RX_PARITY_ERR	BIT(5)
#define AR933X_UART_INT_RX_BREAK_ON	BIT(6)
#define AR933X_UART_INT_RX_BREAK_OFF	BIT(7)
#define AR933X_UART_INT_RX_FULL		BIT(8)
#define AR933X_UART_INT_TX_EMPTY	BIT(9)
#define AR933X_UART_INT_ALLINTS		0x3ff

#endif /* __AR933X_UART_H */
