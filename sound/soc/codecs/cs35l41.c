<<<<<<< HEAD
/*
 * cs35l41.c -- CS35l41 ALSA SoC audio driver
 *
 * Copyright 2017 Cirrus Logic, Inc.
 *
 * Author:	David Rhodes	<david.rhodes@cirrus.com>
 *		Brian Austin	<brian.austin@cirrus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio/consumer.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/regmap.h>
#include <sound/core.h>
=======
// SPDX-License-Identifier: GPL-2.0
//
// cs35l41.c -- CS35l41 ALSA SoC audio driver
//
// Copyright 2017-2021 Cirrus Logic, Inc.
//
// Author: David Rhodes <david.rhodes@cirrus.com>

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of_device.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <sound/initval.h>
>>>>>>> upstream/android-13
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <sound/initval.h>
#include <sound/tlv.h>
#include <linux/of_irq.h>
#include <linux/completion.h>
#include <linux/spi/spi.h>

#include "wm_adsp.h"
#include <sound/cs35l41.h>
#include "cs35l41.h"

#include <sound/cirrus/core.h>
#include <sound/cirrus/big_data.h>
#include <sound/cirrus/calibration.h>
#include <sound/cirrus/power.h>

#ifdef CS35L41_DEBUG_LOG
#define cs35l41_dbg(dev, fmt, ...) dev_info(dev, fmt, ##__VA_ARGS__)
#else
#define cs35l41_dbg(dev, fmt, ...) dev_dbg(dev, fmt, ##__VA_ARGS__)
#endif
=======
#include <sound/tlv.h>

#include "cs35l41.h"

static const char * const cs35l41_supplies[CS35L41_NUM_SUPPLIES] = {
	"VA",
	"VP",
};
>>>>>>> upstream/android-13

struct cs35l41_pll_sysclk_config {
	int freq;
	int clk_cfg;
};

static const struct cs35l41_pll_sysclk_config cs35l41_pll_sysclk[] = {
	{ 32768,	0x00 },
	{ 8000,		0x01 },
	{ 11025,	0x02 },
	{ 12000,	0x03 },
	{ 16000,	0x04 },
	{ 22050,	0x05 },
	{ 24000,	0x06 },
	{ 32000,	0x07 },
	{ 44100,	0x08 },
	{ 48000,	0x09 },
	{ 88200,	0x0A },
	{ 96000,	0x0B },
	{ 128000,	0x0C },
	{ 176400,	0x0D },
	{ 192000,	0x0E },
	{ 256000,	0x0F },
	{ 352800,	0x10 },
	{ 384000,	0x11 },
	{ 512000,	0x12 },
	{ 705600,	0x13 },
	{ 750000,	0x14 },
	{ 768000,	0x15 },
	{ 1000000,	0x16 },
	{ 1024000,	0x17 },
	{ 1200000,	0x18 },
	{ 1411200,	0x19 },
	{ 1500000,	0x1A },
	{ 1536000,	0x1B },
	{ 2000000,	0x1C },
	{ 2048000,	0x1D },
	{ 2400000,	0x1E },
	{ 2822400,	0x1F },
	{ 3000000,	0x20 },
	{ 3072000,	0x21 },
	{ 3200000,	0x22 },
	{ 4000000,	0x23 },
	{ 4096000,	0x24 },
	{ 4800000,	0x25 },
	{ 5644800,	0x26 },
	{ 6000000,	0x27 },
	{ 6144000,	0x28 },
	{ 6250000,	0x29 },
	{ 6400000,	0x2A },
	{ 6500000,	0x2B },
	{ 6750000,	0x2C },
	{ 7526400,	0x2D },
	{ 8000000,	0x2E },
	{ 8192000,	0x2F },
	{ 9600000,	0x30 },
	{ 11289600,	0x31 },
	{ 12000000,	0x32 },
	{ 12288000,	0x33 },
	{ 12500000,	0x34 },
	{ 12800000,	0x35 },
	{ 13000000,	0x36 },
	{ 13500000,	0x37 },
	{ 19200000,	0x38 },
	{ 22579200,	0x39 },
	{ 24000000,	0x3A },
	{ 24576000,	0x3B },
	{ 25000000,	0x3C },
	{ 25600000,	0x3D },
	{ 26000000,	0x3E },
	{ 27000000,	0x3F },
};

<<<<<<< HEAD
static const char * const cs35l41_supplies[] = {
	"VA",
	"VP",
};

static void cs35l41_log_regmap_fail(struct cs35l41_private *cs35l41,
					unsigned int reg)
{
	struct cirrus_amp *amp = cirrus_get_amp_from_suffix(cs35l41->pdata.mfd_suffix);

	dev_crit(cs35l41->dev, "Failed to access regmap, reg = 0x%x\n", reg);
	if (amp && amp->i2c_callback)
		amp->i2c_callback(cs35l41->pdata.mfd_suffix);
}

static void cs35l41_log_status(struct cs35l41_private *cs35l41, int mute)
{
	unsigned int status;
	int ret;

	if (!mute) {
		ret = regmap_read(cs35l41->regmap,
			CS35L41_PWR_CTRL1, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41, CS35L41_PWR_CTRL1);
			return;
		}
		dev_info(cs35l41->dev, "PWR_CTRL1 = 0x%x\n", status);

		ret = regmap_read(cs35l41->regmap,
			CS35L41_PWR_CTRL2, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41, CS35L41_PWR_CTRL2);
			return;
		}
		dev_info(cs35l41->dev, "PWR_CTRL2 = 0x%x\n", status);

		ret = regmap_read(cs35l41->regmap,
			CS35L41_AMP_DIG_VOL_CTRL, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41,
				CS35L41_AMP_DIG_VOL_CTRL);
			return;
		}
		dev_info(cs35l41->dev, "DIG_VOL_CTRL = 0x%x\n", status);

		ret = regmap_read(cs35l41->regmap,
			CS35L41_AMP_GAIN_CTRL, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41, CS35L41_AMP_GAIN_CTRL);
			return;
		}
		dev_info(cs35l41->dev, "GAIN_CTRL = 0x%x\n", status);
	} else {
		ret = regmap_read(cs35l41->regmap,
			CS35L41_IRQ1_STATUS1, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41, CS35L41_IRQ1_STATUS1);
			return;
		}
		dev_info(cs35l41->dev, "IRQ1_STATUS1 = 0x%x\n", status);

		ret = regmap_read(cs35l41->regmap,
			CS35L41_CSPL_MBOX_STS, &status);
		if (ret < 0) {
			cs35l41_log_regmap_fail(cs35l41, CS35L41_CSPL_MBOX_STS);
			return;
		}
		dev_info(cs35l41->dev, "MBOX status = 0x%x\n", status);
	}
}

static int cs35l41_dsp_power_ev(struct snd_soc_dapm_widget *w,
		       struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	struct cirrus_amp *amp =
			  cirrus_get_amp_from_suffix(cs35l41->pdata.mfd_suffix);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		if (cs35l41->halo_booted == false)
			wm_adsp_early_event(w, kcontrol, event);
		else {
			mutex_lock(&cs35l41->dsp.pwr_lock);
			cs35l41->dsp.booted = true;
			mutex_unlock(&cs35l41->dsp.pwr_lock);
		}

		return 0;

	case SND_SOC_DAPM_POST_PMU:

		if (cs35l41->halo_booted == false) {
			wm_adsp_event(w, kcontrol, event);
			dev_info(cs35l41->dev, "%s: loaded\n", __func__);
			cs35l41->halo_booted = true;
			cs35l41->halo_played = false;
			cirrus_amp_write_ctl(amp, "ALGO_FRAME_DELAY",
					WMFW_ADSP2_XM,
					CIRRUS_AMP_ALG_ID_CSPL,
					cs35l41->pdata.algo_frame_delay);
		}
		cirrus_cal_apply(cs35l41->pdata.mfd_suffix);

		return 0;
	case SND_SOC_DAPM_PRE_PMD:
		if (cs35l41->halo_booted == false) {
			wm_adsp_early_event(w, kcontrol, event);
			wm_adsp_event(w, kcontrol, event);
		}
		return 0;
	default:
		return 0;
	}
}

static int cs35l41_halo_booted_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	ucontrol->value.integer.value[0] = cs35l41->halo_booted;

	return 0;
}

static int cs35l41_halo_booted_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	cs35l41->halo_booted = ucontrol->value.integer.value[0];

	return 0;
}

static int cs35l41_pcm_vol_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	ucontrol->value.integer.value[0] = cs35l41->pcm_vol;

	return 0;
}

static int cs35l41_pcm_vol_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	if (ucontrol->value.integer.value[0] > 0x391 ||
		ucontrol->value.integer.value[0] < 0) {
		dev_err(cs35l41->dev, "%s: Invalid value 0x%lx\n",
			__func__, ucontrol->value.integer.value[0]);
		return -EINVAL;
	}

	dev_info(cs35l41->dev, "%s: 0x%lx\n", __func__,
	ucontrol->value.integer.value[0]);

	cs35l41->pcm_vol = ucontrol->value.integer.value[0];

	//Amp is unmuted
	if (cs35l41->amp_mute == 1 && cs35l41->pcm_active == 1)
		snd_soc_put_volsw_sx(kcontrol, ucontrol);

	return 0;
}

static int cs35l41_amp_mute_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	ucontrol->value.integer.value[0] = cs35l41->amp_mute;

	return 0;
}

static int cs35l41_amp_mute_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	unsigned int val;

	if (ucontrol->value.integer.value[0] > 1 ||
		ucontrol->value.integer.value[0] < 0) {
		dev_err(cs35l41->dev, "%s: Invalid value 0x%lx\n",
			__func__, ucontrol->value.integer.value[0]);
		return -EINVAL;
	}

	cs35l41->amp_mute = ucontrol->value.integer.value[0];

	if (cs35l41->pcm_active == 1) {
		if (cs35l41->amp_mute == 0) //Mute
			val = 0;
		else //Unmute
			val = cs35l41->pcm_vol;

		//convert control val to register val
		if (val < CS35L41_AMP_VOL_CTRL_DEFAULT)
			val += CS35L41_AMP_VOL_PCM_MUTE;
		else
			val -= CS35L41_AMP_VOL_CTRL_DEFAULT;

		regmap_update_bits(cs35l41->regmap, CS35L41_AMP_DIG_VOL_CTRL,
				CS35L41_AMP_VOL_PCM_MASK <<
				CS35L41_AMP_VOL_PCM_SHIFT,
				val << CS35L41_AMP_VOL_PCM_SHIFT);
	}

	dev_info(cs35l41->dev, "%s: %s\n", __func__,
		(cs35l41->amp_mute == 0) ? "Muted" : "Unmuted");

	return 0;
}

static int cs35l41_convert_ramp_rate(struct cs35l41_private *cs35l41,
	unsigned int ramp_rate)
{
	const unsigned int ramp_conv_table[] = {0, 1, 2, 4, 8, 16, 30, 60};

	/* Convert the ramp_rate register setting into a time delay in ms.
	 * This assumes the starting volume is 0 dB
	 */
	if (ramp_rate >= ARRAY_SIZE(ramp_conv_table)) {
		dev_err(cs35l41->dev,
			"Invalid rate (%d) to convert\n", ramp_rate);
		return -EINVAL;
	}

	return ramp_conv_table[ramp_rate] * CS35L41_AMP_VOL_MIN / 12;
}

static const char * const cs35l41_amp_mute_text[] = {"Muted", "Unmuted"};
static const unsigned int cs35l41_amp_mute_values[] = {1, 0};

static SOC_VALUE_ENUM_SINGLE_DECL(amp_mute_ctl,
				SND_SOC_NOPM,
				0,
				0,
				cs35l41_amp_mute_text,
				cs35l41_amp_mute_values);


static const DECLARE_TLV_DB_RANGE(dig_vol_tlv,
		0, 0, TLV_DB_SCALE_ITEM(TLV_DB_GAIN_MUTE, 0, 1),
		1, 913, TLV_DB_SCALE_ITEM(-10200, 25, 0));
static DECLARE_TLV_DB_SCALE(amp_gain_tlv, 0, 1, 1);

static const struct snd_kcontrol_new amp_enable_ctrl =
	SOC_DAPM_SINGLE("Switch", SND_SOC_NOPM, 0, 1, 0);

=======
struct cs35l41_fs_mon_config {
	int freq;
	unsigned int fs1;
	unsigned int fs2;
};

static const struct cs35l41_fs_mon_config cs35l41_fs_mon[] = {
	{ 32768,	2254,	3754 },
	{ 8000,		9220,	15364 },
	{ 11025,	6148,	10244 },
	{ 12000,	6148,	10244 },
	{ 16000,	4612,	7684 },
	{ 22050,	3076,	5124 },
	{ 24000,	3076,	5124 },
	{ 32000,	2308,	3844 },
	{ 44100,	1540,	2564 },
	{ 48000,	1540,	2564 },
	{ 88200,	772,	1284 },
	{ 96000,	772,	1284 },
	{ 128000,	580,	964 },
	{ 176400,	388,	644 },
	{ 192000,	388,	644 },
	{ 256000,	292,	484 },
	{ 352800,	196,	324 },
	{ 384000,	196,	324 },
	{ 512000,	148,	244 },
	{ 705600,	100,	164 },
	{ 750000,	100,	164 },
	{ 768000,	100,	164 },
	{ 1000000,	76,	124 },
	{ 1024000,	76,	124 },
	{ 1200000,	64,	104 },
	{ 1411200,	52,	84 },
	{ 1500000,	52,	84 },
	{ 1536000,	52,	84 },
	{ 2000000,	40,	64 },
	{ 2048000,	40,	64 },
	{ 2400000,	34,	54 },
	{ 2822400,	28,	44 },
	{ 3000000,	28,	44 },
	{ 3072000,	28,	44 },
	{ 3200000,	27,	42 },
	{ 4000000,	22,	34 },
	{ 4096000,	22,	34 },
	{ 4800000,	19,	29 },
	{ 5644800,	16,	24 },
	{ 6000000,	16,	24 },
	{ 6144000,	16,	24 },
};

static const unsigned char cs35l41_bst_k1_table[4][5] = {
	{ 0x24, 0x32, 0x32, 0x4F, 0x57 },
	{ 0x24, 0x32, 0x32, 0x4F, 0x57 },
	{ 0x40, 0x32, 0x32, 0x4F, 0x57 },
	{ 0x40, 0x32, 0x32, 0x4F, 0x57 }
};

static const unsigned char cs35l41_bst_k2_table[4][5] = {
	{ 0x24, 0x49, 0x66, 0xA3, 0xEA },
	{ 0x24, 0x49, 0x66, 0xA3, 0xEA },
	{ 0x48, 0x49, 0x66, 0xA3, 0xEA },
	{ 0x48, 0x49, 0x66, 0xA3, 0xEA }
};

static const unsigned char cs35l41_bst_slope_table[4] = {
	0x75, 0x6B, 0x3B, 0x28
};

static int cs35l41_get_fs_mon_config_index(int freq)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cs35l41_fs_mon); i++) {
		if (cs35l41_fs_mon[i].freq == freq)
			return i;
	}

	return -EINVAL;
}

static const DECLARE_TLV_DB_RANGE(dig_vol_tlv,
		0, 0, TLV_DB_SCALE_ITEM(TLV_DB_GAIN_MUTE, 0, 1),
		1, 913, TLV_DB_MINMAX_ITEM(-10200, 1200));
static DECLARE_TLV_DB_SCALE(amp_gain_tlv, 0, 1, 1);

>>>>>>> upstream/android-13
static const struct snd_kcontrol_new dre_ctrl =
	SOC_DAPM_SINGLE("Switch", CS35L41_PWR_CTRL3, 20, 1, 0);

static const char * const cs35l41_pcm_sftramp_text[] =  {
<<<<<<< HEAD
	"Off", ".5ms", "1ms", "2ms", "4ms", "8ms", "15ms", "30ms"};
=======
	"Off", ".5ms", "1ms", "2ms", "4ms", "8ms", "15ms", "30ms"
};
>>>>>>> upstream/android-13

static SOC_ENUM_SINGLE_DECL(pcm_sft_ramp,
			    CS35L41_AMP_DIG_VOL_CTRL, 0,
			    cs35l41_pcm_sftramp_text);

<<<<<<< HEAD
static const char * const cs35l41_bst_en_text[] = {"Disabled", "Enabled"};
static const unsigned int cs35l41_bst_en_values[] = {
				CS35L41_BST_EN_DISABLE,
				CS35L41_BST_EN_DEFAULT};

static SOC_VALUE_ENUM_SINGLE_DECL(bst_en_ctl,
				CS35L41_PWR_CTRL2,
				CS35L41_BST_EN_SHIFT,
				CS35L41_BST_EN_MASK,
				cs35l41_bst_en_text,
				cs35l41_bst_en_values);

static const char * const cs35l41_pcm_inv_text[] = {"Disabled", "Enabled"};

static SOC_ENUM_SINGLE_DECL(pcm_inv, CS35L41_AMP_DIG_VOL_CTRL, 14,
				cs35l41_pcm_inv_text);

static const char * const cs35l41_vpbr_rel_rate_text[] = {
	"5ms", "10ms", "25ms", "50ms", "100ms", "250ms", "500ms", "1000ms"};

static SOC_ENUM_SINGLE_DECL(vpbr_rel_rate, CS35L41_VPBR_CFG, 21,
				cs35l41_vpbr_rel_rate_text);

static const char * const cs35l41_vpbr_wait_text[] = {
	"10ms", "100ms", "250ms", "500ms"};

static SOC_ENUM_SINGLE_DECL(vpbr_wait, CS35L41_VPBR_CFG, 19,
				cs35l41_vpbr_wait_text);

static const char * const cs35l41_vpbr_atk_rate_text[] = {
	"2.5us", "5us", "10us", "25us", "50us", "100us", "250us", "500us"};

static SOC_ENUM_SINGLE_DECL(vpbr_atk_rate, CS35L41_VPBR_CFG, 16,
				cs35l41_vpbr_atk_rate_text);

static const char * const cs35l41_vpbr_atk_vol_text[] = {
	"0.0625dB", "0.125dB", "0.25dB", "0.5dB",
	"0.75dB", "1dB", "1.25dB", "1.5dB"};

static SOC_ENUM_SINGLE_DECL(vpbr_atk_vol, CS35L41_VPBR_CFG, 12,
				cs35l41_vpbr_atk_vol_text);

static const char * const cs35l41_vpbr_thld1_text[] = {
	"2.402", "2.449", "2.497", "2.544", "2.592", "2.639", "2.687", "2.734",
	"2.782", "2.829", "2.877", "2.924", "2.972", "3.019", "3.067", "3.114",
	"3.162", "3.209", "3.257", "3.304", "3.352", "3.399", "3.447", "3.494",
	"3.542", "3.589", "3.637", "3.684", "3.732", "3.779", "3.827", "3.874"};

static SOC_ENUM_SINGLE_DECL(vpbr_thld1, CS35L41_VPBR_CFG, 0,
				cs35l41_vpbr_thld1_text);

static const char * const cs35l41_vpbr_en_text[] = {"Disabled", "Enabled"};

