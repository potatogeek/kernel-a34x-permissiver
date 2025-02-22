<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Intel Haswell Lynxpoint SST Audio
 *
 * Copyright (C) 2013, Intel Corporation. All rights reserved.
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
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
<<<<<<< HEAD
#include <sound/pcm_params.h>

#include "../common/sst-dsp.h"
#include "../haswell/sst-haswell-ipc.h"

=======
#include <sound/soc-acpi.h>
#include <sound/pcm_params.h>

>>>>>>> upstream/android-13
#include "../../codecs/rt5640.h"

/* Haswell ULT platforms have a Headphone and Mic jack */
static const struct snd_soc_dapm_widget haswell_widgets[] = {
	SND_SOC_DAPM_HP("Headphones", NULL),
	SND_SOC_DAPM_MIC("Mic", NULL),
};

static const struct snd_soc_dapm_route haswell_rt5640_map[] = {

	{"Headphones", NULL, "HPOR"},
	{"Headphones", NULL, "HPOL"},
	{"IN2P", NULL, "Mic"},

	/* CODEC BE connections */
	{"SSP0 CODEC IN", NULL, "AIF1 Capture"},
	{"AIF1 Playback", NULL, "SSP0 CODEC OUT"},
};

static int haswell_ssp0_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_RATE);
	struct snd_interval *channels = hw_param_interval(params,
						SNDRV_PCM_HW_PARAM_CHANNELS);

	/* The ADSP will covert the FE rate to 48k, stereo */
	rate->min = rate->max = 48000;
	channels->min = channels->max = 2;

	/* set SSP0 to 16 bit */
	params_set_format(params, SNDRV_PCM_FORMAT_S16_LE);
	return 0;
}

static int haswell_rt5640_hw_params(struct snd_pcm_substream *substream,
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

	ret = snd_soc_dai_set_sysclk(codec_dai, RT5640_SCLK_S_MCLK, 12288000,
		SND_SOC_CLOCK_IN);

	if (ret < 0) {
		dev_err(rtd->dev, "can't set codec sysclk configuration\n");
		return ret;
	}

	/* set correct codec filter for DAI format and clock config */
	snd_soc_component_update_bits(codec_dai->component, 0x83, 0xffff, 0x8000);

	return ret;
}

static const struct snd_soc_ops haswell_rt5640_ops = {
	.hw_params = haswell_rt5640_hw_params,
};

<<<<<<< HEAD
static int haswell_rtd_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(rtd, DRV_NAME);
	struct sst_pdata *pdata = dev_get_platdata(component->dev);
	struct sst_hsw *haswell = pdata->dsp;
	int ret;

	/* Set ADSP SSP port settings */
	ret = sst_hsw_device_set_config(haswell, SST_HSW_DEVICE_SSP_0,
		SST_HSW_DEVICE_MCLK_FREQ_24_MHZ,
		SST_HSW_DEVICE_CLOCK_MASTER, 9);
	if (ret < 0) {
		dev_err(rtd->dev, "failed to set device config\n");
		return ret;
	}

	return 0;
}
=======
SND_SOC_DAILINK_DEF(dummy,
	DAILINK_COMP_ARRAY(COMP_DUMMY()));

SND_SOC_DAILINK_DEF(system,
	DAILINK_COMP_ARRAY(COMP_CPU("System Pin")));

SND_SOC_DAILINK_DEF(offload0,
	DAILINK_COMP_ARRAY(COMP_CPU("Offload0 Pin")));

SND_SOC_DAILINK_DEF(offload1,
	DAILINK_COMP_ARRAY(COMP_CPU("Offload1 Pin")));

SND_SOC_DAILINK_DEF(loopback,
	DAILINK_COMP_ARRAY(COMP_CPU("Loopback Pin")));

SND_SOC_DAILINK_DEF(codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("i2c-INT33CA:00", "rt5640-aif1")));

