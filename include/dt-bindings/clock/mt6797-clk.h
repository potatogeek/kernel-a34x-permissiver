<<<<<<< HEAD
/*
 * Copyright (c) 2017 MediaTek Inc.
 * Author: Kevin Chen <kevin-cw.chen@mediatek.com>
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
 * Author: Kevin Chen <kevin-cw.chen@mediatek.com>
>>>>>>> upstream/android-13
 */

#ifndef _DT_BINDINGS_CLK_MT6797_H
#define _DT_BINDINGS_CLK_MT6797_H

/* TOPCKGEN */
#define	CLK_TOP_MUX_ULPOSC_AXI_CK_MUX_PRE	1
#define	CLK_TOP_MUX_ULPOSC_AXI_CK_MUX		2
#define	CLK_TOP_MUX_AXI				3
#define	CLK_TOP_MUX_MEM				4
#define	CLK_TOP_MUX_DDRPHYCFG			5
#define	CLK_TOP_MUX_MM				6
#define	CLK_TOP_MUX_PWM				7
#define	CLK_TOP_MUX_VDEC			8
#define	CLK_TOP_MUX_VENC			9
#define	CLK_TOP_MUX_MFG				10
#define	CLK_TOP_MUX_CAMTG			11
#define	CLK_TOP_MUX_UART			12
#define	CLK_TOP_MUX_SPI				13
#define	CLK_TOP_MUX_ULPOSC_SPI_CK_MUX		14
#define	CLK_TOP_MUX_USB20			15
#define	CLK_TOP_MUX_MSDC50_0_HCLK		16
#define	CLK_TOP_MUX_MSDC50_0			17
#define	CLK_TOP_MUX_MSDC30_1			18
#define	CLK_TOP_MUX_MSDC30_2			19
#define	CLK_TOP_MUX_AUDIO			20
#define	CLK_TOP_MUX_AUD_INTBUS			21
#define	CLK_TOP_MUX_PMICSPI			22
#define	CLK_TOP_MUX_SCP				23
#define	CLK_TOP_MUX_ATB				24
#define	CLK_TOP_MUX_MJC				25
#define	CLK_TOP_MUX_DPI0			26
#define	CLK_TOP_MUX_AUD_1			27
#define	CLK_TOP_MUX_AUD_2			28
#define	CLK_TOP_MUX_SSUSB_TOP_SYS		29
#define	CLK_TOP_MUX_SPM				30
#define	CLK_TOP_MUX_BSI_SPI			31
#define	CLK_TOP_MUX_AUDIO_H			32
#define	CLK_TOP_MUX_ANC_MD32			33
#define	CLK_TOP_MUX_MFG_52M			34
#define	CLK_TOP_SYSPLL_CK			35
#define	CLK_TOP_SYSPLL_D2			36
#define	CLK_TOP_SYSPLL1_D2			37
#define	CLK_TOP_SYSPLL1_D4			38
#define	CLK_TOP_SYSPLL1_D8			39
#define	CLK_TOP_SYSPLL1_D16			40
#define	CLK_TOP_SYSPLL_D3			41
#define	CLK_TOP_SYSPLL_D3_D3			42
#define	CLK_TOP_SYSPLL2_D2			43
#define	CLK_TOP_SYSPLL2_D4			44
#define	CLK_TOP_SYSPLL2_D8			45
#define	CLK_TOP_SYSPLL_D5			46
#define	CLK_TOP_SYSPLL3_D2			47
#define	CLK_TOP_SYSPLL3_D4			48
#define	CLK_TOP_SYSPLL_D7			49
#define	CLK_TOP_SYSPLL4_D2			50
#define	CLK_TOP_SYSPLL4_D4			51
#define	CLK_TOP_UNIVPLL_CK			52
#define	CLK_TOP_UNIVPLL_D7			53
#define	CLK_TOP_UNIVPLL_D26			54
#define	CLK_TOP_SSUSB_PHY_48M_CK		55
#define	CLK_TOP_USB_PHY48M_CK			56
#define	CLK_TOP_UNIVPLL_D2			57
#define	CLK_TOP_UNIVPLL1_D2			58
#define	CLK_TOP_UNIVPLL1_D4			59
#define	CLK_TOP_UNIVPLL1_D8			60
#define	CLK_TOP_UNIVPLL_D3			61
#define	CLK_TOP_UNIVPLL2_D2			62
#define	CLK_TOP_UNIVPLL2_D4			63
#define	CLK_TOP_UNIVPLL2_D8			64
#define	CLK_TOP_UNIVPLL_D5			65
#define	CLK_TOP_UNIVPLL3_D2			66
#define	CLK_TOP_UNIVPLL3_D4			67
#define	CLK_TOP_UNIVPLL3_D8			68
#define	CLK_TOP_ULPOSC_CK_ORG			69
#define	CLK_TOP_ULPOSC_CK			70
#define	CLK_TOP_ULPOSC_D2			71
#define	CLK_TOP_ULPOSC_D3			72
#define	CLK_TOP_ULPOSC_D4			73
#define	CLK_TOP_ULPOSC_D8			74
#define	CLK_TOP_ULPOSC_D10			75
#define	CLK_TOP_APLL1_CK			76
#define	CLK_TOP_APLL2_CK			77
#define	CLK_TOP_MFGPLL_CK			78
#define	CLK_TOP_MFGPLL_D2			79
#define	CLK_TOP_IMGPLL_CK			80
#define	CLK_TOP_IMGPLL_D2			81
#define	CLK_TOP_IMGPLL_D4			82
#define	CLK_TOP_CODECPLL_CK			83
#define	CLK_TOP_CODECPLL_D2			84
#define	CLK_TOP_VDECPLL_CK			85
#define	CLK_TOP_TVDPLL_CK			86
#define	CLK_TOP_TVDPLL_D2			87
#define	CLK_TOP_TVDPLL_D4			88
#define	CLK_TOP_TVDPLL_D8			89
#define	CLK_TOP_TVDPLL_D16			90
#define	CLK_TOP_MSDCPLL_CK			91
#define	CLK_TOP_MSDCPLL_D2			92
#define	CLK_TOP_MSDCPLL_D4			93
#define	CLK_TOP_MSDCPLL_D8			94
#define	CLK_TOP_NR				95

