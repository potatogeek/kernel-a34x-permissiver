/*
 *
 * Copyright (c) 2016 BayLibre, SAS.
 * Author: Neil Armstrong <narmstrong@baylibre.com>
 *
 * Copyright (c) 2017 Amlogic, inc.
 * Author: Yixun Lan <yixun.lan@amlogic.com>
 *
 * SPDX-License-Identifier: (GPL-2.0+ OR BSD)
 */

#ifndef _DT_BINDINGS_AMLOGIC_MESON_AXG_RESET_H
#define _DT_BINDINGS_AMLOGIC_MESON_AXG_RESET_H


#define RESET_HIU			0
#define RESET_PCIE_A			1
#define RESET_PCIE_B			2
#define RESET_DDR_TOP			3

#define RESET_VIU			5
#define RESET_PCIE_PHY			6
#define RESET_PCIE_APB			7


#define RESET_VENC			10
#define RESET_ASSIST			11

#define RESET_VCBUS			13


#define RESET_GIC			16
#define RESET_CAPB3_DECODE		17

#define RESET_SYS_CPU_CAPB3		22
#define RESET_CBUS_CAPB3		23
#define RESET_AHB_CNTL			24
#define RESET_AHB_DATA			25
#define RESET_VCBUS_CLK81		26
#define RESET_MMC			27




#define RESET_USB_OTG			34
#define RESET_DDR			35
#define RESET_AO_RESET			36

#define RESET_AHB_SRAM			38


#define RESET_DMA			41
#define RESET_ISA			42
#define RESET_ETHERNET			43

#define RESET_SD_EMMC_B			45
#define RESET_SD_EMMC_C			46
#define RESET_ROM_BOOT			47
#define RESET_SYS_CPU_0			48
#define RESET_SYS_CPU_1			49
#define RESET_SYS_CPU_2			50
#define RESET_SYS_CPU_3			51
#define RESET_SYS_CPU_CORE_0		52
#define RESET_SYS_CPU_CORE_1		53
#define RESET_SYS_CPU_CORE_2		54
#define RESET_SYS_CPU_CORE_3		55
#define RESET_SYS_PLL_DIV		56
#define RESET_SYS_CPU_AXI		57
#define RESET_SYS_CPU_L2		58
#define RESET_SYS_CPU_P			59
#define RESET_SYS_CPU_MBIST		60




#define RESET_AUDIO			66

#define RESET_MIPI_HOST			68
#define RESET_AUDIO_LOCKER		69
#define RESET_GE2D			70

#define RESET_AO_CPU_RESET		77


#define RESET_RING_OSCILLATOR		96




#define RESET_MIPI_PHY			130

#define RESET_VENCL			141
#define RESET_I2C_MASTER_2		142
#define RESET_I2C_MASTER_1		143




#define RESET_PERIPHS_GENERAL		192
#define RESET_PERIPHS_SPICC		193


#define RESET_PERIPHS_I2C_MASTER_0	196

#define RESET_PERIPHS_UART_0		201
#define RESET_PERIPHS_UART_1		202

#define RESET_PERIPHS_SPI_0		205
#define RESET_PERIPHS_I2C_MASTER_3	206


#define RESET_USB_DDR_0			224
#define RESET_USB_DDR_1			225
#define RESET_USB_DDR_2			226
#define RESET_USB_DDR_3			227

#define RESET_DEVICE_MMC_ARB		229

#define RESET_VID_LOCK			231
#define RESET_A9_DMC_PIPEL		232
#define RESET_DMC_VPU_PIPEL		233


#endif