SND_SOC_DAILINK_DEF(platform,
	DAILINK_COMP_ARRAY(COMP_PLATFORM("haswell-pcm-audio")));

SND_SOC_DAILINK_DEF(ssp0_port,
	    DAILINK_COMP_ARRAY(COMP_CPU("ssp0-port")));
>>>>>>> upstream/android-13

static struct snd_soc_dai_link haswell_rt5640_dais[] = {
	/* Front End DAI links */
	{
		.name = "System",
		.stream_name = "System Playback/Capture",
<<<<<<< HEAD
		.cpu_dai_name = "System Pin",
		.platform_name = "haswell-pcm-audio",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.init = haswell_rtd_init,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.dpcm_capture = 1,
=======
		.nonatomic = 1,
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "Offload0",
		.stream_name = "Offload0 Playback",
<<<<<<< HEAD
		.cpu_dai_name = "Offload0 Pin",
		.platform_name = "haswell-pcm-audio",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
=======
		.nonatomic = 1,
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(offload0, dummy, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "Offload1",
		.stream_name = "Offload1 Playback",
<<<<<<< HEAD
		.cpu_dai_name = "Offload1 Pin",
		.platform_name = "haswell-pcm-audio",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
=======
		.nonatomic = 1,
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(offload1, dummy, platform),
>>>>>>> upstream/android-13
	},
	{
		.name = "Loopback",
		.stream_name = "Loopback",
<<<<<<< HEAD
		.cpu_dai_name = "Loopback Pin",
		.platform_name = "haswell-pcm-audio",
		.dynamic = 1,
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_capture = 1,
=======
		.nonatomic = 1,
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(loopback, dummy, platform),
>>>>>>> upstream/android-13
	},

	/* Back End DAI links */
	{
		/* SSP0 - Codec */
		.name = "Codec",
		.id = 0,
<<<<<<< HEAD
		.cpu_dai_name = "snd-soc-dummy-dai",
		.platform_name = "snd-soc-dummy",
		.no_pcm = 1,
		.codec_name = "i2c-INT33CA:00",
		.codec_dai_name = "rt5640-aif1",
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.ignore_suspend = 1,
=======
		.no_pcm = 1,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
>>>>>>> upstream/android-13
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = haswell_ssp0_fixup,
		.ops = &haswell_rt5640_ops,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(ssp0_port, codec, platform),
>>>>>>> upstream/android-13
	},
};

/* audio machine driver for Haswell Lynxpoint DSP + RT5640 */
static struct snd_soc_card haswell_rt5640 = {
	.name = "haswell-rt5640",
	.owner = THIS_MODULE,
	.dai_link = haswell_rt5640_dais,
	.num_links = ARRAY_SIZE(haswell_rt5640_dais),
	.dapm_widgets = haswell_widgets,
	.num_dapm_widgets = ARRAY_SIZE(haswell_widgets),
	.dapm_routes = haswell_rt5640_map,
	.num_dapm_routes = ARRAY_SIZE(haswell_rt5640_map),
	.fully_routed = true,
};

static int haswell_audio_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	haswell_rt5640.dev = &pdev->dev;

=======
	struct snd_soc_acpi_mach *mach;
	int ret;

	haswell_rt5640.dev = &pdev->dev;

	/* override plaform name, if required */
	mach = pdev->dev.platform_data;
	ret = snd_soc_fixup_dai_links_platform_name(&haswell_rt5640,
						    mach->mach_params.platform);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	return devm_snd_soc_register_card(&pdev->dev, &haswell_rt5640);
}

static struct platform_driver haswell_audio = {
	.probe = haswell_audio_probe,
	.driver = {
		.name = "haswell-audio",
		.pm = &snd_soc_pm_ops,
	},
};

module_platform_driver(haswell_audio)

/* Module information */
MODULE_AUTHOR("Liam Girdwood, Xingchao Wang");
MODULE_DESCRIPTION("Intel SST Audio for Haswell Lynxpoint");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:haswell-audio");