static SOC_ENUM_SINGLE_DECL(vpbr_enable, CS35L41_PWR_CTRL3, 12,
				cs35l41_vpbr_en_text);
=======
static int cs35l41_dsp_preload_ev(struct snd_soc_dapm_widget *w,
				  struct snd_kcontrol *kcontrol, int event)
{
	int ret;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		return wm_adsp_early_event(w, kcontrol, event);
	case SND_SOC_DAPM_PRE_PMD:
		ret = wm_adsp_early_event(w, kcontrol, event);
		if (ret)
			return ret;

		return wm_adsp_event(w, kcontrol, event);
	default:
		return 0;
	}
}

static bool cs35l41_check_cspl_mbox_sts(enum cs35l41_cspl_mbox_cmd cmd,
					enum cs35l41_cspl_mbox_status sts)
{
	switch (cmd) {
	case CSPL_MBOX_CMD_NONE:
	case CSPL_MBOX_CMD_UNKNOWN_CMD:
		return true;
	case CSPL_MBOX_CMD_PAUSE:
		return (sts == CSPL_MBOX_STS_PAUSED);
	case CSPL_MBOX_CMD_RESUME:
		return (sts == CSPL_MBOX_STS_RUNNING);
	case CSPL_MBOX_CMD_REINIT:
		return (sts == CSPL_MBOX_STS_RUNNING);
	case CSPL_MBOX_CMD_STOP_PRE_REINIT:
		return (sts == CSPL_MBOX_STS_RDY_FOR_REINIT);
	default:
		return false;
	}
}

static int cs35l41_set_cspl_mbox_cmd(struct cs35l41_private *cs35l41,
				     enum cs35l41_cspl_mbox_cmd cmd)
{
	unsigned int sts = 0, i;
	int ret;

	// Set mailbox cmd
	ret = regmap_write(cs35l41->regmap, CS35L41_DSP_VIRT1_MBOX_1, cmd);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Failed to write MBOX: %d\n", ret);
		return ret;
	}

	// Read mailbox status and verify it is appropriate for the given cmd
	for (i = 0; i < 5; i++) {
		usleep_range(1000, 1100);

		ret = regmap_read(cs35l41->regmap, CS35L41_DSP_MBOX_2, &sts);
		if (ret < 0) {
			dev_err(cs35l41->dev, "Failed to read MBOX STS: %d\n", ret);
			continue;
		}

		if (!cs35l41_check_cspl_mbox_sts(cmd, sts)) {
			dev_dbg(cs35l41->dev,
				"[%u] cmd %u returned invalid sts %u",
				i, cmd, sts);
		} else {
			return 0;
		}
	}

	dev_err(cs35l41->dev,
		"Failed to set mailbox cmd %u (status %u)\n",
		cmd, sts);

	return -ENOMSG;
}

static int cs35l41_dsp_audio_ev(struct snd_soc_dapm_widget *w,
				struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	unsigned int fw_status;
	int ret;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		if (!cs35l41->dsp.running)
			return wm_adsp_event(w, kcontrol, event);

		ret = regmap_read(cs35l41->regmap, CS35L41_DSP_MBOX_2, &fw_status);
		if (ret < 0) {
			dev_err(cs35l41->dev,
				"Failed to read firmware status: %d\n", ret);
			return ret;
		}

		switch (fw_status) {
		case CSPL_MBOX_STS_RUNNING:
		case CSPL_MBOX_STS_PAUSED:
			break;
		default:
			dev_err(cs35l41->dev, "Firmware status is invalid: %u\n",
				fw_status);
			return -EINVAL;
		}

		return cs35l41_set_cspl_mbox_cmd(cs35l41, CSPL_MBOX_CMD_RESUME);
	case SND_SOC_DAPM_PRE_PMD:
		return cs35l41_set_cspl_mbox_cmd(cs35l41, CSPL_MBOX_CMD_PAUSE);
	default:
		return 0;
	}
}
>>>>>>> upstream/android-13

static const char * const cs35l41_pcm_source_texts[] = {"ASP", "DSP"};
static const unsigned int cs35l41_pcm_source_values[] = {0x08, 0x32};
static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_pcm_source_enum,
<<<<<<< HEAD
				CS35L41_DAC_PCM1_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_pcm_source_texts,
				cs35l41_pcm_source_values);

=======
				  CS35L41_DAC_PCM1_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_pcm_source_texts,
				  cs35l41_pcm_source_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new pcm_source_mux =
	SOC_DAPM_ENUM("PCM Source", cs35l41_pcm_source_enum);

<<<<<<< HEAD
static const char * const cs35l41_tx_input_texts[] = {"Zero", "ASPRX1",
							"ASPRX2", "VMON",
							"IMON", "VPMON",
							"VBSTMON",
							"DSPTX1", "DSPTX2"};
static const unsigned int cs35l41_tx_input_values[] = {0x00,
						CS35L41_INPUT_SRC_ASPRX1,
						CS35L41_INPUT_SRC_ASPRX2,
						CS35L41_INPUT_SRC_VMON,
						CS35L41_INPUT_SRC_IMON,
						CS35L41_INPUT_SRC_VPMON,
						CS35L41_INPUT_SRC_VBSTMON,
						CS35L41_INPUT_DSP_TX1,
						CS35L41_INPUT_DSP_TX2};

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_asptx1_enum,
				CS35L41_ASP_TX1_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_tx_input_texts,
				cs35l41_tx_input_values);
=======
static const char * const cs35l41_tx_input_texts[] = {
	"Zero", "ASPRX1", "ASPRX2", "VMON", "IMON",
	"VPMON", "VBSTMON", "DSPTX1", "DSPTX2"
};

static const unsigned int cs35l41_tx_input_values[] = {
	0x00, CS35L41_INPUT_SRC_ASPRX1, CS35L41_INPUT_SRC_ASPRX2,
	CS35L41_INPUT_SRC_VMON, CS35L41_INPUT_SRC_IMON, CS35L41_INPUT_SRC_VPMON,
	CS35L41_INPUT_SRC_VBSTMON, CS35L41_INPUT_DSP_TX1, CS35L41_INPUT_DSP_TX2
};

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_asptx1_enum,
				  CS35L41_ASP_TX1_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new asp_tx1_mux =
	SOC_DAPM_ENUM("ASPTX1 SRC", cs35l41_asptx1_enum);

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_asptx2_enum,
<<<<<<< HEAD
				CS35L41_ASP_TX2_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_tx_input_texts,
				cs35l41_tx_input_values);
=======
				  CS35L41_ASP_TX2_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new asp_tx2_mux =
	SOC_DAPM_ENUM("ASPTX2 SRC", cs35l41_asptx2_enum);

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_asptx3_enum,
<<<<<<< HEAD
				CS35L41_ASP_TX3_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_tx_input_texts,
				cs35l41_tx_input_values);
=======
				  CS35L41_ASP_TX3_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new asp_tx3_mux =
	SOC_DAPM_ENUM("ASPTX3 SRC", cs35l41_asptx3_enum);

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_asptx4_enum,
<<<<<<< HEAD
				CS35L41_ASP_TX4_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_tx_input_texts,
				cs35l41_tx_input_values);
=======
				  CS35L41_ASP_TX4_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new asp_tx4_mux =
	SOC_DAPM_ENUM("ASPTX4 SRC", cs35l41_asptx4_enum);

<<<<<<< HEAD
static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_dsprx2_enum,
				CS35L41_DSP1_RX2_SRC,
				0, CS35L41_ASP_SOURCE_MASK,
				cs35l41_tx_input_texts,
				cs35l41_tx_input_values);
=======
static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_dsprx1_enum,
				  CS35L41_DSP1_RX1_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);

static const struct snd_kcontrol_new dsp_rx1_mux =
	SOC_DAPM_ENUM("DSPRX1 SRC", cs35l41_dsprx1_enum);

static SOC_VALUE_ENUM_SINGLE_DECL(cs35l41_dsprx2_enum,
				  CS35L41_DSP1_RX2_SRC,
				  0, CS35L41_ASP_SOURCE_MASK,
				  cs35l41_tx_input_texts,
				  cs35l41_tx_input_values);
>>>>>>> upstream/android-13

static const struct snd_kcontrol_new dsp_rx2_mux =
	SOC_DAPM_ENUM("DSPRX2 SRC", cs35l41_dsprx2_enum);

static const struct snd_kcontrol_new cs35l41_aud_controls[] = {
<<<<<<< HEAD
	{.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = "Digital PCM Volume",
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |
	SNDRV_CTL_ELEM_ACCESS_READWRITE,
	.tlv.p  = dig_vol_tlv,
	.info = snd_soc_info_volsw_sx,
	.get = cs35l41_pcm_vol_get,
	.put = cs35l41_pcm_vol_put,
	.private_value = (unsigned long)&(struct soc_mixer_control)
		{.reg = CS35L41_AMP_DIG_VOL_CTRL, .rreg = CS35L41_AMP_DIG_VOL_CTRL,
		.shift = 3, .rshift = 3,
		.max = 0x391, .min = CS35L41_AMP_VOL_PCM_MUTE} },
	SOC_ENUM("Invert PCM", pcm_inv),
	SOC_SINGLE_TLV("AMP PCM Gain", CS35L41_AMP_GAIN_CTRL, 5, 0x14, 0,
			amp_gain_tlv),
	SOC_SINGLE_RANGE("ASPTX1 Slot Position", CS35L41_SP_FRAME_TX_SLOT, 0,
			 0, 7, 0),
	SOC_SINGLE_RANGE("ASPTX2 Slot Position", CS35L41_SP_FRAME_TX_SLOT, 8,
			 0, 7, 0),
	SOC_SINGLE_RANGE("ASPTX3 Slot Position", CS35L41_SP_FRAME_TX_SLOT, 16,
			 0, 7, 0),
	SOC_SINGLE_RANGE("ASPTX4 Slot Position", CS35L41_SP_FRAME_TX_SLOT, 24,
			 0, 7, 0),
	SOC_SINGLE_RANGE("ASPRX1 Slot Position", CS35L41_SP_FRAME_RX_SLOT, 0,
			 0, 7, 0),
	SOC_SINGLE_RANGE("ASPRX2 Slot Position", CS35L41_SP_FRAME_RX_SLOT, 8,
			 0, 7, 0),
	SOC_ENUM("VPBR Release Rate", vpbr_rel_rate),
	SOC_ENUM("VPBR Wait", vpbr_wait),
	SOC_ENUM("VPBR Attack Rate", vpbr_atk_rate),
	SOC_ENUM("VPBR Attack Volume", vpbr_atk_vol),
	SOC_SINGLE_RANGE("VPBR Max Attenuation", CS35L41_VPBR_CFG, 8, 0, 15, 0),
	SOC_ENUM("VPBR Threshold 1", vpbr_thld1),
	SOC_ENUM("VPBR Enable", vpbr_enable),
	SOC_ENUM("PCM Soft Ramp", pcm_sft_ramp),
	SOC_ENUM_EXT("AMP Mute", amp_mute_ctl, cs35l41_amp_mute_get,
						cs35l41_amp_mute_put),
	SOC_ENUM("Boost Enable", bst_en_ctl),
	SOC_SINGLE_EXT("DSP Booted", SND_SOC_NOPM, 0, 1, 0,
			cs35l41_halo_booted_get, cs35l41_halo_booted_put),
	WM_ADSP2_PRELOAD_SWITCH("DSP1", 1),
};

static const struct otp_map_element_t *find_otp_map(u32 otp_id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(otp_map_map); i++) {
		if (otp_map_map[i].id == otp_id)
			return &otp_map_map[i];
=======
	SOC_SINGLE_SX_TLV("Digital PCM Volume", CS35L41_AMP_DIG_VOL_CTRL,
			  3, 0x4CF, 0x391, dig_vol_tlv),
	SOC_SINGLE_TLV("Analog PCM Volume", CS35L41_AMP_GAIN_CTRL, 5, 0x14, 0,
		       amp_gain_tlv),
	SOC_ENUM("PCM Soft Ramp", pcm_sft_ramp),
	SOC_SINGLE("HW Noise Gate Enable", CS35L41_NG_CFG, 8, 63, 0),
	SOC_SINGLE("HW Noise Gate Delay", CS35L41_NG_CFG, 4, 7, 0),
	SOC_SINGLE("HW Noise Gate Threshold", CS35L41_NG_CFG, 0, 7, 0),
	SOC_SINGLE("Aux Noise Gate CH1 Enable",
		   CS35L41_MIXER_NGATE_CH1_CFG, 16, 1, 0),
	SOC_SINGLE("Aux Noise Gate CH1 Entry Delay",
		   CS35L41_MIXER_NGATE_CH1_CFG, 8, 15, 0),
	SOC_SINGLE("Aux Noise Gate CH1 Threshold",
		   CS35L41_MIXER_NGATE_CH1_CFG, 0, 7, 0),
	SOC_SINGLE("Aux Noise Gate CH2 Entry Delay",
		   CS35L41_MIXER_NGATE_CH2_CFG, 8, 15, 0),
	SOC_SINGLE("Aux Noise Gate CH2 Enable",
		   CS35L41_MIXER_NGATE_CH2_CFG, 16, 1, 0),
	SOC_SINGLE("Aux Noise Gate CH2 Threshold",
		   CS35L41_MIXER_NGATE_CH2_CFG, 0, 7, 0),
	SOC_SINGLE("SCLK Force", CS35L41_SP_FORMAT, CS35L41_SCLK_FRC_SHIFT, 1, 0),
	SOC_SINGLE("LRCLK Force", CS35L41_SP_FORMAT, CS35L41_LRCLK_FRC_SHIFT, 1, 0),
	SOC_SINGLE("Invert Class D", CS35L41_AMP_DIG_VOL_CTRL,
		   CS35L41_AMP_INV_PCM_SHIFT, 1, 0),
	SOC_SINGLE("Amp Gain ZC", CS35L41_AMP_GAIN_CTRL,
		   CS35L41_AMP_GAIN_ZC_SHIFT, 1, 0),
	WM_ADSP2_PRELOAD_SWITCH("DSP1", 1),
	WM_ADSP_FW_CONTROL("DSP1", 0),
};

static const struct cs35l41_otp_map_element_t *cs35l41_find_otp_map(u32 otp_id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cs35l41_otp_map_map); i++) {
		if (cs35l41_otp_map_map[i].id == otp_id)
			return &cs35l41_otp_map_map[i];
>>>>>>> upstream/android-13
	}

	return NULL;
}

static int cs35l41_otp_unpack(void *data)
{
<<<<<<< HEAD
	struct cs35l41_private *cs35l41 = data;
	u32 otp_mem[32];
	int i;
	/* unpack area starts at byte 10 (0-indexed) */
	int bit_offset = 16, array_offset = 2;
	unsigned int bit_sum = 8;
	u32 otp_val, otp_id_reg;
	const struct otp_map_element_t *otp_map_match;
	const struct otp_packed_element_t *otp_map;

	/*
	 * We need to make sure we are using the bus
	 * for these reads and writes so bypass
	 * cache completely to ensure we hit the
	 * registers correctly
	 */
	regcache_cache_bypass(cs35l41->regmap, true);

	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00005555);
	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x0000AAAA);

	regmap_read(cs35l41->regmap, CS35L41_OTPID, &otp_id_reg);
	/* Read from OTP_MEM_IF */
	for (i = 0; i < 32; i++) {
		regmap_read(cs35l41->regmap, CS35L41_OTP_MEM0 + i * 4, &(otp_mem[i]));
		usleep_range(1, 10);
	}

	if (((otp_mem[1] & CS35L41_OTP_HDR_MASK_1) != CS35L41_OTP_HDR_VAL_1)
			|| (otp_mem[2] & CS35L41_OTP_HDR_MASK_2) != CS35L41_OTP_HDR_VAL_2) {
		dev_err(cs35l41->dev, "Bad OTP header vals\n");
		return -EINVAL;
	}

	otp_map_match = find_otp_map(otp_id_reg);

	if (otp_map_match == NULL) {
		dev_err(cs35l41->dev, "OTP Map matching ID %d not found\n",
				otp_id_reg);
		return -EINVAL;
	}

	otp_map = otp_map_match->map;

	for (i = 0; i < otp_map_match->num_elements; i++) {
		cs35l41_dbg(cs35l41->dev, "bitoffset= %d, array_offset=%d, bit_sum mod 32=%d\n",
					bit_offset, array_offset, bit_sum % 32);
		cs35l41_dbg(cs35l41->dev, "i: %d reg: %d, shift: %d size: %d\n",
			i, otp_map[i].reg, otp_map[i].shift, otp_map[i].size);
		if (bit_offset + otp_map[i].size - 1 >= 32) {
			otp_val = (otp_mem[array_offset] &
					GENMASK(31, bit_offset)) >>
					bit_offset;
			otp_val |= (otp_mem[++array_offset] &
=======
	const struct cs35l41_otp_map_element_t *otp_map_match;
	const struct cs35l41_otp_packed_element_t *otp_map;
	struct cs35l41_private *cs35l41 = data;
	int bit_offset, word_offset, ret, i;
	unsigned int orig_spi_freq;
	unsigned int bit_sum = 8;
	u32 otp_val, otp_id_reg;
	u32 *otp_mem;

	otp_mem = kmalloc_array(CS35L41_OTP_SIZE_WORDS, sizeof(*otp_mem), GFP_KERNEL);
	if (!otp_mem)
		return -ENOMEM;

	ret = regmap_read(cs35l41->regmap, CS35L41_OTPID, &otp_id_reg);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Read OTP ID failed: %d\n", ret);
		goto err_otp_unpack;
	}

	otp_map_match = cs35l41_find_otp_map(otp_id_reg);

	if (!otp_map_match) {
		dev_err(cs35l41->dev, "OTP Map matching ID %d not found\n",
			otp_id_reg);
		ret = -EINVAL;
		goto err_otp_unpack;
	}

	if (cs35l41->otp_setup)
		cs35l41->otp_setup(cs35l41, true, &orig_spi_freq);

	ret = regmap_bulk_read(cs35l41->regmap, CS35L41_OTP_MEM0, otp_mem,
			       CS35L41_OTP_SIZE_WORDS);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Read OTP Mem failed: %d\n", ret);
		goto err_otp_unpack;
	}

	if (cs35l41->otp_setup)
		cs35l41->otp_setup(cs35l41, false, &orig_spi_freq);

	otp_map = otp_map_match->map;

	bit_offset = otp_map_match->bit_offset;
	word_offset = otp_map_match->word_offset;

	ret = regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00000055);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write Unlock key failed 1/2: %d\n", ret);
		goto err_otp_unpack;
	}
	ret = regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x000000AA);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write Unlock key failed 2/2: %d\n", ret);
		goto err_otp_unpack;
	}

	for (i = 0; i < otp_map_match->num_elements; i++) {
		dev_dbg(cs35l41->dev,
			"bitoffset= %d, word_offset=%d, bit_sum mod 32=%d\n",
			bit_offset, word_offset, bit_sum % 32);
		if (bit_offset + otp_map[i].size - 1 >= 32) {
			otp_val = (otp_mem[word_offset] &
					GENMASK(31, bit_offset)) >>
					bit_offset;
			otp_val |= (otp_mem[++word_offset] &
>>>>>>> upstream/android-13
					GENMASK(bit_offset +
						otp_map[i].size - 33, 0)) <<
					(32 - bit_offset);
			bit_offset += otp_map[i].size - 32;
		} else {
<<<<<<< HEAD

			otp_val = (otp_mem[array_offset] &
=======
			otp_val = (otp_mem[word_offset] &
>>>>>>> upstream/android-13
				GENMASK(bit_offset + otp_map[i].size - 1,
					bit_offset)) >>	bit_offset;
			bit_offset += otp_map[i].size;
		}
		bit_sum += otp_map[i].size;

		if (bit_offset == 32) {
			bit_offset = 0;
<<<<<<< HEAD
			array_offset++;
		}

		if (otp_map[i].reg != 0)
			regmap_update_bits(cs35l41->regmap, otp_map[i].reg,
					GENMASK(otp_map[i].shift +
							otp_map[i].size - 1,
						otp_map[i].shift),
					otp_val << otp_map[i].shift);
	}

	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x0000CCCC);
	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00003333);

	regcache_cache_bypass(cs35l41->regmap, false);

	return 0;
