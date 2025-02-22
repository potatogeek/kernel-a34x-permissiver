<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * cs35l34.h -- CS35L34 ALSA SoC audio driver
 *
 * Copyright 2016 Cirrus Logic, Inc.
 *
 * Author: Paul Handrigan <Paul.Handrigan@cirrus.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __CS35L34_H__
#define __CS35L34_H__

#define CS35L34_CHIP_ID			0x00035A34
#define CS35L34_DEVID_AB		0x01	/* Device ID A & B [RO] */
#define CS35L34_DEVID_CD		0x02    /* Device ID C & D [RO] */
#define CS35L34_DEVID_E			0x03    /* Device ID E [RO] */
#define CS35L34_FAB_ID			0x04	/* Fab ID [RO] */
#define CS35L34_REV_ID			0x05	/* Revision ID [RO] */
#define CS35L34_PWRCTL1			0x06    /* Power Ctl 1 */
#define CS35L34_PWRCTL2			0x07    /* Power Ctl 2 */
#define CS35L34_PWRCTL3			0x08	/* Power Ctl 3 */
#define CS35L34_ADSP_CLK_CTL		0x0A	/* (ADSP) Clock Ctl */
#define CS35L34_MCLK_CTL		0x0B	/* Master Clocking Ctl */
#define CS35L34_AMP_INP_DRV_CTL		0x14	/* Amp Input Drive Ctl */
#define CS35L34_AMP_DIG_VOL_CTL		0x15	/* Amplifier Dig Volume Ctl */
#define CS35L34_AMP_DIG_VOL		0x16	/* Amplifier Dig Volume */
#define CS35L34_AMP_ANLG_GAIN_CTL	0x17	/* Amplifier Analog Gain Ctl */
#define CS35L34_PROTECT_CTL		0x18	/* Amp Gain - Prot Ctl Param */
#define CS35L34_AMP_KEEP_ALIVE_CTL	0x1A	/* Amplifier Keep Alive Ctl */
#define CS35L34_BST_CVTR_V_CTL		0x1D	/* Boost Conv Voltage Ctl */
#define CS35L34_BST_PEAK_I		0x1E	/* Boost Conv Peak Current */
#define CS35L34_BST_RAMP_CTL		0x20	/* Boost Conv Soft Ramp Ctl */
#define CS35L34_BST_CONV_COEF_1		0x21	/* Boost Conv Coefficients 1 */
#define CS35L34_BST_CONV_COEF_2		0x22	/* Boost Conv Coefficients 2 */
#define CS35L34_BST_CONV_SLOPE_COMP	0x23	/* Boost Conv Slope Comp */
#define CS35L34_BST_CONV_SW_FREQ	0x24	/* Boost Conv L BST SW Freq */
#define CS35L34_CLASS_H_CTL		0x30	/* CLS H Control */
#define CS35L34_CLASS_H_HEADRM_CTL	0x31	/* CLS H Headroom Ctl */
#define CS35L34_CLASS_H_RELEASE_RATE	0x32	/* CLS H Release Rate */
#define CS35L34_CLASS_H_FET_DRIVE_CTL	0x33	/* CLS H Weak FET Drive Ctl */
#define CS35L34_CLASS_H_STATUS		0x38	/* CLS H Status */
#define CS35L34_VPBR_CTL		0x3A	/* VPBR Ctl */
#define CS35L34_VPBR_VOL_CTL		0x3B	/* VPBR Volume Ctl */
#define CS35L34_VPBR_TIMING_CTL		0x3C	/* VPBR Timing Ctl */
#define CS35L34_PRED_MAX_ATTEN_SPK_LOAD	0x40	/* PRD Max Atten / Spkr Load */
#define CS35L34_PRED_BROWNOUT_THRESH	0x41	/* PRD Brownout Threshold */
#define CS35L34_PRED_BROWNOUT_VOL_CTL	0x42	/* PRD Brownout Volume Ctl */
#define CS35L34_PRED_BROWNOUT_RATE_CTL	0x43	/* PRD Brownout Rate Ctl */
#define CS35L34_PRED_WAIT_CTL		0x44	/* PRD Wait Ctl */
#define CS35L34_PRED_ZVP_INIT_IMP_CTL	0x46	/* PRD ZVP Initial Imp Ctl */
#define CS35L34_PRED_MAN_SAFE_VPI_CTL	0x47	/* PRD Manual Safe VPI Ctl */
#define CS35L34_VPBR_ATTEN_STATUS	0x4B	/* VPBR Attenuation Status */
#define CS35L34_PRED_BRWNOUT_ATT_STATUS	0x4C	/* PRD Brownout Atten Status */
#define CS35L34_SPKR_MON_CTL		0x4E	/* Speaker Monitoring Ctl */
#define CS35L34_ADSP_I2S_CTL		0x50	/* ADSP I2S Ctl */
#define CS35L34_ADSP_TDM_CTL		0x51	/* ADSP TDM Ctl */
#define CS35L34_TDM_TX_CTL_1_VMON	0x52	/* TDM TX Ctl 1 (VMON) */
#define CS35L34_TDM_TX_CTL_2_IMON	0x53	/* TDM TX Ctl 2 (IMON) */
#define CS35L34_TDM_TX_CTL_3_VPMON	0x54	/* TDM TX Ctl 3 (VPMON) */
#define CS35L34_TDM_TX_CTL_4_VBSTMON	0x55	/* TDM TX Ctl 4 (VBSTMON) */
#define CS35L34_TDM_TX_CTL_5_FLAG1	0x56	/* TDM TX Ctl 5 (FLAG1) */
#define CS35L34_TDM_TX_CTL_6_FLAG2	0x57	/* TDM TX Ctl 6 (FLAG2) */
#define CS35L34_TDM_TX_SLOT_EN_1	0x5A	/* TDM TX Slot Enable */
#define CS35L34_TDM_TX_SLOT_EN_2	0x5B	/* TDM TX Slot Enable */
#define CS35L34_TDM_TX_SLOT_EN_3	0x5C	/* TDM TX Slot Enable */
#define CS35L34_TDM_TX_SLOT_EN_4	0x5D	/* TDM TX Slot Enable */
#define CS35L34_TDM_RX_CTL_1_AUDIN	0x5E	/* TDM RX Ctl 1 */
#define CS35L34_TDM_RX_CTL_3_ALIVE	0x60	/* TDM RX Ctl 3 (ALIVE) */
#define CS35L34_MULT_DEV_SYNCH1		0x62	/* Multidevice Synch */
#define CS35L34_MULT_DEV_SYNCH2		0x63	/* Multidevice Synch 2 */
#define CS35L34_PROT_RELEASE_CTL	0x64	/* Protection Release Ctl */
#define CS35L34_DIAG_MODE_REG_LOCK	0x68	/* Diagnostic Mode Reg Lock */
#define CS35L34_DIAG_MODE_CTL_1		0x69	/* Diagnostic Mode Ctl 1 */
#define CS35L34_DIAG_MODE_CTL_2		0x6A	/* Diagnostic Mode Ctl 2 */
#define CS35L34_INT_MASK_1		0x70	/* Interrupt Mask 1 */
#define CS35L34_INT_MASK_2		0x71	/* Interrupt Mask 2 */
#define CS35L34_INT_MASK_3		0x72	/* Interrupt Mask 3 */
#define CS35L34_INT_MASK_4		0x73	/* Interrupt Mask 4 */
#define CS35L34_INT_STATUS_1		0x74	/* Interrupt Status 1 */
#define CS35L34_INT_STATUS_2		0x75	/* Interrupt Status 2 */
#define CS35L34_INT_STATUS_3		0x76	/* Interrupt Status 3 */
#define CS35L34_INT_STATUS_4		0x77	/* Interrupt Status 4 */
#define CS35L34_OTP_TRIM_STATUS		0x7E	/* OTP Trim Status */

