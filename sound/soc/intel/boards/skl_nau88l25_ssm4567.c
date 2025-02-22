<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Intel Skylake I2S Machine Driver for NAU88L25+SSM4567
 *
 * Copyright (C) 2015, Intel Corporation. All rights reserved.
 *
 * Modified from:
 *   Intel Skylake I2S Machine Driver for NAU88L25 and SSM4567
 *
 *   Copyright (C) 2015, Intel Corporation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
<<<<<<< HEAD
=======
#include <sound/soc-acpi.h>
>>>>>>> upstream/android-13
#include <sound/jack.h>
#include <sound/pcm_params.h>
#include "../../codecs/nau8825.h"
#include "../../codecs/hdac_hdmi.h"
<<<<<<< HEAD
#include "../skylake/skl.h"
=======
>>>>>>> upstream/android-13

#define SKL_NUVOTON_CODEC_DAI	"nau8825-hifi"
#define SKL_SSM_CODEC_DAI	"ssm4567-hifi"
#define DMIC_CH(p)     p->list[p->count-1]

static struct snd_soc_jack skylake_headset;
static struct snd_soc_card skylake_audio_card;
static const struct snd_pcm_hw_constraint_list *dmic_constraints;
static struct snd_soc_jack skylake_hdmi[3];

struct skl_hdmi_pcm {
	struct list_head head;
	struct snd_soc_dai *codec_dai;
	int device;
};

struct skl_nau88125_private {
	struct list_head hdmi_pcm_list;
};
enum {
	SKL_DPCM_AUDIO_PB = 0,
	SKL_DPCM_AUDIO_CP,
	SKL_DPCM_AUDIO_REF_CP,
	SKL_DPCM_AUDIO_DMIC_CP,
	SKL_DPCM_AUDIO_HDMI1_PB,
	SKL_DPCM_AUDIO_HDMI2_PB,
	SKL_DPCM_AUDIO_HDMI3_PB,
};

static const struct snd_kcontrol_new skylake_controls[] = {
	SOC_DAPM_PIN_SWITCH("Headphone Jack"),
	SOC_DAPM_PIN_SWITCH("Headset Mic"),
	SOC_DAPM_PIN_SWITCH("Left Speaker"),
	SOC_DAPM_PIN_SWITCH("Right Speaker"),
};

static int platform_clock_control(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *k, int  event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;
	struct snd_soc_dai *codec_dai;
	int ret;

	codec_dai = snd_soc_card_get_codec_dai(card, SKL_NUVOTON_CODEC_DAI);
	if (!codec_dai) {
		dev_err(card->dev, "Codec dai not found\n");
		return -EIO;
	}

	if (SND_SOC_DAPM_EVENT_ON(event)) {
		ret = snd_soc_dai_set_sysclk(codec_dai,
				NAU8825_CLK_MCLK, 24000000, SND_SOC_CLOCK_IN);
		if (ret < 0) {
			dev_err(card->dev, "set sysclk err = %d\n", ret);
			return -EIO;
		}
	} else {
		ret = snd_soc_dai_set_sysclk(codec_dai,
				NAU8825_CLK_INTERNAL, 0, SND_SOC_CLOCK_IN);
		if (ret < 0) {
			dev_err(card->dev, "set sysclk err = %d\n", ret);
			return -EIO;
		}
	}
	return ret;
}

static const struct snd_soc_dapm_widget skylake_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_MIC("Headset Mic", NULL),
	SND_SOC_DAPM_SPK("Left Speaker", NULL),
	SND_SOC_DAPM_SPK("Right Speaker", NULL),
	SND_SOC_DAPM_MIC("SoC DMIC", NULL),
	SND_SOC_DAPM_SPK("DP1", NULL),
	SND_SOC_DAPM_SPK("DP2", NULL),
	SND_SOC_DAPM_SUPPLY("Platform Clock", SND_SOC_NOPM, 0, 0,
			platform_clock_control, SND_SOC_DAPM_PRE_PMU |
			SND_SOC_DAPM_POST_PMD),
};