=======
			word_offset++;
		}

		if (otp_map[i].reg != 0) {
			ret = regmap_update_bits(cs35l41->regmap,
						 otp_map[i].reg,
						 GENMASK(otp_map[i].shift +
							 otp_map[i].size - 1,
						 otp_map[i].shift),
						 otp_val << otp_map[i].shift);
			if (ret < 0) {
				dev_err(cs35l41->dev, "Write OTP val failed: %d\n",
					ret);
				goto err_otp_unpack;
			}
		}
	}

	ret = regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x000000CC);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write Lock key failed 1/2: %d\n", ret);
		goto err_otp_unpack;
	}
	ret = regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00000033);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write Lock key failed 2/2: %d\n", ret);
		goto err_otp_unpack;
	}
	ret = 0;

err_otp_unpack:
	kfree(otp_mem);
	return ret;
>>>>>>> upstream/android-13
}

static irqreturn_t cs35l41_irq(int irq, void *data)
{
	struct cs35l41_private *cs35l41 = data;
<<<<<<< HEAD
	unsigned int status[4];
	unsigned int masks[4];
	int i;
=======
	unsigned int status[4] = { 0, 0, 0, 0 };
	unsigned int masks[4] = { 0, 0, 0, 0 };
	int ret = IRQ_NONE;
	unsigned int i;
>>>>>>> upstream/android-13

	for (i = 0; i < ARRAY_SIZE(status); i++) {
		regmap_read(cs35l41->regmap,
			    CS35L41_IRQ1_STATUS1 + (i * CS35L41_REGSTRIDE),
			    &status[i]);
		regmap_read(cs35l41->regmap,
			    CS35L41_IRQ1_MASK1 + (i * CS35L41_REGSTRIDE),
			    &masks[i]);
	}

	/* Check to see if unmasked bits are active */
	if (!(status[0] & ~masks[0]) && !(status[1] & ~masks[1]) &&
<<<<<<< HEAD
		!(status[2] & ~masks[2]) && !(status[3] & ~masks[3]))
		return IRQ_NONE;


	if (status[0] & CS35L41_PUP_DONE_MASK) {
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_PUP_DONE_MASK);
		complete(&cs35l41->global_pup_done);
	}

	if (status[0] & CS35L41_PDN_DONE_MASK) {
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_PDN_DONE_MASK);
		complete(&cs35l41->global_pdn_done);
=======
	    !(status[2] & ~masks[2]) && !(status[3] & ~masks[3]))
		return IRQ_NONE;

	if (status[3] & CS35L41_OTP_BOOT_DONE) {
		regmap_update_bits(cs35l41->regmap, CS35L41_IRQ1_MASK4,
				   CS35L41_OTP_BOOT_DONE, CS35L41_OTP_BOOT_DONE);
>>>>>>> upstream/android-13
	}

	/*
	 * The following interrupts require a
	 * protection release cycle to get the
	 * speaker out of Safe-Mode.
	 */
	if (status[0] & CS35L41_AMP_SHORT_ERR) {
<<<<<<< HEAD
		dev_crit(cs35l41->dev, "Amp short error\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_AMP_SHORT_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_AMP_SHORT_ERR_RLS,
					CS35L41_AMP_SHORT_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_AMP_SHORT_ERR_RLS, 0);
		cirrus_bd_amp_err(cs35l41->pdata.mfd_suffix);
	}

	if (status[0] & CS35L41_TEMP_WARN) {
		dev_crit(cs35l41->dev, "Over temperature warning\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_TEMP_WARN);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_TEMP_WARN_ERR_RLS,
					CS35L41_TEMP_WARN_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_TEMP_WARN_ERR_RLS, 0);
	}

	if (status[0] & CS35L41_TEMP_ERR) {
		dev_crit(cs35l41->dev, "Over temperature error\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_TEMP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_TEMP_ERR_RLS,
					CS35L41_TEMP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_TEMP_ERR_RLS, 0);
	}

	if (status[0] & CS35L41_BST_OVP_ERR) {
		dev_crit(cs35l41->dev, "VBST Over Voltage error\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_BST_OVP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_OVP_ERR_RLS,
					CS35L41_BST_OVP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_OVP_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT,
					CS35L41_BST_EN_DEFAULT <<
					CS35L41_BST_EN_SHIFT);
	}

	if (status[0] & CS35L41_BST_DCM_UVP_ERR) {
		dev_crit(cs35l41->dev, "DCM VBST Under Voltage Error\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_BST_DCM_UVP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_UVP_ERR_RLS,
					CS35L41_BST_UVP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_UVP_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT,
					CS35L41_BST_EN_DEFAULT <<
					CS35L41_BST_EN_SHIFT);
	}

	if (status[0] & CS35L41_BST_SHORT_ERR) {
		dev_crit(cs35l41->dev, "LBST error: powering off!\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					CS35L41_BST_SHORT_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_SHORT_ERR_RLS,
					CS35L41_BST_SHORT_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
					CS35L41_BST_SHORT_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
					CS35L41_BST_EN_MASK <<
					CS35L41_BST_EN_SHIFT,
					CS35L41_BST_EN_DEFAULT <<
					CS35L41_BST_EN_SHIFT);
		cirrus_bd_bst_short(cs35l41->pdata.mfd_suffix);
	}

	if (status[3] & CS35L41_OTP_BOOT_DONE) {
		regmap_update_bits(cs35l41->regmap, CS35L41_IRQ1_MASK4,
				CS35L41_OTP_BOOT_DONE, CS35L41_OTP_BOOT_DONE);
	}

	return IRQ_HANDLED;
}

static const struct reg_sequence cs35l41_pup_patch[] = {
	{0x00000040, 0x00000055},
	{0x00000040, 0x000000AA},
	{0x00002084, 0x002F1AA0},
	{0x00000040, 0x000000CC},
	{0x00000040, 0x00000033},
};

static const struct reg_sequence cs35l41_pdn_patch[] = {
	{0x00000040, 0x00000055},
	{0x00000040, 0x000000AA},
	{0x00002084, 0x002F1AA3},
	{0x00000040, 0x000000CC},
	{0x00000040, 0x00000033},
};

static bool cs35l41_is_csplmboxsts_correct(enum cs35l41_cspl_mboxcmd cmd,
					   enum cs35l41_cspl_mboxstate sts)
{
	switch (cmd) {
	case CSPL_MBOX_CMD_NONE:
	case CSPL_MBOX_CMD_UNKNOWN_CMD:
		return true;
	case CSPL_MBOX_CMD_PAUSE:
		return (sts == CSPL_MBOX_STS_PAUSED);
	case CSPL_MBOX_CMD_RESUME:
		return (sts == CSPL_MBOX_STS_RUNNING);
	default:
		return false;
	}
}

static int cs35l41_set_csplmboxcmd(struct cs35l41_private *cs35l41,
				   enum cs35l41_cspl_mboxcmd cmd)
{
	int		ret = 0;
	unsigned int	sts, i;
	bool		ack = false;

	/* Reset DSP sticky bit */
	regmap_write(cs35l41->regmap, CS35L41_IRQ2_STATUS2,
		     1 << CS35L41_CSPL_MBOX_CMD_DRV_SHIFT);

	/* Reset AP sticky bit */
	regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS2,
		     1 << CS35L41_CSPL_MBOX_CMD_FW_SHIFT);

	/*
	 * Set mailbox cmd
	 */
	/* Unmask DSP INT */
	regmap_update_bits(cs35l41->regmap, CS35L41_IRQ2_MASK2,
			   1 << CS35L41_CSPL_MBOX_CMD_DRV_SHIFT, 0);
	regmap_write(cs35l41->regmap, CS35L41_CSPL_MBOX_CMD_DRV, cmd);

	/* Poll for DSP ACK */
	for (i = 0; i < 15; i++) {
		usleep_range(1000, 1010);
		ret = regmap_read(cs35l41->regmap, CS35L41_IRQ1_STATUS2, &sts);
		if (ret < 0) {
			dev_err(cs35l41->dev, "regmap_read failed (%d)\n", ret);
			cs35l41_log_regmap_fail(cs35l41, CS35L41_IRQ1_STATUS2);
			continue;
		}
		if (sts & (1 << CS35L41_CSPL_MBOX_CMD_FW_SHIFT)) {
			cs35l41_dbg(cs35l41->dev,
				"%u: Received ACK in EINT for mbox cmd (%d)\n",
				i, cmd);
			regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS2,
			     1 << CS35L41_CSPL_MBOX_CMD_FW_SHIFT);
			ack = true;
			break;
		}
	}

	if (!ack) {
		dev_err(cs35l41->dev,
			"Timeout waiting for DSP to set mbox cmd\n");
		ret = -ETIMEDOUT;
	}

	/* Mask DSP INT */
	regmap_update_bits(cs35l41->regmap, CS35L41_IRQ2_MASK2,
			   1 << CS35L41_CSPL_MBOX_CMD_DRV_SHIFT,
			   1 << CS35L41_CSPL_MBOX_CMD_DRV_SHIFT);

	if (regmap_read(cs35l41->regmap,
			CS35L41_CSPL_MBOX_STS, &sts) < 0) {
		dev_err(cs35l41->dev, "Failed to read %u\n",
			CS35L41_CSPL_MBOX_STS);
		cs35l41_log_regmap_fail(cs35l41, CS35L41_CSPL_MBOX_STS);
		ret = -EACCES;
	}

	if (!cs35l41_is_csplmboxsts_correct(cmd,
					    (enum cs35l41_cspl_mboxstate)sts)) {
		dev_err(cs35l41->dev,
			"Failed to set mailbox(cmd: %u, sts: %u)\n", cmd, sts);
		ret = -ENOMSG;
=======
		dev_crit_ratelimited(cs35l41->dev, "Amp short error\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_AMP_SHORT_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_AMP_SHORT_ERR_RLS,
				   CS35L41_AMP_SHORT_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_AMP_SHORT_ERR_RLS, 0);
		ret = IRQ_HANDLED;
	}

	if (status[0] & CS35L41_TEMP_WARN) {
		dev_crit_ratelimited(cs35l41->dev, "Over temperature warning\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_TEMP_WARN);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_TEMP_WARN_ERR_RLS,
				   CS35L41_TEMP_WARN_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_TEMP_WARN_ERR_RLS, 0);
		ret = IRQ_HANDLED;
	}

	if (status[0] & CS35L41_TEMP_ERR) {
		dev_crit_ratelimited(cs35l41->dev, "Over temperature error\n");
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_TEMP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_TEMP_ERR_RLS,
				   CS35L41_TEMP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_TEMP_ERR_RLS, 0);
		ret = IRQ_HANDLED;
	}

	if (status[0] & CS35L41_BST_OVP_ERR) {
		dev_crit_ratelimited(cs35l41->dev, "VBST Over Voltage error\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_BST_OVP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_OVP_ERR_RLS,
				   CS35L41_BST_OVP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_OVP_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK,
				   CS35L41_BST_EN_DEFAULT << CS35L41_BST_EN_SHIFT);
		ret = IRQ_HANDLED;
	}

	if (status[0] & CS35L41_BST_DCM_UVP_ERR) {
		dev_crit_ratelimited(cs35l41->dev, "DCM VBST Under Voltage Error\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_BST_DCM_UVP_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_UVP_ERR_RLS,
				   CS35L41_BST_UVP_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_UVP_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK,
				   CS35L41_BST_EN_DEFAULT << CS35L41_BST_EN_SHIFT);
		ret = IRQ_HANDLED;
	}

	if (status[0] & CS35L41_BST_SHORT_ERR) {
		dev_crit_ratelimited(cs35l41->dev, "LBST error: powering off!\n");
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK, 0);
		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_BST_SHORT_ERR);
		regmap_write(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_SHORT_ERR_RLS,
				   CS35L41_BST_SHORT_ERR_RLS);
		regmap_update_bits(cs35l41->regmap, CS35L41_PROTECT_REL_ERR_IGN,
				   CS35L41_BST_SHORT_ERR_RLS, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				   CS35L41_BST_EN_MASK,
				   CS35L41_BST_EN_DEFAULT << CS35L41_BST_EN_SHIFT);
		ret = IRQ_HANDLED;
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
static int cs35l41_cap_trim(struct cs35l41_private *cs35l41, bool rcv_mode)
{
	cs35l41_dbg(cs35l41->dev, "%s rcv_mode=%d\n", __func__, rcv_mode);

	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00005555);
	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x0000AAAA);

	if (rcv_mode) {
		regmap_update_bits(cs35l41->regmap, CS35L41_OTP_TRIM_30,
				CS35L41_INT1_CAP_TRIM_MASK, 0);
		regmap_update_bits(cs35l41->regmap, CS35L41_OTP_TRIM_31,
				CS35L41_INT2_CAP_TRIM_MASK, 0);
	} else {
		regmap_write(cs35l41->regmap, CS35L41_OTP_TRIM_30,
				cs35l41->spk_3_trim);
		regmap_write(cs35l41->regmap, CS35L41_OTP_TRIM_31,
				cs35l41->spk_4_trim);
	}

	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x0000CCCC);
	regmap_write(cs35l41->regmap, CS35L41_TEST_KEY_CTL, 0x00003333);

	return 0;
}

static int cs35l41_pcm_source_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	unsigned int source, global_en;

	regmap_read(cs35l41->regmap, CS35L41_DAC_PCM1_SRC, &source);

	if (source == CS35L41_INPUT_SRC_ASPRX1)
		cs35l41->halo_routed = false;
	else if (source == CS35L41_INPUT_DSP_TX1)
		cs35l41->halo_routed = true;

	if (source != cs35l41->pcm_source_last) {
		cs35l41_dbg(cs35l41->dev, "PCM Source changed\n");
		cs35l41_cap_trim(cs35l41, source == CS35L41_INPUT_SRC_ASPRX1);
		regmap_read(cs35l41->regmap, CS35L41_PWR_CTRL1, &global_en);
		if (cs35l41->halo_booted && global_en & CS35L41_GLOBAL_EN_MASK) {
			if (cs35l41->halo_routed) {
				cs35l41_set_csplmboxcmd(cs35l41,
							CSPL_MBOX_CMD_RESUME);
			} else if (!cs35l41->halo_routed) {
				cs35l41_set_csplmboxcmd(cs35l41,
							CSPL_MBOX_CMD_PAUSE);
				regcache_drop_region(cs35l41->regmap,
							CS35L41_DAC_PCM1_SRC,
							CS35L41_DAC_PCM1_SRC);
				regmap_write(cs35l41->regmap,
						CS35L41_DAC_PCM1_SRC, source);
			}
		}
	}

	cs35l41->pcm_source_last = source;

	cs35l41_dbg(cs35l41->dev, "PCM Source: %s\n",
			(source == CS35L41_INPUT_SRC_ASPRX1) ?
			"ASPRX1" : "DSPTX1");

	return 0;
}

static int cs35l41_dsp_rx2_src_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	unsigned int source;

	regmap_read(cs35l41->regmap, CS35L41_DSP1_RX2_SRC, &source);
	if (source != 0)
		cs35l41->dsprx2_src = source;

	return 0;
}

static int cs35l41_main_amp_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
=======
static const struct reg_sequence cs35l41_pup_patch[] = {
	{ 0x00000040, 0x00000055 },
	{ 0x00000040, 0x000000AA },
	{ 0x00002084, 0x002F1AA0 },
	{ 0x00000040, 0x000000CC },
	{ 0x00000040, 0x00000033 },
};

static const struct reg_sequence cs35l41_pdn_patch[] = {
	{ 0x00000040, 0x00000055 },
	{ 0x00000040, 0x000000AA },
	{ 0x00002084, 0x002F1AA3 },
	{ 0x00000040, 0x000000CC },
	{ 0x00000040, 0x00000033 },
};

static int cs35l41_main_amp_event(struct snd_soc_dapm_widget *w,
				  struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	unsigned int val;
>>>>>>> upstream/android-13
	int ret = 0;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		regmap_multi_reg_write_bypassed(cs35l41->regmap,
<<<<<<< HEAD
					cs35l41_pup_patch,
					ARRAY_SIZE(cs35l41_pup_patch));

		ret = regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL1,
				CS35L41_GLOBAL_EN_MASK,
				1 << CS35L41_GLOBAL_EN_SHIFT);
		if (ret < 0)
			cs35l41_log_regmap_fail(cs35l41, CS35L41_PWR_CTRL1);

		usleep_range(1000, 1100);

		if (cs35l41->halo_booted && cs35l41->halo_routed) {
			cs35l41_set_csplmboxcmd(cs35l41,
						CSPL_MBOX_CMD_RESUME);
		} else {
			if (cs35l41->dsp.preloaded) {
				if (cs35l41->halo_played == false) {
					cs35l41_set_csplmboxcmd(cs35l41,
							CSPL_MBOX_CMD_PAUSE);
					regcache_drop_region(cs35l41->regmap,
							CS35L41_DAC_PCM1_SRC,
							CS35L41_DAC_PCM1_SRC);
					regmap_write(cs35l41->regmap,
						CS35L41_DAC_PCM1_SRC,
						CS35L41_INPUT_SRC_ASPRX1);
				}
			}
			usleep_range(1000, 1100);
		}

		cirrus_pwr_start(cs35l41->pdata.mfd_suffix);

		cs35l41->halo_played = true;
		dev_info(cs35l41->dev, "%s PMU\n", __func__);
		break;
	case SND_SOC_DAPM_POST_PMD:
		if (cs35l41->halo_booted && cs35l41->halo_routed)
			cs35l41_set_csplmboxcmd(cs35l41,
						CSPL_MBOX_CMD_PAUSE);

		ret = regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL1,
				CS35L41_GLOBAL_EN_MASK, 0);
		if (ret < 0)
			cs35l41_log_regmap_fail(cs35l41, CS35L41_PWR_CTRL1);

		regmap_multi_reg_write_bypassed(cs35l41->regmap,
					cs35l41_pdn_patch,
					ARRAY_SIZE(cs35l41_pdn_patch));

		cirrus_pwr_stop(cs35l41->pdata.mfd_suffix);

		dev_info(cs35l41->dev, "%s PMD\n", __func__);
