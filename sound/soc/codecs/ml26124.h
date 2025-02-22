<<<<<<< HEAD
/*
 * Copyright (C) 2011 LAPIS Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2011 LAPIS Semiconductor Co., Ltd.
>>>>>>> upstream/android-13
 */

#ifndef ML26124_H
#define ML26124_H

/* Clock Control Register */
#define ML26124_SMPLING_RATE		0x00
#define ML26124_PLLNL			0x02
#define ML26124_PLLNH			0x04
#define ML26124_PLLML			0x06
#define ML26124_PLLMH			0x08
#define ML26124_PLLDIV			0x0a
#define ML26124_CLK_EN			0x0c
#define ML26124_CLK_CTL			0x0e

/* System Control Register */
#define ML26124_SW_RST			0x10
#define ML26124_REC_PLYBAK_RUN		0x12
#define ML26124_MIC_TIM			0x14

/* Power Mnagement Register */
#define ML26124_PW_REF_PW_MNG		0x20
#define ML26124_PW_IN_PW_MNG		0x22
#define ML26124_PW_DAC_PW_MNG		0x24
#define ML26124_PW_SPAMP_PW_MNG		0x26
#define ML26124_PW_LOUT_PW_MNG		0x28
#define ML26124_PW_VOUT_PW_MNG		0x2a
#define ML26124_PW_ZCCMP_PW_MNG		0x2e

/* Analog Reference Control Register */
#define ML26124_PW_MICBIAS_VOL		0x30

/* Input/Output Amplifier Control Register */
#define ML26124_PW_MIC_IN_VOL		0x32
#define ML26124_PW_MIC_BOST_VOL		0x38
#define ML26124_PW_SPK_AMP_VOL		0x3a
#define ML26124_PW_AMP_VOL_FUNC		0x48
#define ML26124_PW_AMP_VOL_FADE		0x4a

/* Analog Path Control Register */
#define ML26124_SPK_AMP_OUT		0x54
#define ML26124_MIC_IF_CTL		0x5a
#define ML26124_MIC_SELECT		0xe8

/* Audio Interface Control Register */
#define ML26124_SAI_TRANS_CTL		0x60
#define ML26124_SAI_RCV_CTL		0x62
#define ML26124_SAI_MODE_SEL		0x64

/* DSP Control Register */
#define ML26124_FILTER_EN		0x66
#define ML26124_DVOL_CTL		0x68
#define ML26124_MIXER_VOL_CTL		0x6a
#define ML26124_RECORD_DIG_VOL		0x6c
#define ML26124_PLBAK_DIG_VOL		0x70
#define ML26124_DIGI_BOOST_VOL		0x72
#define ML26124_EQ_GAIN_BRAND0		0x74
#define ML26124_EQ_GAIN_BRAND1		0x76
#define ML26124_EQ_GAIN_BRAND2		0x78
#define ML26124_EQ_GAIN_BRAND3		0x7a
#define ML26124_EQ_GAIN_BRAND4		0x7c
#define ML26124_HPF2_CUTOFF		0x7e
#define ML26124_EQBRAND0_F0L		0x80
#define ML26124_EQBRAND0_F0H		0x82
#define ML26124_EQBRAND0_F1L		0x84
#define ML26124_EQBRAND0_F1H		0x86
#define ML26124_EQBRAND1_F0L		0x88
#define ML26124_EQBRAND1_F0H		0x8a
#define ML26124_EQBRAND1_F1L		0x8c
#define ML26124_EQBRAND1_F1H		0x8e
#define ML26124_EQBRAND2_F0L		0x90
#define ML26124_EQBRAND2_F0H		0x92
#define ML26124_EQBRAND2_F1L		0x94
#define ML26124_EQBRAND2_F1H		0x96
#define ML26124_EQBRAND3_F0L		0x98
#define ML26124_EQBRAND3_F0H		0x9a
#define ML26124_EQBRAND3_F1L		0x9c
#define ML26124_EQBRAND3_F1H		0x9e
#define ML26124_EQBRAND4_F0L		0xa0
#define ML26124_EQBRAND4_F0H		0xa2
#define ML26124_EQBRAND4_F1L		0xa4
#define ML26124_EQBRAND4_F1H		0xa6

