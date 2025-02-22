<<<<<<< HEAD
/*
 * Copyright 2015 Linaro Limited
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
 * Copyright 2015 Linaro Limited
>>>>>>> upstream/android-13
 */

#ifndef _DT_BINDINGS_CLK_MSM_GCC_8916_H
#define _DT_BINDINGS_CLK_MSM_GCC_8916_H

#define GPLL0					0
#define GPLL0_VOTE				1
#define BIMC_PLL				2
#define BIMC_PLL_VOTE				3
#define GPLL1					4
#define GPLL1_VOTE				5
#define GPLL2					6
#define GPLL2_VOTE				7
#define PCNOC_BFDCD_CLK_SRC			8
#define SYSTEM_NOC_BFDCD_CLK_SRC		9
#define CAMSS_AHB_CLK_SRC			10
#define APSS_AHB_CLK_SRC			11
#define CSI0_CLK_SRC				12
#define CSI1_CLK_SRC				13
#define GFX3D_CLK_SRC				14
#define VFE0_CLK_SRC				15
#define BLSP1_QUP1_I2C_APPS_CLK_SRC		16
#define BLSP1_QUP1_SPI_APPS_CLK_SRC		17
#define BLSP1_QUP2_I2C_APPS_CLK_SRC		18
#define BLSP1_QUP2_SPI_APPS_CLK_SRC		19
#define BLSP1_QUP3_I2C_APPS_CLK_SRC		20
#define BLSP1_QUP3_SPI_APPS_CLK_SRC		21
#define BLSP1_QUP4_I2C_APPS_CLK_SRC		22
#define BLSP1_QUP4_SPI_APPS_CLK_SRC		23
#define BLSP1_QUP5_I2C_APPS_CLK_SRC		24
#define BLSP1_QUP5_SPI_APPS_CLK_SRC		25
#define BLSP1_QUP6_I2C_APPS_CLK_SRC		26
#define BLSP1_QUP6_SPI_APPS_CLK_SRC		27
#define BLSP1_UART1_APPS_CLK_SRC		28
#define BLSP1_UART2_APPS_CLK_SRC		29
#define CCI_CLK_SRC				30
#define CAMSS_GP0_CLK_SRC			31
#define CAMSS_GP1_CLK_SRC			32
#define JPEG0_CLK_SRC				33
#define MCLK0_CLK_SRC				34
#define MCLK1_CLK_SRC				35
#define CSI0PHYTIMER_CLK_SRC			36
#define CSI1PHYTIMER_CLK_SRC			37
#define CPP_CLK_SRC				38
#define CRYPTO_CLK_SRC				39
#define GP1_CLK_SRC				40
#define GP2_CLK_SRC				41
#define GP3_CLK_SRC				42
#define BYTE0_CLK_SRC				43
#define ESC0_CLK_SRC				44
#define MDP_CLK_SRC				45
#define PCLK0_CLK_SRC				46
#define VSYNC_CLK_SRC				47
#define PDM2_CLK_SRC				48
#define SDCC1_APPS_CLK_SRC			49
#define SDCC2_APPS_CLK_SRC			50
#define APSS_TCU_CLK_SRC			51
#define USB_HS_SYSTEM_CLK_SRC			52
#define VCODEC0_CLK_SRC				53
#define GCC_BLSP1_AHB_CLK			54
#define GCC_BLSP1_SLEEP_CLK			55
#define GCC_BLSP1_QUP1_I2C_APPS_CLK		56
#define GCC_BLSP1_QUP1_SPI_APPS_CLK		57
#define GCC_BLSP1_QUP2_I2C_APPS_CLK		58
#define GCC_BLSP1_QUP2_SPI_APPS_CLK		59
#define GCC_BLSP1_QUP3_I2C_APPS_CLK		60
#define GCC_BLSP1_QUP3_SPI_APPS_CLK		61
#define GCC_BLSP1_QUP4_I2C_APPS_CLK		62
#define GCC_BLSP1_QUP4_SPI_APPS_CLK		63
#define GCC_BLSP1_QUP5_I2C_APPS_CLK		64
#define GCC_BLSP1_QUP5_SPI_APPS_CLK		65
#define GCC_BLSP1_QUP6_I2C_APPS_CLK		66
#define GCC_BLSP1_QUP6_SPI_APPS_CLK		67
#define GCC_BLSP1_UART1_APPS_CLK		68
#define GCC_BLSP1_UART2_APPS_CLK		69
#define GCC_BOOT_ROM_AHB_CLK			70
#define GCC_CAMSS_CCI_AHB_CLK			71
#define GCC_CAMSS_CCI_CLK			72
#define GCC_CAMSS_CSI0_AHB_CLK			73
#define GCC_CAMSS_CSI0_CLK			74
#define GCC_CAMSS_CSI0PHY_CLK			75
#define GCC_CAMSS_CSI0PIX_CLK			76
#define GCC_CAMSS_CSI0RDI_CLK			77
#define GCC_CAMSS_CSI1_AHB_CLK			78
#define GCC_CAMSS_CSI1_CLK			79
#define GCC_CAMSS_CSI1PHY_CLK			80
#define GCC_CAMSS_CSI1PIX_CLK			81
#define GCC_CAMSS_CSI1RDI_CLK			82
#define GCC_CAMSS_CSI_VFE0_CLK			83
#define GCC_CAMSS_GP0_CLK			84
#define GCC_CAMSS_GP1_CLK			85
#define GCC_CAMSS_ISPIF_AHB_CLK			86
#define GCC_CAMSS_JPEG0_CLK			87
#define GCC_CAMSS_JPEG_AHB_CLK			88
#define GCC_CAMSS_JPEG_AXI_CLK			89
#define GCC_CAMSS_MCLK0_CLK			90
#define GCC_CAMSS_MCLK1_CLK			91
#define GCC_CAMSS_MICRO_AHB_CLK			92
#define GCC_CAMSS_CSI0PHYTIMER_CLK		93
#define GCC_CAMSS_CSI1PHYTIMER_CLK		94
#define GCC_CAMSS_AHB_CLK			95
#define GCC_CAMSS_TOP_AHB_CLK			96
#define GCC_CAMSS_CPP_AHB_CLK			97
#define GCC_CAMSS_CPP_CLK			98
#define GCC_CAMSS_VFE0_CLK			99
#define GCC_CAMSS_VFE_AHB_CLK			100
#define GCC_CAMSS_VFE_AXI_CLK			101
#define GCC_CRYPTO_AHB_CLK			102
#define GCC_CRYPTO_AXI_CLK			103
#define GCC_CRYPTO_CLK				104
#define GCC_OXILI_GMEM_CLK			105
#define GCC_GP1_CLK				106
#define GCC_GP2_CLK				107
#define GCC_GP3_CLK				108
#define GCC_MDSS_AHB_CLK			109
#define GCC_MDSS_AXI_CLK			110
#define GCC_MDSS_BYTE0_CLK			111
#define GCC_MDSS_ESC0_CLK			112
#define GCC_MDSS_MDP_CLK			113
#define GCC_MDSS_PCLK0_CLK			114
#define GCC_MDSS_VSYNC_CLK			115
#define GCC_MSS_CFG_AHB_CLK			116
#define GCC_OXILI_AHB_CLK			117
#define GCC_OXILI_GFX3D_CLK			118
#define GCC_PDM2_CLK				119
#define GCC_PDM_AHB_CLK				120
#define GCC_PRNG_AHB_CLK			121
#define GCC_SDCC1_AHB_CLK			122
#define GCC_SDCC1_APPS_CLK			123
#define GCC_SDCC2_AHB_CLK			124
#define GCC_SDCC2_APPS_CLK			125
#define GCC_GTCU_AHB_CLK			126
#define GCC_JPEG_TBU_CLK			127
#define GCC_MDP_TBU_CLK				128
#define GCC_SMMU_CFG_CLK			129
#define GCC_VENUS_TBU_CLK			130
#define GCC_VFE_TBU_CLK				131
#define GCC_USB2A_PHY_SLEEP_CLK			132
#define GCC_USB_HS_AHB_CLK			133
#define GCC_USB_HS_SYSTEM_CLK			134
#define GCC_VENUS0_AHB_CLK			135
#define GCC_VENUS0_AXI_CLK			136
#define GCC_VENUS0_VCODEC0_CLK			137
#define BIMC_DDR_CLK_SRC			138
#define GCC_APSS_TCU_CLK			139
#define GCC_GFX_TCU_CLK				140
#define BIMC_GPU_CLK_SRC			141
#define GCC_BIMC_GFX_CLK			142
#define GCC_BIMC_GPU_CLK			143
#define ULTAUDIO_LPAIF_PRI_I2S_CLK_SRC		144
#define ULTAUDIO_LPAIF_SEC_I2S_CLK_SRC		145
#define ULTAUDIO_LPAIF_AUX_I2S_CLK_SRC		146
#define ULTAUDIO_XO_CLK_SRC			147
#define ULTAUDIO_AHBFABRIC_CLK_SRC		148
#define CODEC_DIGCODEC_CLK_SRC			149
#define GCC_ULTAUDIO_PCNOC_MPORT_CLK		150
#define GCC_ULTAUDIO_PCNOC_SWAY_CLK		151
#define GCC_ULTAUDIO_AVSYNC_XO_CLK		152
#define GCC_ULTAUDIO_STC_XO_CLK			153
#define GCC_ULTAUDIO_AHBFABRIC_IXFABRIC_CLK	154
#define GCC_ULTAUDIO_AHBFABRIC_IXFABRIC_LPM_CLK	155
#define GCC_ULTAUDIO_LPAIF_PRI_I2S_CLK		156
#define GCC_ULTAUDIO_LPAIF_SEC_I2S_CLK		157
#define GCC_ULTAUDIO_LPAIF_AUX_I2S_CLK		158
#define GCC_CODEC_DIGCODEC_CLK			159
#define GCC_MSS_Q6_BIMC_AXI_CLK			160

/* Indexes for GDSCs */
#define BIMC_GDSC				0
#define VENUS_GDSC				1
#define MDSS_GDSC				2
#define JPEG_GDSC				3
#define VFE_GDSC				4
#define OXILI_GDSC				5

#endif
