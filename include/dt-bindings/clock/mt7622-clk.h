<<<<<<< HEAD
/*
 * Copyright (c) 2017 MediaTek Inc.
 * Author: Chen Zhong <chen.zhong@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017 MediaTek Inc.
 * Author: Chen Zhong <chen.zhong@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _DT_BINDINGS_CLK_MT7622_H
#define _DT_BINDINGS_CLK_MT7622_H

/* TOPCKGEN */

#define CLK_TOP_TO_U2_PHY		0
#define CLK_TOP_TO_U2_PHY_1P		1
#define CLK_TOP_PCIE0_PIPE_EN		2
#define CLK_TOP_PCIE1_PIPE_EN		3
#define CLK_TOP_SSUSB_TX250M		4
#define CLK_TOP_SSUSB_EQ_RX250M		5
#define CLK_TOP_SSUSB_CDR_REF		6
#define CLK_TOP_SSUSB_CDR_FB		7
#define CLK_TOP_SATA_ASIC		8
#define CLK_TOP_SATA_RBC		9
#define CLK_TOP_TO_USB3_SYS		10
#define CLK_TOP_P1_1MHZ			11
#define CLK_TOP_4MHZ			12
#define CLK_TOP_P0_1MHZ			13
#define CLK_TOP_TXCLK_SRC_PRE		14
#define CLK_TOP_RTC			15
#define CLK_TOP_MEMPLL			16
#define CLK_TOP_DMPLL			17
#define CLK_TOP_SYSPLL_D2		18
#define CLK_TOP_SYSPLL1_D2		19
#define CLK_TOP_SYSPLL1_D4		20
#define CLK_TOP_SYSPLL1_D8		21
#define CLK_TOP_SYSPLL2_D4		22
#define CLK_TOP_SYSPLL2_D8		23
#define CLK_TOP_SYSPLL_D5		24
#define CLK_TOP_SYSPLL3_D2		25
#define CLK_TOP_SYSPLL3_D4		26
#define CLK_TOP_SYSPLL4_D2		27
#define CLK_TOP_SYSPLL4_D4		28
#define CLK_TOP_SYSPLL4_D16		29
#define CLK_TOP_UNIVPLL			30
#define CLK_TOP_UNIVPLL_D2		31
#define CLK_TOP_UNIVPLL1_D2		32
#define CLK_TOP_UNIVPLL1_D4		33
#define CLK_TOP_UNIVPLL1_D8		34
#define CLK_TOP_UNIVPLL1_D16		35
#define CLK_TOP_UNIVPLL2_D2		36
#define CLK_TOP_UNIVPLL2_D4		37
#define CLK_TOP_UNIVPLL2_D8		38
#define CLK_TOP_UNIVPLL2_D16		39
#define CLK_TOP_UNIVPLL_D5		40
#define CLK_TOP_UNIVPLL3_D2		41
#define CLK_TOP_UNIVPLL3_D4		42
#define CLK_TOP_UNIVPLL3_D16		43
#define CLK_TOP_UNIVPLL_D7		44
#define CLK_TOP_UNIVPLL_D80_D4		45
#define CLK_TOP_UNIV48M			46
#define CLK_TOP_SGMIIPLL		47
#define CLK_TOP_SGMIIPLL_D2		48
#define CLK_TOP_AUD1PLL			49
#define CLK_TOP_AUD2PLL			50
#define CLK_TOP_AUD_I2S2_MCK		51
#define CLK_TOP_TO_USB3_REF		52
#define CLK_TOP_PCIE1_MAC_EN		53
#define CLK_TOP_PCIE0_MAC_EN		54
#define CLK_TOP_ETH_500M		55
#define CLK_TOP_AXI_SEL			56
#define CLK_TOP_MEM_SEL			57
#define CLK_TOP_DDRPHYCFG_SEL		58
#define CLK_TOP_ETH_SEL			59
#define CLK_TOP_PWM_SEL			60
#define CLK_TOP_F10M_REF_SEL		61
#define CLK_TOP_NFI_INFRA_SEL		62
#define CLK_TOP_FLASH_SEL		63
#define CLK_TOP_UART_SEL		64
#define CLK_TOP_SPI0_SEL		65
#define CLK_TOP_SPI1_SEL		66
#define CLK_TOP_MSDC50_0_SEL		67
#define CLK_TOP_MSDC30_0_SEL		68
#define CLK_TOP_MSDC30_1_SEL		69
#define CLK_TOP_A1SYS_HP_SEL		70
#define CLK_TOP_A2SYS_HP_SEL		71
#define CLK_TOP_INTDIR_SEL		72
#define CLK_TOP_AUD_INTBUS_SEL		73
#define CLK_TOP_PMICSPI_SEL		74
#define CLK_TOP_SCP_SEL			75
#define CLK_TOP_ATB_SEL			76
#define CLK_TOP_HIF_SEL			77
#define CLK_TOP_AUDIO_SEL		78
#define CLK_TOP_U2_SEL			79
#define CLK_TOP_AUD1_SEL		80
#define CLK_TOP_AUD2_SEL		81
#define CLK_TOP_IRRX_SEL		82
#define CLK_TOP_IRTX_SEL		83
#define CLK_TOP_ASM_L_SEL		84
#define CLK_TOP_ASM_M_SEL		85
#define CLK_TOP_ASM_H_SEL		86
#define CLK_TOP_APLL1_SEL		87
#define CLK_TOP_APLL2_SEL		88
#define CLK_TOP_I2S0_MCK_SEL		89
#define CLK_TOP_I2S1_MCK_SEL		90
#define CLK_TOP_I2S2_MCK_SEL		91
#define CLK_TOP_I2S3_MCK_SEL		92
#define CLK_TOP_APLL1_DIV		93
#define CLK_TOP_APLL2_DIV		94
#define CLK_TOP_I2S0_MCK_DIV		95
#define CLK_TOP_I2S1_MCK_DIV		96
#define CLK_TOP_I2S2_MCK_DIV		97
#define CLK_TOP_I2S3_MCK_DIV		98
#define CLK_TOP_A1SYS_HP_DIV		99
#define CLK_TOP_A2SYS_HP_DIV		100
#define CLK_TOP_APLL1_DIV_PD		101
#define CLK_TOP_APLL2_DIV_PD		102
#define CLK_TOP_I2S0_MCK_DIV_PD		103
#define CLK_TOP_I2S1_MCK_DIV_PD		104
#define CLK_TOP_I2S2_MCK_DIV_PD		105
#define CLK_TOP_I2S3_MCK_DIV_PD		106
#define CLK_TOP_A1SYS_HP_DIV_PD		107
#define CLK_TOP_A2SYS_HP_DIV_PD		108
#define CLK_TOP_NR_CLK			109

