<<<<<<< HEAD
/*
 * Copyright (c) 2014 MundoReader S.L.
 * Author: Heiko Stuebner <heiko@sntech.de>
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
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (c) 2014 MundoReader S.L.
 * Author: Heiko Stuebner <heiko@sntech.de>
>>>>>>> upstream/android-13
 */

#ifndef _DT_BINDINGS_CLK_ROCKCHIP_RK3288_H
#define _DT_BINDINGS_CLK_ROCKCHIP_RK3288_H

/* core clocks */
#define PLL_APLL		1
#define PLL_DPLL		2
#define PLL_CPLL		3
#define PLL_GPLL		4
#define PLL_NPLL		5
#define ARMCLK			6

/* sclk gates (special clocks) */
#define SCLK_GPU		64
#define SCLK_SPI0		65
#define SCLK_SPI1		66
#define SCLK_SPI2		67
#define SCLK_SDMMC		68
#define SCLK_SDIO0		69
#define SCLK_SDIO1		70
#define SCLK_EMMC		71
#define SCLK_TSADC		72
#define SCLK_SARADC		73
#define SCLK_PS2C		74
#define SCLK_NANDC0		75
#define SCLK_NANDC1		76
#define SCLK_UART0		77
#define SCLK_UART1		78
#define SCLK_UART2		79
#define SCLK_UART3		80
#define SCLK_UART4		81
#define SCLK_I2S0		82
#define SCLK_SPDIF		83
#define SCLK_SPDIF8CH		84
#define SCLK_TIMER0		85
#define SCLK_TIMER1		86
#define SCLK_TIMER2		87
#define SCLK_TIMER3		88
#define SCLK_TIMER4		89
#define SCLK_TIMER5		90
#define SCLK_TIMER6		91
#define SCLK_HSADC		92
#define SCLK_OTGPHY0		93
#define SCLK_OTGPHY1		94
#define SCLK_OTGPHY2		95
#define SCLK_OTG_ADP		96
#define SCLK_HSICPHY480M	97
#define SCLK_HSICPHY12M		98
#define SCLK_MACREF		99
#define SCLK_LCDC_PWM0		100
#define SCLK_LCDC_PWM1		101
#define SCLK_MAC_RX		102
#define SCLK_MAC_TX		103
#define SCLK_EDP_24M		104
#define SCLK_EDP		105
#define SCLK_RGA		106
#define SCLK_ISP		107
#define SCLK_ISP_JPE		108
#define SCLK_HDMI_HDCP		109
#define SCLK_HDMI_CEC		110
#define SCLK_HEVC_CABAC		111
#define SCLK_HEVC_CORE		112
#define SCLK_I2S0_OUT		113
#define SCLK_SDMMC_DRV		114
#define SCLK_SDIO0_DRV		115
#define SCLK_SDIO1_DRV		116
#define SCLK_EMMC_DRV		117
#define SCLK_SDMMC_SAMPLE	118
#define SCLK_SDIO0_SAMPLE	119
#define SCLK_SDIO1_SAMPLE	120
#define SCLK_EMMC_SAMPLE	121
#define SCLK_USBPHY480M_SRC	122
#define SCLK_PVTM_CORE		123
#define SCLK_PVTM_GPU		124
#define SCLK_CRYPTO		125
#define SCLK_MIPIDSI_24M	126
#define SCLK_VIP_OUT		127

#define SCLK_MAC		151
#define SCLK_MACREF_OUT		152

#define DCLK_VOP0		190
#define DCLK_VOP1		191

/* aclk gates */
#define ACLK_GPU		192
#define ACLK_DMAC1		193
#define ACLK_DMAC2		194
#define ACLK_MMU		195
#define ACLK_GMAC		196
#define ACLK_VOP0		197
#define ACLK_VOP1		198
#define ACLK_CRYPTO		199
#define ACLK_RGA		200
#define ACLK_RGA_NIU		201
#define ACLK_IEP		202
#define ACLK_VIO0_NIU		203
#define ACLK_VIP		204
#define ACLK_ISP		205
#define ACLK_VIO1_NIU		206
#define ACLK_HEVC		207
#define ACLK_VCODEC		208
#define ACLK_CPU		209
#define ACLK_PERI		210

