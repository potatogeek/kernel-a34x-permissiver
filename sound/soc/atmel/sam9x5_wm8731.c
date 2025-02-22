<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * sam9x5_wm8731   --	SoC audio for AT91SAM9X5-based boards
 *			that are using WM8731 as codec.
 *
 *  Copyright (C) 2011 Atmel,
 *		  Nicolas Ferre <nicolas.ferre@atmel.com>
 *
 *  Copyright (C) 2013 Paratronic,
 *		  Richard Genoud <richard.genoud@gmail.com>
 *
 * Based on sam9g20_wm8731.c by:
 * Sedji Gaouaou <sedji.gaouaou@atmel.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/of.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/device.h>

#include <sound/soc.h>
#include <sound/soc-dai.h>
#include <sound/soc-dapm.h>

#include "../codecs/wm8731.h"
#include "atmel_ssc_dai.h"


#define MCLK_RATE 12288000

#define DRV_NAME "sam9x5-snd-wm8731"

struct sam9x5_drvdata {
	int ssc_id;
};

/*
 * Logic for a wm8731 as connected on a at91sam9x5ek based board.
 */
static int sam9x5_wm8731_init(struct snd_soc_pcm_runtime *rtd)
{
<<<<<<< HEAD
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
=======
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct device *dev = rtd->dev;
	int ret;

	dev_dbg(dev, "%s called\n", __func__);

	/* set the codec system clock for DAC and ADC */
	ret = snd_soc_dai_set_sysclk(codec_dai, WM8731_SYSCLK_XTAL,
				     MCLK_RATE, SND_SOC_CLOCK_IN);
	if (ret < 0) {
		dev_err(dev, "Failed to set WM8731 SYSCLK: %d\n", ret);
		return ret;
	}

	return 0;
}

/*
 * Audio paths on at91sam9x5ek board:
 *
 *  |A| ------------> |      | ---R----> Headphone Jack
 *  |T| <----\        |  WM  | ---L--/
 *  |9| ---> CLK <--> | 8731 | <--R----- Line In Jack
 *  |1| <------------ |      | <--L--/
 */
static const struct snd_soc_dapm_widget sam9x5_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_LINE("Line In Jack", NULL),
};

static int sam9x5_wm8731_driver_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device_node *codec_np, *cpu_np;
	struct snd_soc_card *card;
	struct snd_soc_dai_link *dai;
	struct sam9x5_drvdata *priv;
<<<<<<< HEAD
=======
	struct snd_soc_dai_link_component *comp;
>>>>>>> upstream/android-13
	int ret;

	if (!np) {
		dev_err(&pdev->dev, "No device node supplied\n");
		return -EINVAL;
	}

	card = devm_kzalloc(&pdev->dev, sizeof(*card), GFP_KERNEL);
	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	dai = devm_kzalloc(&pdev->dev, sizeof(*dai), GFP_KERNEL);
