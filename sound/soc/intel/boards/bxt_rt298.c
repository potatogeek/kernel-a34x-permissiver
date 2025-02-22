<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Intel Broxton-P I2S Machine Driver
 *
 * Copyright (C) 2014-2016, Intel Corporation. All rights reserved.
 *
 * Modified from:
 *   Intel Skylake I2S Machine driver
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
#include "../../codecs/hdac_hdmi.h"
#include "../../codecs/rt298.h"
<<<<<<< HEAD
=======
#include "hda_dsp_common.h"
>>>>>>> upstream/android-13

/* Headset jack detection DAPM pins */
static struct snd_soc_jack broxton_headset;
static struct snd_soc_jack broxton_hdmi[3];

struct bxt_hdmi_pcm {
	struct list_head head;
	struct snd_soc_dai *codec_dai;
	int device;
};

struct bxt_rt286_private {
	struct list_head hdmi_pcm_list;
<<<<<<< HEAD
=======
	bool common_hdmi_codec_drv;
>>>>>>> upstream/android-13
};

enum {
	BXT_DPCM_AUDIO_PB = 0,
	BXT_DPCM_AUDIO_CP,
	BXT_DPCM_AUDIO_REF_CP,
	BXT_DPCM_AUDIO_DMIC_CP,
	BXT_DPCM_AUDIO_HDMI1_PB,
	BXT_DPCM_AUDIO_HDMI2_PB,
	BXT_DPCM_AUDIO_HDMI3_PB,
};

static struct snd_soc_jack_pin broxton_headset_pins[] = {
	{
		.pin = "Mic Jack",
		.mask = SND_JACK_MICROPHONE,
	},
	{
		.pin = "Headphone Jack",
		.mask = SND_JACK_HEADPHONE,
	},
};

static const struct snd_kcontrol_new broxton_controls[] = {
	SOC_DAPM_PIN_SWITCH("Speaker"),
	SOC_DAPM_PIN_SWITCH("Headphone Jack"),
	SOC_DAPM_PIN_SWITCH("Mic Jack"),
};

static const struct snd_soc_dapm_widget broxton_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_SPK("Speaker", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
	SND_SOC_DAPM_MIC("DMIC2", NULL),
	SND_SOC_DAPM_MIC("SoC DMIC", NULL),
	SND_SOC_DAPM_SPK("HDMI1", NULL),
	SND_SOC_DAPM_SPK("HDMI2", NULL),
	SND_SOC_DAPM_SPK("HDMI3", NULL),
};

static const struct snd_soc_dapm_route broxton_rt298_map[] = {
	/* speaker */
	{"Speaker", NULL, "SPOR"},
	{"Speaker", NULL, "SPOL"},

	/* HP jack connectors - unknown if we have jack detect */
	{"Headphone Jack", NULL, "HPO Pin"},

	/* other jacks */
	{"MIC1", NULL, "Mic Jack"},

	/* digital mics */
	{"DMIC1 Pin", NULL, "DMIC2"},
	{"DMic", NULL, "SoC DMIC"},

	{"HDMI1", NULL, "hif5-0 Output"},
	{"HDMI2", NULL, "hif6-0 Output"},
	{"HDMI2", NULL, "hif7-0 Output"},

	/* CODEC BE connections */
	{ "AIF1 Playback", NULL, "ssp5 Tx"},
	{ "ssp5 Tx", NULL, "codec0_out"},
	{ "ssp5 Tx", NULL, "codec1_out"},

	{ "codec0_in", NULL, "ssp5 Rx" },
	{ "ssp5 Rx", NULL, "AIF1 Capture" },

	{ "dmic01_hifi", NULL, "DMIC01 Rx" },
	{ "DMIC01 Rx", NULL, "Capture" },

	{ "hifi3", NULL, "iDisp3 Tx"},
	{ "iDisp3 Tx", NULL, "iDisp3_out"},
	{ "hifi2", NULL, "iDisp2 Tx"},
	{ "iDisp2 Tx", NULL, "iDisp2_out"},
	{ "hifi1", NULL, "iDisp1 Tx"},
	{ "iDisp1 Tx", NULL, "iDisp1_out"},
};