/* pclk gates */
#define PCLK_GPIO0		320
#define PCLK_GPIO1		321
#define PCLK_GPIO2		322
#define PCLK_GPIO3		323
#define PCLK_GPIO4		324
#define PCLK_GPIO5		325
#define PCLK_GPIO6		326
#define PCLK_GPIO7		327
#define PCLK_GPIO8		328
#define PCLK_GRF		329
#define PCLK_SGRF		330
#define PCLK_PMU		331
#define PCLK_I2C0		332
#define PCLK_I2C1		333
#define PCLK_I2C2		334
#define PCLK_I2C3		335
#define PCLK_I2C4		336
#define PCLK_I2C5		337
#define PCLK_SPI0		338
#define PCLK_SPI1		339
#define PCLK_SPI2		340
#define PCLK_UART0		341
#define PCLK_UART1		342
#define PCLK_UART2		343
#define PCLK_UART3		344
#define PCLK_UART4		345
#define PCLK_TSADC		346
#define PCLK_SARADC		347
#define PCLK_SIM		348
#define PCLK_GMAC		349
#define PCLK_PWM		350
#define PCLK_RKPWM		351
#define PCLK_PS2C		352
#define PCLK_TIMER		353
#define PCLK_TZPC		354
#define PCLK_EDP_CTRL		355
#define PCLK_MIPI_DSI0		356
#define PCLK_MIPI_DSI1		357
#define PCLK_MIPI_CSI		358
#define PCLK_LVDS_PHY		359
#define PCLK_HDMI_CTRL		360
#define PCLK_VIO2_H2P		361
#define PCLK_CPU		362
#define PCLK_PERI		363
#define PCLK_DDRUPCTL0		364
#define PCLK_PUBL0		365
#define PCLK_DDRUPCTL1		366
#define PCLK_PUBL1		367
#define PCLK_WDT		368
#define PCLK_EFUSE256		369
#define PCLK_EFUSE1024		370
#define PCLK_ISP_IN		371

/* hclk gates */
#define HCLK_GPS		448
#define HCLK_OTG0		449
#define HCLK_USBHOST0		450
#define HCLK_USBHOST1		451
#define HCLK_HSIC		452
#define HCLK_NANDC0		453
#define HCLK_NANDC1		454
#define HCLK_TSP		455
#define HCLK_SDMMC		456
#define HCLK_SDIO0		457
#define HCLK_SDIO1		458
#define HCLK_EMMC		459
#define HCLK_HSADC		460
#define HCLK_CRYPTO		461
#define HCLK_I2S0		462
#define HCLK_SPDIF		463
#define HCLK_SPDIF8CH		464
#define HCLK_VOP0		465
#define HCLK_VOP1		466
#define HCLK_ROM		467
#define HCLK_IEP		468
#define HCLK_ISP		469
#define HCLK_RGA		470
#define HCLK_VIO_AHB_ARBI	471
#define HCLK_VIO_NIU		472
#define HCLK_VIP		473
#define HCLK_VIO2_H2P		474
#define HCLK_HEVC		475
#define HCLK_VCODEC		476
#define HCLK_CPU		477
#define HCLK_PERI		478

#define CLK_NR_CLKS		(HCLK_PERI + 1)

/* soft-reset indices */
#define SRST_CORE0		0
#define SRST_CORE1		1
#define SRST_CORE2		2
#define SRST_CORE3		3
#define SRST_CORE0_PO		4
#define SRST_CORE1_PO		5
#define SRST_CORE2_PO		6
#define SRST_CORE3_PO		7
#define SRST_PDCORE_STRSYS	8
#define SRST_PDBUS_STRSYS	9
#define SRST_L2C		10
#define SRST_TOPDBG		11
#define SRST_CORE0_DBG		12
#define SRST_CORE1_DBG		13
#define SRST_CORE2_DBG		14
#define SRST_CORE3_DBG		15

#define SRST_PDBUG_AHB_ARBITOR	16
#define SRST_EFUSE256		17
#define SRST_DMAC1		18
#define SRST_INTMEM		19
#define SRST_ROM		20
#define SRST_SPDIF8CH		21
#define SRST_TIMER		22
#define SRST_I2S0		23
#define SRST_SPDIF		24
#define SRST_TIMER0		25
#define SRST_TIMER1		26
#define SRST_TIMER2		27
#define SRST_TIMER3		28
#define SRST_TIMER4		29
#define SRST_TIMER5		30
#define SRST_EFUSE		31

#define SRST_GPIO0		32
#define SRST_GPIO1		33
#define SRST_GPIO2		34
#define SRST_GPIO3		35
#define SRST_GPIO4		36
#define SRST_GPIO5		37
#define SRST_GPIO6		38
#define SRST_GPIO7		39
#define SRST_GPIO8		40
#define SRST_I2C0		42
#define SRST_I2C1		43
#define SRST_I2C2		44
#define SRST_I2C3		45
#define SRST_I2C4		46
#define SRST_I2C5		47