<<<<<<< HEAD
	if (!dai || !card || !priv) {
=======
	comp = devm_kzalloc(&pdev->dev, 3 * sizeof(*comp), GFP_KERNEL);
	if (!dai || !card || !priv || !comp) {
>>>>>>> upstream/android-13
		ret = -ENOMEM;
		goto out;
	}

	snd_soc_card_set_drvdata(card, priv);

	card->dev = &pdev->dev;
	card->owner = THIS_MODULE;
	card->dai_link = dai;
	card->num_links = 1;
	card->dapm_widgets = sam9x5_dapm_widgets;
	card->num_dapm_widgets = ARRAY_SIZE(sam9x5_dapm_widgets);
<<<<<<< HEAD
	dai->name = "WM8731";
	dai->stream_name = "WM8731 PCM";
	dai->codec_dai_name = "wm8731-hifi";
=======

	dai->cpus = &comp[0];
	dai->num_cpus = 1;
	dai->codecs = &comp[1];
	dai->num_codecs = 1;
	dai->platforms = &comp[2];
	dai->num_platforms = 1;

	dai->name = "WM8731";
	dai->stream_name = "WM8731 PCM";
	dai->codecs->dai_name = "wm8731-hifi";
>>>>>>> upstream/android-13
	dai->init = sam9x5_wm8731_init;
	dai->dai_fmt = SND_SOC_DAIFMT_DSP_A | SND_SOC_DAIFMT_NB_NF
		| SND_SOC_DAIFMT_CBM_CFM;

	ret = snd_soc_of_parse_card_name(card, "atmel,model");
	if (ret) {
		dev_err(&pdev->dev, "atmel,model node missing\n");
		goto out;
	}

	ret = snd_soc_of_parse_audio_routing(card, "atmel,audio-routing");
	if (ret) {
		dev_err(&pdev->dev, "atmel,audio-routing node missing\n");
		goto out;
	}

	codec_np = of_parse_phandle(np, "atmel,audio-codec", 0);
	if (!codec_np) {
		dev_err(&pdev->dev, "atmel,audio-codec node missing\n");
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	dai->codec_of_node = codec_np;
=======
	dai->codecs->of_node = codec_np;
>>>>>>> upstream/android-13

	cpu_np = of_parse_phandle(np, "atmel,ssc-controller", 0);
	if (!cpu_np) {
		dev_err(&pdev->dev, "atmel,ssc-controller node missing\n");
		ret = -EINVAL;
<<<<<<< HEAD
		goto out;
	}
	dai->cpu_of_node = cpu_np;
	dai->platform_of_node = cpu_np;
=======
		goto out_put_codec_np;
	}
	dai->cpus->of_node = cpu_np;
	dai->platforms->of_node = cpu_np;
>>>>>>> upstream/android-13

	priv->ssc_id = of_alias_get_id(cpu_np, "ssc");

	ret = atmel_ssc_set_audio(priv->ssc_id);
	if (ret != 0) {
		dev_err(&pdev->dev, "Failed to set SSC %d for audio: %d\n",
			ret, priv->ssc_id);
<<<<<<< HEAD
		goto out;
	}

	of_node_put(codec_np);
	of_node_put(cpu_np);

	ret = snd_soc_register_card(card);
=======
		goto out_put_cpu_np;
	}

	ret = devm_snd_soc_register_card(&pdev->dev, card);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(&pdev->dev, "Platform device allocation failed\n");
		goto out_put_audio;
	}

	dev_dbg(&pdev->dev, "%s ok\n", __func__);

<<<<<<< HEAD
	return ret;

out_put_audio:
	atmel_ssc_put_audio(priv->ssc_id);
=======
	goto out_put_cpu_np;

out_put_audio:
	atmel_ssc_put_audio(priv->ssc_id);
out_put_cpu_np:
	of_node_put(cpu_np);
out_put_codec_np:
	of_node_put(codec_np);
>>>>>>> upstream/android-13
out:
	return ret;
}

static int sam9x5_wm8731_driver_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct sam9x5_drvdata *priv = card->drvdata;

<<<<<<< HEAD
	snd_soc_unregister_card(card);
=======
>>>>>>> upstream/android-13
	atmel_ssc_put_audio(priv->ssc_id);

	return 0;
}

static const struct of_device_id sam9x5_wm8731_of_match[] = {
	{ .compatible = "atmel,sam9x5-wm8731-audio", },
	{},
};
MODULE_DEVICE_TABLE(of, sam9x5_wm8731_of_match);

static struct platform_driver sam9x5_wm8731_driver = {
	.driver = {
		.name = DRV_NAME,
		.of_match_table = of_match_ptr(sam9x5_wm8731_of_match),
	},
	.probe = sam9x5_wm8731_driver_probe,
	.remove = sam9x5_wm8731_driver_remove,
};
module_platform_driver(sam9x5_wm8731_driver);

/* Module information */
MODULE_AUTHOR("Nicolas Ferre <nicolas.ferre@atmel.com>");
MODULE_AUTHOR("Richard Genoud <richard.genoud@gmail.com>");
MODULE_DESCRIPTION("ALSA SoC machine driver for AT91SAM9x5 - WM8731");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