/* APMIXED_SYS */
#define CLK_APMIXED_MAINPLL			1
#define CLK_APMIXED_UNIVPLL			2
#define CLK_APMIXED_MFGPLL			3
#define CLK_APMIXED_MSDCPLL			4
#define CLK_APMIXED_IMGPLL			5
#define CLK_APMIXED_TVDPLL			6
#define CLK_APMIXED_CODECPLL			7
#define CLK_APMIXED_VDECPLL			8
#define CLK_APMIXED_APLL1			9
#define CLK_APMIXED_APLL2			10
#define CLK_APMIXED_NR				11

/* INFRA_SYS */
#define	CLK_INFRA_PMIC_TMR			1
#define	CLK_INFRA_PMIC_AP			2
#define	CLK_INFRA_PMIC_MD			3
#define	CLK_INFRA_PMIC_CONN			4
#define	CLK_INFRA_SCP				5
#define	CLK_INFRA_SEJ				6
#define	CLK_INFRA_APXGPT			7
#define	CLK_INFRA_SEJ_13M			8
#define	CLK_INFRA_ICUSB				9
#define	CLK_INFRA_GCE				10
#define	CLK_INFRA_THERM				11
#define	CLK_INFRA_I2C0				12
#define	CLK_INFRA_I2C1				13
#define	CLK_INFRA_I2C2				14
#define	CLK_INFRA_I2C3				15
#define	CLK_INFRA_PWM_HCLK			16
#define	CLK_INFRA_PWM1				17
#define	CLK_INFRA_PWM2				18
#define	CLK_INFRA_PWM3				19
#define	CLK_INFRA_PWM4				20
#define	CLK_INFRA_PWM				21
#define	CLK_INFRA_UART0				22
#define	CLK_INFRA_UART1				23
#define	CLK_INFRA_UART2				24
#define	CLK_INFRA_UART3				25
#define	CLK_INFRA_MD2MD_CCIF_0			26
#define	CLK_INFRA_MD2MD_CCIF_1			27
#define	CLK_INFRA_MD2MD_CCIF_2			28
#define	CLK_INFRA_FHCTL				29
#define	CLK_INFRA_BTIF				30
#define	CLK_INFRA_MD2MD_CCIF_3			31
#define	CLK_INFRA_SPI				32
#define	CLK_INFRA_MSDC0				33
#define	CLK_INFRA_MD2MD_CCIF_4			34
#define	CLK_INFRA_MSDC1				35
#define	CLK_INFRA_MSDC2				36
#define	CLK_INFRA_MD2MD_CCIF_5			37
#define	CLK_INFRA_GCPU				38
#define	CLK_INFRA_TRNG				39
#define	CLK_INFRA_AUXADC			40
#define	CLK_INFRA_CPUM				41
#define	CLK_INFRA_AP_C2K_CCIF_0			42
#define	CLK_INFRA_AP_C2K_CCIF_1			43
#define	CLK_INFRA_CLDMA				44
#define	CLK_INFRA_DISP_PWM			45
#define	CLK_INFRA_AP_DMA			46
#define	CLK_INFRA_DEVICE_APC			47
#define	CLK_INFRA_L2C_SRAM			48
#define	CLK_INFRA_CCIF_AP			49
#define	CLK_INFRA_AUDIO				50
#define	CLK_INFRA_CCIF_MD			51
#define	CLK_INFRA_DRAMC_F26M			52
#define	CLK_INFRA_I2C4				53
#define	CLK_INFRA_I2C_APPM			54
#define	CLK_INFRA_I2C_GPUPM			55
#define	CLK_INFRA_I2C2_IMM			56
#define	CLK_INFRA_I2C2_ARB			57
#define	CLK_INFRA_I2C3_IMM			58
#define	CLK_INFRA_I2C3_ARB			59
#define	CLK_INFRA_I2C5				60
#define	CLK_INFRA_SYS_CIRQ			61
#define	CLK_INFRA_SPI1				62
#define	CLK_INFRA_DRAMC_B_F26M			63
#define	CLK_INFRA_ANC_MD32			64
#define	CLK_INFRA_ANC_MD32_32K			65
#define	CLK_INFRA_DVFS_SPM1			66
#define	CLK_INFRA_AES_TOP0			67
#define	CLK_INFRA_AES_TOP1			68
#define	CLK_INFRA_SSUSB_BUS			69
#define	CLK_INFRA_SPI2				70
#define	CLK_INFRA_SPI3				71
#define	CLK_INFRA_SPI4				72
#define	CLK_INFRA_SPI5				73
#define	CLK_INFRA_IRTX				74
#define	CLK_INFRA_SSUSB_SYS			75
#define	CLK_INFRA_SSUSB_REF			76
#define	CLK_INFRA_AUDIO_26M			77
#define	CLK_INFRA_AUDIO_26M_PAD_TOP		78
#define	CLK_INFRA_MODEM_TEMP_SHARE		79
#define	CLK_INFRA_VAD_WRAP_SOC			80
#define	CLK_INFRA_DRAMC_CONF			81
#define	CLK_INFRA_DRAMC_B_CONF			82
#define	CLK_INFRA_MFG_VCG			83
#define	CLK_INFRA_13M				84
#define	CLK_INFRA_NR				85