static const struct snd_soc_dapm_route geminilake_rt298_map[] = {
	/* speaker */
	{"Speaker", NULL, "SPOR"},
	{"Speaker", NULL, "SPOL"},

	/* HP jack connectors - unknown if we have jack detect */
	{"Headphone Jack", NULL, "HPO Pin"},

	/* other jacks */
	{"MIC1", NULL, "Mic Jack"},

	/* digital mics */
	{"DMIC1 Pin", NULL, "DMIC2"},
	{"DMic", NULL, "SoC DMIC"},

	{"HDMI1", NULL, "hif5-0 Output"},
	{"HDMI2", NULL, "hif6-0 Output"},
	{"HDMI2", NULL, "hif7-0 Output"},

	/* CODEC BE connections */
	{ "AIF1 Playback", NULL, "ssp2 Tx"},
	{ "ssp2 Tx", NULL, "codec0_out"},
	{ "ssp2 Tx", NULL, "codec1_out"},

	{ "codec0_in", NULL, "ssp2 Rx" },
	{ "ssp2 Rx", NULL, "AIF1 Capture" },

	{ "dmic01_hifi", NULL, "DMIC01 Rx" },
	{ "DMIC01 Rx", NULL, "Capture" },

	{ "dmic_voice", NULL, "DMIC16k Rx" },
	{ "DMIC16k Rx", NULL, "Capture" },

	{ "hifi3", NULL, "iDisp3 Tx"},
	{ "iDisp3 Tx", NULL, "iDisp3_out"},
	{ "hifi2", NULL, "iDisp2 Tx"},
	{ "iDisp2 Tx", NULL, "iDisp2_out"},
	{ "hifi1", NULL, "iDisp1 Tx"},
	{ "iDisp1 Tx", NULL, "iDisp1_out"},
};

static int broxton_rt298_fe_init(struct snd_soc_pcm_runtime *rtd)
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

static int broxton_rt298_codec_init(struct snd_soc_pcm_runtime *rtd)
{
<<<<<<< HEAD
	struct snd_soc_component *component = rtd->codec_dai->component;
=======
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13
	int ret = 0;

	ret = snd_soc_card_jack_new(rtd->card, "Headset",
		SND_JACK_HEADSET | SND_JACK_BTN_0,
		&broxton_headset,
		broxton_headset_pins, ARRAY_SIZE(broxton_headset_pins));

	if (ret)
		return ret;

	rt298_mic_detect(component, &broxton_headset);

	snd_soc_dapm_ignore_suspend(&rtd->card->dapm, "SoC DMIC");

	return 0;
}

static int broxton_hdmi_init(struct snd_soc_pcm_runtime *rtd)
{
	struct bxt_rt286_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_dai *dai = rtd->codec_dai;
=======
	struct snd_soc_dai *dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct bxt_hdmi_pcm *pcm;

	pcm = devm_kzalloc(rtd->card->dev, sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	pcm->device = BXT_DPCM_AUDIO_HDMI1_PB + dai->id;
	pcm->codec_dai = dai;

	list_add_tail(&pcm->head, &ctx->hdmi_pcm_list);

	return 0;
}

static int broxton_ssp5_fixup(struct snd_soc_pcm_runtime *rtd,
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

	/* set SSP5 to 24 bit */
	snd_mask_none(fmt);
	snd_mask_set_format(fmt, SNDRV_PCM_FORMAT_S24_LE);

	return 0;
}

static int broxton_rt298_hw_params(struct snd_pcm_substream *substream,
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

	ret = snd_soc_dai_set_sysclk(codec_dai, RT298_SCLK_S_PLL,
					19200000, SND_SOC_CLOCK_IN);
	if (ret < 0) {
		dev_err(rtd->dev, "can't set codec sysclk configuration\n");
		return ret;
	}

	return ret;
}

static const struct snd_soc_ops broxton_rt298_ops = {
	.hw_params = broxton_rt298_hw_params,
};

static const unsigned int rates[] = {
	48000,
};

static const struct snd_pcm_hw_constraint_list constraints_rates = {
	.count = ARRAY_SIZE(rates),
	.list  = rates,
	.mask = 0,
};

static int broxton_dmic_fixup(struct snd_soc_pcm_runtime *rtd,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_interval *channels = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);
	channels->min = channels->max = 4;
=======
	struct snd_interval *chan = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);
	chan->min = chan->max = 4;
>>>>>>> upstream/android-13

	return 0;
}

static const unsigned int channels_dmic[] = {
	1, 2, 3, 4,
};

static const struct snd_pcm_hw_constraint_list constraints_dmic_channels = {
	.count = ARRAY_SIZE(channels_dmic),
	.list = channels_dmic,
	.mask = 0,
};

static int broxton_dmic_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	runtime->hw.channels_max = 4;
	snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_CHANNELS,
					&constraints_dmic_channels);

	return snd_pcm_hw_constraint_list(substream->runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE, &constraints_rates);
}