=======
						cs35l41_pup_patch,
						ARRAY_SIZE(cs35l41_pup_patch));

		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL1,
				   CS35L41_GLOBAL_EN_MASK,
				   1 << CS35L41_GLOBAL_EN_SHIFT);

		usleep_range(1000, 1100);
		break;
	case SND_SOC_DAPM_POST_PMD:
		regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL1,
				   CS35L41_GLOBAL_EN_MASK, 0);

		ret = regmap_read_poll_timeout(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
					       val, val &  CS35L41_PDN_DONE_MASK,
					       1000, 100000);
		if (ret)
			dev_warn(cs35l41->dev, "PDN failed: %d\n", ret);

		regmap_write(cs35l41->regmap, CS35L41_IRQ1_STATUS1,
			     CS35L41_PDN_DONE_MASK);

		regmap_multi_reg_write_bypassed(cs35l41->regmap,
						cs35l41_pdn_patch,
						ARRAY_SIZE(cs35l41_pdn_patch));
>>>>>>> upstream/android-13
		break;
	default:
		dev_err(cs35l41->dev, "Invalid event = 0x%x\n", event);
		ret = -EINVAL;
	}
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	return ret;
}

static const struct snd_soc_dapm_widget cs35l41_dapm_widgets[] = {
<<<<<<< HEAD

	SND_SOC_DAPM_SPK("DSP1 Preload", NULL),
	{	.id = snd_soc_dapm_supply, .name = "DSP1 Preloader",
		.reg = SND_SOC_NOPM, .shift = 0, .event = cs35l41_dsp_power_ev,
		.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD,
		.subseq = 100,},
	{       .id = snd_soc_dapm_out_drv, .name = "DSP1",
		.reg = SND_SOC_NOPM, .shift = 0},

	SND_SOC_DAPM_OUTPUT("AMP SPK"),
=======
	SND_SOC_DAPM_SPK("DSP1 Preload", NULL),
	SND_SOC_DAPM_SUPPLY_S("DSP1 Preloader", 100, SND_SOC_NOPM, 0, 0,
			      cs35l41_dsp_preload_ev,
			      SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_PRE_PMD),
	SND_SOC_DAPM_OUT_DRV_E("DSP1", SND_SOC_NOPM, 0, 0, NULL, 0,
			       cs35l41_dsp_audio_ev,
			       SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),

	SND_SOC_DAPM_OUTPUT("SPK"),
>>>>>>> upstream/android-13

	SND_SOC_DAPM_AIF_IN("ASPRX1", NULL, 0, CS35L41_SP_ENABLES, 16, 0),
	SND_SOC_DAPM_AIF_IN("ASPRX2", NULL, 0, CS35L41_SP_ENABLES, 17, 0),
	SND_SOC_DAPM_AIF_OUT("ASPTX1", NULL, 0, CS35L41_SP_ENABLES, 0, 0),
	SND_SOC_DAPM_AIF_OUT("ASPTX2", NULL, 0, CS35L41_SP_ENABLES, 1, 0),
	SND_SOC_DAPM_AIF_OUT("ASPTX3", NULL, 0, CS35L41_SP_ENABLES, 2, 0),
	SND_SOC_DAPM_AIF_OUT("ASPTX4", NULL, 0, CS35L41_SP_ENABLES, 3, 0),

<<<<<<< HEAD
	SND_SOC_DAPM_ADC("VMON ADC", NULL, CS35L41_PWR_CTRL2, 12, 0),
	SND_SOC_DAPM_ADC("IMON ADC", NULL, CS35L41_PWR_CTRL2, 13, 0),
	SND_SOC_DAPM_ADC("VPMON ADC", NULL, CS35L41_PWR_CTRL2, 8, 0),
	SND_SOC_DAPM_ADC("VBSTMON ADC", NULL, CS35L41_PWR_CTRL2, 9, 0),
	SND_SOC_DAPM_ADC("TEMPMON ADC", NULL, CS35L41_PWR_CTRL2, 10, 0),
	SND_SOC_DAPM_ADC("CLASS H", NULL, CS35L41_PWR_CTRL3, 4, 0),

	SND_SOC_DAPM_OUT_DRV_E("Main AMP", CS35L41_PWR_CTRL2, 0, 0, NULL, 0,
				cs35l41_main_amp_event,
				SND_SOC_DAPM_POST_PMD |	SND_SOC_DAPM_POST_PMU),

	SND_SOC_DAPM_INPUT("VP"),
	SND_SOC_DAPM_INPUT("VBST"),
	SND_SOC_DAPM_INPUT("ISENSE"),
	SND_SOC_DAPM_INPUT("VSENSE"),
	SND_SOC_DAPM_INPUT("TEMP"),
=======
	SND_SOC_DAPM_SIGGEN("VSENSE"),
	SND_SOC_DAPM_SIGGEN("ISENSE"),
	SND_SOC_DAPM_SIGGEN("VP"),
	SND_SOC_DAPM_SIGGEN("VBST"),
	SND_SOC_DAPM_SIGGEN("TEMP"),

	SND_SOC_DAPM_SUPPLY("VMON", CS35L41_PWR_CTRL2, 12, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("IMON", CS35L41_PWR_CTRL2, 13, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("VPMON", CS35L41_PWR_CTRL2, 8, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("VBSTMON", CS35L41_PWR_CTRL2, 9, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("TEMPMON", CS35L41_PWR_CTRL2, 10, 0, NULL, 0),

	SND_SOC_DAPM_ADC("VMON ADC", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_ADC("IMON ADC", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_ADC("VPMON ADC", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_ADC("VBSTMON ADC", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_ADC("TEMPMON ADC", NULL, SND_SOC_NOPM, 0, 0),

	SND_SOC_DAPM_ADC("CLASS H", NULL, CS35L41_PWR_CTRL3, 4, 0),

	SND_SOC_DAPM_OUT_DRV_E("Main AMP", CS35L41_PWR_CTRL2, 0, 0, NULL, 0,
			       cs35l41_main_amp_event,
			       SND_SOC_DAPM_POST_PMD |	SND_SOC_DAPM_POST_PMU),
>>>>>>> upstream/android-13

	SND_SOC_DAPM_MUX("ASP TX1 Source", SND_SOC_NOPM, 0, 0, &asp_tx1_mux),
	SND_SOC_DAPM_MUX("ASP TX2 Source", SND_SOC_NOPM, 0, 0, &asp_tx2_mux),
	SND_SOC_DAPM_MUX("ASP TX3 Source", SND_SOC_NOPM, 0, 0, &asp_tx3_mux),
	SND_SOC_DAPM_MUX("ASP TX4 Source", SND_SOC_NOPM, 0, 0, &asp_tx4_mux),
<<<<<<< HEAD
	SND_SOC_DAPM_MUX_E("DSP RX2 Source", SND_SOC_NOPM, 0, 0, &dsp_rx2_mux,
			cs35l41_dsp_rx2_src_event, SND_SOC_DAPM_PRE_PMU),
	SND_SOC_DAPM_MUX_E("PCM Source", SND_SOC_NOPM, 0, 0, &pcm_source_mux,
				cs35l41_pcm_source_event, SND_SOC_DAPM_PRE_PMU |
				SND_SOC_DAPM_POST_REG),
	SND_SOC_DAPM_SWITCH("DRE", SND_SOC_NOPM, 0, 0, &dre_ctrl),
	SND_SOC_DAPM_SWITCH("AMP Enable", SND_SOC_NOPM, 0, 1, &amp_enable_ctrl),
};

static const struct snd_soc_dapm_route cs35l41_audio_map[] = {

	{ "DSP1", NULL, "ASPRX1" },
	{ "DSP1 Preload", NULL, "DSP1 Preloader" },

	{"DSP RX2 Source", "VMON", "VMON ADC"},
	{"DSP RX2 Source", "IMON", "IMON ADC"},
	{"DSP RX2 Source", "VPMON", "VPMON ADC"},
	{"DSP RX2 Source", "DSPTX1", "DSP1"},
	{"DSP RX2 Source", "DSPTX2", "DSP1"},
	{"DSP RX2 Source", "ASPRX1", "ASPRX1" },
	{"DSP RX2 Source", "ASPRX2", "ASPRX2" },
	{"DSP RX2 Source", "Zero", "ASPRX1" },
=======
	SND_SOC_DAPM_MUX("DSP RX1 Source", SND_SOC_NOPM, 0, 0, &dsp_rx1_mux),
	SND_SOC_DAPM_MUX("DSP RX2 Source", SND_SOC_NOPM, 0, 0, &dsp_rx2_mux),
	SND_SOC_DAPM_MUX("PCM Source", SND_SOC_NOPM, 0, 0, &pcm_source_mux),
	SND_SOC_DAPM_SWITCH("DRE", SND_SOC_NOPM, 0, 0, &dre_ctrl),
};

static const struct snd_soc_dapm_route cs35l41_audio_map[] = {
	{"DSP RX1 Source", "ASPRX1", "ASPRX1"},
	{"DSP RX1 Source", "ASPRX2", "ASPRX2"},
	{"DSP RX2 Source", "ASPRX1", "ASPRX1"},
	{"DSP RX2 Source", "ASPRX2", "ASPRX2"},

	{"DSP1", NULL, "DSP RX1 Source"},
>>>>>>> upstream/android-13
	{"DSP1", NULL, "DSP RX2 Source"},

	{"ASP TX1 Source", "VMON", "VMON ADC"},
	{"ASP TX1 Source", "IMON", "IMON ADC"},
	{"ASP TX1 Source", "VPMON", "VPMON ADC"},
	{"ASP TX1 Source", "VBSTMON", "VBSTMON ADC"},
<<<<<<< HEAD
	{"ASP TX1 Source", "DSPTX1", "ASPRX1"},
	{"ASP TX1 Source", "DSPTX2", "ASPRX1"},
=======
	{"ASP TX1 Source", "DSPTX1", "DSP1"},
	{"ASP TX1 Source", "DSPTX2", "DSP1"},
>>>>>>> upstream/android-13
	{"ASP TX1 Source", "ASPRX1", "ASPRX1" },
	{"ASP TX1 Source", "ASPRX2", "ASPRX2" },
	{"ASP TX2 Source", "VMON", "VMON ADC"},
	{"ASP TX2 Source", "IMON", "IMON ADC"},
	{"ASP TX2 Source", "VPMON", "VPMON ADC"},
	{"ASP TX2 Source", "VBSTMON", "VBSTMON ADC"},
<<<<<<< HEAD
	{"ASP TX2 Source", "DSPTX1", "ASPRX1"},
	{"ASP TX2 Source", "DSPTX2", "ASPRX1"},
=======
	{"ASP TX2 Source", "DSPTX1", "DSP1"},
	{"ASP TX2 Source", "DSPTX2", "DSP1"},
>>>>>>> upstream/android-13
	{"ASP TX2 Source", "ASPRX1", "ASPRX1" },
	{"ASP TX2 Source", "ASPRX2", "ASPRX2" },
	{"ASP TX3 Source", "VMON", "VMON ADC"},
	{"ASP TX3 Source", "IMON", "IMON ADC"},
	{"ASP TX3 Source", "VPMON", "VPMON ADC"},
	{"ASP TX3 Source", "VBSTMON", "VBSTMON ADC"},
<<<<<<< HEAD
	{"ASP TX3 Source", "DSPTX1", "ASPRX1"},
	{"ASP TX3 Source", "DSPTX2", "ASPRX1"},
=======
	{"ASP TX3 Source", "DSPTX1", "DSP1"},
	{"ASP TX3 Source", "DSPTX2", "DSP1"},
>>>>>>> upstream/android-13
	{"ASP TX3 Source", "ASPRX1", "ASPRX1" },
	{"ASP TX3 Source", "ASPRX2", "ASPRX2" },
	{"ASP TX4 Source", "VMON", "VMON ADC"},
	{"ASP TX4 Source", "IMON", "IMON ADC"},
	{"ASP TX4 Source", "VPMON", "VPMON ADC"},
	{"ASP TX4 Source", "VBSTMON", "VBSTMON ADC"},
<<<<<<< HEAD
	{"ASP TX4 Source", "DSPTX1", "ASPRX1"},
	{"ASP TX4 Source", "DSPTX2", "ASPRX1"},
=======
	{"ASP TX4 Source", "DSPTX1", "DSP1"},
	{"ASP TX4 Source", "DSPTX2", "DSP1"},
>>>>>>> upstream/android-13
	{"ASP TX4 Source", "ASPRX1", "ASPRX1" },
	{"ASP TX4 Source", "ASPRX2", "ASPRX2" },
	{"ASPTX1", NULL, "ASP TX1 Source"},
	{"ASPTX2", NULL, "ASP TX2 Source"},
	{"ASPTX3", NULL, "ASP TX3 Source"},
	{"ASPTX4", NULL, "ASP TX4 Source"},
	{"AMP Capture", NULL, "ASPTX1"},
	{"AMP Capture", NULL, "ASPTX2"},
	{"AMP Capture", NULL, "ASPTX3"},
	{"AMP Capture", NULL, "ASPTX4"},

<<<<<<< HEAD
	{"VMON ADC", NULL, "ASPRX1"},
	{"IMON ADC", NULL, "ASPRX1"},
	{"VPMON ADC", NULL, "ASPRX1"},
	{"TEMPMON ADC", NULL, "ASPRX1"},
	{"VBSTMON ADC", NULL, "ASPRX1"},

	{"DSP1", NULL, "IMON ADC"},
	{"DSP1", NULL, "VMON ADC"},
	{"DSP1", NULL, "VBSTMON ADC"},
	{"DSP1", NULL, "VPMON ADC"},
	{"DSP1", NULL, "TEMPMON ADC"},

	{"AMP Enable", "Switch", "AMP Playback"},
	{"ASPRX1", NULL, "AMP Enable"},
	{"ASPRX2", NULL, "AMP Enable"},
	{"DRE", "Switch", "CLASS H"},
	{"Main AMP", NULL, "CLASS H"},
	{"Main AMP", NULL, "DRE"},
	{"AMP SPK", NULL, "Main AMP"},
=======
	{"DSP1", NULL, "VMON"},
	{"DSP1", NULL, "IMON"},
	{"DSP1", NULL, "VPMON"},
	{"DSP1", NULL, "VBSTMON"},
	{"DSP1", NULL, "TEMPMON"},

	{"VMON ADC", NULL, "VMON"},
	{"IMON ADC", NULL, "IMON"},
	{"VPMON ADC", NULL, "VPMON"},
	{"VBSTMON ADC", NULL, "VBSTMON"},
	{"TEMPMON ADC", NULL, "TEMPMON"},

	{"VMON ADC", NULL, "VSENSE"},
	{"IMON ADC", NULL, "ISENSE"},
	{"VPMON ADC", NULL, "VP"},
	{"VBSTMON ADC", NULL, "VBST"},
	{"TEMPMON ADC", NULL, "TEMP"},

	{"DSP1 Preload", NULL, "DSP1 Preloader"},
	{"DSP1", NULL, "DSP1 Preloader"},

	{"ASPRX1", NULL, "AMP Playback"},
	{"ASPRX2", NULL, "AMP Playback"},
	{"DRE", "Switch", "CLASS H"},
	{"Main AMP", NULL, "CLASS H"},
	{"Main AMP", NULL, "DRE"},
	{"SPK", NULL, "Main AMP"},
>>>>>>> upstream/android-13

	{"PCM Source", "ASP", "ASPRX1"},
	{"PCM Source", "DSP", "DSP1"},
	{"CLASS H", NULL, "PCM Source"},
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
};

static const struct wm_adsp_region cs35l41_dsp1_regions[] = {
	{ .type = WMFW_HALO_PM_PACKED,	.base = CS35L41_DSP1_PMEM_0 },
	{ .type = WMFW_HALO_XM_PACKED,	.base = CS35L41_DSP1_XMEM_PACK_0 },
	{ .type = WMFW_HALO_YM_PACKED,	.base = CS35L41_DSP1_YMEM_PACK_0 },
	{. type = WMFW_ADSP2_XM,	.base = CS35L41_DSP1_XMEM_UNPACK24_0},
	{. type = WMFW_ADSP2_YM,	.base = CS35L41_DSP1_YMEM_UNPACK24_0},
};

<<<<<<< HEAD
static int cs35l41_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	struct cs35l41_private *cs35l41 =
			snd_soc_component_get_drvdata(codec_dai->component);
	unsigned int asp_fmt, lrclk_fmt, sclk_fmt, clock_mode;

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		clock_mode = 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		clock_mode = 0;
		break;
	default:
		dev_warn(cs35l41->dev, "%s: Unsupported clock mode\n", __func__);
		return -EINVAL;
	}

	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_SCLK_MSTR_MASK,
				clock_mode << CS35L41_SCLK_MSTR_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_LRCLK_MSTR_MASK,
				clock_mode << CS35L41_LRCLK_MSTR_SHIFT);

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_DSP_A:
		asp_fmt = 0;
		cs35l41->i2s_mode = false;
		cs35l41->dspa_mode = true;
		break;
	case SND_SOC_DAIFMT_I2S:
		asp_fmt = 2;
		cs35l41->i2s_mode = true;
		cs35l41->dspa_mode = false;
		break;
	default:
		dev_warn(cs35l41->dev, "%s: Invalid or unsupported DAI format\n", __func__);
		return -EINVAL;
	}

	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
					CS35L41_ASP_FMT_MASK,
					asp_fmt << CS35L41_ASP_FMT_SHIFT);

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_IF:
		lrclk_fmt = 1;
		sclk_fmt = 0;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		lrclk_fmt = 0;
		sclk_fmt = 1;
		break;
	case SND_SOC_DAIFMT_IB_IF:
		lrclk_fmt = 1;
		sclk_fmt = 1;
		break;
	case SND_SOC_DAIFMT_NB_NF:
		lrclk_fmt = 0;
		sclk_fmt = 0;
		break;
	default:
		dev_warn(cs35l41->dev, "%s: Invalid DAI clock INV\n", __func__);
		return -EINVAL;
	}

	cs35l41->lrclk_fmt = lrclk_fmt;
	cs35l41->sclk_fmt = sclk_fmt;

	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_LRCLK_INV_MASK,
				lrclk_fmt << CS35L41_LRCLK_INV_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_SCLK_INV_MASK,
				sclk_fmt << CS35L41_SCLK_INV_SHIFT);

	return 0;
=======
static int cs35l41_set_channel_map(struct snd_soc_dai *dai, unsigned int tx_num,
				   unsigned int *tx_slot, unsigned int rx_num,
				   unsigned int *rx_slot)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(dai->component);
	unsigned int val, mask;
	int i;

	if (tx_num > 4 || rx_num > 2)
		return -EINVAL;

	val = 0;
	mask = 0;
	for (i = 0; i < rx_num; i++) {
		dev_dbg(cs35l41->dev, "rx slot %d position = %d\n", i, rx_slot[i]);
		val |= rx_slot[i] << (i * 8);
		mask |= 0x3F << (i * 8);
	}
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FRAME_RX_SLOT, mask, val);

	val = 0;
	mask = 0;
	for (i = 0; i < tx_num; i++) {
		dev_dbg(cs35l41->dev, "tx slot %d position = %d\n", i, tx_slot[i]);
		val |= tx_slot[i] << (i * 8);
		mask |= 0x3F << (i * 8);
	}
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FRAME_TX_SLOT, mask, val);

	return 0;
}