static const struct snd_soc_dapm_route skylake_map[] = {
	/* HP jack connectors - unknown if we have jack detection */
	{"Headphone Jack", NULL, "HPOL"},
	{"Headphone Jack", NULL, "HPOR"},

	/* speaker */
	{"Left Speaker", NULL, "Left OUT"},
	{"Right Speaker", NULL, "Right OUT"},

	/* other jacks */
	{"MIC", NULL, "Headset Mic"},
	{"DMic", NULL, "SoC DMIC"},

	/* CODEC BE connections */
	{ "Left Playback", NULL, "ssp0 Tx"},
	{ "Right Playback", NULL, "ssp0 Tx"},
	{ "ssp0 Tx", NULL, "codec0_out"},

	/* IV feedback path */
	{ "codec0_lp_in", NULL, "ssp0 Rx"},
	{ "ssp0 Rx", NULL, "Left Capture Sense" },
	{ "ssp0 Rx", NULL, "Right Capture Sense" },

	{ "Playback", NULL, "ssp1 Tx"},
	{ "ssp1 Tx", NULL, "codec1_out"},

	{ "codec0_in", NULL, "ssp1 Rx" },
	{ "ssp1 Rx", NULL, "Capture" },

	/* DMIC */
	{ "dmic01_hifi", NULL, "DMIC01 Rx" },
	{ "DMIC01 Rx", NULL, "DMIC AIF" },

	{ "hifi3", NULL, "iDisp3 Tx"},
	{ "iDisp3 Tx", NULL, "iDisp3_out"},
	{ "hifi2", NULL, "iDisp2 Tx"},
	{ "iDisp2 Tx", NULL, "iDisp2_out"},
	{ "hifi1", NULL, "iDisp1 Tx"},
	{ "iDisp1 Tx", NULL, "iDisp1_out"},

	{ "Headphone Jack", NULL, "Platform Clock" },
	{ "Headset Mic", NULL, "Platform Clock" },
};

static struct snd_soc_codec_conf ssm4567_codec_conf[] = {
	{
<<<<<<< HEAD
		.dev_name = "i2c-INT343B:00",
		.name_prefix = "Left",
	},
	{
		.dev_name = "i2c-INT343B:01",
=======
		.dlc = COMP_CODEC_CONF("i2c-INT343B:00"),
		.name_prefix = "Left",
	},
	{
		.dlc = COMP_CODEC_CONF("i2c-INT343B:01"),
>>>>>>> upstream/android-13
		.name_prefix = "Right",
	},
};

<<<<<<< HEAD
static struct snd_soc_dai_link_component ssm4567_codec_components[] = {
	{ /* Left */
		.name = "i2c-INT343B:00",
		.dai_name = SKL_SSM_CODEC_DAI,
	},
	{ /* Right */
		.name = "i2c-INT343B:01",
		.dai_name = SKL_SSM_CODEC_DAI,
	},
};

=======
>>>>>>> upstream/android-13
static int skylake_ssm4567_codec_init(struct snd_soc_pcm_runtime *rtd)
{
	int ret;

	/* Slot 1 for left */
<<<<<<< HEAD
	ret = snd_soc_dai_set_tdm_slot(rtd->codec_dais[0], 0x01, 0x01, 2, 48);
=======
	ret = snd_soc_dai_set_tdm_slot(asoc_rtd_to_codec(rtd, 0), 0x01, 0x01, 2, 48);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	/* Slot 2 for right */
<<<<<<< HEAD
	ret = snd_soc_dai_set_tdm_slot(rtd->codec_dais[1], 0x02, 0x02, 2, 48);
=======
	ret = snd_soc_dai_set_tdm_slot(asoc_rtd_to_codec(rtd, 1), 0x02, 0x02, 2, 48);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	return ret;
}