#define CS35L34_MAX_REGISTER		0x7F
#define CS35L34_REGISTER_COUNT		0x4E

#define CS35L34_MCLK_5644		5644800
#define CS35L34_MCLK_6144		6144000
#define CS35L34_MCLK_6			6000000
#define CS35L34_MCLK_11289		11289600
#define CS35L34_MCLK_12			12000000
#define CS35L34_MCLK_12288		12288000

/* CS35L34_PWRCTL1 */
#define CS35L34_SFT_RST			(1 << 7)
#define CS35L34_DISCHG_FLT		(1 << 1)
#define CS35L34_PDN_ALL			1

/* CS35L34_PWRCTL2 */
#define CS35L34_PDN_VMON		(1 << 7)
#define CS35L34_PDN_IMON		(1 << 6)
#define CS35L34_PDN_CLASSH		(1 << 5)
#define CS35L34_PDN_VPBR		(1 << 4)
#define CS35L34_PDN_PRED		(1 << 3)
#define CS35L34_PDN_BST			(1 << 2)
#define CS35L34_PDN_AMP			1

/* CS35L34_PWRCTL3 */
#define CS35L34_MCLK_DIS		(1 << 7)
#define CS35L34_PDN_VBSTMON_OUT		(1 << 4)
#define CS35L34_PDN_VMON_OUT		(1 << 3)
/* Tristate the ADSP SDOUT when in I2C mode */
#define CS35L34_PDN_SDOUT		(1 << 2)
#define CS35L34_PDN_SDIN		(1 << 1)
#define CS35L34_PDN_TDM			1

/* CS35L34_ADSP_CLK_CTL */
#define CS35L34_ADSP_RATE		0xF
#define CS35L34_ADSP_DRIVE		(1 << 4)
#define CS35L34_ADSP_M_S		(1 << 7)