static int cs35l41_set_dai_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(dai->component);
	unsigned int daifmt = 0;

	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_CBP_CFP:
		daifmt |= CS35L41_SCLK_MSTR_MASK | CS35L41_LRCLK_MSTR_MASK;
		break;
	case SND_SOC_DAIFMT_CBC_CFC:
		break;
	default:
		dev_warn(cs35l41->dev, "Mixed provider/consumer mode unsupported\n");
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_DSP_A:
		break;
	case SND_SOC_DAIFMT_I2S:
		daifmt |= 2 << CS35L41_ASP_FMT_SHIFT;
		break;
	default:
		dev_warn(cs35l41->dev, "Invalid or unsupported DAI format\n");
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_IF:
		daifmt |= CS35L41_LRCLK_INV_MASK;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		daifmt |= CS35L41_SCLK_INV_MASK;
		break;
	case SND_SOC_DAIFMT_IB_IF:
		daifmt |= CS35L41_LRCLK_INV_MASK | CS35L41_SCLK_INV_MASK;
		break;
	case SND_SOC_DAIFMT_NB_NF:
		break;
	default:
		dev_warn(cs35l41->dev, "Invalid DAI clock INV\n");
		return -EINVAL;
	}

	return regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				  CS35L41_SCLK_MSTR_MASK | CS35L41_LRCLK_MSTR_MASK |
				  CS35L41_ASP_FMT_MASK | CS35L41_LRCLK_INV_MASK |
				  CS35L41_SCLK_INV_MASK, daifmt);
>>>>>>> upstream/android-13
}

struct cs35l41_global_fs_config {
	int rate;
	int fs_cfg;
};

static const struct cs35l41_global_fs_config cs35l41_fs_rates[] = {
	{ 12000,	0x01 },
	{ 24000,	0x02 },
	{ 48000,	0x03 },
	{ 96000,	0x04 },
	{ 192000,	0x05 },
	{ 11025,	0x09 },
	{ 22050,	0x0A },
	{ 44100,	0x0B },
	{ 88200,	0x0C },
	{ 176400,	0x0D },
	{ 8000,		0x11 },
	{ 16000,	0x12 },
	{ 32000,	0x13 },
};

<<<<<<< HEAD
static int cs35l41_pcm_mute(struct snd_soc_dai *dai, int mute)
{
	struct cs35l41_private *cs35l41 =
				  snd_soc_component_get_drvdata(dai->component);
	unsigned int vol, vol_ramp, dsprx2_src, status;
	int vol_ramp_ms;

	dev_info(cs35l41->dev, "%s mute=%d\n", __func__, mute);

	if (mute) {
		regmap_update_bits(cs35l41->regmap,
			CS35L41_AMP_DIG_VOL_CTRL,
			CS35L41_AMP_VOL_PCM_MASK <<
			CS35L41_AMP_VOL_PCM_SHIFT,
			CS35L41_AMP_VOL_PCM_MUTE <<
			CS35L41_AMP_VOL_PCM_SHIFT);
		cs35l41->pcm_active = 0;

		regmap_read(cs35l41->regmap,
			CS35L41_AMP_DIG_VOL_CTRL, &vol_ramp);
		vol_ramp &= CS35L41_AMP_VOL_RAMP_MASK;

		regmap_read(cs35l41->regmap,
			CS35L41_IRQ1_STATUS2, &status);

		if (status & CS35L41_AUX_NG_CH1_ENTRY_MASK &&
			status & CS35L41_AUX_NG_CH2_ENTRY_MASK &&
			cs35l41->halo_booted && cs35l41->halo_routed) {
			cs35l41_dbg(cs35l41->dev,
				"Aux NG Active, skipping mute sleeps\n");
		} else {

			vol_ramp_ms = cs35l41_convert_ramp_rate(cs35l41,
								vol_ramp);
			if (vol_ramp_ms < 0)
				dev_err(cs35l41->dev,
					"%s: Could not convert ramp rate\n",
					__func__);
			else if (vol_ramp_ms < 20)
				usleep_range(vol_ramp_ms * 1000,
						vol_ramp_ms * 1000 + 100);
			else
				msleep(vol_ramp_ms);
		}

		regmap_write(cs35l41->regmap,
				CS35L41_CSPL_COMMAND,
				CS35L41_CSPL_CMD_MUTE);

		regmap_update_bits(cs35l41->regmap,
				CS35L41_AMP_OUT_MUTE,
				CS35L41_AMP_MUTE_MASK <<
				CS35L41_AMP_MUTE_SHIFT,
				CS35L41_AMP_MUTE_MASK <<
				CS35L41_AMP_MUTE_SHIFT);

		regmap_read(cs35l41->regmap, CS35L41_DSP1_RX2_SRC, &dsprx2_src);
		if (dsprx2_src == CS35L41_INPUT_SRC_ASPRX1 ||
			dsprx2_src == CS35L41_INPUT_SRC_ASPRX2)
			cs35l41->dsprx2_src = dsprx2_src;
		regmap_write(cs35l41->regmap, CS35L41_DSP1_RX1_SRC, 0);
		regmap_write(cs35l41->regmap, CS35L41_DSP1_RX2_SRC, 0);

	} else {
		regmap_update_bits(cs35l41->regmap,
				CS35L41_AMP_OUT_MUTE,
				CS35L41_AMP_MUTE_MASK <<
				CS35L41_AMP_MUTE_SHIFT, 0);
		regmap_write(cs35l41->regmap, CS35L41_DSP1_RX1_SRC,
					CS35L41_INPUT_SRC_ASPRX1);
		if (cs35l41->dsprx2_src == CS35L41_INPUT_SRC_ASPRX1 ||
			cs35l41->dsprx2_src == CS35L41_INPUT_SRC_ASPRX2)
			regmap_write(cs35l41->regmap, CS35L41_DSP1_RX2_SRC,
					cs35l41->dsprx2_src);

		regmap_write(cs35l41->regmap,
				CS35L41_CSPL_COMMAND,
				CS35L41_CSPL_CMD_UNMUTE);

		dev_info(cs35l41->dev, "%s: %s\n", __func__,
				(cs35l41->amp_mute == 0) ? "Muted" : "Unmuted");

		if (cs35l41->amp_mute) {
			vol = cs35l41->pcm_vol;
			/* convert control val to register val */
			if (vol < CS35L41_AMP_VOL_CTRL_DEFAULT)
				vol += CS35L41_AMP_VOL_PCM_MUTE;
			else
				vol -= CS35L41_AMP_VOL_CTRL_DEFAULT;
			/* unmute */
			regmap_update_bits(cs35l41->regmap,
					CS35L41_AMP_DIG_VOL_CTRL,
					CS35L41_AMP_VOL_PCM_MASK <<
					CS35L41_AMP_VOL_PCM_SHIFT,
					vol << CS35L41_AMP_VOL_PCM_SHIFT);
		}
		cs35l41->pcm_active = 1;
	}

	cs35l41_log_status(cs35l41, mute);
	cs35l41_dbg(cs35l41->dev, "%s exit\n", __func__);

	return 0;
}

=======
>>>>>>> upstream/android-13
static int cs35l41_pcm_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct cs35l41_private *cs35l41 =
			snd_soc_component_get_drvdata(dai->component);
	int i;
	unsigned int rate;
	u8 asp_width, asp_wl;

	if (cs35l41->pdata.fixed_params) {
		if (cs35l41->pdata.fixed_rate)
			rate = cs35l41->pdata.fixed_rate;
		else
			rate = params_rate(params);

		if (cs35l41->pdata.fixed_width)
			asp_width = cs35l41->pdata.fixed_width;
		else
			asp_width = params_physical_width(params);

		if (cs35l41->pdata.fixed_wl)
			asp_wl = cs35l41->pdata.fixed_wl;
		else
			asp_wl = params_width(params);

	} else {
		rate = params_rate(params);
		asp_width = params_physical_width(params);
		asp_wl = params_width(params);
	}

	dev_info(cs35l41->dev, "%s\trate:%d, width:%d, wl:%d\n",
			__func__, rate, asp_width, asp_wl);
=======
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(dai->component);
	unsigned int rate = params_rate(params);
	u8 asp_wl;
	int i;
>>>>>>> upstream/android-13

	for (i = 0; i < ARRAY_SIZE(cs35l41_fs_rates); i++) {
		if (rate == cs35l41_fs_rates[i].rate)
			break;
	}
<<<<<<< HEAD
	if (i < ARRAY_SIZE(cs35l41_fs_rates))
		regmap_update_bits(cs35l41->regmap, CS35L41_GLOBAL_CLK_CTRL,
			CS35L41_GLOBAL_FS_MASK,
			cs35l41_fs_rates[i].fs_cfg << CS35L41_GLOBAL_FS_SHIFT);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_ASP_WIDTH_RX_MASK,
				asp_width << CS35L41_ASP_WIDTH_RX_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_RX_WL,
				CS35L41_ASP_RX_WL_MASK,
				asp_wl << CS35L41_ASP_RX_WL_SHIFT);
		if (cs35l41->i2s_mode) {
			regmap_update_bits(cs35l41->regmap,
					CS35L41_SP_FRAME_RX_SLOT,
					CS35L41_ASP_RX1_SLOT_MASK,
					((cs35l41->pdata.right_channel) ? 1 : 0)
					 << CS35L41_ASP_RX1_SLOT_SHIFT);
			regmap_update_bits(cs35l41->regmap,
					CS35L41_SP_FRAME_RX_SLOT,
					CS35L41_ASP_RX2_SLOT_MASK,
					((cs35l41->pdata.right_channel) ? 0 : 1)
					 << CS35L41_ASP_RX2_SLOT_SHIFT);
		}
	} else {
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_ASP_WIDTH_TX_MASK,
				asp_width << CS35L41_ASP_WIDTH_TX_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_TX_WL,
				CS35L41_ASP_TX_WL_MASK,
				asp_wl << CS35L41_ASP_TX_WL_SHIFT);
=======

	if (i >= ARRAY_SIZE(cs35l41_fs_rates)) {
		dev_err(cs35l41->dev, "Unsupported rate: %u\n", rate);
		return -EINVAL;
	}

	asp_wl = params_width(params);

	if (i < ARRAY_SIZE(cs35l41_fs_rates))
		regmap_update_bits(cs35l41->regmap, CS35L41_GLOBAL_CLK_CTRL,
				   CS35L41_GLOBAL_FS_MASK,
				   cs35l41_fs_rates[i].fs_cfg << CS35L41_GLOBAL_FS_SHIFT);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				   CS35L41_ASP_WIDTH_RX_MASK,
				   asp_wl << CS35L41_ASP_WIDTH_RX_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_RX_WL,
				   CS35L41_ASP_RX_WL_MASK,
				   asp_wl << CS35L41_ASP_RX_WL_SHIFT);
	} else {
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				   CS35L41_ASP_WIDTH_TX_MASK,
				   asp_wl << CS35L41_ASP_WIDTH_TX_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_TX_WL,
				   CS35L41_ASP_TX_WL_MASK,
				   asp_wl << CS35L41_ASP_TX_WL_SHIFT);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int cs35l41_get_clk_config(int freq)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cs35l41_pll_sysclk); i++) {
		if (cs35l41_pll_sysclk[i].freq == freq)
			return cs35l41_pll_sysclk[i].clk_cfg;
	}

	return -EINVAL;
}

static const unsigned int cs35l41_src_rates[] = {
	8000, 12000, 11025, 16000, 22050, 24000, 32000,
	44100, 48000, 88200, 96000, 176400, 192000
};

static const struct snd_pcm_hw_constraint_list cs35l41_constraints = {
<<<<<<< HEAD
	.count  = ARRAY_SIZE(cs35l41_src_rates),
	.list   = cs35l41_src_rates,
=======
	.count = ARRAY_SIZE(cs35l41_src_rates),
	.list = cs35l41_src_rates,
>>>>>>> upstream/android-13
};

static int cs35l41_pcm_startup(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai)
{
	if (substream->runtime)
		return snd_pcm_hw_constraint_list(substream->runtime, 0,
<<<<<<< HEAD
				SNDRV_PCM_HW_PARAM_RATE, &cs35l41_constraints);
=======
						  SNDRV_PCM_HW_PARAM_RATE,
						  &cs35l41_constraints);
>>>>>>> upstream/android-13
	return 0;
}

static int cs35l41_component_set_sysclk(struct snd_soc_component *component,
<<<<<<< HEAD
				int clk_id, int source, unsigned int freq,
				int dir)
{
	struct cs35l41_private *cs35l41 =
				snd_soc_component_get_drvdata(component);

	cs35l41_dbg(cs35l41->dev, "%s\n", __func__);

	cs35l41->extclk_freq = freq;

	switch (clk_id) {
	case 0:
		cs35l41->clksrc = CS35L41_PLLSRC_SCLK;
		break;
	case 1:
		cs35l41->clksrc = CS35L41_PLLSRC_LRCLK;
		break;
	case 2:
		cs35l41->clksrc = CS35L41_PLLSRC_PDMCLK;
		break;
	case 3:
		cs35l41->clksrc = CS35L41_PLLSRC_SELF;
		break;
	case 4:
		cs35l41->clksrc = CS35L41_PLLSRC_MCLK;
=======
					int clk_id, int source,
					unsigned int freq, int dir)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	int extclk_cfg, clksrc;

	switch (clk_id) {
	case CS35L41_CLKID_SCLK:
		clksrc = CS35L41_PLLSRC_SCLK;
		break;
	case CS35L41_CLKID_LRCLK:
		clksrc = CS35L41_PLLSRC_LRCLK;
		break;
	case CS35L41_CLKID_MCLK:
		clksrc = CS35L41_PLLSRC_MCLK;
>>>>>>> upstream/android-13
		break;
	default:
		dev_err(cs35l41->dev, "Invalid CLK Config\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	cs35l41->extclk_cfg = cs35l41_get_clk_config(freq);

	if (cs35l41->extclk_cfg < 0) {
		dev_err(cs35l41->dev, "Invalid CLK Config: %d, freq: %u\n",
			cs35l41->extclk_cfg, freq);
		return -EINVAL;
	}

	if (freq != cs35l41->pll_freq_last) {
		cs35l41_dbg(cs35l41->dev, "PLL freq changed\n");

		if (cs35l41->clksrc == CS35L41_PLLSRC_SCLK)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_SP_RATE_CTRL,
						0x3F, cs35l41->extclk_cfg);

		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_PLL_OPENLOOP_MASK,
				1 << CS35L41_PLL_OPENLOOP_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_REFCLK_FREQ_MASK,
				cs35l41->extclk_cfg << CS35L41_REFCLK_FREQ_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_PLL_CLK_EN_MASK,
				0 << CS35L41_PLL_CLK_EN_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_PLL_CLK_SEL_MASK, cs35l41->clksrc);
		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_PLL_OPENLOOP_MASK,
				0 << CS35L41_PLL_OPENLOOP_SHIFT);
		regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
				CS35L41_PLL_CLK_EN_MASK,
				1 << CS35L41_PLL_CLK_EN_SHIFT);
	}

	cs35l41->pll_freq_last = freq;
=======
	extclk_cfg = cs35l41_get_clk_config(freq);

	if (extclk_cfg < 0) {
		dev_err(cs35l41->dev, "Invalid CLK Config: %d, freq: %u\n",
			extclk_cfg, freq);
		return -EINVAL;
	}

	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_PLL_OPENLOOP_MASK,
			   1 << CS35L41_PLL_OPENLOOP_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_REFCLK_FREQ_MASK,
			   extclk_cfg << CS35L41_REFCLK_FREQ_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_PLL_CLK_EN_MASK,
			   0 << CS35L41_PLL_CLK_EN_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_PLL_CLK_SEL_MASK, clksrc);
	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_PLL_OPENLOOP_MASK,
			   0 << CS35L41_PLL_OPENLOOP_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_PLL_CLK_CTRL,
			   CS35L41_PLL_CLK_EN_MASK,
			   1 << CS35L41_PLL_CLK_EN_SHIFT);
>>>>>>> upstream/android-13

	return 0;
}