static int skylake_nau8825_codec_init(struct snd_soc_pcm_runtime *rtd)
{
	int ret;
<<<<<<< HEAD
	struct snd_soc_component *component = rtd->codec_dai->component;
=======
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13

	/*
	 * 4 buttons here map to the google Reference headset
	 * The use of these buttons can be decided by the user space.
	 */
	ret = snd_soc_card_jack_new(&skylake_audio_card, "Headset Jack",
		SND_JACK_HEADSET | SND_JACK_BTN_0 | SND_JACK_BTN_1 |
		SND_JACK_BTN_2 | SND_JACK_BTN_3, &skylake_headset,
		NULL, 0);
	if (ret) {
		dev_err(rtd->dev, "Headset Jack creation failed %d\n", ret);
		return ret;
	}

	nau8825_enable_jack_detect(component, &skylake_headset);

	snd_soc_dapm_ignore_suspend(&rtd->card->dapm, "SoC DMIC");

	return ret;
}

static int skylake_hdmi1_init(struct snd_soc_pcm_runtime *rtd)
{
	struct skl_nau88125_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_dai *dai = rtd->codec_dai;
=======
	struct snd_soc_dai *dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct skl_hdmi_pcm *pcm;

	pcm = devm_kzalloc(rtd->card->dev, sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	pcm->device = SKL_DPCM_AUDIO_HDMI1_PB;
	pcm->codec_dai = dai;

	list_add_tail(&pcm->head, &ctx->hdmi_pcm_list);

	return 0;
}

static int skylake_hdmi2_init(struct snd_soc_pcm_runtime *rtd)
{
	struct skl_nau88125_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_dai *dai = rtd->codec_dai;
=======
	struct snd_soc_dai *dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct skl_hdmi_pcm *pcm;

	pcm = devm_kzalloc(rtd->card->dev, sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	pcm->device = SKL_DPCM_AUDIO_HDMI2_PB;
	pcm->codec_dai = dai;

	list_add_tail(&pcm->head, &ctx->hdmi_pcm_list);

	return 0;
}


static int skylake_hdmi3_init(struct snd_soc_pcm_runtime *rtd)
{
	struct skl_nau88125_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_dai *dai = rtd->codec_dai;
=======
	struct snd_soc_dai *dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct skl_hdmi_pcm *pcm;

	pcm = devm_kzalloc(rtd->card->dev, sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	pcm->device = SKL_DPCM_AUDIO_HDMI3_PB;
	pcm->codec_dai = dai;

	list_add_tail(&pcm->head, &ctx->hdmi_pcm_list);

	return 0;
}

static int skylake_nau8825_fe_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dapm_context *dapm;
<<<<<<< HEAD
	struct snd_soc_component *component = rtd->cpu_dai->component;
=======
	struct snd_soc_component *component = asoc_rtd_to_cpu(rtd, 0)->component;
>>>>>>> upstream/android-13

	dapm = snd_soc_component_get_dapm(component);
	snd_soc_dapm_ignore_suspend(dapm, "Reference Capture");

	return 0;
}

static const unsigned int rates[] = {
	48000,
};

static const struct snd_pcm_hw_constraint_list constraints_rates = {
	.count = ARRAY_SIZE(rates),
	.list  = rates,
	.mask = 0,
};

static const unsigned int channels[] = {
	2,
};

static const struct snd_pcm_hw_constraint_list constraints_channels = {
	.count = ARRAY_SIZE(channels),
	.list = channels,
	.mask = 0,
};

static int skl_fe_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	/*
	 * on this platform for PCM device we support,
	 *	48Khz
	 *	stereo
	 *	16 bit audio
	 */

	runtime->hw.channels_max = 2;
	snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_CHANNELS,
					   &constraints_channels);

	runtime->hw.formats = SNDRV_PCM_FMTBIT_S16_LE;
	snd_pcm_hw_constraint_msbits(runtime, 0, 16, 16);

	snd_pcm_hw_constraint_list(runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE, &constraints_rates);

	return 0;
}

static const struct snd_soc_ops skylake_nau8825_fe_ops = {
	.startup = skl_fe_startup,
};

static int skylake_ssp_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_RATE);
<<<<<<< HEAD
	struct snd_interval *channels = hw_param_interval(params,
=======
	struct snd_interval *chan = hw_param_interval(params,
>>>>>>> upstream/android-13
						SNDRV_PCM_HW_PARAM_CHANNELS);
	struct snd_mask *fmt = hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT);

	/* The ADSP will covert the FE rate to 48k, stereo */
	rate->min = rate->max = 48000;