/* INFRACFG */

#define CLK_INFRA_MUX1_SEL		0
#define CLK_INFRA_DBGCLK_PD		1
#define CLK_INFRA_AUDIO_PD		2
#define CLK_INFRA_IRRX_PD		3
#define CLK_INFRA_APXGPT_PD		4
#define CLK_INFRA_PMIC_PD		5
#define CLK_INFRA_TRNG			6
#define CLK_INFRA_NR_CLK		7

/* PERICFG */

#define CLK_PERIBUS_SEL			0
#define CLK_PERI_THERM_PD		1
#define CLK_PERI_PWM1_PD		2
#define CLK_PERI_PWM2_PD		3
#define CLK_PERI_PWM3_PD		4
#define CLK_PERI_PWM4_PD		5
#define CLK_PERI_PWM5_PD		6
#define CLK_PERI_PWM6_PD		7
#define CLK_PERI_PWM7_PD		8
#define CLK_PERI_PWM_PD			9
#define CLK_PERI_AP_DMA_PD		10
#define CLK_PERI_MSDC30_0_PD		11
#define CLK_PERI_MSDC30_1_PD		12
#define CLK_PERI_UART0_PD		13
#define CLK_PERI_UART1_PD		14
#define CLK_PERI_UART2_PD		15
#define CLK_PERI_UART3_PD		16
#define CLK_PERI_UART4_PD		17
#define CLK_PERI_BTIF_PD		18
#define CLK_PERI_I2C0_PD		19
#define CLK_PERI_I2C1_PD		20
#define CLK_PERI_I2C2_PD		21
#define CLK_PERI_SPI1_PD		22
#define CLK_PERI_AUXADC_PD		23
#define CLK_PERI_SPI0_PD		24
#define CLK_PERI_SNFI_PD		25
#define CLK_PERI_NFI_PD			26
#define CLK_PERI_NFIECC_PD		27
#define CLK_PERI_FLASH_PD		28
#define CLK_PERI_IRTX_PD		29
#define CLK_PERI_NR_CLK			30

/* APMIXEDSYS */

#define CLK_APMIXED_ARMPLL		0
#define CLK_APMIXED_MAINPLL		1
#define CLK_APMIXED_UNIV2PLL		2
#define CLK_APMIXED_ETH1PLL		3
#define CLK_APMIXED_ETH2PLL		4
#define CLK_APMIXED_AUD1PLL		5
#define CLK_APMIXED_AUD2PLL		6
#define CLK_APMIXED_TRGPLL		7
#define CLK_APMIXED_SGMIPLL		8
#define CLK_APMIXED_MAIN_CORE_EN	9
#define CLK_APMIXED_NR_CLK		10