/* CS35L34_MCLK_CTL */
#define CS35L34_MCLK_DIV		(1 << 4)
#define CS35L34_MCLK_RATE_MASK		0x7
#define CS35L34_MCLK_RATE_6P1440	0x2
#define CS35L34_MCLK_RATE_6P0000	0x1
#define CS35L34_MCLK_RATE_5P6448	0x0
#define CS35L34_MCLKDIS			(1 << 7)
#define CS35L34_MCLKDIV2		(1 << 6)
#define CS35L34_SDOUT_3ST_TDM		(1 << 5)
#define CS35L34_INT_FS_RATE		(1 << 4)
#define CS35L34_ADSP_FS			0xF

/* CS35L34_AMP_INP_DRV_CTL */
#define CS35L34_DRV_STR_SRC		(1 << 1)
#define CS35L34_DRV_STR			1

/* CS35L34_AMP_DIG_VOL_CTL */
#define CS35L34_AMP_DSR_RATE_MASK	0xF0
#define CS35L34_AMP_DSR_RATE_SHIFT	(1 << 4)
#define CS35L34_NOTCH_DIS		(1 << 3)
#define CS35L34_AMP_DIGSFT		(1 << 1)
#define CS35L34_INV			1

/* CS35L34_PROTECT_CTL */
#define CS35L34_OTW_ATTN_MASK		0xC
#define CS35L34_OTW_THRD_MASK		0x3
#define CS35L34_MUTE			(1 << 5)
#define CS35L34_GAIN_ZC			(1 << 4)
#define CS35L34_GAIN_ZC_MASK		0x10
#define CS35L34_GAIN_ZC_SHIFT		4

/* CS35L34_AMP_KEEP_ALIVE_CTL */
#define CS35L34_ALIVE_WD_DIS		(1 << 2)

/* CS35L34_BST_CVTR_V_CTL */
#define CS35L34_BST_CVTL_MASK		0x3F

/* CS35L34_BST_PEAK_I */
#define CS35L34_BST_PEAK_MASK		0x3F

/* CS35L34_ADSP_I2S_CTL */
#define CS35L34_I2S_LOC_MASK		0xC
#define CS35L34_I2S_LOC_SHIFT		2

/* CS35L34_MULT_DEV_SYNCH2 */
#define CS35L34_SYNC2_MASK		0xF

/* CS35L34_PROT_RELEASE_CTL */
#define CS35L34_CAL_ERR_RLS		(1 << 7)
#define CS35L34_SHORT_RLS		(1 << 2)
#define CS35L34_OTW_RLS			(1 << 1)
#define CS35L34_OTE_RLS			1

/* CS35L34_INT_MASK_1 */
#define CS35L34_M_CAL_ERR_SHIFT		7
#define CS35L34_M_CAL_ERR		(1 << CS35L34_M_CAL_ERR_SHIFT)
#define CS35L34_M_ALIVE_ERR_SHIFT	5
#define CS35L34_M_ALIVE_ERR		(1 << CS35L34_M_ALIVE_ERR_SHIFT)
#define CS35L34_M_ADSP_CLK_SHIFT	4
#define CS35L34_M_ADSP_CLK_ERR		(1 << CS35L34_M_ADSP_CLK_SHIFT)
#define CS35L34_M_MCLK_SHIFT		3
#define CS35L34_M_MCLK_ERR		(1 << CS35L34_M_MCLK_SHIFT)
#define CS35L34_M_AMP_SHORT_SHIFT	2
#define CS35L34_M_AMP_SHORT		(1 << CS35L34_M_AMP_SHORT_SHIFT)
#define CS35L34_M_OTW_SHIFT		1
#define CS35L34_M_OTW			(1 << CS35L34_M_OTW_SHIFT)
#define CS35L34_M_OTE_SHIFT		0
#define CS35L34_M_OTE			(1 << CS35L34_M_OTE_SHIFT)

/* CS35L34_INT_MASK_2 */
#define CS35L34_M_PDN_DONE_SHIFT	4
#define CS35L34_M_PDN_DONE		(1 << CS35L34_M_PDN_DONE_SHIFT)
#define CS35L34_M_PRED_SHIFT		3
#define CS35L34_M_PRED_ERR		(1 << CS35L34_M_PRED_SHIFT)
#define CS35L34_M_PRED_CLR_SHIFT	2
#define CS35L34_M_PRED_CLR		(1 << CS35L34_M_PRED_CLR_SHIFT)
#define CS35L34_M_VPBR_SHIFT		1
#define CS35L34_M_VPBR_ERR		(1 << CS35L34_M_VPBR_SHIFT)
#define CS35L34_M_VPBR_CLR_SHIFT	0
#define CS35L34_M_VPBR_CLR		(1 << CS35L34_M_VPBR_CLR_SHIFT)

