<<<<<<< HEAD
/*
 * Copyright 2017 Texas Instruments, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2017 Texas Instruments, Inc.
>>>>>>> upstream/android-13
 */
#ifndef __DT_BINDINGS_CLK_DM814_H
#define __DT_BINDINGS_CLK_DM814_H

#define DM814_CLKCTRL_OFFSET	0x0
#define DM814_CLKCTRL_INDEX(offset)	((offset) - DM814_CLKCTRL_OFFSET)

/* default clocks */
#define DM814_USB_OTG_HS_CLKCTRL	DM814_CLKCTRL_INDEX(0x58)

/* alwon clocks */
#define DM814_UART1_CLKCTRL	DM814_CLKCTRL_INDEX(0x150)
#define DM814_UART2_CLKCTRL	DM814_CLKCTRL_INDEX(0x154)
#define DM814_UART3_CLKCTRL	DM814_CLKCTRL_INDEX(0x158)
#define DM814_GPIO1_CLKCTRL	DM814_CLKCTRL_INDEX(0x15c)
#define DM814_GPIO2_CLKCTRL	DM814_CLKCTRL_INDEX(0x160)
#define DM814_I2C1_CLKCTRL	DM814_CLKCTRL_INDEX(0x164)
#define DM814_I2C2_CLKCTRL	DM814_CLKCTRL_INDEX(0x168)
#define DM814_WD_TIMER_CLKCTRL	DM814_CLKCTRL_INDEX(0x18c)
#define DM814_MCSPI1_CLKCTRL	DM814_CLKCTRL_INDEX(0x190)
#define DM814_GPMC_CLKCTRL	DM814_CLKCTRL_INDEX(0x1d0)
#define DM814_CPGMAC0_CLKCTRL	DM814_CLKCTRL_INDEX(0x1d4)
#define DM814_MPU_CLKCTRL	DM814_CLKCTRL_INDEX(0x1dc)
#define DM814_RTC_CLKCTRL	DM814_CLKCTRL_INDEX(0x1f0)
#define DM814_TPCC_CLKCTRL	DM814_CLKCTRL_INDEX(0x1f4)
#define DM814_TPTC0_CLKCTRL	DM814_CLKCTRL_INDEX(0x1f8)
#define DM814_TPTC1_CLKCTRL	DM814_CLKCTRL_INDEX(0x1fc)
#define DM814_TPTC2_CLKCTRL	DM814_CLKCTRL_INDEX(0x200)
#define DM814_TPTC3_CLKCTRL	DM814_CLKCTRL_INDEX(0x204)
#define DM814_MMC1_CLKCTRL	DM814_CLKCTRL_INDEX(0x21c)
#define DM814_MMC2_CLKCTRL	DM814_CLKCTRL_INDEX(0x220)
#define DM814_MMC3_CLKCTRL	DM814_CLKCTRL_INDEX(0x224)

<<<<<<< HEAD
=======
/* alwon_ethernet clocks */
#define DM814_ETHERNET_CLKCTRL_OFFSET	0x1d4
#define DM814_ETHERNET_CLKCTRL_INDEX(offset)	((offset) - DM814_ETHERNET_CLKCTRL_OFFSET)
#define DM814_ETHERNET_CPGMAC0_CLKCTRL	DM814_ETHERNET_CLKCTRL_INDEX(0x1d4)

>>>>>>> upstream/android-13
#endif