/* AUDIOSYS */

#define CLK_AUDIO_AFE			0
#define CLK_AUDIO_HDMI			1
#define CLK_AUDIO_SPDF			2
#define CLK_AUDIO_APLL			3
#define CLK_AUDIO_I2SIN1		4
#define CLK_AUDIO_I2SIN2		5
#define CLK_AUDIO_I2SIN3		6
#define CLK_AUDIO_I2SIN4		7
#define CLK_AUDIO_I2SO1			8
#define CLK_AUDIO_I2SO2			9
#define CLK_AUDIO_I2SO3			10
#define CLK_AUDIO_I2SO4			11
#define CLK_AUDIO_ASRCI1		12
#define CLK_AUDIO_ASRCI2		13
#define CLK_AUDIO_ASRCO1		14
#define CLK_AUDIO_ASRCO2		15
#define CLK_AUDIO_INTDIR		16
#define CLK_AUDIO_A1SYS			17
#define CLK_AUDIO_A2SYS			18
#define CLK_AUDIO_UL1			19
#define CLK_AUDIO_UL2			20
#define CLK_AUDIO_UL3			21
#define CLK_AUDIO_UL4			22
#define CLK_AUDIO_UL5			23
#define CLK_AUDIO_UL6			24
#define CLK_AUDIO_DL1			25
#define CLK_AUDIO_DL2			26
#define CLK_AUDIO_DL3			27
#define CLK_AUDIO_DL4			28
#define CLK_AUDIO_DL5			29
#define CLK_AUDIO_DL6			30
#define CLK_AUDIO_DLMCH			31
#define CLK_AUDIO_ARB1			32
#define CLK_AUDIO_AWB			33
#define CLK_AUDIO_AWB2			34
#define CLK_AUDIO_DAI			35
#define CLK_AUDIO_MOD			36
#define CLK_AUDIO_ASRCI3		37
#define CLK_AUDIO_ASRCI4		38
#define CLK_AUDIO_ASRCO3		39
#define CLK_AUDIO_ASRCO4		40
#define CLK_AUDIO_MEM_ASRC1		41
#define CLK_AUDIO_MEM_ASRC2		42
#define CLK_AUDIO_MEM_ASRC3		43
#define CLK_AUDIO_MEM_ASRC4		44
#define CLK_AUDIO_MEM_ASRC5		45
#define CLK_AUDIO_AFE_CONN		46
#define CLK_AUDIO_NR_CLK		47

/* SSUSBSYS */

#define CLK_SSUSB_U2_PHY_1P_EN		0
#define CLK_SSUSB_U2_PHY_EN		1
#define CLK_SSUSB_REF_EN		2
#define CLK_SSUSB_SYS_EN		3
#define CLK_SSUSB_MCU_EN		4
#define CLK_SSUSB_DMA_EN		5
#define CLK_SSUSB_NR_CLK		6

/* PCIESYS */

#define CLK_PCIE_P1_AUX_EN		0
#define CLK_PCIE_P1_OBFF_EN		1
#define CLK_PCIE_P1_AHB_EN		2
#define CLK_PCIE_P1_AXI_EN		3
#define CLK_PCIE_P1_MAC_EN		4
#define CLK_PCIE_P1_PIPE_EN		5
#define CLK_PCIE_P0_AUX_EN		6
#define CLK_PCIE_P0_OBFF_EN		7
#define CLK_PCIE_P0_AHB_EN		8
#define CLK_PCIE_P0_AXI_EN		9
#define CLK_PCIE_P0_MAC_EN		10
#define CLK_PCIE_P0_PIPE_EN		11
#define CLK_SATA_AHB_EN			12
#define CLK_SATA_AXI_EN			13
#define CLK_SATA_ASIC_EN		14
#define CLK_SATA_RBC_EN			15
#define CLK_SATA_PM_EN			16
#define CLK_PCIE_NR_CLK			17

/* ETHSYS */

#define CLK_ETH_HSDMA_EN		0
#define CLK_ETH_ESW_EN			1
#define CLK_ETH_GP2_EN			2
#define CLK_ETH_GP1_EN			3
#define CLK_ETH_GP0_EN			4
#define CLK_ETH_NR_CLK			5

/* SGMIISYS */

#define CLK_SGMII_TX250M_EN		0
#define CLK_SGMII_RX250M_EN		1
#define CLK_SGMII_CDR_REF		2
#define CLK_SGMII_CDR_FB		3
#define CLK_SGMII_NR_CLK		4

#endif /* _DT_BINDINGS_CLK_MT7622_H */