<<<<<<< HEAD
	channels->min = channels->max = 2;
=======
	chan->min = chan->max = 2;
>>>>>>> upstream/android-13

	/* set SSP0 to 24 bit */
	snd_mask_none(fmt);
	snd_mask_set_format(fmt, SNDRV_PCM_FORMAT_S24_LE);
	return 0;
}

static int skylake_dmic_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_interval *channels = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);
	if (params_channels(params) == 2 || DMIC_CH(dmic_constraints) == 2)
		channels->min = channels->max = 2;
	else
		channels->min = channels->max = 4;
=======
	struct snd_interval *chan = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);
	if (params_channels(params) == 2 || DMIC_CH(dmic_constraints) == 2)
		chan->min = chan->max = 2;
	else
		chan->min = chan->max = 4;
>>>>>>> upstream/android-13

	return 0;
}

static int skylake_nau8825_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	int ret;

	ret = snd_soc_dai_set_sysclk(codec_dai,
			NAU8825_CLK_MCLK, 24000000, SND_SOC_CLOCK_IN);

	if (ret < 0)
		dev_err(rtd->dev, "snd_soc_dai_set_sysclk err = %d\n", ret);

	return ret;
}

static const struct snd_soc_ops skylake_nau8825_ops = {
	.hw_params = skylake_nau8825_hw_params,
};

static const unsigned int channels_dmic[] = {
	2, 4,
};

static const struct snd_pcm_hw_constraint_list constraints_dmic_channels = {
	.count = ARRAY_SIZE(channels_dmic),
	.list = channels_dmic,
	.mask = 0,
};

static const unsigned int dmic_2ch[] = {
	2,
};

static const struct snd_pcm_hw_constraint_list constraints_dmic_2ch = {
	.count = ARRAY_SIZE(dmic_2ch),
	.list = dmic_2ch,
	.mask = 0,
};

static int skylake_dmic_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	runtime->hw.channels_max = DMIC_CH(dmic_constraints);
	snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_CHANNELS,
			dmic_constraints);

	return snd_pcm_hw_constraint_list(substream->runtime, 0,
			SNDRV_PCM_HW_PARAM_RATE, &constraints_rates);
}

static const struct snd_soc_ops skylake_dmic_ops = {
	.startup = skylake_dmic_startup,
};

static const unsigned int rates_16000[] = {
	16000,
};

static const struct snd_pcm_hw_constraint_list constraints_16000 = {
	.count = ARRAY_SIZE(rates_16000),
	.list  = rates_16000,
};

static const unsigned int ch_mono[] = {
	1,
};

static const struct snd_pcm_hw_constraint_list constraints_refcap = {
	.count = ARRAY_SIZE(ch_mono),
	.list  = ch_mono,
};

static int skylake_refcap_startup(struct snd_pcm_substream *substream)
{
	substream->runtime->hw.channels_max = 1;
	snd_pcm_hw_constraint_list(substream->runtime, 0,
					SNDRV_PCM_HW_PARAM_CHANNELS,
					&constraints_refcap);

	return snd_pcm_hw_constraint_list(substream->runtime, 0,
			SNDRV_PCM_HW_PARAM_RATE,
			&constraints_16000);
}

<<<<<<< HEAD
static const struct snd_soc_ops skylaye_refcap_ops = {
	.startup = skylake_refcap_startup,
};

=======
static const struct snd_soc_ops skylake_refcap_ops = {
	.startup = skylake_refcap_startup,
};

SND_SOC_DAILINK_DEF(dummy,
	DAILINK_COMP_ARRAY(COMP_DUMMY()));

SND_SOC_DAILINK_DEF(system,
	DAILINK_COMP_ARRAY(COMP_CPU("System Pin")));

SND_SOC_DAILINK_DEF(reference,
	DAILINK_COMP_ARRAY(COMP_CPU("Reference Pin")));

