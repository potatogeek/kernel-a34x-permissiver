<<<<<<< HEAD
/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2013 Freescale Semiconductor, Inc.
>>>>>>> upstream/android-13
 */

#ifndef __DT_BINDINGS_CLOCK_VF610_H
#define __DT_BINDINGS_CLOCK_VF610_H

#define VF610_CLK_DUMMY			0
#define VF610_CLK_SIRC_128K		1
#define VF610_CLK_SIRC_32K		2
#define VF610_CLK_FIRC			3
#define VF610_CLK_SXOSC			4
#define VF610_CLK_FXOSC			5
#define VF610_CLK_FXOSC_HALF		6
#define VF610_CLK_SLOW_CLK_SEL		7
#define VF610_CLK_FASK_CLK_SEL		8
#define VF610_CLK_AUDIO_EXT		9
#define VF610_CLK_ENET_EXT		10
#define VF610_CLK_PLL1_SYS		11
#define VF610_CLK_PLL1_PFD1		12
#define VF610_CLK_PLL1_PFD2		13
#define VF610_CLK_PLL1_PFD3		14
#define VF610_CLK_PLL1_PFD4		15
#define VF610_CLK_PLL2_BUS		16
#define VF610_CLK_PLL2_PFD1		17
#define VF610_CLK_PLL2_PFD2		18
#define VF610_CLK_PLL2_PFD3		19
#define VF610_CLK_PLL2_PFD4		20
#define VF610_CLK_PLL3_USB_OTG		21
#define VF610_CLK_PLL3_PFD1		22
#define VF610_CLK_PLL3_PFD2		23
#define VF610_CLK_PLL3_PFD3		24
#define VF610_CLK_PLL3_PFD4		25
#define VF610_CLK_PLL4_AUDIO		26
#define VF610_CLK_PLL5_ENET		27
#define VF610_CLK_PLL6_VIDEO		28
#define VF610_CLK_PLL3_MAIN_DIV		29
#define VF610_CLK_PLL4_MAIN_DIV		30
#define VF610_CLK_PLL6_MAIN_DIV		31
#define VF610_CLK_PLL1_PFD_SEL		32
#define VF610_CLK_PLL2_PFD_SEL		33
#define VF610_CLK_SYS_SEL		34
#define VF610_CLK_DDR_SEL		35
#define VF610_CLK_SYS_BUS		36
#define VF610_CLK_PLATFORM_BUS		37
#define VF610_CLK_IPG_BUS		38
#define VF610_CLK_UART0			39
#define VF610_CLK_UART1			40
#define VF610_CLK_UART2			41
#define VF610_CLK_UART3			42
#define VF610_CLK_UART4			43
#define VF610_CLK_UART5			44
#define VF610_CLK_PIT			45
#define VF610_CLK_I2C0			46
#define VF610_CLK_I2C1			47
#define VF610_CLK_I2C2			48
#define VF610_CLK_I2C3			49
#define VF610_CLK_FTM0_EXT_SEL		50
#define VF610_CLK_FTM0_FIX_SEL		51
#define VF610_CLK_FTM0_EXT_FIX_EN	52
#define VF610_CLK_FTM1_EXT_SEL		53
#define VF610_CLK_FTM1_FIX_SEL		54
#define VF610_CLK_FTM1_EXT_FIX_EN	55
#define VF610_CLK_FTM2_EXT_SEL		56
#define VF610_CLK_FTM2_FIX_SEL		57
#define VF610_CLK_FTM2_EXT_FIX_EN	58
#define VF610_CLK_FTM3_EXT_SEL		59
#define VF610_CLK_FTM3_FIX_SEL		60
#define VF610_CLK_FTM3_EXT_FIX_EN	61
#define VF610_CLK_FTM0			62
#define VF610_CLK_FTM1			63
#define VF610_CLK_FTM2			64
#define VF610_CLK_FTM3			65
#define VF610_CLK_ENET_50M		66
#define VF610_CLK_ENET_25M		67
#define VF610_CLK_ENET_SEL		68
#define VF610_CLK_ENET			69
#define VF610_CLK_ENET_TS_SEL		70
#define VF610_CLK_ENET_TS		71
#define VF610_CLK_DSPI0			72
#define VF610_CLK_DSPI1			73
#define VF610_CLK_DSPI2			74
#define VF610_CLK_DSPI3			75
#define VF610_CLK_WDT			76
#define VF610_CLK_ESDHC0_SEL		77
#define VF610_CLK_ESDHC0_EN		78
#define VF610_CLK_ESDHC0_DIV		79
#define VF610_CLK_ESDHC0		80
#define VF610_CLK_ESDHC1_SEL		81
#define VF610_CLK_ESDHC1_EN		82
#define VF610_CLK_ESDHC1_DIV		83
#define VF610_CLK_ESDHC1		84
#define VF610_CLK_DCU0_SEL		85
#define VF610_CLK_DCU0_EN		86
#define VF610_CLK_DCU0_DIV		87
#define VF610_CLK_DCU0			88
#define VF610_CLK_DCU1_SEL		89
#define VF610_CLK_DCU1_EN		90
#define VF610_CLK_DCU1_DIV		91
#define VF610_CLK_DCU1			92
#define VF610_CLK_ESAI_SEL		93
#define VF610_CLK_ESAI_EN		94
#define VF610_CLK_ESAI_DIV		95
#define VF610_CLK_ESAI			96
#define VF610_CLK_SAI0_SEL		97
#define VF610_CLK_SAI0_EN		98
#define VF610_CLK_SAI0_DIV		99
#define VF610_CLK_SAI0			100
#define VF610_CLK_SAI1_SEL		101
#define VF610_CLK_SAI1_EN		102
#define VF610_CLK_SAI1_DIV		103
#define VF610_CLK_SAI1			104
#define VF610_CLK_SAI2_SEL		105
#define VF610_CLK_SAI2_EN		106
#define VF610_CLK_SAI2_DIV		107
#define VF610_CLK_SAI2			108
#define VF610_CLK_SAI3_SEL		109
#define VF610_CLK_SAI3_EN		110
#define VF610_CLK_SAI3_DIV		111
#define VF610_CLK_SAI3			112
#define VF610_CLK_USBC0			113
#define VF610_CLK_USBC1			114
#define VF610_CLK_QSPI0_SEL		115
#define VF610_CLK_QSPI0_EN		116
#define VF610_CLK_QSPI0_X4_DIV		117
#define VF610_CLK_QSPI0_X2_DIV		118
#define VF610_CLK_QSPI0_X1_DIV		119
#define VF610_CLK_QSPI1_SEL		120
#define VF610_CLK_QSPI1_EN		121
#define VF610_CLK_QSPI1_X4_DIV		122
#define VF610_CLK_QSPI1_X2_DIV		123
#define VF610_CLK_QSPI1_X1_DIV		124
#define VF610_CLK_QSPI0			125
#define VF610_CLK_QSPI1			126
#define VF610_CLK_NFC_SEL		127
#define VF610_CLK_NFC_EN		128
#define VF610_CLK_NFC_PRE_DIV		129
#define VF610_CLK_NFC_FRAC_DIV		130
#define VF610_CLK_NFC_INV		131
#define VF610_CLK_NFC			132
#define VF610_CLK_VADC_SEL		133
#define VF610_CLK_VADC_EN		134
#define VF610_CLK_VADC_DIV		135
#define VF610_CLK_VADC_DIV_HALF		136
#define VF610_CLK_VADC			137
#define VF610_CLK_ADC0			138
#define VF610_CLK_ADC1			139
#define VF610_CLK_DAC0			140
#define VF610_CLK_DAC1			141
#define VF610_CLK_FLEXCAN0		142
#define VF610_CLK_FLEXCAN1		143
#define VF610_CLK_ASRC			144
#define VF610_CLK_GPU_SEL		145
#define VF610_CLK_GPU_EN		146
#define VF610_CLK_GPU2D			147
#define VF610_CLK_ENET0			148
#define VF610_CLK_ENET1			149
#define VF610_CLK_DMAMUX0		150
#define VF610_CLK_DMAMUX1		151
#define VF610_CLK_DMAMUX2		152
#define VF610_CLK_DMAMUX3		153
#define VF610_CLK_FLEXCAN0_EN		154
#define VF610_CLK_FLEXCAN1_EN		155
#define VF610_CLK_PLL7_USB_HOST		156
#define VF610_CLK_USBPHY0		157
#define VF610_CLK_USBPHY1		158
#define VF610_CLK_LVDS1_IN		159
#define VF610_CLK_ANACLK1		160
#define VF610_CLK_PLL1_BYPASS_SRC	161
#define VF610_CLK_PLL2_BYPASS_SRC	162
#define VF610_CLK_PLL3_BYPASS_SRC	163
#define VF610_CLK_PLL4_BYPASS_SRC	164
#define VF610_CLK_PLL5_BYPASS_SRC	165
#define VF610_CLK_PLL6_BYPASS_SRC	166
#define VF610_CLK_PLL7_BYPASS_SRC	167
#define VF610_CLK_PLL1			168
#define VF610_CLK_PLL2			169
#define VF610_CLK_PLL3			170
#define VF610_CLK_PLL4			171
#define VF610_CLK_PLL5			172
#define VF610_CLK_PLL6			173
#define VF610_CLK_PLL7			174
#define VF610_PLL1_BYPASS		175
#define VF610_PLL2_BYPASS		176
#define VF610_PLL3_BYPASS		177
#define VF610_PLL4_BYPASS		178
#define VF610_PLL5_BYPASS		179
#define VF610_PLL6_BYPASS		180
#define VF610_PLL7_BYPASS		181
#define VF610_CLK_SNVS			182
#define VF610_CLK_DAP			183
#define VF610_CLK_OCOTP			184
#define VF610_CLK_DDRMC			185
#define VF610_CLK_WKPU			186
#define VF610_CLK_TCON0			187
#define VF610_CLK_TCON1			188
<<<<<<< HEAD
#define VF610_CLK_END			189
=======
#define VF610_CLK_CAAM			189
#define VF610_CLK_CRC			190
#define VF610_CLK_END			191
>>>>>>> upstream/android-13

#endif /* __DT_BINDINGS_CLOCK_VF610_H */