/* CS35L34_INT_MASK_3 */
#define CS35L34_M_BST_HIGH_SHIFT	4
#define CS35L34_M_BST_HIGH		(1 << CS35L34_M_BST_HIGH_SHIFT)
#define CS35L34_M_BST_HIGH_FLAG_SHIFT	3
#define CS35L34_M_BST_HIGH_FLAG		(1 << CS35L34_M_BST_HIGH_FLAG_SHIFT)
#define CS35L34_M_BST_IPK_FLAG_SHIFT	2
#define CS35L34_M_BST_IPK_FLAG		(1 << CS35L34_M_BST_IPK_FLAG_SHIFT)
#define CS35L34_M_LBST_SHORT_SHIFT	0
#define CS35L34_M_LBST_SHORT		(1 << CS35L34_M_LBST_SHORT_SHIFT)

/* CS35L34_INT_MASK_4 */
#define CS35L34_M_VMON_OVFL_SHIFT	3
#define CS35L34_M_VMON_OVFL		(1 << CS35L34_M_VMON_OVFL_SHIFT)
#define CS35L34_M_IMON_OVFL_SHIFT	2
#define CS35L34_M_IMON_OVFL		(1 << CS35L34_M_IMON_OVFL_SHIFT)
#define CS35L34_M_VPMON_OVFL_SHIFT	1
#define CS35L34_M_VPMON_OVFL		(1 << CS35L34_M_VPMON_OVFL_SHIFT)
#define CS35L34_M_VBSTMON_OVFL_SHIFT	1
#define CS35L34_M_VBSTMON_OVFL		(1 << CS35L34_M_VBSTMON_OVFL_SHIFT)

/* CS35L34_INT_1 */
#define CS35L34_CAL_ERR			(1 << CS35L34_M_CAL_ERR_SHIFT)
#define CS35L34_ALIVE_ERR		(1 << CS35L34_M_ALIVE_ERR_SHIFT)
#define CS35L34_M_ADSP_CLK_ERR		(1 << CS35L34_M_ADSP_CLK_SHIFT)
#define CS35L34_MCLK_ERR		(1 << CS35L34_M_MCLK_SHIFT)
#define CS35L34_AMP_SHORT		(1 << CS35L34_M_AMP_SHORT_SHIFT)
#define CS35L34_OTW			(1 << CS35L34_M_OTW_SHIFT)
#define CS35L34_OTE			(1 << CS35L34_M_OTE_SHIFT)

/* CS35L34_INT_2 */
#define CS35L34_PDN_DONE		(1 << CS35L34_M_PDN_DONE_SHIFT)
#define CS35L34_PRED_ERR		(1 << CS35L34_M_PRED_SHIFT)
#define CS35L34_PRED_CLR		(1 << CS35L34_M_PRED_CLR_SHIFT)
#define CS35L34_VPBR_ERR		(1 << CS35L34_M_VPBR_SHIFT)
#define CS35L34_VPBR_CLR		(1 << CS35L34_M_VPBR_CLR_SHIFT)

/* CS35L34_INT_3 */
#define CS35L34_BST_HIGH		(1 << CS35L34_M_BST_HIGH_SHIFT)
#define CS35L34_BST_HIGH_FLAG		(1 << CS35L34_M_BST_HIGH_FLAG_SHIFT)
#define CS35L34_BST_IPK_FLAG		(1 << CS35L34_M_BST_IPK_FLAG_SHIFT)
#define CS35L34_LBST_SHORT		(1 << CS35L34_M_LBST_SHORT_SHIFT)

/* CS35L34_INT_4 */
#define CS35L34_VMON_OVFL		(1 << CS35L34_M_VMON_OVFL_SHIFT)
#define CS35L34_IMON_OVFL		(1 << CS35L34_M_IMON_OVFL_SHIFT)
#define CS35L34_VPMON_OVFL		(1 << CS35L34_M_VPMON_OVFL_SHIFT)
#define CS35L34_VBSTMON_OVFL		(1 << CS35L34_M_VBSTMON_OVFL_SHIFT)

/* CS35L34_{RX,TX}_X */
#define CS35L34_X_STATE_SHIFT		7
#define CS35L34_X_STATE			(1 << CS35L34_X_STATE_SHIFT)
#define CS35L34_X_LOC_SHIFT		0
#define CS35L34_X_LOC			(0x1F << CS35L34_X_LOC_SHIFT)

#define CS35L34_RATES (SNDRV_PCM_RATE_48000 | \
			SNDRV_PCM_RATE_44100 | \
			SNDRV_PCM_RATE_32000)
#define CS35L34_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \
			SNDRV_PCM_FMTBIT_S24_LE | \
			SNDRV_PCM_FMTBIT_S32_LE)

#endif