SND_SOC_DAILINK_DEF(dmic,
	DAILINK_COMP_ARRAY(COMP_CPU("DMIC Pin")));

SND_SOC_DAILINK_DEF(hdmi1,
	DAILINK_COMP_ARRAY(COMP_CPU("HDMI1 Pin")));

SND_SOC_DAILINK_DEF(hdmi2,
	DAILINK_COMP_ARRAY(COMP_CPU("HDMI2 Pin")));

SND_SOC_DAILINK_DEF(hdmi3,
	DAILINK_COMP_ARRAY(COMP_CPU("HDMI3 Pin")));

SND_SOC_DAILINK_DEF(ssp0_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("SSP0 Pin")));
SND_SOC_DAILINK_DEF(ssp0_codec,
	DAILINK_COMP_ARRAY(
	/* Left */	COMP_CODEC("i2c-INT343B:00", SKL_SSM_CODEC_DAI),
	/* Right */	COMP_CODEC("i2c-INT343B:01", SKL_SSM_CODEC_DAI)));

SND_SOC_DAILINK_DEF(ssp1_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("SSP1 Pin")));
SND_SOC_DAILINK_DEF(ssp1_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("i2c-10508825:00", SKL_NUVOTON_CODEC_DAI)));

SND_SOC_DAILINK_DEF(dmic01_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("DMIC01 Pin")));
SND_SOC_DAILINK_DEF(dmic_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("dmic-codec", "dmic-hifi")));