static int cs35l41_dai_set_sysclk(struct snd_soc_dai *dai,
<<<<<<< HEAD
					int clk_id, unsigned int freq, int dir)
{
	struct cs35l41_private *cs35l41 =
				  snd_soc_component_get_drvdata(dai->component);

	if (cs35l41_get_clk_config(freq) < 0) {
=======
				  int clk_id, unsigned int freq, int dir)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(dai->component);
	unsigned int fs1_val;
	unsigned int fs2_val;
	unsigned int val;
	int fsindex;

	fsindex = cs35l41_get_fs_mon_config_index(freq);
	if (fsindex < 0) {
>>>>>>> upstream/android-13
		dev_err(cs35l41->dev, "Invalid CLK Config freq: %u\n", freq);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (clk_id == CS35L41_PLLSRC_SCLK)
		cs35l41->sclk = freq;
=======
	dev_dbg(cs35l41->dev, "Set DAI sysclk %d\n", freq);

	if (freq <= 6144000) {
		/* Use the lookup table */
		fs1_val = cs35l41_fs_mon[fsindex].fs1;
		fs2_val = cs35l41_fs_mon[fsindex].fs2;
	} else {
		/* Use hard-coded values */
		fs1_val = 0x10;
		fs2_val = 0x24;
	}

	val = fs1_val;
	val |= (fs2_val << CS35L41_FS2_WINDOW_SHIFT) & CS35L41_FS2_WINDOW_MASK;
	regmap_write(cs35l41->regmap, CS35L41_TST_FS_MON0, val);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static const struct reg_sequence cs35l41_fsync_errata_patch[] = {
	{0x00000040,			0x00005555},
	{0x00000040,			0x0000AAAA},
	{CS35L41_VIMON_SPKMON_RESYNC,	0x00000000},
	{0x00004310,			0x00000000},
	{CS35L41_VPVBST_FS_SEL,		0x00000000},
	{CS35L41_ASP_CONTROL4,		0x01010000},
	{0x00000040,			0x0000CCCC},
	{0x00000040,			0x00003333},
};

static int cs35l41_apply_pdata(struct snd_soc_component *component)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	struct classh_cfg *classh = &cs35l41->pdata.classh_config;

	/* Set Platform Data */
	if (cs35l41->pdata.sclk_frc)
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_SCLK_FRC_MASK,
				cs35l41->pdata.sclk_frc <<
				CS35L41_SCLK_FRC_SHIFT);

	if (cs35l41->pdata.lrclk_frc)
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_LRCLK_FRC_MASK,
				cs35l41->pdata.lrclk_frc <<
				CS35L41_LRCLK_FRC_SHIFT);

	if (cs35l41->pdata.amp_gain_zc)
		regmap_update_bits(cs35l41->regmap, CS35L41_AMP_GAIN_CTRL,
				CS35L41_AMP_GAIN_ZC_MASK,
				cs35l41->pdata.amp_gain_zc <<
				CS35L41_AMP_GAIN_ZC_SHIFT);

	if (cs35l41->pdata.bst_vctrl)
		regmap_update_bits(cs35l41->regmap, CS35L41_BSTCVRT_VCTRL1,
				CS35L41_BST_CTL_MASK, cs35l41->pdata.bst_vctrl);

	if (cs35l41->pdata.bst_ipk)
		regmap_update_bits(cs35l41->regmap, CS35L41_BSTCVRT_PEAK_CUR,
				CS35L41_BST_IPK_MASK, cs35l41->pdata.bst_ipk);

	if (cs35l41->pdata.temp_warn_thld)
		regmap_update_bits(cs35l41->regmap, CS35L41_DTEMP_WARN_THLD,
				CS35L41_TEMP_THLD_MASK,
				cs35l41->pdata.temp_warn_thld);

	if (cs35l41->pdata.dout_hiz <= CS35L41_ASP_DOUT_HIZ_MASK &&
	    cs35l41->pdata.dout_hiz >= 0)
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_HIZ_CTRL,
				CS35L41_ASP_DOUT_HIZ_MASK,
				cs35l41->pdata.dout_hiz);

	if (cs35l41->pdata.inv_pcm)
		regmap_update_bits(cs35l41->regmap, CS35L41_AMP_DIG_VOL_CTRL,
				CS35l41_INV_PCM_MASK, CS35l41_INV_PCM_MASK);

	if (cs35l41->pdata.use_fsync_errata)
		regmap_register_patch(cs35l41->regmap,
				cs35l41_fsync_errata_patch,
				ARRAY_SIZE(cs35l41_fsync_errata_patch));

	if (cs35l41->pdata.dsp_ng_enable) {
		regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH1_CFG,
				CS35L41_DSP_NG_ENABLE_MASK,
				CS35L41_DSP_NG_ENABLE_MASK);
		regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH2_CFG,
				CS35L41_DSP_NG_ENABLE_MASK,
				CS35L41_DSP_NG_ENABLE_MASK);

		if (cs35l41->pdata.dsp_ng_pcm_thld) {
			regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH1_CFG,
				CS35L41_DSP_NG_THLD_MASK,
				cs35l41->pdata.dsp_ng_pcm_thld);
			regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH2_CFG,
				CS35L41_DSP_NG_THLD_MASK,
				cs35l41->pdata.dsp_ng_pcm_thld);
		}

		if (cs35l41->pdata.dsp_ng_delay) {
			regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH1_CFG,
				CS35L41_DSP_NG_DELAY_MASK,
				cs35l41->pdata.dsp_ng_delay <<
				CS35L41_DSP_NG_DELAY_SHIFT);
			regmap_update_bits(cs35l41->regmap,
				CS35L41_MIXER_NGATE_CH2_CFG,
				CS35L41_DSP_NG_DELAY_MASK,
				cs35l41->pdata.dsp_ng_delay <<
				CS35L41_DSP_NG_DELAY_SHIFT);
		}
	}

	if (cs35l41->pdata.hw_ng_sel)
		regmap_update_bits(cs35l41->regmap,
				CS35L41_NG_CFG,
				CS35L41_HW_NG_SEL_MASK,
				cs35l41->pdata.hw_ng_sel <<
				CS35L41_HW_NG_SEL_SHIFT);

	if (cs35l41->pdata.hw_ng_thld)
		regmap_update_bits(cs35l41->regmap,
				CS35L41_NG_CFG,
				CS35L41_HW_NG_THLD_MASK,
				cs35l41->pdata.hw_ng_thld <<
				CS35L41_HW_NG_THLD_SHIFT);

	if (cs35l41->pdata.hw_ng_delay)
		regmap_update_bits(cs35l41->regmap,
				CS35L41_NG_CFG,
				CS35L41_HW_NG_DLY_MASK,
				cs35l41->pdata.hw_ng_delay <<
				CS35L41_HW_NG_DLY_SHIFT);

	if (classh->classh_algo_enable) {
		if (classh->classh_bst_override)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_BSTCVRT_VCTRL2,
					CS35L41_BST_CTL_SEL_MASK,
					CS35L41_BST_CTL_SEL_REG);
		if (classh->classh_bst_max_limit)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_BSTCVRT_VCTRL2,
					CS35L41_BST_LIM_MASK,
					classh->classh_bst_max_limit <<
					CS35L41_BST_LIM_SHIFT);
		if (classh->classh_mem_depth)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_CLASSH_CFG,
					CS35L41_CH_MEM_DEPTH_MASK,
					classh->classh_mem_depth <<
					CS35L41_CH_MEM_DEPTH_SHIFT);
		if (classh->classh_headroom)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_CLASSH_CFG,
					CS35L41_CH_HDRM_CTL_MASK,
					classh->classh_headroom <<
					CS35L41_CH_HDRM_CTL_SHIFT);
		if (classh->classh_release_rate)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_CLASSH_CFG,
					CS35L41_CH_REL_RATE_MASK,
					classh->classh_release_rate <<
					CS35L41_CH_REL_RATE_SHIFT);
		if (classh->classh_wk_fet_delay)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_WKFET_CFG,
					CS35L41_CH_WKFET_DLY_MASK,
					classh->classh_wk_fet_delay <<
					CS35L41_CH_WKFET_DLY_SHIFT);
		if (classh->classh_wk_fet_thld)
			regmap_update_bits(cs35l41->regmap,
					CS35L41_WKFET_CFG,
					CS35L41_CH_WKFET_THLD_MASK,
					classh->classh_wk_fet_thld <<
					CS35L41_CH_WKFET_THLD_SHIFT);
	}

	return 0;
}



static struct reg_sequence cs35l41_cal_pre_config[] = {
	{CS35L41_MIXER_NGATE_CH1_CFG,	0},
	{CS35L41_MIXER_NGATE_CH2_CFG,	0},
	{CS35L41_NG_CFG,		0},
	{CS35L41_PWR_CTRL3,		0x00000010},
	{CS35L41_BSTCVRT_VCTRL1,	CS35L41_VBST_CTL_11},
};

static struct reg_sequence cs35l41_cal_post_config[] = {
	{CS35L41_MIXER_NGATE_CH1_CFG,	0},
	{CS35L41_MIXER_NGATE_CH2_CFG,	0},
	{CS35L41_NG_CFG,		0},
	{CS35L41_PWR_CTRL3,		0x01000010},
	{CS35L41_BSTCVRT_VCTRL1,	0},
};

#define CS35L41_CAL_N_CONFIGS	5

static int cs35l41_cirrus_amp_probe(struct cs35l41_private *cs35l41,
				struct snd_soc_component *component)
{
	unsigned int property, target_temp = 0, exit_temp = 0;
	struct device_node *pwr_params;
	bool pwr_enable = false;
	const char *dsp_part_name;
	const char *mfd_suffix;
	int ret, bd_max_temp;
	struct cirrus_amp_config amp_cfg = {0};
	bool calibration_disable;
	unsigned int default_redc;

	ret = of_property_read_string(cs35l41->dev->of_node,
						"cirrus,dsp-part-name",
						&dsp_part_name);
	if (ret < 0)
		dsp_part_name = "cs35l41";

	ret = of_property_read_string(cs35l41->dev->of_node,
						"cirrus,mfd-suffix",
						&mfd_suffix);

	ret = of_property_read_u32(cs35l41->dev->of_node,
					"cirrus,bd-max-temp", &bd_max_temp);
	if (ret < 0)
		bd_max_temp = -1;

	ret = of_property_read_u32(cs35l41->dev->of_node,
					"cirrus,default-redc", &default_redc);
	if (ret < 0)
		default_redc = 0;

	calibration_disable = of_property_read_bool(cs35l41->dev->of_node,
					"cirrus,calibration-disable");

	regmap_read(cs35l41->regmap, CS35L41_MIXER_NGATE_CH1_CFG,
			&cs35l41_cal_post_config[0].def);
	regmap_read(cs35l41->regmap, CS35L41_MIXER_NGATE_CH2_CFG,
			&cs35l41_cal_post_config[1].def);
	regmap_read(cs35l41->regmap, CS35L41_NG_CFG,
			&cs35l41_cal_post_config[2].def);

	pwr_params = of_get_child_by_name(cs35l41->dev->of_node,
						"cirrus,pwr-params");
	if (pwr_params) {
		pwr_enable = of_property_read_bool(pwr_params,
						"cirrus,pwr-global-enable");
		ret = of_property_read_u32(pwr_params, "cirrus,pwr-target-temp",
					&property);
		if (ret >= 0)
			target_temp = property;

		ret = of_property_read_u32(pwr_params, "cirrus,pwr-exit-temp",
					&property);
		if (ret >= 0)
			exit_temp = property;
	}
	of_node_put(pwr_params);

	amp_cfg.component = component;
	amp_cfg.regmap = cs35l41->regmap;
	amp_cfg.pre_config = cs35l41_cal_pre_config;
	amp_cfg.post_config = cs35l41_cal_post_config;
	amp_cfg.dsp_part_name = dsp_part_name;
	amp_cfg.num_pre_configs = ARRAY_SIZE(cs35l41_cal_pre_config);
	amp_cfg.num_post_configs = ARRAY_SIZE(cs35l41_cal_post_config);
	amp_cfg.mbox_cmd = CS35L41_CSPL_MBOX_CMD_DRV;
	amp_cfg.mbox_sts = CS35L41_CSPL_MBOX_STS;
	amp_cfg.global_en = CS35L41_PWR_CTRL1;
	amp_cfg.global_en_mask = 1;
	amp_cfg.bd_max_temp = bd_max_temp;
	amp_cfg.target_temp = target_temp;
	amp_cfg.exit_temp = exit_temp;
	amp_cfg.pwr_enable = pwr_enable;
	amp_cfg.default_redc = default_redc;
	amp_cfg.perform_vimon_cal = true;
	amp_cfg.calibration_disable = calibration_disable;
	amp_cfg.vimon_alg_id = CS35L41_ALG_ID_HALO;
	amp_cfg.halo_alg_id = CS35L41_ALG_ID_HALO;
	amp_cfg.cal_vpk_id = CS35L41_CAL_RTLOG_ID_V_PEAK;
	amp_cfg.cal_ipk_id = CS35L41_CAL_RTLOG_ID_I_PEAK;
	amp_cfg.amp_reinit = cs35l41_reinit;

	ret = cirrus_amp_add(mfd_suffix, amp_cfg);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Failed to register cirrus amp (%d)\n",
			ret);
		return -EPROBE_DEFER;
	}

	return 0;
}


static int cs35l41_component_probe(struct snd_soc_component *component)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	cs35l41_apply_pdata(component);

	wm_adsp2_component_probe(&cs35l41->dsp, component);

	snd_soc_add_component_controls(component, &cs35l41->dsp.fw_ctrl, 1);

	cs35l41_cirrus_amp_probe(cs35l41, component);
=======
static int cs35l41_boost_config(struct cs35l41_private *cs35l41,
				int boost_ind, int boost_cap, int boost_ipk)
{
	unsigned char bst_lbst_val, bst_cbst_range, bst_ipk_scaled;
	struct regmap *regmap = cs35l41->regmap;
	struct device *dev = cs35l41->dev;
	int ret;

	switch (boost_ind) {
	case 1000:	/* 1.0 uH */
		bst_lbst_val = 0;
		break;
	case 1200:	/* 1.2 uH */
		bst_lbst_val = 1;
		break;
	case 1500:	/* 1.5 uH */
		bst_lbst_val = 2;
		break;
	case 2200:	/* 2.2 uH */
		bst_lbst_val = 3;
		break;
	default:
		dev_err(dev, "Invalid boost inductor value: %d nH\n", boost_ind);
		return -EINVAL;
	}

	switch (boost_cap) {
	case 0 ... 19:
		bst_cbst_range = 0;
		break;
	case 20 ... 50:
		bst_cbst_range = 1;
		break;
	case 51 ... 100:
		bst_cbst_range = 2;
		break;
	case 101 ... 200:
		bst_cbst_range = 3;
		break;
	default:	/* 201 uF and greater */
		bst_cbst_range = 4;
	}

	ret = regmap_update_bits(regmap, CS35L41_BSTCVRT_COEFF,
				 CS35L41_BST_K1_MASK | CS35L41_BST_K2_MASK,
				 cs35l41_bst_k1_table[bst_lbst_val][bst_cbst_range]
					<< CS35L41_BST_K1_SHIFT |
				 cs35l41_bst_k2_table[bst_lbst_val][bst_cbst_range]
					<< CS35L41_BST_K2_SHIFT);
	if (ret) {
		dev_err(dev, "Failed to write boost coefficients: %d\n", ret);
		return ret;
	}

	ret = regmap_update_bits(regmap, CS35L41_BSTCVRT_SLOPE_LBST,
				 CS35L41_BST_SLOPE_MASK | CS35L41_BST_LBST_VAL_MASK,
				 cs35l41_bst_slope_table[bst_lbst_val]
					<< CS35L41_BST_SLOPE_SHIFT |
				 bst_lbst_val << CS35L41_BST_LBST_VAL_SHIFT);
	if (ret) {
		dev_err(dev, "Failed to write boost slope/inductor value: %d\n", ret);
		return ret;
	}

	if (boost_ipk < 1600 || boost_ipk > 4500) {
		dev_err(dev, "Invalid boost inductor peak current: %d mA\n",
			boost_ipk);
		return -EINVAL;
	}
	bst_ipk_scaled = ((boost_ipk - 1600) / 50) + 0x10;

	ret = regmap_update_bits(regmap, CS35L41_BSTCVRT_PEAK_CUR,
				 CS35L41_BST_IPK_MASK,
				 bst_ipk_scaled << CS35L41_BST_IPK_SHIFT);
	if (ret) {
		dev_err(dev, "Failed to write boost inductor peak current: %d\n", ret);
		return ret;
	}

	return 0;
}

static int cs35l41_set_pdata(struct cs35l41_private *cs35l41)
{
	int ret;

	/* Set Platform Data */
	/* Required */
	if (cs35l41->pdata.bst_ipk &&
	    cs35l41->pdata.bst_ind && cs35l41->pdata.bst_cap) {
		ret = cs35l41_boost_config(cs35l41, cs35l41->pdata.bst_ind,
					   cs35l41->pdata.bst_cap,
					   cs35l41->pdata.bst_ipk);
		if (ret) {
			dev_err(cs35l41->dev, "Error in Boost DT config: %d\n", ret);
			return ret;
		}
	} else {
		dev_err(cs35l41->dev, "Incomplete Boost component DT config\n");
		return -EINVAL;
	}

	/* Optional */
	if (cs35l41->pdata.dout_hiz <= CS35L41_ASP_DOUT_HIZ_MASK &&
	    cs35l41->pdata.dout_hiz >= 0)
		regmap_update_bits(cs35l41->regmap, CS35L41_SP_HIZ_CTRL,
				   CS35L41_ASP_DOUT_HIZ_MASK,
				   cs35l41->pdata.dout_hiz);
>>>>>>> upstream/android-13

	return 0;
}

static int cs35l41_irq_gpio_config(struct cs35l41_private *cs35l41)
{
	struct cs35l41_irq_cfg *irq_gpio_cfg1 = &cs35l41->pdata.irq_config1;
	struct cs35l41_irq_cfg *irq_gpio_cfg2 = &cs35l41->pdata.irq_config2;
<<<<<<< HEAD
	int irq_pol = 0;

	if (irq_gpio_cfg1->is_present) {
		if (irq_gpio_cfg1->irq_pol_inv)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO1_CTRL1,
						CS35L41_GPIO_POL_MASK,
						CS35L41_GPIO_POL_MASK);
		if (irq_gpio_cfg1->irq_out_en)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO1_CTRL1,
						CS35L41_GPIO_DIR_MASK,
						0);
		if (irq_gpio_cfg1->irq_src_sel)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO_PAD_CONTROL,
						CS35L41_GPIO1_CTRL_MASK,
						irq_gpio_cfg1->irq_src_sel <<
						CS35L41_GPIO1_CTRL_SHIFT);
	}

	if (irq_gpio_cfg2->is_present) {
		if (irq_gpio_cfg2->irq_pol_inv)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO2_CTRL1,
						CS35L41_GPIO_POL_MASK,
						CS35L41_GPIO_POL_MASK);
		if (irq_gpio_cfg2->irq_out_en)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO2_CTRL1,
						CS35L41_GPIO_DIR_MASK,
						0);
		if (irq_gpio_cfg2->irq_src_sel)
			regmap_update_bits(cs35l41->regmap,
						CS35L41_GPIO_PAD_CONTROL,
						CS35L41_GPIO2_CTRL_MASK,
						irq_gpio_cfg2->irq_src_sel <<
						CS35L41_GPIO2_CTRL_SHIFT);

	}
=======
	int irq_pol = IRQF_TRIGGER_NONE;

	regmap_update_bits(cs35l41->regmap, CS35L41_GPIO1_CTRL1,
			   CS35L41_GPIO_POL_MASK | CS35L41_GPIO_DIR_MASK,
			   irq_gpio_cfg1->irq_pol_inv << CS35L41_GPIO_POL_SHIFT |
			   !irq_gpio_cfg1->irq_out_en << CS35L41_GPIO_DIR_SHIFT);

	regmap_update_bits(cs35l41->regmap, CS35L41_GPIO2_CTRL1,
			   CS35L41_GPIO_POL_MASK | CS35L41_GPIO_DIR_MASK,
			   irq_gpio_cfg1->irq_pol_inv << CS35L41_GPIO_POL_SHIFT |
			   !irq_gpio_cfg1->irq_out_en << CS35L41_GPIO_DIR_SHIFT);

	regmap_update_bits(cs35l41->regmap, CS35L41_GPIO_PAD_CONTROL,
			   CS35L41_GPIO1_CTRL_MASK | CS35L41_GPIO2_CTRL_MASK,
			   irq_gpio_cfg1->irq_src_sel << CS35L41_GPIO1_CTRL_SHIFT |
			   irq_gpio_cfg2->irq_src_sel << CS35L41_GPIO2_CTRL_SHIFT);
>>>>>>> upstream/android-13

	if ((irq_gpio_cfg2->irq_src_sel ==
			(CS35L41_GPIO_CTRL_ACTV_LO | CS35L41_VALID_PDATA)) ||
		(irq_gpio_cfg2->irq_src_sel ==
			(CS35L41_GPIO_CTRL_OPEN_INT | CS35L41_VALID_PDATA)))
		irq_pol = IRQF_TRIGGER_LOW;
	else if (irq_gpio_cfg2->irq_src_sel ==
			(CS35L41_GPIO_CTRL_ACTV_HI | CS35L41_VALID_PDATA))
		irq_pol = IRQF_TRIGGER_HIGH;

	return irq_pol;
}