/* IMG_SYS */
#define	CLK_IMG_FDVT				1
#define	CLK_IMG_DPE				2
#define	CLK_IMG_DIP				3
#define	CLK_IMG_LARB6				4
#define	CLK_IMG_NR				5

/* MM_SYS */
#define	CLK_MM_SMI_COMMON			1
#define	CLK_MM_SMI_LARB0			2
#define	CLK_MM_SMI_LARB5			3
#define	CLK_MM_CAM_MDP				4
#define	CLK_MM_MDP_RDMA0			5
#define	CLK_MM_MDP_RDMA1			6
#define	CLK_MM_MDP_RSZ0				7
#define	CLK_MM_MDP_RSZ1				8
#define	CLK_MM_MDP_RSZ2				9
#define	CLK_MM_MDP_TDSHP			10
#define	CLK_MM_MDP_COLOR			11
#define	CLK_MM_MDP_WDMA				12
#define	CLK_MM_MDP_WROT0			13
#define	CLK_MM_MDP_WROT1			14
#define	CLK_MM_FAKE_ENG				15
#define	CLK_MM_DISP_OVL0			16
#define	CLK_MM_DISP_OVL1			17
#define	CLK_MM_DISP_OVL0_2L			18
#define	CLK_MM_DISP_OVL1_2L			19
#define	CLK_MM_DISP_RDMA0			20
#define	CLK_MM_DISP_RDMA1			21
#define	CLK_MM_DISP_WDMA0			22
#define	CLK_MM_DISP_WDMA1			23
#define	CLK_MM_DISP_COLOR			24
#define	CLK_MM_DISP_CCORR			25
#define	CLK_MM_DISP_AAL				26
#define	CLK_MM_DISP_GAMMA			27
#define	CLK_MM_DISP_OD				28
#define	CLK_MM_DISP_DITHER			29
#define	CLK_MM_DISP_UFOE			30
#define	CLK_MM_DISP_DSC				31
#define	CLK_MM_DISP_SPLIT			32
#define	CLK_MM_DSI0_MM_CLOCK			33
#define	CLK_MM_DSI1_MM_CLOCK			34
#define	CLK_MM_DPI_MM_CLOCK			35
#define	CLK_MM_DPI_INTERFACE_CLOCK		36
#define	CLK_MM_LARB4_AXI_ASIF_MM_CLOCK		37
#define	CLK_MM_LARB4_AXI_ASIF_MJC_CLOCK		38
#define	CLK_MM_DISP_OVL0_MOUT_CLOCK		39
#define	CLK_MM_FAKE_ENG2			40
#define	CLK_MM_DSI0_INTERFACE_CLOCK		41
#define	CLK_MM_DSI1_INTERFACE_CLOCK		42
#define	CLK_MM_NR				43

/* VDEC_SYS */
#define	CLK_VDEC_CKEN_ENG			1
#define	CLK_VDEC_ACTIVE				2
#define	CLK_VDEC_CKEN				3
#define	CLK_VDEC_LARB1_CKEN			4
#define	CLK_VDEC_NR				5

/* VENC_SYS */
#define	CLK_VENC_0				1
#define	CLK_VENC_1				2
#define	CLK_VENC_2				3
#define	CLK_VENC_3				4
#define	CLK_VENC_NR				5

#endif /* _DT_BINDINGS_CLK_MT6797_H */
