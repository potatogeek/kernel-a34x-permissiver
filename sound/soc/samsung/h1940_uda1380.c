<<<<<<< HEAD
/*
 * h1940-uda1380.c  --  ALSA Soc Audio Layer
 *
 * Copyright (c) 2010 Arnaud Patard <arnaud.patard@rtp-net.org>
 * Copyright (c) 2010 Vasily Khoruzhick <anarsoul@gmail.com>
 *
 * Based on version from Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// h1940_uda1380.c - ALSA SoC Audio Layer
//
// Copyright (c) 2010 Arnaud Patard <arnaud.patard@rtp-net.org>
// Copyright (c) 2010 Vasily Khoruzhick <anarsoul@gmail.com>
//
// Based on version from Arnaud Patard <arnaud.patard@rtp-net.org>
>>>>>>> upstream/android-13

#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/module.h>

#include <sound/soc.h>
#include <sound/jack.h>

#include "regs-iis.h"
<<<<<<< HEAD
#include <asm/mach-types.h>

#include <mach/gpio-samsung.h>
=======
>>>>>>> upstream/android-13
#include "s3c24xx-i2s.h"

static const unsigned int rates[] = {
	11025,
	22050,
	44100,
};

static const struct snd_pcm_hw_constraint_list hw_rates = {
	.count = ARRAY_SIZE(rates),
	.list = rates,
};

<<<<<<< HEAD
=======
static struct gpio_desc *gpiod_speaker_power;

>>>>>>> upstream/android-13
static struct snd_soc_jack hp_jack;

static struct snd_soc_jack_pin hp_jack_pins[] = {
	{
		.pin	= "Headphone Jack",
		.mask	= SND_JACK_HEADPHONE,
	},
	{
		.pin	= "Speaker",
		.mask	= SND_JACK_HEADPHONE,
		.invert	= 1,
	},
};

static struct snd_soc_jack_gpio hp_jack_gpios[] = {
	{
<<<<<<< HEAD
		.gpio			= S3C2410_GPG(4),
=======
>>>>>>> upstream/android-13
		.name			= "hp-gpio",
		.report			= SND_JACK_HEADPHONE,
		.invert			= 1,
		.debounce_time		= 200,
	},
};

static int h1940_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	return snd_pcm_hw_constraint_list(runtime, 0,
					SNDRV_PCM_HW_PARAM_RATE,
					&hw_rates);
}

static int h1940_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
>>>>>>> upstream/android-13
	int div;
	int ret;
	unsigned int rate = params_rate(params);

	switch (rate) {
	case 11025:
	case 22050:
	case 44100:
		div = s3c24xx_i2s_get_clockrate() / (384 * rate);
		if (s3c24xx_i2s_get_clockrate() % (384 * rate) > (192 * rate))
			div++;
		break;
	default:
		dev_err(rtd->dev, "%s: rate %d is not supported\n",
			__func__, rate);
		return -EINVAL;
	}

	/* select clock source */
	ret = snd_soc_dai_set_sysclk(cpu_dai, S3C24XX_CLKSRC_PCLK, rate,
			SND_SOC_CLOCK_OUT);
	if (ret < 0)
		return ret;

	/* set MCLK division for sample rate */
	ret = snd_soc_dai_set_clkdiv(cpu_dai, S3C24XX_DIV_MCLK,
		S3C2410_IISMOD_384FS);
	if (ret < 0)
		return ret;

	/* set BCLK division for sample rate */
	ret = snd_soc_dai_set_clkdiv(cpu_dai, S3C24XX_DIV_BCLK,
		S3C2410_IISMOD_32FS);
	if (ret < 0)
		return ret;

	/* set prescaler division for sample rate */
	ret = snd_soc_dai_set_clkdiv(cpu_dai, S3C24XX_DIV_PRESCALER,
		S3C24XX_PRESCALE(div, div));
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static struct snd_soc_ops h1940_ops = {
=======
static const struct snd_soc_ops h1940_ops = {
>>>>>>> upstream/android-13
	.startup	= h1940_startup,
	.hw_params	= h1940_hw_params,
};

static int h1940_spk_power(struct snd_soc_dapm_widget *w,
				struct snd_kcontrol *kcontrol, int event)
{
	if (SND_SOC_DAPM_EVENT_ON(event))
<<<<<<< HEAD
		gpio_set_value(S3C_GPIO_END + 9, 1);
	else
		gpio_set_value(S3C_GPIO_END + 9, 0);
=======
		gpiod_set_value(gpiod_speaker_power, 1);
	else
		gpiod_set_value(gpiod_speaker_power, 0);
>>>>>>> upstream/android-13

	return 0;
}

/* h1940 machine dapm widgets */
static const struct snd_soc_dapm_widget uda1380_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
	SND_SOC_DAPM_SPK("Speaker", h1940_spk_power),
};