<<<<<<< HEAD
=======
static int cs35l41_component_probe(struct snd_soc_component *component)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	return wm_adsp2_component_probe(&cs35l41->dsp, component);
}

>>>>>>> upstream/android-13
static void cs35l41_component_remove(struct snd_soc_component *component)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);

	wm_adsp2_component_remove(&cs35l41->dsp, component);
}

<<<<<<< HEAD
static int cs35l41_component_write(struct snd_soc_component *component,
				unsigned int reg, unsigned int val)
{
	struct cs35l41_private *cs35l41 =
				snd_soc_component_get_drvdata(component);
	int ret;
	cs35l41_dbg(cs35l41->dev, "%s: reg = 0x%x, value = 0x%x\n",
			__func__, reg, val);
	ret = regmap_write(cs35l41->regmap, reg, val);
	if (ret < 0) {
		cs35l41_log_regmap_fail(cs35l41, reg);
		return -EIO;
	}
	return 0;
}
static unsigned int cs35l41_component_read(struct snd_soc_component *component,
				unsigned int reg)
{
	struct cs35l41_private *cs35l41 =
				snd_soc_component_get_drvdata(component);
	unsigned int val;
	int ret;
	ret = regmap_read(cs35l41->regmap, reg, &val);
	if (ret < 0)
		cs35l41_log_regmap_fail(cs35l41, reg);
	cs35l41_dbg(cs35l41->dev, "%s: reg = 0x%x, value = 0x%x\n",
			__func__, reg, val);
	return val;
}

static const struct snd_soc_dai_ops cs35l41_ops = {
	.digital_mute = cs35l41_pcm_mute,
=======
static const struct snd_soc_dai_ops cs35l41_ops = {
>>>>>>> upstream/android-13
	.startup = cs35l41_pcm_startup,
	.set_fmt = cs35l41_set_dai_fmt,
	.hw_params = cs35l41_pcm_hw_params,
	.set_sysclk = cs35l41_dai_set_sysclk,
<<<<<<< HEAD
=======
	.set_channel_map = cs35l41_set_channel_map,
>>>>>>> upstream/android-13
};

static struct snd_soc_dai_driver cs35l41_dai[] = {
	{
		.name = "cs35l41-pcm",
		.id = 0,
		.playback = {
			.stream_name = "AMP Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = SNDRV_PCM_RATE_KNOT,
			.formats = CS35L41_RX_FORMATS,
		},
		.capture = {
			.stream_name = "AMP Capture",
			.channels_min = 1,
			.channels_max = 8,
			.rates = SNDRV_PCM_RATE_KNOT,
			.formats = CS35L41_TX_FORMATS,
		},
		.ops = &cs35l41_ops,
<<<<<<< HEAD
	},
};

static struct snd_soc_component_driver soc_component_dev_cs35l41 = {
=======
		.symmetric_rate = 1,
	},
};

static const struct snd_soc_component_driver soc_component_dev_cs35l41 = {
	.name = "cs35l41-codec",
>>>>>>> upstream/android-13
	.probe = cs35l41_component_probe,
	.remove = cs35l41_component_remove,

	.dapm_widgets = cs35l41_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(cs35l41_dapm_widgets),
	.dapm_routes = cs35l41_audio_map,
	.num_dapm_routes = ARRAY_SIZE(cs35l41_audio_map),

	.controls = cs35l41_aud_controls,
	.num_controls = ARRAY_SIZE(cs35l41_aud_controls),
	.set_sysclk = cs35l41_component_set_sysclk,
<<<<<<< HEAD

	.write = cs35l41_component_write,
	.read = cs35l41_component_read,
};

static int cs35l41_handle_of_data(struct device *dev,
				struct cs35l41_platform_data *pdata)
{
	struct device_node *np = dev->of_node;
	unsigned int val;
	int ret;
	struct device_node *classh, *irq_gpio1, *irq_gpio2, *fixed_params;
	struct classh_cfg *classh_config = &pdata->classh_config;
	struct cs35l41_irq_cfg *irq_gpio1_config = &pdata->irq_config1;
	struct cs35l41_irq_cfg *irq_gpio2_config = &pdata->irq_config2;

	if (!np)
		return 0;

	pdata->right_channel = of_property_read_bool(np,
					"cirrus,right-channel-amp");
	pdata->sclk_frc = of_property_read_bool(np,
					"cirrus,sclk-force-output");
	pdata->lrclk_frc = of_property_read_bool(np,
					"cirrus,lrclk-force-output");
	pdata->amp_gain_zc = of_property_read_bool(np,
					"cirrus,amp-gain-zc");

	if (of_property_read_u32(np, "cirrus,temp-warn_threshold", &val) >= 0)
		pdata->temp_warn_thld = val | CS35L41_VALID_PDATA;

	if (of_property_read_u32(np, "cirrus,algo-frame-delay", &val) >= 0)
		pdata->algo_frame_delay = val;
	else
		pdata->algo_frame_delay = 0x4;

	ret = of_property_read_u32(np, "cirrus,boost-ctl-millivolt", &val);
	if (ret >= 0) {
		if (val < 2550 || val > 11000) {
			dev_err(dev,
				"Invalid Boost Voltage %u mV\n", val);
			return -EINVAL;
		}
		pdata->bst_vctrl = ((val - 2550) / 50) + 1;
	}

	ret = of_property_read_u32(np, "cirrus,boost-peak-milliamp", &val);
	if (ret >= 0) {
		if (val < 1600 || val > 4500) {
			dev_err(dev,
				"Invalid Boost Peak Current %u mA\n", val);
			return -EINVAL;
		}
		pdata->bst_ipk = ((val - 1600) / 50) + 0x10;
	}

	ret = of_property_read_u32(np, "cirrus,asp-sdout-hiz", &val);
=======
};

static int cs35l41_handle_pdata(struct device *dev,
				struct cs35l41_platform_data *pdata,
				struct cs35l41_private *cs35l41)
{
	struct cs35l41_irq_cfg *irq_gpio1_config = &pdata->irq_config1;
	struct cs35l41_irq_cfg *irq_gpio2_config = &pdata->irq_config2;
	unsigned int val;
	int ret;

	ret = device_property_read_u32(dev, "cirrus,boost-peak-milliamp", &val);
	if (ret >= 0)
		pdata->bst_ipk = val;

	ret = device_property_read_u32(dev, "cirrus,boost-ind-nanohenry", &val);
	if (ret >= 0)
		pdata->bst_ind = val;

	ret = device_property_read_u32(dev, "cirrus,boost-cap-microfarad", &val);
	if (ret >= 0)
		pdata->bst_cap = val;

	ret = device_property_read_u32(dev, "cirrus,asp-sdout-hiz", &val);
>>>>>>> upstream/android-13
	if (ret >= 0)
		pdata->dout_hiz = val;
	else
		pdata->dout_hiz = -1;

<<<<<<< HEAD
	pdata->dsp_ng_enable = of_property_read_bool(np,
					"cirrus,dsp-noise-gate-enable");
	if (of_property_read_u32(np, "cirrus,dsp-noise-gate-threshold", &val) >= 0)
		pdata->dsp_ng_pcm_thld = val | CS35L41_VALID_PDATA;
	if (of_property_read_u32(np, "cirrus,dsp-noise-gate-delay", &val) >= 0)
		pdata->dsp_ng_delay = val | CS35L41_VALID_PDATA;

	if (of_property_read_u32(np, "cirrus,hw-noise-gate-select", &val) >= 0)
		pdata->hw_ng_sel = val | CS35L41_VALID_PDATA;
	if (of_property_read_u32(np, "cirrus,hw-noise-gate-threshold", &val) >= 0)
		pdata->hw_ng_thld = val | CS35L41_VALID_PDATA;
	if (of_property_read_u32(np, "cirrus,hw-noise-gate-delay", &val) >= 0)
		pdata->hw_ng_delay = val | CS35L41_VALID_PDATA;

	classh = of_get_child_by_name(np, "cirrus,classh-internal-algo");
	classh_config->classh_algo_enable = classh ? true : false;

	pdata->inv_pcm = of_property_read_bool(np, "cirrus,invert-pcm");
	pdata->use_fsync_errata = of_property_read_bool(np,
					"cirrus,use-fsync-errata");

	if (classh_config->classh_algo_enable) {
		classh_config->classh_bst_override =
			of_property_read_bool(classh, "cirrus,classh-bst-override");

		ret = of_property_read_u32(classh,
					   "cirrus,classh-bst-max-limit",
					   &val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			classh_config->classh_bst_max_limit = val;
		}

		ret = of_property_read_u32(classh, "cirrus,classh-mem-depth",
					   &val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			classh_config->classh_mem_depth = val;
		}

		ret = of_property_read_u32(classh, "cirrus,classh-release-rate",
					   &val);
		if (ret >= 0)
			classh_config->classh_release_rate = val;

		ret = of_property_read_u32(classh, "cirrus,classh-headroom",
					   &val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			classh_config->classh_headroom = val;
		}

		ret = of_property_read_u32(classh, "cirrus,classh-wk-fet-delay",
					   &val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			classh_config->classh_wk_fet_delay = val;
		}

		ret = of_property_read_u32(classh, "cirrus,classh-wk-fet-thld",
					   &val);
		if (ret >= 0)
			classh_config->classh_wk_fet_thld = val;
	}
	of_node_put(classh);

	/* GPIO1 Pin Config */
	irq_gpio1 = of_get_child_by_name(np, "cirrus,gpio-config1");
	irq_gpio1_config->is_present = irq_gpio1 ? true : false;
	if (irq_gpio1_config->is_present) {
		irq_gpio1_config->irq_pol_inv = of_property_read_bool(irq_gpio1,
						"cirrus,gpio-polarity-invert");
		irq_gpio1_config->irq_out_en = of_property_read_bool(irq_gpio1,
						"cirrus,gpio-output-enable");
		ret = of_property_read_u32(irq_gpio1, "cirrus,gpio-src-select",
					&val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			irq_gpio1_config->irq_src_sel = val;
		}
	}
	of_node_put(irq_gpio1);

	/* GPIO2 Pin Config */
	irq_gpio2 = of_get_child_by_name(np, "cirrus,gpio-config2");
	irq_gpio2_config->is_present = irq_gpio2 ? true : false;
	if (irq_gpio2_config->is_present) {
		irq_gpio2_config->irq_pol_inv = of_property_read_bool(irq_gpio2,
						"cirrus,gpio-polarity-invert");
		irq_gpio2_config->irq_out_en = of_property_read_bool(irq_gpio2,
						"cirrus,gpio-output-enable");
		ret = of_property_read_u32(irq_gpio2, "cirrus,gpio-src-select",
					&val);
		if (ret >= 0) {
			val |= CS35L41_VALID_PDATA;
			irq_gpio2_config->irq_src_sel = val;
		}
	}
	of_node_put(irq_gpio2);

	fixed_params = of_get_child_by_name(np, "cirrus,fixed-hw-params");
	pdata->fixed_params = fixed_params ? true : false;
	if (fixed_params) {
		ret = of_property_read_u32(fixed_params,
					"cirrus,fixed-rate",
					   &val);
		if (ret >= 0)
			pdata->fixed_rate = val;
		else
			pdata->fixed_rate = 0;

		ret = of_property_read_u32(fixed_params,
					"cirrus,fixed-width",
					   &val);
		if (ret >= 0)
			pdata->fixed_width = val;
		else
			pdata->fixed_width = 0;

		ret = of_property_read_u32(fixed_params,
					"cirrus,fixed-wl",
					   &val);
		if (ret >= 0)
			pdata->fixed_wl = val;
		else
			pdata->fixed_wl = 0;
	}
	of_node_put(fixed_params);

	ret = of_property_read_string(np, "cirrus,dsp-part-name",
						&pdata->dsp_part_name);
	if (ret < 0)
		pdata->dsp_part_name = "cs35l41";

	ret = of_property_read_string(np, "cirrus,mfd-suffix",
						&pdata->mfd_suffix);
	if (ret < 0)
		pdata->mfd_suffix = "";
=======
	/* GPIO1 Pin Config */
	irq_gpio1_config->irq_pol_inv = device_property_read_bool(dev,
					"cirrus,gpio1-polarity-invert");
	irq_gpio1_config->irq_out_en = device_property_read_bool(dev,
					"cirrus,gpio1-output-enable");
	ret = device_property_read_u32(dev, "cirrus,gpio1-src-select",
				       &val);
	if (ret >= 0)
		irq_gpio1_config->irq_src_sel = val | CS35L41_VALID_PDATA;

	/* GPIO2 Pin Config */
	irq_gpio2_config->irq_pol_inv = device_property_read_bool(dev,
					"cirrus,gpio2-polarity-invert");
	irq_gpio2_config->irq_out_en = device_property_read_bool(dev,
					"cirrus,gpio2-output-enable");
	ret = device_property_read_u32(dev, "cirrus,gpio2-src-select",
				       &val);
	if (ret >= 0)
		irq_gpio2_config->irq_src_sel = val | CS35L41_VALID_PDATA;
>>>>>>> upstream/android-13

	return 0;
}

static const struct reg_sequence cs35l41_reva0_errata_patch[] = {
<<<<<<< HEAD
	{0x00000040,			0x00005555},
	{0x00000040,			0x0000AAAA},
	{0x00003854,			0x05180240},
	{CS35L41_OTP_TRIM_30,		0x9091A1C8},
	{0x00003014,			0x0200EE0E},
	{CS35L41_BSTCVRT_DCM_CTRL,	0x00000051},
	{0x00000054,			0x00000004},
	{CS35L41_IRQ1_DB3,		0x00000000},
	{CS35L41_IRQ2_DB3,		0x00000000},
	{0x00000040,			0x0000CCCC},
	{0x00000040,			0x00003333},
=======
	{ 0x00000040,			 0x00005555 },
	{ 0x00000040,			 0x0000AAAA },
	{ 0x00003854,			 0x05180240 },
	{ CS35L41_VIMON_SPKMON_RESYNC,	 0x00000000 },
	{ 0x00004310,			 0x00000000 },
	{ CS35L41_VPVBST_FS_SEL,	 0x00000000 },
	{ CS35L41_OTP_TRIM_30,		 0x9091A1C8 },
	{ 0x00003014,			 0x0200EE0E },
	{ CS35L41_BSTCVRT_DCM_CTRL,	 0x00000051 },
	{ 0x00000054,			 0x00000004 },
	{ CS35L41_IRQ1_DB3,		 0x00000000 },
	{ CS35L41_IRQ2_DB3,		 0x00000000 },
	{ CS35L41_DSP1_YM_ACCEL_PL0_PRI, 0x00000000 },
	{ CS35L41_DSP1_XM_ACCEL_PL0_PRI, 0x00000000 },
	{ 0x00000040,			 0x0000CCCC },
	{ 0x00000040,			 0x00003333 },
};

static const struct reg_sequence cs35l41_revb0_errata_patch[] = {
	{ 0x00000040,			 0x00005555 },
	{ 0x00000040,			 0x0000AAAA },
	{ CS35L41_VIMON_SPKMON_RESYNC,	 0x00000000 },
	{ 0x00004310,			 0x00000000 },
	{ CS35L41_VPVBST_FS_SEL,	 0x00000000 },
	{ CS35L41_BSTCVRT_DCM_CTRL,	 0x00000051 },
	{ CS35L41_DSP1_YM_ACCEL_PL0_PRI, 0x00000000 },
	{ CS35L41_DSP1_XM_ACCEL_PL0_PRI, 0x00000000 },
	{ 0x00000040,			 0x0000CCCC },
	{ 0x00000040,			 0x00003333 },
};

static const struct reg_sequence cs35l41_revb2_errata_patch[] = {
	{ 0x00000040,			 0x00005555 },
	{ 0x00000040,			 0x0000AAAA },
	{ CS35L41_VIMON_SPKMON_RESYNC,	 0x00000000 },
	{ 0x00004310,			 0x00000000 },
	{ CS35L41_VPVBST_FS_SEL,	 0x00000000 },
	{ CS35L41_BSTCVRT_DCM_CTRL,	 0x00000051 },
	{ CS35L41_DSP1_YM_ACCEL_PL0_PRI, 0x00000000 },
	{ CS35L41_DSP1_XM_ACCEL_PL0_PRI, 0x00000000 },
	{ 0x00000040,			 0x0000CCCC },
	{ 0x00000040,			 0x00003333 },
};

static const struct reg_sequence cs35l41_fs_errata_patch[] = {
	{ CS35L41_DSP1_RX1_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX2_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX3_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX4_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX5_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX6_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX7_RATE,	0x00000001 },
	{ CS35L41_DSP1_RX8_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX1_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX2_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX3_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX4_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX5_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX6_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX7_RATE,	0x00000001 },
	{ CS35L41_DSP1_TX8_RATE,	0x00000001 },
>>>>>>> upstream/android-13
};

static int cs35l41_dsp_init(struct cs35l41_private *cs35l41)
{
	struct wm_adsp *dsp;
<<<<<<< HEAD
	int ret, i;

	dsp = &cs35l41->dsp;
	dsp->part = cs35l41->pdata.dsp_part_name;
	dsp->num = 1;
	dsp->type = WMFW_HALO;
	dsp->rev = 0;
	dsp->dev = cs35l41->dev;
	dsp->regmap = cs35l41->regmap;

=======
	int ret;

	dsp = &cs35l41->dsp;
	dsp->part = "cs35l41";
	dsp->num = 1;
	dsp->type = WMFW_HALO;
	dsp->rev = 0;
	dsp->fw = 9; /* 9 is WM_ADSP_FW_SPK_PROT in wm_adsp.c */
	dsp->dev = cs35l41->dev;
	dsp->regmap = cs35l41->regmap;
>>>>>>> upstream/android-13
	dsp->base = CS35L41_DSP1_CTRL_BASE;
	dsp->base_sysinfo = CS35L41_DSP1_SYS_ID;
	dsp->mem = cs35l41_dsp1_regions;
	dsp->num_mems = ARRAY_SIZE(cs35l41_dsp1_regions);
	dsp->lock_regions = 0xFFFFFFFF;
<<<<<<< HEAD
	cs35l41->halo_booted = false;

	dsp->n_rx_channels = CS35L41_DSP_N_RX_RATES;
	dsp->n_tx_channels = CS35L41_DSP_N_TX_RATES;
	mutex_init(&cs35l41->rate_lock);
	ret = wm_halo_init(dsp, &cs35l41->rate_lock);

	if (cs35l41->pdata.use_fsync_errata) {
		for (i = 0; i < CS35L41_DSP_N_RX_RATES; i++)
			dsp->rx_rate_cache[i] = 0x1;
		for (i = 0; i < CS35L41_DSP_N_TX_RATES; i++)
			dsp->tx_rate_cache[i] = 0x1;
	}

	return ret;
}