#define SRST_DWPWM		48
#define SRST_MMC_PERI		49
#define SRST_PERIPH_MMU		50
#define SRST_DAP		51
#define SRST_DAP_SYS		52
#define SRST_TPIU		53
#define SRST_PMU_APB		54
#define SRST_GRF		55
#define SRST_PMU		56
#define SRST_PERIPH_AXI		57
#define SRST_PERIPH_AHB		58
#define SRST_PERIPH_APB		59
#define SRST_PERIPH_NIU		60
#define SRST_PDPERI_AHB_ARBI	61
#define SRST_EMEM		62
#define SRST_USB_PERI		63

#define SRST_DMAC2		64
#define SRST_MAC		66
#define SRST_GPS		67
#define SRST_RKPWM		69
#define SRST_CCP		71
#define SRST_USBHOST0		72
#define SRST_HSIC		73
#define SRST_HSIC_AUX		74
#define SRST_HSIC_PHY		75
#define SRST_HSADC		76
#define SRST_NANDC0		77
#define SRST_NANDC1		78

#define SRST_TZPC		80
#define SRST_SPI0		83
#define SRST_SPI1		84
#define SRST_SPI2		85
#define SRST_SARADC		87
#define SRST_PDALIVE_NIU	88
#define SRST_PDPMU_INTMEM	89
#define SRST_PDPMU_NIU		90
#define SRST_SGRF		91

#define SRST_VIO_ARBI		96
#define SRST_RGA_NIU		97
#define SRST_VIO0_NIU_AXI	98
#define SRST_VIO_NIU_AHB	99
#define SRST_LCDC0_AXI		100
#define SRST_LCDC0_AHB		101
#define SRST_LCDC0_DCLK		102
#define SRST_VIO1_NIU_AXI	103
#define SRST_VIP		104
#define SRST_RGA_CORE		105
#define SRST_IEP_AXI		106
#define SRST_IEP_AHB		107
#define SRST_RGA_AXI		108
#define SRST_RGA_AHB		109
#define SRST_ISP		110
#define SRST_EDP		111

#define SRST_VCODEC_AXI		112
#define SRST_VCODEC_AHB		113
#define SRST_VIO_H2P		114
#define SRST_MIPIDSI0		115
#define SRST_MIPIDSI1		116
#define SRST_MIPICSI		117
#define SRST_LVDS_PHY		118
#define SRST_LVDS_CON		119
#define SRST_GPU		120
#define SRST_HDMI		121
#define SRST_CORE_PVTM		124
#define SRST_GPU_PVTM		125

#define SRST_MMC0		128
#define SRST_SDIO0		129
#define SRST_SDIO1		130
#define SRST_EMMC		131
#define SRST_USBOTG_AHB		132
#define SRST_USBOTG_PHY		133
#define SRST_USBOTG_CON		134
#define SRST_USBHOST0_AHB	135
#define SRST_USBHOST0_PHY	136
#define SRST_USBHOST0_CON	137
#define SRST_USBHOST1_AHB	138
#define SRST_USBHOST1_PHY	139
#define SRST_USBHOST1_CON	140
#define SRST_USB_ADP		141
#define SRST_ACC_EFUSE		142

#define SRST_CORESIGHT		144
#define SRST_PD_CORE_AHB_NOC	145
#define SRST_PD_CORE_APB_NOC	146
#define SRST_PD_CORE_MP_AXI	147
#define SRST_GIC		148
#define SRST_LCDC_PWM0		149
#define SRST_LCDC_PWM1		150
#define SRST_VIO0_H2P_BRG	151
#define SRST_VIO1_H2P_BRG	152
#define SRST_RGA_H2P_BRG	153
#define SRST_HEVC		154
#define SRST_TSADC		159

#define SRST_DDRPHY0		160
#define SRST_DDRPHY0_APB	161
#define SRST_DDRCTRL0		162
#define SRST_DDRCTRL0_APB	163
#define SRST_DDRPHY0_CTRL	164
#define SRST_DDRPHY1		165
#define SRST_DDRPHY1_APB	166
#define SRST_DDRCTRL1		167
#define SRST_DDRCTRL1_APB	168
#define SRST_DDRPHY1_CTRL	169
#define SRST_DDRMSCH0		170
#define SRST_DDRMSCH1		171
#define SRST_CRYPTO		174
#define SRST_C2C_HOST		175

#define SRST_LCDC1_AXI		176
#define SRST_LCDC1_AHB		177
#define SRST_LCDC1_DCLK		178
#define SRST_UART0		179
#define SRST_UART1		180
#define SRST_UART2		181
#define SRST_UART3		182
#define SRST_UART4		183
#define SRST_SIMC		186
#define SRST_PS2C		187
#define SRST_TSP		188
#define SRST_TSP_CLKIN0		189
#define SRST_TSP_CLKIN1		190
#define SRST_TSP_27M		191

#endif
