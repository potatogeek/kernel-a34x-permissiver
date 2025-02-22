<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * simone.c -- ASoC audio for Simplemachines Sim.One board
 *
 * Copyright (c) 2010 Mika Westerberg
 *
 * Based on snappercl15 machine driver by Ryan Mallon.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/soc/cirrus/ep93xx.h>
>>>>>>> upstream/android-13

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

#include <asm/mach-types.h>
<<<<<<< HEAD
#include <mach/hardware.h>
=======

SND_SOC_DAILINK_DEFS(hifi,
	DAILINK_COMP_ARRAY(COMP_CPU("ep93xx-ac97")),
	DAILINK_COMP_ARRAY(COMP_CODEC("ac97-codec", "ac97-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("ep93xx-ac97")));
>>>>>>> upstream/android-13

static struct snd_soc_dai_link simone_dai = {
	.name		= "AC97",
	.stream_name	= "AC97 HiFi",
<<<<<<< HEAD
	.cpu_dai_name	= "ep93xx-ac97",
	.codec_dai_name	= "ac97-hifi",
	.codec_name	= "ac97-codec",
	.platform_name	= "ep93xx-ac97",
=======
	SND_SOC_DAILINK_REG(hifi),
>>>>>>> upstream/android-13
};

static struct snd_soc_card snd_soc_simone = {
	.name		= "Sim.One",
	.owner		= THIS_MODULE,
	.dai_link	= &simone_dai,
	.num_links	= 1,
};

static struct platform_device *simone_snd_ac97_device;

static int simone_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &snd_soc_simone;
	int ret;

	simone_snd_ac97_device = platform_device_register_simple("ac97-codec",
								 -1, NULL, 0);
	if (IS_ERR(simone_snd_ac97_device))
		return PTR_ERR(simone_snd_ac97_device);

	card->dev = &pdev->dev;

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
			ret);
		platform_device_unregister(simone_snd_ac97_device);
	}

	return ret;
}

static int simone_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	snd_soc_unregister_card(card);
	platform_device_unregister(simone_snd_ac97_device);

	return 0;
}

static struct platform_driver simone_driver = {
	.driver		= {
		.name	= "simone-audio",
	},
	.probe		= simone_probe,
	.remove		= simone_remove,
};

module_platform_driver(simone_driver);

MODULE_DESCRIPTION("ALSA SoC Simplemachines Sim.One");
MODULE_AUTHOR("Mika Westerberg <mika.westerberg@iki.fi>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:simone-audio");