static const struct snd_soc_ops broxton_dmic_ops = {
	.startup = broxton_dmic_startup,
};

static const unsigned int channels[] = {
	2,
};

static const struct snd_pcm_hw_constraint_list constraints_channels = {
	.count = ARRAY_SIZE(channels),
	.list = channels,
	.mask = 0,
};

static int bxt_fe_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	/*
	 * on this platform for PCM device we support:
	 *      48Khz
	 *      stereo
	 *	16-bit audio
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

static const struct snd_soc_ops broxton_rt286_fe_ops = {
	.startup = bxt_fe_startup,
};

<<<<<<< HEAD
=======
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

SND_SOC_DAILINK_DEF(ssp5_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("SSP5 Pin")));
SND_SOC_DAILINK_DEF(ssp5_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("i2c-INT343A:00",
				      "rt298-aif1")));

SND_SOC_DAILINK_DEF(dmic_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("DMIC01 Pin")));

SND_SOC_DAILINK_DEF(dmic_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("dmic-codec",
				      "dmic-hifi")));

SND_SOC_DAILINK_DEF(dmic16k,
	DAILINK_COMP_ARRAY(COMP_CPU("DMIC16k Pin")));

SND_SOC_DAILINK_DEF(idisp1_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp1 Pin")));
SND_SOC_DAILINK_DEF(idisp1_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2",
				      "intel-hdmi-hifi1")));

SND_SOC_DAILINK_DEF(idisp2_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp2 Pin")));
SND_SOC_DAILINK_DEF(idisp2_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2",
				      "intel-hdmi-hifi2")));

SND_SOC_DAILINK_DEF(idisp3_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp3 Pin")));
SND_SOC_DAILINK_DEF(idisp3_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2",
				      "intel-hdmi-hifi3")));

SND_SOC_DAILINK_DEF(platform,
	DAILINK_COMP_ARRAY(COMP_PLATFORM("0000:00:0e.0")));

>>>>>>> upstream/android-13
/* broxton digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link broxton_rt298_dais[] = {
	/* Front End DAI links */
	[BXT_DPCM_AUDIO_PB] =
	{
		.name = "Bxt Audio Port",
		.stream_name = "Audio",
<<<<<<< HEAD
		.cpu_dai_name = "System Pin",
		.platform_name = "0000:00:0e.0",
		.nonatomic = 1,
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
		.nonatomic = 1,
		.dynamic = 1,
>>>>>>> upstream/android-13
		.init = broxton_rt298_fe_init,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.ops = &broxton_rt286_fe_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_CP] =
	{
		.name = "Bxt Audio Capture Port",
		.stream_name = "Audio Record",
<<<<<<< HEAD
		.cpu_dai_name = "System Pin",
		.platform_name = "0000:00:0e.0",
		.nonatomic = 1,
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_capture = 1,
		.ops = &broxton_rt286_fe_ops,
=======
		.nonatomic = 1,
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_capture = 1,
		.ops = &broxton_rt286_fe_ops,
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_REF_CP] =
	{
		.name = "Bxt Audio Reference cap",
		.stream_name = "refcap",
<<<<<<< HEAD
		.cpu_dai_name = "Reference Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.init = NULL,
		.dpcm_capture = 1,
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(reference, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_DMIC_CP] =
	{
		.name = "Bxt Audio DMIC cap",
		.stream_name = "dmiccap",
<<<<<<< HEAD
		.cpu_dai_name = "DMIC Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.init = NULL,
		.dpcm_capture = 1,
		.nonatomic = 1,
		.dynamic = 1,
		.ops = &broxton_dmic_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_HDMI1_PB] =
	{
		.name = "Bxt HDMI Port1",
		.stream_name = "Hdmi1",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI1 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.dpcm_playback = 1,
		.init = NULL,
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi1, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_HDMI2_PB] =
	{
		.name = "Bxt HDMI Port2",
		.stream_name = "Hdmi2",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI2 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.dpcm_playback = 1,
		.init = NULL,
		.nonatomic = 1,
		.dynamic = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi2, dummy, platform),
>>>>>>> upstream/android-13
	},
	[BXT_DPCM_AUDIO_HDMI3_PB] =
	{
		.name = "Bxt HDMI Port3",
		.stream_name = "Hdmi3",
<<<<<<< HEAD
		.cpu_dai_name = "HDMI3 Pin",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
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
		/* SSP5 - Codec */
		.name = "SSP5-Codec",
		.id = 0,
<<<<<<< HEAD
		.cpu_dai_name = "SSP5 Pin",
		.platform_name = "0000:00:0e.0",
		.no_pcm = 1,
		.codec_name = "i2c-INT343A:00",
		.codec_dai_name = "rt298-aif1",
=======
		.no_pcm = 1,
>>>>>>> upstream/android-13
		.init = broxton_rt298_codec_init,
		.dai_fmt = SND_SOC_DAIFMT_DSP_A | SND_SOC_DAIFMT_NB_NF |
						SND_SOC_DAIFMT_CBS_CFS,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = broxton_ssp5_fixup,
		.ops = &broxton_rt298_ops,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(ssp5_pin, ssp5_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "dmic01",
		.id = 1,
<<<<<<< HEAD
		.cpu_dai_name = "DMIC01 Pin",
		.codec_name = "dmic-codec",
		.codec_dai_name = "dmic-hifi",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.be_hw_params_fixup = broxton_dmic_fixup,
		.ignore_suspend = 1,
		.dpcm_capture = 1,
		.no_pcm = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic_pin, dmic_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "dmic16k",
		.id = 2,
<<<<<<< HEAD
		.cpu_dai_name = "DMIC16k Pin",
		.codec_name = "dmic-codec",
		.codec_dai_name = "dmic-hifi",
		.platform_name = "0000:00:0e.0",
=======
>>>>>>> upstream/android-13
		.be_hw_params_fixup = broxton_dmic_fixup,
		.ignore_suspend = 1,
		.dpcm_capture = 1,
		.no_pcm = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic16k, dmic_codec, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "iDisp1",
		.id = 3,
<<<<<<< HEAD
		.cpu_dai_name = "iDisp1 Pin",
		.codec_name = "ehdaudio0D2",
		.codec_dai_name = "intel-hdmi-hifi1",
		.platform_name = "0000:00:0e.0",
		.init = broxton_hdmi_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = broxton_hdmi_init,
		.dpcm_playback = 1,
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
		.platform_name = "0000:00:0e.0",
		.init = broxton_hdmi_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = broxton_hdmi_init,
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
		.platform_name = "0000:00:0e.0",
		.init = broxton_hdmi_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = broxton_hdmi_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
		SND_SOC_DAILINK_REG(idisp3_pin, idisp3_codec, platform),
>>>>>>> upstream/android-13
	},
};