/* ALC Control Register */
#define ML26124_ALC_MODE		0xb0
#define ML26124_ALC_ATTACK_TIM		0xb2
#define ML26124_ALC_DECAY_TIM		0xb4
#define ML26124_ALC_HOLD_TIM		0xb6
#define ML26124_ALC_TARGET_LEV		0xb8
#define ML26124_ALC_MAXMIN_GAIN		0xba
#define ML26124_NOIS_GATE_THRSH		0xbc
#define ML26124_ALC_ZERO_TIMOUT		0xbe

/* Playback Limiter Control Register */
#define ML26124_PL_ATTACKTIME		0xc0
#define ML26124_PL_DECAYTIME		0xc2
#define ML26124_PL_TARGETTIME		0xc4
#define ML26124_PL_MAXMIN_GAIN		0xc6
#define ML26124_PLYBAK_BOST_VOL		0xc8
#define ML26124_PL_0CROSS_TIMOUT	0xca

/* Video Amplifer Control Register */
#define ML26124_VIDEO_AMP_GAIN_CTL	0xd0
#define ML26124_VIDEO_AMP_SETUP1	0xd2
#define ML26124_VIDEO_AMP_CTL2		0xd4

/* Clock select for machine driver */
#define ML26124_USE_PLL			0
#define ML26124_USE_MCLKI_256FS		1
#define ML26124_USE_MCLKI_512FS		2
#define ML26124_USE_MCLKI_1024FS	3

/* Register Mask */
#define ML26124_R0_MASK	0xf
#define ML26124_R2_MASK	0xff
#define ML26124_R4_MASK	0x1
#define ML26124_R6_MASK	0xf
#define ML26124_R8_MASK	0x3f
#define ML26124_Ra_MASK	0x1f
#define ML26124_Rc_MASK	0x1f
#define ML26124_Re_MASK	0x7
#define ML26124_R10_MASK	0x1
#define ML26124_R12_MASK	0x17
#define ML26124_R14_MASK	0x3f
#define ML26124_R20_MASK	0x47
#define ML26124_R22_MASK	0xa
#define ML26124_R24_MASK	0x2
#define ML26124_R26_MASK	0x1f
#define ML26124_R28_MASK	0x2
#define ML26124_R2a_MASK	0x2
#define ML26124_R2e_MASK	0x2
#define ML26124_R30_MASK	0x7
#define ML26124_R32_MASK	0x3f
#define ML26124_R38_MASK	0x38
#define ML26124_R3a_MASK	0x3f
#define ML26124_R48_MASK	0x3
#define ML26124_R4a_MASK	0x7
#define ML26124_R54_MASK	0x2a
#define ML26124_R5a_MASK	0x3
#define ML26124_Re8_MASK	0x3
#define ML26124_R60_MASK	0xff
#define ML26124_R62_MASK	0xff
#define ML26124_R64_MASK	0x1
#define ML26124_R66_MASK	0xff
#define ML26124_R68_MASK	0x3b
#define ML26124_R6a_MASK	0xf3
#define ML26124_R6c_MASK	0xff
#define ML26124_R70_MASK	0xff

#define ML26124_MCLKEN		BIT(0)
#define ML26124_PLLEN		BIT(1)
#define ML26124_PLLOE		BIT(2)
#define ML26124_MCLKOE		BIT(3)

#define ML26124_BLT_ALL_ON	0x1f
#define ML26124_BLT_PREAMP_ON	0x13

#define ML26124_MICBEN_ON	BIT(2)

enum ml26124_regs {
	ML26124_MCLK = 0,
};

enum ml26124_clk_in {
	ML26124_USE_PLLOUT = 0,
	ML26124_USE_MCLKI,
};

#endif
