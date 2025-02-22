<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * da732x.h -- Dialog DA732X ALSA SoC Audio Driver Header File
 *
 * Copyright (C) 2012 Dialog Semiconductor GmbH
 *
 * Author: Michal Hajduk <Michal.Hajduk@diasemi.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __DA732X_H_
#define __DA732X_H_

#include <sound/soc.h>

/* General */
#define	DA732X_U8_MASK			0xFF
#define	DA732X_4BYTES			4
#define	DA732X_3BYTES			3
#define	DA732X_2BYTES			2
#define	DA732X_1BYTE			1
#define	DA732X_1BYTE_SHIFT		8
#define	DA732X_2BYTES_SHIFT		16
#define	DA732X_3BYTES_SHIFT		24
#define	DA732X_4BYTES_SHIFT		32

#define	DA732X_DACS_DIS			0x0
#define	DA732X_HP_DIS			0x0
#define	DA732X_CLEAR_REG		0x0

/* Calibration */
#define	DA732X_DAC_OFFSET_STEP		0x20
#define	DA732X_OUTPUT_OFFSET_STEP	0x80
#define	DA732X_HP_OUT_TRIM_VAL		0x0
#define	DA732X_WAIT_FOR_STABILIZATION	1
#define	DA732X_HPL_DAC			0
#define	DA732X_HPR_DAC			1
#define	DA732X_HP_DACS			2
#define	DA732X_HPL_AMP			0
#define	DA732X_HPR_AMP			1
#define	DA732X_HP_AMPS			2

/* Clock settings */
#define DA732X_STARTUP_DELAY		100
#define	DA732X_PLL_OUT_196608		196608000
#define	DA732X_PLL_OUT_180634		180633600
#define	DA732X_PLL_OUT_SRM		188620800
#define	DA732X_MCLK_10MHZ		10000000
#define	DA732X_MCLK_20MHZ		20000000
#define	DA732X_MCLK_40MHZ		40000000
#define	DA732X_MCLK_54MHZ		54000000
<<<<<<< HEAD
#define	DA732X_MCLK_RET_0_10MHZ		0
#define	DA732X_MCLK_VAL_0_10MHZ		1
#define	DA732X_MCLK_RET_10_20MHZ	1
#define	DA732X_MCLK_VAL_10_20MHZ	2
#define	DA732X_MCLK_RET_20_40MHZ	2
#define	DA732X_MCLK_VAL_20_40MHZ	4
#define	DA732X_MCLK_RET_40_54MHZ	3
#define	DA732X_MCLK_VAL_40_54MHZ	8
=======
#define	DA732X_MCLK_VAL_0_10MHZ		0
#define	DA732X_MCLK_VAL_10_20MHZ	1
#define	DA732X_MCLK_VAL_20_40MHZ	2
#define	DA732X_MCLK_VAL_40_54MHZ	3
>>>>>>> upstream/android-13
#define	DA732X_DAI_ID1			0
#define	DA732X_DAI_ID2			1
#define	DA732X_SRCCLK_PLL		0
#define	DA732X_SRCCLK_MCLK		1

#define	DA732X_LIN_LP_VOL		0x4F
#define	DA732X_LP_VOL			0x40

/* Kcontrols */
#define	DA732X_DAC_EN_MAX		2
#define	DA732X_ADCL_MUX_MAX		2
#define	DA732X_ADCR_MUX_MAX		3
#define	DA732X_HPF_MODE_MAX		3
#define	DA732X_HPF_MODE_SHIFT		4
#define	DA732X_HPF_MUSIC_SHIFT		0
#define	DA732X_HPF_MUSIC_MAX		4
#define	DA732X_HPF_VOICE_SHIFT		4
#define	DA732X_HPF_VOICE_MAX		8
#define	DA732X_EQ_EN_MAX		1
#define	DA732X_HPF_VOICE		1
#define	DA732X_HPF_MUSIC		2
#define	DA732X_HPF_DISABLED		0
#define	DA732X_NO_INVERT		0
#define	DA732X_INVERT			1
#define	DA732X_SWITCH_MAX		1
#define	DA732X_ENABLE_CP		1
#define	DA732X_DISABLE_CP		0
#define	DA732X_DISABLE_ALL_CLKS		0
#define	DA732X_RESET_ADCS		0

/* dB values */
#define DA732X_MIC_VOL_DB_MIN		0
#define DA732X_MIC_VOL_DB_INC		50
#define DA732X_MIC_PRE_VOL_DB_MIN	0
#define DA732X_MIC_PRE_VOL_DB_INC	600
#define DA732X_AUX_VOL_DB_MIN		-6000
#define DA732X_AUX_VOL_DB_INC		150
#define DA732X_HP_VOL_DB_MIN		-2250
#define DA732X_HP_VOL_DB_INC		150
#define	DA732X_LIN2_VOL_DB_MIN		-1650
#define	DA732X_LIN2_VOL_DB_INC		150
#define	DA732X_LIN3_VOL_DB_MIN		-1650
#define DA732X_LIN3_VOL_DB_INC		150
#define	DA732X_LIN4_VOL_DB_MIN		-2250
#define DA732X_LIN4_VOL_DB_INC		150
#define	DA732X_EQ_BAND_VOL_DB_MIN	-1050
#define	DA732X_EQ_BAND_VOL_DB_INC	150
#define DA732X_DAC_VOL_DB_MIN		-7725
#define DA732X_DAC_VOL_DB_INC		75
#define DA732X_ADC_VOL_DB_MIN		0
#define DA732X_ADC_VOL_DB_INC		-1
#define	DA732X_EQ_OVERALL_VOL_DB_MIN	-1800
#define	DA732X_EQ_OVERALL_VOL_DB_INC	600

enum da732x_sysctl {
	DA732X_SR_8KHZ		= 0x1,
	DA732X_SR_11_025KHZ	= 0x2,
	DA732X_SR_12KHZ		= 0x3,
	DA732X_SR_16KHZ		= 0x5,
	DA732X_SR_22_05KHZ	= 0x6,
	DA732X_SR_24KHZ		= 0x7,
	DA732X_SR_32KHZ		= 0x9,
	DA732X_SR_44_1KHZ	= 0xA,
	DA732X_SR_48KHZ		= 0xB,
	DA732X_SR_88_1KHZ	= 0xE,
	DA732X_SR_96KHZ		= 0xF,
};

#endif /* __DA732X_H_ */