#define NAME_SIZE	32
static int bxt_card_late_probe(struct snd_soc_card *card)
{
	struct bxt_rt286_private *ctx = snd_soc_card_get_drvdata(card);
	struct bxt_hdmi_pcm *pcm;
	struct snd_soc_component *component = NULL;
	int err, i = 0;
	char jack_name[NAME_SIZE];

<<<<<<< HEAD
=======
	if (list_empty(&ctx->hdmi_pcm_list))
		return -EINVAL;

	if (ctx->common_hdmi_codec_drv) {
		pcm = list_first_entry(&ctx->hdmi_pcm_list, struct bxt_hdmi_pcm,
				       head);
		component = pcm->codec_dai->component;
		return hda_dsp_hdmi_build_controls(card, component);
	}

>>>>>>> upstream/android-13
	list_for_each_entry(pcm, &ctx->hdmi_pcm_list, head) {
		component = pcm->codec_dai->component;
		snprintf(jack_name, sizeof(jack_name),
			"HDMI/DP, pcm=%d Jack", pcm->device);
		err = snd_soc_card_jack_new(card, jack_name,
					SND_JACK_AVOUT, &broxton_hdmi[i],
					NULL, 0);

		if (err)
			return err;

		err = hdac_hdmi_jack_init(pcm->codec_dai, pcm->device,
						&broxton_hdmi[i]);
		if (err < 0)
			return err;

		i++;
	}

<<<<<<< HEAD
	if (!component)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	return hdac_hdmi_jack_port_init(component, &card->dapm);
}


/* broxton audio machine driver for SPT + RT298S */
static struct snd_soc_card broxton_rt298 = {
	.name = "broxton-rt298",
	.owner = THIS_MODULE,
	.dai_link = broxton_rt298_dais,
	.num_links = ARRAY_SIZE(broxton_rt298_dais),
	.controls = broxton_controls,
	.num_controls = ARRAY_SIZE(broxton_controls),
	.dapm_widgets = broxton_widgets,
	.num_dapm_widgets = ARRAY_SIZE(broxton_widgets),
	.dapm_routes = broxton_rt298_map,
	.num_dapm_routes = ARRAY_SIZE(broxton_rt298_map),
	.fully_routed = true,
	.late_probe = bxt_card_late_probe,

};