/* h1940 machine audio_map */
static const struct snd_soc_dapm_route audio_map[] = {
	/* headphone connected to VOUTLHP, VOUTRHP */
	{"Headphone Jack", NULL, "VOUTLHP"},
	{"Headphone Jack", NULL, "VOUTRHP"},

	/* ext speaker connected to VOUTL, VOUTR  */
	{"Speaker", NULL, "VOUTL"},
	{"Speaker", NULL, "VOUTR"},

	/* mic is connected to VINM */
	{"VINM", NULL, "Mic Jack"},
};

<<<<<<< HEAD
static struct platform_device *s3c24xx_snd_device;

=======
>>>>>>> upstream/android-13
static int h1940_uda1380_init(struct snd_soc_pcm_runtime *rtd)
{
	snd_soc_card_jack_new(rtd->card, "Headphone Jack", SND_JACK_HEADPHONE,
		&hp_jack, hp_jack_pins, ARRAY_SIZE(hp_jack_pins));

	snd_soc_jack_add_gpios(&hp_jack, ARRAY_SIZE(hp_jack_gpios),
		hp_jack_gpios);

	return 0;
}

/* s3c24xx digital audio interface glue - connects codec <--> CPU */
<<<<<<< HEAD
=======
SND_SOC_DAILINK_DEFS(uda1380,
	DAILINK_COMP_ARRAY(COMP_CPU("s3c24xx-iis")),
	DAILINK_COMP_ARRAY(COMP_CODEC("uda1380-codec.0-001a", "uda1380-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("s3c24xx-iis")));

>>>>>>> upstream/android-13
static struct snd_soc_dai_link h1940_uda1380_dai[] = {
	{
		.name		= "uda1380",
		.stream_name	= "UDA1380 Duplex",
<<<<<<< HEAD
		.cpu_dai_name	= "s3c24xx-iis",
		.codec_dai_name	= "uda1380-hifi",
		.init		= h1940_uda1380_init,
		.platform_name	= "s3c24xx-iis",
		.codec_name	= "uda1380-codec.0-001a",
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBS_CFS,
		.ops		= &h1940_ops,
=======
		.init		= h1940_uda1380_init,
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBS_CFS,
		.ops		= &h1940_ops,
		SND_SOC_DAILINK_REG(uda1380),
>>>>>>> upstream/android-13
	},
};

static struct snd_soc_card h1940_asoc = {
	.name = "h1940",
	.owner = THIS_MODULE,
	.dai_link = h1940_uda1380_dai,
	.num_links = ARRAY_SIZE(h1940_uda1380_dai),

	.dapm_widgets = uda1380_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(uda1380_dapm_widgets),
	.dapm_routes = audio_map,
	.num_dapm_routes = ARRAY_SIZE(audio_map),
};

<<<<<<< HEAD
static int __init h1940_init(void)
{
	int ret;

	if (!machine_is_h1940())
		return -ENODEV;

	/* configure some gpios */
	ret = gpio_request(S3C_GPIO_END + 9, "speaker-power");
	if (ret)
		goto err_out;

	ret = gpio_direction_output(S3C_GPIO_END + 9, 0);
	if (ret)
		goto err_gpio;

	s3c24xx_snd_device = platform_device_alloc("soc-audio", -1);
	if (!s3c24xx_snd_device) {
		ret = -ENOMEM;
		goto err_gpio;
	}

	platform_set_drvdata(s3c24xx_snd_device, &h1940_asoc);
	ret = platform_device_add(s3c24xx_snd_device);

	if (ret)
		goto err_plat;

	return 0;

err_plat:
	platform_device_put(s3c24xx_snd_device);
err_gpio:
	gpio_free(S3C_GPIO_END + 9);

err_out:
	return ret;
}

static void __exit h1940_exit(void)
{
	platform_device_unregister(s3c24xx_snd_device);
	gpio_free(S3C_GPIO_END + 9);
}

module_init(h1940_init);
module_exit(h1940_exit);
=======
static int h1940_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	h1940_asoc.dev = dev;
	hp_jack_gpios[0].gpiod_dev = dev;
	gpiod_speaker_power = devm_gpiod_get(&pdev->dev, "speaker-power",
					     GPIOD_OUT_LOW);

	if (IS_ERR(gpiod_speaker_power)) {
		dev_err(dev, "Could not get gpio\n");
		return PTR_ERR(gpiod_speaker_power);
	}

	return devm_snd_soc_register_card(dev, &h1940_asoc);
}

static struct platform_driver h1940_audio_driver = {
	.driver = {
		.name = "h1940-audio",
		.pm = &snd_soc_pm_ops,
	},
	.probe = h1940_probe,
};
module_platform_driver(h1940_audio_driver);
>>>>>>> upstream/android-13

/* Module information */
MODULE_AUTHOR("Arnaud Patard, Vasily Khoruzhick");
MODULE_DESCRIPTION("ALSA SoC H1940");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:h1940-audio");
>>>>>>> upstream/android-13
