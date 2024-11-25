<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2014 Marvell
 *
 * Thomas Petazzoni <thomas.petazzoni@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <sound/soc.h>
#include <linux/of.h>
#include <linux/platform_data/asoc-kirkwood.h>
#include "../codecs/cs42l51.h"

static int a370db_hw_params(struct snd_pcm_substream *substream,
			    struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	unsigned int freq;

	switch (params_rate(params)) {
	default:
	case 44100:
		freq = 11289600;
		break;
	case 48000:
		freq = 12288000;
		break;
	case 96000:
		freq = 24576000;
		break;
	}

	return snd_soc_dai_set_sysclk(codec_dai, 0, freq, SND_SOC_CLOCK_IN);
}

static const struct snd_soc_ops a370db_ops = {
	.hw_params = a370db_hw_params,
};

static const struct snd_soc_dapm_widget a370db_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Out Jack", NULL),
	SND_SOC_DAPM_LINE("In Jack", NULL),
};

static const struct snd_soc_dapm_route a370db_route[] = {
	{ "Out Jack",	NULL,	"HPL" },
	{ "Out Jack",	NULL,	"HPR" },
	{ "AIN1L",	NULL,	"In Jack" },
	{ "AIN1L",	NULL,	"In Jack" },
};

<<<<<<< HEAD
=======
SND_SOC_DAILINK_DEFS(analog,
	DAILINK_COMP_ARRAY(COMP_CPU("i2s")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "cs42l51-hifi")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(spdif_out,
	DAILINK_COMP_ARRAY(COMP_CPU("spdif")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "dit-hifi")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(spdif_in,
	DAILINK_COMP_ARRAY(COMP_CPU("spdif")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "dir-hifi")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

>>>>>>> upstream/android-13
static struct snd_soc_dai_link a370db_dai[] = {
{
	.name = "CS42L51",
	.stream_name = "analog",
<<<<<<< HEAD
	.cpu_dai_name = "i2s",
	.codec_dai_name = "cs42l51-hifi",
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
	.ops = &a370db_ops,
=======
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
	.ops = &a370db_ops,
	SND_SOC_DAILINK_REG(analog),
>>>>>>> upstream/android-13
},
{
	.name = "S/PDIF out",
	.stream_name = "spdif-out",
<<<<<<< HEAD
	.cpu_dai_name = "spdif",
	.codec_dai_name = "dit-hifi",
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
=======
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
	SND_SOC_DAILINK_REG(spdif_out),
>>>>>>> upstream/android-13
},
{
	.name = "S/PDIF in",
	.stream_name = "spdif-in",
<<<<<<< HEAD
	.cpu_dai_name = "spdif",
	.codec_dai_name = "dir-hifi",
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
=======
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_CBS_CFS,
	SND_SOC_DAILINK_REG(spdif_in),
>>>>>>> upstream/android-13
},
};

static struct snd_soc_card a370db = {
	.name = "a370db",
	.owner = THIS_MODULE,
	.dai_link = a370db_dai,
	.num_links = ARRAY_SIZE(a370db_dai),
	.dapm_widgets = a370db_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(a370db_dapm_widgets),
	.dapm_routes = a370db_route,
	.num_dapm_routes = ARRAY_SIZE(a370db_route),
};

static int a370db_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &a370db;

	card->dev = &pdev->dev;

<<<<<<< HEAD
	a370db_dai[0].cpu_of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-controller", 0);
	a370db_dai[0].platform_of_node = a370db_dai[0].cpu_of_node;

	a370db_dai[0].codec_of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-codec", 0);

	a370db_dai[1].cpu_of_node = a370db_dai[0].cpu_of_node;
	a370db_dai[1].platform_of_node = a370db_dai[0].cpu_of_node;

	a370db_dai[1].codec_of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-codec", 1);

	a370db_dai[2].cpu_of_node = a370db_dai[0].cpu_of_node;
	a370db_dai[2].platform_of_node = a370db_dai[0].cpu_of_node;

	a370db_dai[2].codec_of_node =
=======
	a370db_dai[0].cpus->of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-controller", 0);
	a370db_dai[0].platforms->of_node = a370db_dai[0].cpus->of_node;

	a370db_dai[0].codecs->of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-codec", 0);

	a370db_dai[1].cpus->of_node = a370db_dai[0].cpus->of_node;
	a370db_dai[1].platforms->of_node = a370db_dai[0].cpus->of_node;

	a370db_dai[1].codecs->of_node =
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-codec", 1);

	a370db_dai[2].cpus->of_node = a370db_dai[0].cpus->of_node;
	a370db_dai[2].platforms->of_node = a370db_dai[0].cpus->of_node;

	a370db_dai[2].codecs->of_node =
>>>>>>> upstream/android-13
		of_parse_phandle(pdev->dev.of_node,
				 "marvell,audio-codec", 2);

	return devm_snd_soc_register_card(card->dev, card);
}

<<<<<<< HEAD
static const struct of_device_id a370db_dt_ids[] = {
=======
static const struct of_device_id a370db_dt_ids[] __maybe_unused = {
>>>>>>> upstream/android-13
	{ .compatible = "marvell,a370db-audio" },
	{ },
};
MODULE_DEVICE_TABLE(of, a370db_dt_ids);

static struct platform_driver a370db_driver = {
	.driver		= {
		.name	= "a370db-audio",
		.of_match_table = of_match_ptr(a370db_dt_ids),
	},
	.probe		= a370db_probe,
};

module_platform_driver(a370db_driver);

MODULE_AUTHOR("Thomas Petazzoni <thomas.petazzoni@free-electrons.com>");
MODULE_DESCRIPTION("ALSA SoC a370db audio client");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:a370db-audio");
