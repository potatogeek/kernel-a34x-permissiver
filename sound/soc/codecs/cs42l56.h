<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * cs42l52.h -- CS42L56 ALSA SoC audio driver
 *
 * Copyright 2014 CirrusLogic, Inc.
 *
 * Author: Brian Austin <brian.austin@cirrus.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __CS42L56_H__
#define __CS42L56_H__

#define CS42L56_CHIP_ID_1		0x01
#define CS42L56_CHIP_ID_2		0x02
#define CS42L56_PWRCTL_1		0x03
#define CS42L56_PWRCTL_2		0x04
#define CS42L56_CLKCTL_1		0x05
#define CS42L56_CLKCTL_2		0x06
#define CS42L56_SERIAL_FMT		0x07
#define CS42L56_CLASSH_CTL		0x08
#define CS42L56_MISC_CTL		0x09
#define CS42L56_INT_STATUS		0x0a
#define CS42L56_PLAYBACK_CTL		0x0b
#define CS42L56_DSP_MUTE_CTL		0x0c
#define CS42L56_ADCA_MIX_VOLUME		0x0d
#define CS42L56_ADCB_MIX_VOLUME		0x0e
#define CS42L56_PCMA_MIX_VOLUME		0x0f
#define CS42L56_PCMB_MIX_VOLUME		0x10
#define CS42L56_ANAINPUT_ADV_VOLUME	0x11
#define CS42L56_DIGINPUT_ADV_VOLUME	0x12
#define CS42L56_MASTER_A_VOLUME		0x13
#define CS42L56_MASTER_B_VOLUME		0x14
#define CS42L56_BEEP_FREQ_ONTIME	0x15
#define CS42L56_BEEP_FREQ_OFFTIME	0x16
#define CS42L56_BEEP_TONE_CFG		0x17
#define CS42L56_TONE_CTL		0x18
#define CS42L56_CHAN_MIX_SWAP		0x19
#define CS42L56_AIN_REFCFG_ADC_MUX	0x1a
#define CS42L56_HPF_CTL			0x1b
#define CS42L56_MISC_ADC_CTL		0x1c
#define CS42L56_GAIN_BIAS_CTL		0x1d
#define CS42L56_PGAA_MUX_VOLUME		0x1e
#define CS42L56_PGAB_MUX_VOLUME		0x1f
#define CS42L56_ADCA_ATTENUATOR		0x20
#define CS42L56_ADCB_ATTENUATOR		0x21
#define CS42L56_ALC_EN_ATTACK_RATE	0x22
#define CS42L56_ALC_RELEASE_RATE	0x23
#define CS42L56_ALC_THRESHOLD		0x24
#define CS42L56_NOISE_GATE_CTL		0x25
#define CS42L56_ALC_LIM_SFT_ZC		0x26
#define CS42L56_AMUTE_HPLO_MUX		0x27
#define CS42L56_HPA_VOLUME		0x28
#define CS42L56_HPB_VOLUME		0x29
#define CS42L56_LOA_VOLUME		0x2a
#define CS42L56_LOB_VOLUME		0x2b
#define CS42L56_LIM_THRESHOLD_CTL	0x2c
#define CS42L56_LIM_CTL_RELEASE_RATE	0x2d
#define CS42L56_LIM_ATTACK_RATE		0x2e

/* Device ID and Rev ID Masks */
#define CS42L56_DEVID			0x56
#define CS42L56_CHIP_ID_MASK		0xff
#define CS42L56_AREV_MASK		0x1c
#define CS42L56_MTLREV_MASK		0x03

/* Power bit masks */
#define CS42L56_PDN_ALL_MASK		0x01
#define CS42L56_PDN_ADCA_MASK		0x02
#define CS42L56_PDN_ADCB_MASK		0x04
#define CS42L56_PDN_CHRG_MASK		0x08
#define CS42L56_PDN_BIAS_MASK		0x10
#define CS42L56_PDN_VBUF_MASK		0x20
#define CS42L56_PDN_LOA_MASK		0x03
#define CS42L56_PDN_LOB_MASK		0x0c
#define CS42L56_PDN_HPA_MASK		0x30
#define CS42L56_PDN_HPB_MASK		0xc0

/* serial port and clk masks */
#define CS42L56_MASTER_MODE		0x40
#define CS42L56_SLAVE_MODE		0
#define CS42L56_MS_MODE_MASK		0x40
#define CS42L56_SCLK_INV		0x20
#define CS42L56_SCLK_INV_MASK		0x20
#define CS42L56_SCLK_MCLK_MASK		0x18
#define CS42L56_MCLK_PREDIV		0x04
#define CS42L56_MCLK_PREDIV_MASK	0x04
#define CS42L56_MCLK_DIV2		0x02
#define CS42L56_MCLK_DIV2_MASK		0x02
#define CS42L56_MCLK_DIS_MASK		0x01
#define CS42L56_CLK_AUTO_MASK		0x20
#define CS42L56_CLK_RATIO_MASK		0x1f
#define CS42L56_DIG_FMT_I2S		0
#define CS42L56_DIG_FMT_LEFT_J		0x08
#define CS42L56_DIG_FMT_MASK		0x08