static struct snd_soc_card geminilake_rt298 = {
	.name = "geminilake-rt298",
	.owner = THIS_MODULE,
	.dai_link = broxton_rt298_dais,
	.num_links = ARRAY_SIZE(broxton_rt298_dais),
	.controls = broxton_controls,
	.num_controls = ARRAY_SIZE(broxton_controls),
	.dapm_widgets = broxton_widgets,
	.num_dapm_widgets = ARRAY_SIZE(broxton_widgets),
	.dapm_routes = geminilake_rt298_map,
	.num_dapm_routes = ARRAY_SIZE(geminilake_rt298_map),
	.fully_routed = true,
	.late_probe = bxt_card_late_probe,
};

static int broxton_audio_probe(struct platform_device *pdev)
{
	struct bxt_rt286_private *ctx;
	struct snd_soc_card *card =
			(struct snd_soc_card *)pdev->id_entry->driver_data;
<<<<<<< HEAD
	int i;

	for (i = 0; i < ARRAY_SIZE(broxton_rt298_dais); i++) {
		if (!strncmp(card->dai_link[i].codec_name, "i2c-INT343A:00",
						I2C_NAME_SIZE)) {
			if (!strncmp(card->name, "broxton-rt298",
						PLATFORM_NAME_SIZE)) {
				card->dai_link[i].name = "SSP5-Codec";
				card->dai_link[i].cpu_dai_name = "SSP5 Pin";
			} else if (!strncmp(card->name, "geminilake-rt298",
						PLATFORM_NAME_SIZE)) {
				card->dai_link[i].name = "SSP2-Codec";
				card->dai_link[i].cpu_dai_name = "SSP2 Pin";
=======
	struct snd_soc_acpi_mach *mach;
	const char *platform_name;
	int ret;
	int i;

	for (i = 0; i < ARRAY_SIZE(broxton_rt298_dais); i++) {
		if (!strncmp(card->dai_link[i].codecs->name, "i2c-INT343A:00",
			     I2C_NAME_SIZE)) {
			if (!strncmp(card->name, "broxton-rt298",
				     PLATFORM_NAME_SIZE)) {
				card->dai_link[i].name = "SSP5-Codec";
				card->dai_link[i].cpus->dai_name = "SSP5 Pin";
			} else if (!strncmp(card->name, "geminilake-rt298",
					    PLATFORM_NAME_SIZE)) {
				card->dai_link[i].name = "SSP2-Codec";
				card->dai_link[i].cpus->dai_name = "SSP2 Pin";
>>>>>>> upstream/android-13
			}
		}
	}

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	INIT_LIST_HEAD(&ctx->hdmi_pcm_list);

	card->dev = &pdev->dev;
	snd_soc_card_set_drvdata(card, ctx);

<<<<<<< HEAD
=======
	/* override plaform name, if required */
	mach = pdev->dev.platform_data;
	platform_name = mach->mach_params.platform;

	ret = snd_soc_fixup_dai_links_platform_name(card,
						    platform_name);
	if (ret)
		return ret;

	ctx->common_hdmi_codec_drv = mach->mach_params.common_hdmi_codec_drv;

>>>>>>> upstream/android-13
	return devm_snd_soc_register_card(&pdev->dev, card);
}

static const struct platform_device_id bxt_board_ids[] = {
	{ .name = "bxt_alc298s_i2s", .driver_data =
				(unsigned long)&broxton_rt298 },
	{ .name = "glk_alc298s_i2s", .driver_data =
				(unsigned long)&geminilake_rt298 },
	{}
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(platform, bxt_board_ids);
>>>>>>> upstream/android-13

static struct platform_driver broxton_audio = {
	.probe = broxton_audio_probe,
	.driver = {
		.name = "bxt_alc298s_i2s",
		.pm = &snd_soc_pm_ops,
	},
	.id_table = bxt_board_ids,
};
module_platform_driver(broxton_audio)

/* Module information */
MODULE_AUTHOR("Ramesh Babu <Ramesh.Babu@intel.com>");
MODULE_AUTHOR("Senthilnathan Veppur <senthilnathanx.veppur@intel.com>");
MODULE_DESCRIPTION("Intel SST Audio for Broxton");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS("platform:bxt_alc298s_i2s");
MODULE_ALIAS("platform:glk_alc298s_i2s");
=======
MODULE_IMPORT_NS(SND_SOC_INTEL_HDA_DSP_COMMON);
>>>>>>> upstream/android-13