SND_SOC_DAILINK_DEF(idisp1_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp1 Pin")));
SND_SOC_DAILINK_DEF(idisp1_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2", "intel-hdmi-hifi1")));

SND_SOC_DAILINK_DEF(idisp2_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp2 Pin")));
SND_SOC_DAILINK_DEF(idisp2_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2", "intel-hdmi-hifi2")));

SND_SOC_DAILINK_DEF(idisp3_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp3 Pin")));
SND_SOC_DAILINK_DEF(idisp3_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2", "intel-hdmi-hifi3")));

SND_SOC_DAILINK_DEF(platform,
	DAILINK_COMP_ARRAY(COMP_PLATFORM("0000:00:1f.3")));

>>>>>>> upstream/android-13
/* skylake digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link skylake_dais[] = {
	/* Front End DAI links */
	[SKL_DPCM_AUDIO_PB] = {
		.name = "Skl Audio Port",
		.stream_name = "Audio",
<<<<<<< HEAD
		.cpu_dai_name = "System Pin",
		.platform_name = "0000:00:1f.3",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
		.dynamic = 1,
>>>>>>> upstream/android-13
		.nonatomic = 1,
		.init = skylake_nau8825_fe_init,
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.ops = &skylake_nau8825_fe_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_CP] = {
		.name = "Skl Audio Capture Port",
		.stream_name = "Audio Record",
<<<<<<< HEAD
		.cpu_dai_name = "System Pin",
		.platform_name = "0000:00:1f.3",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
		.dynamic = 1,
>>>>>>> upstream/android-13
		.nonatomic = 1,
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_capture = 1,
		.ops = &skylake_nau8825_fe_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_REF_CP] = {
		.name = "Skl Audio Reference cap",
		.stream_name = "Wake on Voice",
<<<<<<< HEAD
		.cpu_dai_name = "Reference Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.init = NULL,
		.dpcm_capture = 1,
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
		.ops = &skylaye_refcap_ops,
=======
		.ops = &skylake_refcap_ops,
		SND_SOC_DAILINK_REG(reference, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_DMIC_CP] = {
		.name = "Skl Audio DMIC cap",
		.stream_name = "dmiccap",
<<<<<<< HEAD
		.cpu_dai_name = "DMIC Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.init = NULL,
		.dpcm_capture = 1,
		.nonatomic = 1,
		.dynamic = 1,
		.ops = &skylake_dmic_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_HDMI1_PB] = {
		.name = "Skl HDMI Port1",
		.stream_name = "Hdmi1",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI1 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.dpcm_playback = 1,
		.init = NULL,
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi1, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_HDMI2_PB] = {
		.name = "Skl HDMI Port2",
		.stream_name = "Hdmi2",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI2 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.dpcm_playback = 1,
		.init = NULL,
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi2, dummy, platform),
>>>>>>> upstream/android-13
	},
	[SKL_DPCM_AUDIO_HDMI3_PB] = {
		.name = "Skl HDMI Port3",
		.stream_name = "Hdmi3",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI3 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.init = NULL,
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi3, dummy, platform),
>>>>>>> upstream/android-13
	},

	/* Back End DAI links */
	{
		/* SSP0 - Codec */
		.name = "SSP0-Codec",
		.id = 0,
<<<<<<< HEAD
		.cpu_dai_name = "SSP0 Pin",
		.platform_name = "0000:00:1f.3",
		.no_pcm = 1,
		.codecs = ssm4567_codec_components,
		.num_codecs = ARRAY_SIZE(ssm4567_codec_components),
=======
		.no_pcm = 1,
>>>>>>> upstream/android-13
		.dai_fmt = SND_SOC_DAIFMT_DSP_A |
			SND_SOC_DAIFMT_IB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.init = skylake_ssm4567_codec_init,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = skylake_ssp_fixup,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(ssp0_pin, ssp0_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		/* SSP1 - Codec */
		.name = "SSP1-Codec",
		.id = 1,
<<<<<<< HEAD
		.cpu_dai_name = "SSP1 Pin",
		.platform_name = "0000:00:1f.3",
		.no_pcm = 1,
		.codec_name = "i2c-10508825:00",
		.codec_dai_name = SKL_NUVOTON_CODEC_DAI,
=======
		.no_pcm = 1,
>>>>>>> upstream/android-13
		.init = skylake_nau8825_codec_init,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = skylake_ssp_fixup,
		.ops = &skylake_nau8825_ops,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(ssp1_pin, ssp1_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "dmic01",
		.id = 2,
<<<<<<< HEAD
		.cpu_dai_name = "DMIC01 Pin",
		.codec_name = "dmic-codec",
		.codec_dai_name = "dmic-hifi",
		.platform_name = "0000:00:1f.3",
=======
>>>>>>> upstream/android-13
		.ignore_suspend = 1,
		.be_hw_params_fixup = skylake_dmic_fixup,
		.dpcm_capture = 1,
		.no_pcm = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic01_pin, dmic_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "iDisp1",
		.id = 3,
<<<<<<< HEAD
		.cpu_dai_name = "iDisp1 Pin",
		.codec_name = "ehdaudio0D2",
		.codec_dai_name = "intel-hdmi-hifi1",
		.platform_name = "0000:00:1f.3",
		.dpcm_playback = 1,
		.init = skylake_hdmi1_init,
		.no_pcm = 1,
=======
		.dpcm_playback = 1,
		.init = skylake_hdmi1_init,
		.no_pcm = 1,
		SND_SOC_DAILINK_REG(idisp1_pin, idisp1_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "iDisp2",
		.id = 4,
<<<<<<< HEAD
		.cpu_dai_name = "iDisp2 Pin",
		.codec_name = "ehdaudio0D2",
		.codec_dai_name = "intel-hdmi-hifi2",
		.platform_name = "0000:00:1f.3",
		.init = skylake_hdmi2_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = skylake_hdmi2_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
		SND_SOC_DAILINK_REG(idisp2_pin, idisp2_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "iDisp3",
		.id = 5,
<<<<<<< HEAD
		.cpu_dai_name = "iDisp3 Pin",
		.codec_name = "ehdaudio0D2",
		.codec_dai_name = "intel-hdmi-hifi3",
		.platform_name = "0000:00:1f.3",
		.init = skylake_hdmi3_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = skylake_hdmi3_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
		SND_SOC_DAILINK_REG(idisp3_pin, idisp3_codec, platform),
>>>>>>> upstream/android-13
	},
};

#define NAME_SIZE	32
static int skylake_card_late_probe(struct snd_soc_card *card)
{
	struct skl_nau88125_private *ctx = snd_soc_card_get_drvdata(card);
	struct skl_hdmi_pcm *pcm;
	struct snd_soc_component *component = NULL;
	int err, i = 0;
	char jack_name[NAME_SIZE];

	list_for_each_entry(pcm, &ctx->hdmi_pcm_list, head) {
		component = pcm->codec_dai->component;
		snprintf(jack_name, sizeof(jack_name),
			"HDMI/DP, pcm=%d Jack", pcm->device);
		err = snd_soc_card_jack_new(card, jack_name,
					SND_JACK_AVOUT,
					&skylake_hdmi[i],
					NULL, 0);

		if (err)
			return err;

		err = hdac_hdmi_jack_init(pcm->codec_dai, pcm->device,
						&skylake_hdmi[i]);
		if (err < 0)
			return err;

		i++;
	}

	if (!component)
		return -EINVAL;

	return hdac_hdmi_jack_port_init(component, &card->dapm);
}

/* skylake audio machine driver for SPT + NAU88L25 */
static struct snd_soc_card skylake_audio_card = {
	.name = "sklnau8825adi",
	.owner = THIS_MODULE,
	.dai_link = skylake_dais,
	.num_links = ARRAY_SIZE(skylake_dais),
	.controls = skylake_controls,
	.num_controls = ARRAY_SIZE(skylake_controls),
	.dapm_widgets = skylake_widgets,
	.num_dapm_widgets = ARRAY_SIZE(skylake_widgets),
	.dapm_routes = skylake_map,
	.num_dapm_routes = ARRAY_SIZE(skylake_map),
	.codec_conf = ssm4567_codec_conf,
	.num_configs = ARRAY_SIZE(ssm4567_codec_conf),
	.fully_routed = true,
<<<<<<< HEAD
=======
	.disable_route_checks = true,
>>>>>>> upstream/android-13
	.late_probe = skylake_card_late_probe,
};

static int skylake_audio_probe(struct platform_device *pdev)
{
	struct skl_nau88125_private *ctx;
<<<<<<< HEAD
	struct skl_machine_pdata *pdata;
=======
	struct snd_soc_acpi_mach *mach;
>>>>>>> upstream/android-13

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	INIT_LIST_HEAD(&ctx->hdmi_pcm_list);

	skylake_audio_card.dev = &pdev->dev;
	snd_soc_card_set_drvdata(&skylake_audio_card, ctx);

<<<<<<< HEAD
	pdata = dev_get_drvdata(&pdev->dev);
	if (pdata)
		dmic_constraints = pdata->dmic_num == 2 ?
=======
	mach = pdev->dev.platform_data;
	if (mach)
		dmic_constraints = mach->mach_params.dmic_num == 2 ?
>>>>>>> upstream/android-13
			&constraints_dmic_2ch : &constraints_dmic_channels;

	return devm_snd_soc_register_card(&pdev->dev, &skylake_audio_card);
}

static const struct platform_device_id skl_board_ids[] = {
	{ .name = "skl_n88l25_s4567" },
	{ .name = "kbl_n88l25_s4567" },
	{ }
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(platform, skl_board_ids);
>>>>>>> upstream/android-13

static struct platform_driver skylake_audio = {
	.probe = skylake_audio_probe,
	.driver = {
		.name = "skl_n88l25_s4567",
		.pm = &snd_soc_pm_ops,
	},
	.id_table = skl_board_ids,
};

module_platform_driver(skylake_audio)

/* Module information */
MODULE_AUTHOR("Conrad Cooke  <conrad.cooke@intel.com>");
MODULE_AUTHOR("Harsha Priya <harshapriya.n@intel.com>");
MODULE_AUTHOR("Naveen M <naveen.m@intel.com>");
MODULE_AUTHOR("Sathya Prakash M R <sathya.prakash.m.r@intel.com>");
MODULE_AUTHOR("Yong Zhi <yong.zhi@intel.com>");
MODULE_DESCRIPTION("Intel Audio Machine driver for SKL with NAU88L25 and SSM4567 in I2S Mode");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS("platform:skl_n88l25_s4567");
MODULE_ALIAS("platform:kbl_n88l25_s4567");
=======
>>>>>>> upstream/android-13