static int cs35l41_init(struct cs35l41_private *cs35l41)
{
	u32 regid, reg_revid, mtl_revid, int_status, chipid_match;
	int ret;
	int timeout = 100;

	if (cs35l41->dev->of_node) {
		ret = cs35l41_handle_of_data(cs35l41->dev, &cs35l41->pdata);
		if (ret != 0)
			return ret;
	} else {
		ret = -EINVAL;
		goto err;
	}

	ret = regmap_read(cs35l41->regmap, CS35L41_DEVID, &regid);
	if (ret != 0) {
		dev_err(cs35l41->dev, "Get Device ID failed\n");
		goto err;
	}

	ret = regmap_read(cs35l41->regmap, CS35L41_REVID, &reg_revid);
	if (ret != 0) {
		dev_err(cs35l41->dev, "Get Revision ID failed\n");
		goto err;
	}

	mtl_revid = reg_revid & CS35L41_MTLREVID_MASK;

	/* CS35L41 will have even MTLREVID */
	/* CS35L41R will have odd MTLREVID */
	chipid_match = (mtl_revid % 2) ? CS35L41R_CHIP_ID : CS35L41_CHIP_ID;
	if (regid != chipid_match) {
		dev_err(cs35l41->dev, "CS35L41 Device ID (%X). Expected ID %X\n",
			regid, chipid_match);
		ret = -ENODEV;
		goto err;
	}

	cs35l41_irq_gpio_config(cs35l41);

	/* Set interrupt masks for critical errors */
	regmap_write(cs35l41->regmap, CS35L41_IRQ1_MASK1,
			CS35L41_INT1_MASK_DEFAULT);

	regmap_write(cs35l41->regmap, CS35L41_DSP1_RX5_SRC, CS35L41_INPUT_SRC_VPMON);
	regmap_write(cs35l41->regmap, CS35L41_DSP1_RX6_SRC, CS35L41_INPUT_SRC_VBSTMON);
	regmap_write(cs35l41->regmap, CS35L41_DSP1_RX7_SRC, CS35L41_INPUT_SRC_TEMPMON);
	regmap_write(cs35l41->regmap, CS35L41_DSP1_RX8_SRC, CS35L41_INPUT_SRC_RSVD);

	switch (reg_revid) {
	case CS35L41_REVID_A0:
		ret = regmap_register_patch(cs35l41->regmap,
				cs35l41_reva0_errata_patch,
				ARRAY_SIZE(cs35l41_reva0_errata_patch));
		if (ret < 0) {
			dev_err(cs35l41->dev, "Failed to apply A0 errata patch %d\n", ret);
			goto err;
		}
	}

	do {
		if (timeout == 0) {
			dev_err(cs35l41->dev,
				"Timeout waiting for OTP_BOOT_DONE\n");
			goto err;
		}
		usleep_range(1000, 1100);
		regmap_read(cs35l41->regmap, CS35L41_IRQ1_STATUS4, &int_status);
		timeout--;
	} while (!(int_status & CS35L41_OTP_BOOT_DONE));

	cs35l41_otp_unpack(cs35l41);

	regmap_read(cs35l41->regmap, CS35L41_OTP_TRIM_30, &cs35l41->spk_3_trim);
	regmap_read(cs35l41->regmap, CS35L41_OTP_TRIM_31, &cs35l41->spk_4_trim);

	cs35l41->pcm_vol = CS35L41_AMP_VOL_CTRL_DEFAULT;
	cs35l41->amp_mute = 1;

	dev_info(cs35l41->dev, "Cirrus Logic CS35L41 (%x), Revision: %02X\n",
		regid, reg_revid);

err:
	return ret;
}

int cs35l41_probe(struct cs35l41_private *cs35l41)
{
	int ret, i;
	int irq_pol = IRQF_TRIGGER_HIGH;

	for (i = 0; i < ARRAY_SIZE(cs35l41_supplies); i++)
		cs35l41->supplies[i].supply = cs35l41_supplies[i];

	cs35l41->num_supplies = ARRAY_SIZE(cs35l41_supplies);

	ret = devm_regulator_bulk_get(cs35l41->dev, cs35l41->num_supplies,
					cs35l41->supplies);
	if (ret != 0) {
		dev_err(cs35l41->dev,
			"Failed to request core supplies: %d\n",
			ret);
		return ret;
	}

	ret = regulator_bulk_enable(cs35l41->num_supplies, cs35l41->supplies);
	if (ret != 0) {
		dev_err(cs35l41->dev,
			"Failed to enable core supplies: %d\n", ret);
=======

	ret = regmap_multi_reg_write(cs35l41->regmap, cs35l41_fs_errata_patch,
				     ARRAY_SIZE(cs35l41_fs_errata_patch));
	if (ret < 0) {
		dev_err(cs35l41->dev, "Failed to write fs errata: %d\n", ret);
		return ret;
	}

	ret = wm_halo_init(dsp);
	if (ret) {
		dev_err(cs35l41->dev, "wm_halo_init failed: %d\n", ret);
		return ret;
	}

	ret = regmap_write(cs35l41->regmap, CS35L41_DSP1_RX5_SRC,
			   CS35L41_INPUT_SRC_VPMON);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write INPUT_SRC_VPMON failed: %d\n", ret);
		goto err_dsp;
	}
	ret = regmap_write(cs35l41->regmap, CS35L41_DSP1_RX6_SRC,
			   CS35L41_INPUT_SRC_CLASSH);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write INPUT_SRC_CLASSH failed: %d\n", ret);
		goto err_dsp;
	}
	ret = regmap_write(cs35l41->regmap, CS35L41_DSP1_RX7_SRC,
			   CS35L41_INPUT_SRC_TEMPMON);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write INPUT_SRC_TEMPMON failed: %d\n", ret);
		goto err_dsp;
	}
	ret = regmap_write(cs35l41->regmap, CS35L41_DSP1_RX8_SRC,
			   CS35L41_INPUT_SRC_RSVD);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write INPUT_SRC_RSVD failed: %d\n", ret);
		goto err_dsp;
	}

	return 0;

err_dsp:
	wm_adsp2_remove(dsp);

	return ret;
}

int cs35l41_probe(struct cs35l41_private *cs35l41,
		  struct cs35l41_platform_data *pdata)
{
	u32 regid, reg_revid, i, mtl_revid, int_status, chipid_match;
	int irq_pol = 0;
	int ret;

	if (pdata) {
		cs35l41->pdata = *pdata;
	} else {
		ret = cs35l41_handle_pdata(cs35l41->dev, &cs35l41->pdata, cs35l41);
		if (ret != 0)
			return ret;
	}

	for (i = 0; i < CS35L41_NUM_SUPPLIES; i++)
		cs35l41->supplies[i].supply = cs35l41_supplies[i];

	ret = devm_regulator_bulk_get(cs35l41->dev, CS35L41_NUM_SUPPLIES,
				      cs35l41->supplies);
	if (ret != 0) {
		dev_err(cs35l41->dev, "Failed to request core supplies: %d\n", ret);
		return ret;
	}

	ret = regulator_bulk_enable(CS35L41_NUM_SUPPLIES, cs35l41->supplies);
	if (ret != 0) {
		dev_err(cs35l41->dev, "Failed to enable core supplies: %d\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	/* returning NULL can be an option if in stereo mode */
	cs35l41->reset_gpio = devm_gpiod_get_optional(cs35l41->dev, "reset",
<<<<<<< HEAD
							GPIOD_OUT_LOW);
=======
						      GPIOD_OUT_LOW);
>>>>>>> upstream/android-13
	if (IS_ERR(cs35l41->reset_gpio)) {
		ret = PTR_ERR(cs35l41->reset_gpio);
		cs35l41->reset_gpio = NULL;
		if (ret == -EBUSY) {
			dev_info(cs35l41->dev,
				 "Reset line busy, assuming shared reset\n");
		} else {
			dev_err(cs35l41->dev,
				"Failed to get reset GPIO: %d\n", ret);
			goto err;
		}
	}
<<<<<<< HEAD

	if (cs35l41->reset_gpio) {
		usleep_range(1000, 1100);
=======
	if (cs35l41->reset_gpio) {
		/* satisfy minimum reset pulse width spec */
		usleep_range(2000, 2100);
>>>>>>> upstream/android-13
		gpiod_set_value_cansleep(cs35l41->reset_gpio, 1);
	}

	usleep_range(2000, 2100);

<<<<<<< HEAD
	mutex_init(&cs35l41->rate_lock);

	ret = cs35l41_init(cs35l41);
	if (ret < 0) {
		dev_err(cs35l41->dev, "%s: cs35l41_init failed\n", __func__);
		goto err;
	}

	cs35l41_dsp_init(cs35l41);

	irq_pol = cs35l41_irq_gpio_config(cs35l41);

	init_completion(&cs35l41->global_pdn_done);
	init_completion(&cs35l41->global_pup_done);

	ret = devm_request_threaded_irq(cs35l41->dev, cs35l41->irq, NULL,
				cs35l41_irq, IRQF_ONESHOT | IRQF_SHARED |
				irq_pol, "cs35l41", cs35l41);
=======
	ret = regmap_read_poll_timeout(cs35l41->regmap, CS35L41_IRQ1_STATUS4,
				       int_status, int_status & CS35L41_OTP_BOOT_DONE,
				       1000, 100000);
	if (ret) {
		dev_err(cs35l41->dev,
			"Failed waiting for OTP_BOOT_DONE: %d\n", ret);
		goto err;
	}

	regmap_read(cs35l41->regmap, CS35L41_IRQ1_STATUS3, &int_status);
	if (int_status & CS35L41_OTP_BOOT_ERR) {
		dev_err(cs35l41->dev, "OTP Boot error\n");
		ret = -EINVAL;
		goto err;
	}

	ret = regmap_read(cs35l41->regmap, CS35L41_DEVID, &regid);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Get Device ID failed: %d\n", ret);
		goto err;
	}

	ret = regmap_read(cs35l41->regmap, CS35L41_REVID, &reg_revid);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Get Revision ID failed: %d\n", ret);
		goto err;
	}

	mtl_revid = reg_revid & CS35L41_MTLREVID_MASK;

	/* CS35L41 will have even MTLREVID
	 * CS35L41R will have odd MTLREVID
	 */
	chipid_match = (mtl_revid % 2) ? CS35L41R_CHIP_ID : CS35L41_CHIP_ID;
	if (regid != chipid_match) {
		dev_err(cs35l41->dev, "CS35L41 Device ID (%X). Expected ID %X\n",
			regid, chipid_match);
		ret = -ENODEV;
		goto err;
	}

	switch (reg_revid) {
	case CS35L41_REVID_A0:
		ret = regmap_register_patch(cs35l41->regmap,
					    cs35l41_reva0_errata_patch,
					    ARRAY_SIZE(cs35l41_reva0_errata_patch));
		if (ret < 0) {
			dev_err(cs35l41->dev,
				"Failed to apply A0 errata patch: %d\n", ret);
			goto err;
		}
		break;
	case CS35L41_REVID_B0:
		ret = regmap_register_patch(cs35l41->regmap,
					    cs35l41_revb0_errata_patch,
					    ARRAY_SIZE(cs35l41_revb0_errata_patch));
		if (ret < 0) {
			dev_err(cs35l41->dev,
				"Failed to apply B0 errata patch: %d\n", ret);
			goto err;
		}
		break;
	case CS35L41_REVID_B2:
		ret = regmap_register_patch(cs35l41->regmap,
					    cs35l41_revb2_errata_patch,
					    ARRAY_SIZE(cs35l41_revb2_errata_patch));
		if (ret < 0) {
			dev_err(cs35l41->dev,
				"Failed to apply B2 errata patch: %d\n", ret);
			goto err;
		}
		break;
	}

	irq_pol = cs35l41_irq_gpio_config(cs35l41);

	/* Set interrupt masks for critical errors */
	regmap_write(cs35l41->regmap, CS35L41_IRQ1_MASK1,
		     CS35L41_INT1_MASK_DEFAULT);

	ret = devm_request_threaded_irq(cs35l41->dev, cs35l41->irq, NULL, cs35l41_irq,
					IRQF_ONESHOT | IRQF_SHARED | irq_pol,
					"cs35l41", cs35l41);
>>>>>>> upstream/android-13

	/* CS35L41 needs INT for PDN_DONE */
	if (ret != 0) {
		dev_err(cs35l41->dev, "Failed to request IRQ: %d\n", ret);
		goto err;
	}

<<<<<<< HEAD
	ret = snd_soc_register_component(cs35l41->dev, &soc_component_dev_cs35l41,
				     cs35l41_dai, ARRAY_SIZE(cs35l41_dai));
	if (ret < 0) {
		dev_err(cs35l41->dev, "%s: Register component failed\n", __func__);
		goto err;
	}

err:
	return ret;
}

int cs35l41_reinit(struct snd_soc_component *component)
{
	struct cs35l41_private *cs35l41 = snd_soc_component_get_drvdata(component);
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
	bool needs_preload = false;

	if (!cs35l41)
		return 0;

	if (cs35l41->reset_gpio) {
		gpiod_direction_output(cs35l41->reset_gpio, 1);

		gpiod_set_value_cansleep(cs35l41->reset_gpio, 0);
		usleep_range(1000, 1100);
		gpiod_set_value_cansleep(cs35l41->reset_gpio, 1);
	}

	usleep_range(2000, 2100);

	cs35l41->halo_booted = 0;

	if (cs35l41->dsp.preloaded) {
		snd_soc_component_disable_pin(component, "DSP1 Preload");
		snd_soc_dapm_sync(dapm);
		needs_preload = true;
	}

	mutex_lock(&cs35l41->dsp.pwr_lock);
	cs35l41->dsp.running = false;
	mutex_unlock(&cs35l41->dsp.pwr_lock);

	usleep_range(2000, 2100);

	/* Sync essential mixer-defined registers */
	regcache_mark_dirty(cs35l41->regmap);
	regcache_sync_region(cs35l41->regmap, CS35L41_SP_FRAME_RX_SLOT,
						CS35L41_SP_FRAME_RX_SLOT);

	regcache_drop_region(cs35l41->regmap, CS35L41_FIRSTREG,
					CS35L41_LASTREG);

	cs35l41_init(cs35l41);

	cs35l41_apply_pdata(component);

	/* restore essential ASP regs */
	regmap_write(cs35l41->regmap, CS35L41_DAC_PCM1_SRC,
		CS35L41_INPUT_DSP_TX1);
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_LRCLK_INV_MASK,
				cs35l41->lrclk_fmt << CS35L41_LRCLK_INV_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
				CS35L41_SCLK_INV_MASK,
				cs35l41->sclk_fmt << CS35L41_SCLK_INV_SHIFT);
	regmap_update_bits(cs35l41->regmap, CS35L41_SP_FORMAT,
					CS35L41_ASP_FMT_MASK,
					cs35l41->i2s_mode ? 2 : 0 <<
					CS35L41_ASP_FMT_SHIFT);

	regmap_update_bits(cs35l41->regmap,
			CS35L41_SP_FRAME_RX_SLOT,
			CS35L41_ASP_RX1_SLOT_MASK,
			((cs35l41->pdata.right_channel) ? 1 : 0)
			 << CS35L41_ASP_RX1_SLOT_SHIFT);
	regmap_update_bits(cs35l41->regmap,
			CS35L41_SP_FRAME_RX_SLOT,
			CS35L41_ASP_RX2_SLOT_MASK,
			((cs35l41->pdata.right_channel) ? 0 : 1)
			 << CS35L41_ASP_RX2_SLOT_SHIFT);
	regmap_write(cs35l41->regmap, CS35L41_DSP1_RX2_SRC,
					CS35L41_INPUT_SRC_ASPRX1);

	cs35l41->pll_freq_last = 0;
	cs35l41->pcm_source_last = CS35L41_DAC_PCM1_SRC;

	if (needs_preload) {
		snd_soc_component_force_enable_pin(component, "DSP1 Preload");
		snd_soc_dapm_sync(dapm);
	}

	/* wait for HALO load completion on workqueue */
	mutex_lock(&cs35l41->dsp.pwr_lock);
	mutex_unlock(&cs35l41->dsp.pwr_lock);

	dev_info(cs35l41->dev, "%s complete\n", __func__);
	return 0;
}
EXPORT_SYMBOL_GPL(cs35l41_reinit);

int cs35l41_remove(struct cs35l41_private *cs35l41)
{
	cs35l41_dbg(cs35l41->dev, "%s\n", __func__);

	regmap_write(cs35l41->regmap, CS35L41_IRQ1_MASK1, 0xFFFFFFFF);
	wm_adsp2_remove(&cs35l41->dsp);
	snd_soc_unregister_component(cs35l41->dev);
	regulator_bulk_disable(cs35l41->num_supplies, cs35l41->supplies);
	return 0;
=======
	ret = cs35l41_otp_unpack(cs35l41);
	if (ret < 0) {
		dev_err(cs35l41->dev, "OTP Unpack failed: %d\n", ret);
		goto err;
	}

	ret = regmap_write(cs35l41->regmap, CS35L41_DSP1_CCM_CORE_CTRL, 0);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write CCM_CORE_CTRL failed: %d\n", ret);
		goto err;
	}

	ret = regmap_update_bits(cs35l41->regmap, CS35L41_PWR_CTRL2,
				 CS35L41_AMP_EN_MASK, 0);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write CS35L41_PWR_CTRL2 failed: %d\n", ret);
		goto err;
	}

	ret = regmap_update_bits(cs35l41->regmap, CS35L41_AMP_GAIN_CTRL,
				 CS35L41_AMP_GAIN_PCM_MASK, 0);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Write CS35L41_AMP_GAIN_CTRL failed: %d\n", ret);
		goto err;
	}

	ret = cs35l41_set_pdata(cs35l41);
	if (ret < 0) {
		dev_err(cs35l41->dev, "Set pdata failed: %d\n", ret);
		goto err;
	}

	ret = cs35l41_dsp_init(cs35l41);
	if (ret < 0)
		goto err;

	ret = devm_snd_soc_register_component(cs35l41->dev,
					      &soc_component_dev_cs35l41,
					      cs35l41_dai, ARRAY_SIZE(cs35l41_dai));
	if (ret < 0) {
		dev_err(cs35l41->dev, "Register codec failed: %d\n", ret);
		goto err_dsp;
	}

	dev_info(cs35l41->dev, "Cirrus Logic CS35L41 (%x), Revision: %02X\n",
		 regid, reg_revid);

	return 0;

err_dsp:
	wm_adsp2_remove(&cs35l41->dsp);
err:
	regulator_bulk_disable(CS35L41_NUM_SUPPLIES, cs35l41->supplies);
	gpiod_set_value_cansleep(cs35l41->reset_gpio, 0);

	return ret;
}

void cs35l41_remove(struct cs35l41_private *cs35l41)
{
	regmap_write(cs35l41->regmap, CS35L41_IRQ1_MASK1, 0xFFFFFFFF);
	wm_adsp2_remove(&cs35l41->dsp);
	regulator_bulk_disable(CS35L41_NUM_SUPPLIES, cs35l41->supplies);
	gpiod_set_value_cansleep(cs35l41->reset_gpio, 0);
>>>>>>> upstream/android-13
}

MODULE_DESCRIPTION("ASoC CS35L41 driver");
MODULE_AUTHOR("David Rhodes, Cirrus Logic Inc, <david.rhodes@cirrus.com>");
MODULE_LICENSE("GPL");