/* Class H and misc ctl masks */
#define CS42L56_ADAPT_PWR_MASK		0xc0
#define CS42L56_CHRG_FREQ_MASK		0x0f
#define CS42L56_DIG_MUX_MASK		0x80
#define CS42L56_ANLGSFT_MASK		0x10
#define CS42L56_ANLGZC_MASK		0x08
#define CS42L56_DIGSFT_MASK		0x04
#define CS42L56_FREEZE_MASK		0x01
#define CS42L56_MIC_BIAS_MASK		0x03
#define CS42L56_HPFA_FREQ_MASK		0x03
#define CS42L56_HPFB_FREQ_MASK		0xc0
#define CS42L56_AIN1A_REF_MASK		0x10
#define CS42L56_AIN2A_REF_MASK		0x40
#define CS42L56_AIN1B_REF_MASK		0x20
#define CS42L56_AIN2B_REF_MASK		0x80

/* Playback Capture ctl masks */
#define CS42L56_PDN_DSP_MASK		0x80
#define CS42L56_DEEMPH_MASK		0x40
#define CS42L56_PLYBCK_GANG_MASK	0x10
#define CS42L56_PCM_INV_MASK		0x0c
#define CS42L56_MUTE_ALL		0xff
#define CS42L56_UNMUTE			0
#define CS42L56_ADCAMIX_MUTE_MASK	0x40
#define CS42L56_ADCBMIX_MUTE_MASK	0x80
#define CS42L56_PCMAMIX_MUTE_MASK	0x10
#define CS42L56_PCMBMIX_MUTE_MASK	0x20
#define CS42L56_MSTB_MUTE_MASK		0x02
#define CS42L56_MSTA_MUTE_MASK		0x01
#define CS42L56_ADCA_MUTE_MASK		0x01
#define CS42L56_ADCB_MUTE_MASK		0x02
#define CS42L56_HP_MUTE_MASK		0x80
#define CS42L56_LO_MUTE_MASK		0x80

/* Beep masks */
#define CS42L56_BEEP_FREQ_MASK		0xf0
#define CS42L56_BEEP_ONTIME_MASK	0x0f
#define CS42L56_BEEP_OFFTIME_MASK	0xe0
#define CS42L56_BEEP_CFG_MASK		0xc0
#define CS42L56_BEEP_TREBCF_MASK	0x18
#define CS42L56_BEEP_BASSCF_MASK	0x06
#define CS42L56_BEEP_TCEN_MASK		0x01
#define CS42L56_BEEP_RATE_SHIFT		4
#define CS42L56_BEEP_EN_MASK		0x3f


/* Supported MCLKS */
#define CS42L56_MCLK_5P6448MHZ		5644800
#define CS42L56_MCLK_6MHZ		6000000
#define CS42L56_MCLK_6P144MHZ		6144000
#define CS42L56_MCLK_11P2896MHZ		11289600
#define CS42L56_MCLK_12MHZ		12000000
#define CS42L56_MCLK_12P288MHZ		12288000
#define CS42L56_MCLK_22P5792MHZ		22579200
#define CS42L56_MCLK_24MHZ		24000000
#define CS42L56_MCLK_24P576MHZ		24576000

/* Clock ratios */
#define CS42L56_MCLK_LRCLK_128		0x08
#define CS42L56_MCLK_LRCLK_125		0x09
#define CS42L56_MCLK_LRCLK_136		0x0b
#define CS42L56_MCLK_LRCLK_192		0x0c
#define CS42L56_MCLK_LRCLK_187P5	0x0d
#define CS42L56_MCLK_LRCLK_256		0x10
#define CS42L56_MCLK_LRCLK_250		0x11
#define CS42L56_MCLK_LRCLK_272		0x13
#define CS42L56_MCLK_LRCLK_384		0x14
#define CS42L56_MCLK_LRCLK_375		0x15
#define CS42L56_MCLK_LRCLK_512		0x18
#define CS42L56_MCLK_LRCLK_500		0x19
#define CS42L56_MCLK_LRCLK_544		0x1b
#define CS42L56_MCLK_LRCLK_750		0x1c
#define CS42L56_MCLK_LRCLK_768		0x1d


#define CS42L56_MAX_REGISTER		0x34

#endif
