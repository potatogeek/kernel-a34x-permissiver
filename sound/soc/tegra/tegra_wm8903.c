<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * tegra_wm8903.c - Tegra machine ASoC driver for boards using WM8903 codec.
 *
 * Author: Stephen Warren <swarren@nvidia.com>
 * Copyright (C) 2010-2012 - NVIDIA, Inc.
 *
 * Based on code copyright/by:
 *
 * (c) 2009, 2010 Nvidia Graphics Pvt. Ltd.
 *
 * Copyright 2007 Wolfson Microelectronics PLC.
 * Author: Graeme Gregory
 *         graeme.gregory@wolfsonmicro.com or linux@wolfsonmicro.com
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include <sound/core.h>
#include <sound/jack.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
=======
 */

#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/jack.h>
>>>>>>> upstream/android-13
#include <sound/soc.h>

#include "../codecs/wm8903.h"

<<<<<<< HEAD
#include "tegra_asoc_utils.h"

#define DRV_NAME "tegra-snd-wm8903"

struct tegra_wm8903 {
	int gpio_spkr_en;
	int gpio_hp_det;
	int gpio_hp_mute;
	int gpio_int_mic_en;
	int gpio_ext_mic_en;
	struct tegra_asoc_utils_data util_data;
};

static int tegra_wm8903_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_card *card = rtd->card;
	struct tegra_wm8903 *machine = snd_soc_card_get_drvdata(card);
	int srate, mclk;
	int err;

	srate = params_rate(params);
=======
#include "tegra_asoc_machine.h"

static struct snd_soc_jack_pin tegra_wm8903_mic_jack_pins[] = {
	{ .pin = "Mic Jack", .mask = SND_JACK_MICROPHONE },
};

static unsigned int tegra_wm8903_mclk_rate(unsigned int srate)
{
	unsigned int mclk;

>>>>>>> upstream/android-13
	switch (srate) {
	case 64000:
	case 88200:
	case 96000:
		mclk = 128 * srate;
		break;
	default:
		mclk = 256 * srate;
		break;
	}
	/* FIXME: Codec only requires >= 3MHz if OSR==0 */
	while (mclk < 6000000)
		mclk *= 2;

<<<<<<< HEAD
	err = tegra_asoc_utils_set_rate(&machine->util_data, srate, mclk);
	if (err < 0) {
		dev_err(card->dev, "Can't configure clocks\n");
		return err;
	}

	err = snd_soc_dai_set_sysclk(codec_dai, 0, mclk,
					SND_SOC_CLOCK_IN);
	if (err < 0) {
		dev_err(card->dev, "codec_dai clock not set\n");
		return err;
	}

	return 0;
}

static const struct snd_soc_ops tegra_wm8903_ops = {
	.hw_params = tegra_wm8903_hw_params,
};

static struct snd_soc_jack tegra_wm8903_hp_jack;

static struct snd_soc_jack_pin tegra_wm8903_hp_jack_pins[] = {
	{
		.pin = "Headphone Jack",
		.mask = SND_JACK_HEADPHONE,
	},
};

static struct snd_soc_jack_gpio tegra_wm8903_hp_jack_gpio = {
	.name = "headphone detect",
	.report = SND_JACK_HEADPHONE,
	.debounce_time = 150,
	.invert = 1,
};

static struct snd_soc_jack tegra_wm8903_mic_jack;

static struct snd_soc_jack_pin tegra_wm8903_mic_jack_pins[] = {
	{
		.pin = "Mic Jack",
		.mask = SND_JACK_MICROPHONE,
	},
};

static int tegra_wm8903_event_int_spk(struct snd_soc_dapm_widget *w,
					struct snd_kcontrol *k, int event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;
	struct tegra_wm8903 *machine = snd_soc_card_get_drvdata(card);

	if (!gpio_is_valid(machine->gpio_spkr_en))
		return 0;

	gpio_set_value_cansleep(machine->gpio_spkr_en,
				SND_SOC_DAPM_EVENT_ON(event));

	return 0;
}

static int tegra_wm8903_event_hp(struct snd_soc_dapm_widget *w,
					struct snd_kcontrol *k, int event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;
	struct tegra_wm8903 *machine = snd_soc_card_get_drvdata(card);

	if (!gpio_is_valid(machine->gpio_hp_mute))
		return 0;

	gpio_set_value_cansleep(machine->gpio_hp_mute,
				!SND_SOC_DAPM_EVENT_ON(event));

	return 0;
}

static const struct snd_soc_dapm_widget tegra_wm8903_dapm_widgets[] = {
	SND_SOC_DAPM_SPK("Int Spk", tegra_wm8903_event_int_spk),
	SND_SOC_DAPM_HP("Headphone Jack", tegra_wm8903_event_hp),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
};

static const struct snd_kcontrol_new tegra_wm8903_controls[] = {
	SOC_DAPM_PIN_SWITCH("Int Spk"),
};

static int tegra_wm8903_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_component *component = codec_dai->component;
	struct snd_soc_card *card = rtd->card;
	struct tegra_wm8903 *machine = snd_soc_card_get_drvdata(card);
	int shrt = 0;

	if (gpio_is_valid(machine->gpio_hp_det)) {
		tegra_wm8903_hp_jack_gpio.gpio = machine->gpio_hp_det;
		snd_soc_card_jack_new(rtd->card, "Headphone Jack",
				      SND_JACK_HEADPHONE, &tegra_wm8903_hp_jack,
				      tegra_wm8903_hp_jack_pins,
				      ARRAY_SIZE(tegra_wm8903_hp_jack_pins));
		snd_soc_jack_add_gpios(&tegra_wm8903_hp_jack,
					1,
					&tegra_wm8903_hp_jack_gpio);
	}

	if (of_property_read_bool(card->dev->of_node, "nvidia,headset"))
		shrt = SND_JACK_MICROPHONE;

	snd_soc_card_jack_new(rtd->card, "Mic Jack", SND_JACK_MICROPHONE,
			      &tegra_wm8903_mic_jack,
			      tegra_wm8903_mic_jack_pins,
			      ARRAY_SIZE(tegra_wm8903_mic_jack_pins));
	wm8903_mic_detect(component, &tegra_wm8903_mic_jack, SND_JACK_MICROPHONE,
				shrt);
=======
	return mclk;
}

static int tegra_wm8903_init(struct snd_soc_pcm_runtime *rtd)
{
	struct tegra_machine *machine = snd_soc_card_get_drvdata(rtd->card);
	struct snd_soc_card *card = rtd->card;
	int err;

	/*
	 * Older version of machine driver was ignoring GPIO polarity,
	 * forcing it to active-low.  This means that all older device-trees
	 * which set the polarity to active-high are wrong and we need to fix
	 * them up.
	 */
	if (machine->asoc->hp_jack_gpio_active_low) {
		bool active_low = gpiod_is_active_low(machine->gpiod_hp_det);

		machine->hp_jack_gpio->invert = !active_low;
	}

	err = tegra_asoc_machine_init(rtd);
	if (err)
		return err;

	if (!machine->gpiod_mic_det && machine->asoc->add_mic_jack) {
		struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
		struct snd_soc_component *component = codec_dai->component;
		int shrt = 0;

		err = snd_soc_card_jack_new(rtd->card, "Mic Jack",
					    SND_JACK_MICROPHONE,
					    machine->mic_jack,
					    tegra_wm8903_mic_jack_pins,
					    ARRAY_SIZE(tegra_wm8903_mic_jack_pins));
		if (err) {
			dev_err(rtd->dev, "Mic Jack creation failed: %d\n", err);
			return err;
		}

		if (of_property_read_bool(card->dev->of_node, "nvidia,headset"))
			shrt = SND_JACK_MICROPHONE;

		wm8903_mic_detect(component, machine->mic_jack,
				  SND_JACK_MICROPHONE, shrt);
	}
>>>>>>> upstream/android-13

	snd_soc_dapm_force_enable_pin(&card->dapm, "MICBIAS");

	return 0;
}

static int tegra_wm8903_remove(struct snd_soc_card *card)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd =
		snd_soc_get_pcm_runtime(card, card->dai_link[0].name);
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
=======
	struct snd_soc_dai_link *link = &card->dai_link[0];
	struct snd_soc_pcm_runtime *rtd = snd_soc_get_pcm_runtime(card, link);
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct snd_soc_component *component = codec_dai->component;

	wm8903_mic_detect(component, NULL, 0, 0);

	return 0;
}

<<<<<<< HEAD
static struct snd_soc_dai_link tegra_wm8903_dai = {
	.name = "WM8903",
	.stream_name = "WM8903 PCM",
	.codec_dai_name = "wm8903-hifi",
	.init = tegra_wm8903_init,
	.ops = &tegra_wm8903_ops,
	.dai_fmt = SND_SOC_DAIFMT_I2S |
		   SND_SOC_DAIFMT_NB_NF |
		   SND_SOC_DAIFMT_CBS_CFS,
};

static struct snd_soc_card snd_soc_tegra_wm8903 = {
	.name = "tegra-wm8903",
=======
SND_SOC_DAILINK_DEFS(hifi,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "wm8903-hifi")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link tegra_wm8903_dai = {
	.name = "WM8903",
	.stream_name = "WM8903 PCM",
	.init = tegra_wm8903_init,
	.dai_fmt = SND_SOC_DAIFMT_I2S |
		   SND_SOC_DAIFMT_NB_NF |
		   SND_SOC_DAIFMT_CBS_CFS,
	SND_SOC_DAILINK_REG(hifi),
};

static struct snd_soc_card snd_soc_tegra_wm8903 = {
	.components = "codec:wm8903",
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
	.dai_link = &tegra_wm8903_dai,
	.num_links = 1,
	.remove = tegra_wm8903_remove,
<<<<<<< HEAD
	.controls = tegra_wm8903_controls,
	.num_controls = ARRAY_SIZE(tegra_wm8903_controls),
	.dapm_widgets = tegra_wm8903_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(tegra_wm8903_dapm_widgets),
	.fully_routed = true,
};

static int tegra_wm8903_driver_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct snd_soc_card *card = &snd_soc_tegra_wm8903;
	struct tegra_wm8903 *machine;
	int ret;

	machine = devm_kzalloc(&pdev->dev, sizeof(struct tegra_wm8903),
			       GFP_KERNEL);
	if (!machine)
		return -ENOMEM;

	card->dev = &pdev->dev;
	snd_soc_card_set_drvdata(card, machine);

	machine->gpio_spkr_en = of_get_named_gpio(np, "nvidia,spkr-en-gpios",
						  0);
	if (machine->gpio_spkr_en == -EPROBE_DEFER)
		return -EPROBE_DEFER;
	if (gpio_is_valid(machine->gpio_spkr_en)) {
		ret = devm_gpio_request_one(&pdev->dev, machine->gpio_spkr_en,
					    GPIOF_OUT_INIT_LOW, "spkr_en");
		if (ret) {
			dev_err(card->dev, "cannot get spkr_en gpio\n");
			return ret;
		}
	}

	machine->gpio_hp_mute = of_get_named_gpio(np, "nvidia,hp-mute-gpios",
						  0);
	if (machine->gpio_hp_mute == -EPROBE_DEFER)
		return -EPROBE_DEFER;
	if (gpio_is_valid(machine->gpio_hp_mute)) {
		ret = devm_gpio_request_one(&pdev->dev, machine->gpio_hp_mute,
					    GPIOF_OUT_INIT_HIGH, "hp_mute");
		if (ret) {
			dev_err(card->dev, "cannot get hp_mute gpio\n");
			return ret;
		}
	}

	machine->gpio_hp_det = of_get_named_gpio(np, "nvidia,hp-det-gpios", 0);
	if (machine->gpio_hp_det == -EPROBE_DEFER)
		return -EPROBE_DEFER;

	machine->gpio_int_mic_en = of_get_named_gpio(np,
						"nvidia,int-mic-en-gpios", 0);
	if (machine->gpio_int_mic_en == -EPROBE_DEFER)
		return -EPROBE_DEFER;
	if (gpio_is_valid(machine->gpio_int_mic_en)) {
		/* Disable int mic; enable signal is active-high */
		ret = devm_gpio_request_one(&pdev->dev,
					    machine->gpio_int_mic_en,
					    GPIOF_OUT_INIT_LOW, "int_mic_en");
		if (ret) {
			dev_err(card->dev, "cannot get int_mic_en gpio\n");
			return ret;
		}
	}

	machine->gpio_ext_mic_en = of_get_named_gpio(np,
						"nvidia,ext-mic-en-gpios", 0);
	if (machine->gpio_ext_mic_en == -EPROBE_DEFER)
		return -EPROBE_DEFER;
	if (gpio_is_valid(machine->gpio_ext_mic_en)) {
		/* Enable ext mic; enable signal is active-low */
		ret = devm_gpio_request_one(&pdev->dev,
					    machine->gpio_ext_mic_en,
					    GPIOF_OUT_INIT_LOW, "ext_mic_en");
		if (ret) {
			dev_err(card->dev, "cannot get ext_mic_en gpio\n");
			return ret;
		}
	}

	ret = snd_soc_of_parse_card_name(card, "nvidia,model");
	if (ret)
		goto err;

	ret = snd_soc_of_parse_audio_routing(card, "nvidia,audio-routing");
	if (ret)
		goto err;

	tegra_wm8903_dai.codec_of_node = of_parse_phandle(np,
						"nvidia,audio-codec", 0);
	if (!tegra_wm8903_dai.codec_of_node) {
		dev_err(&pdev->dev,
			"Property 'nvidia,audio-codec' missing or invalid\n");
		ret = -EINVAL;
		goto err;
	}

	tegra_wm8903_dai.cpu_of_node = of_parse_phandle(np,
			"nvidia,i2s-controller", 0);
	if (!tegra_wm8903_dai.cpu_of_node) {
		dev_err(&pdev->dev,
			"Property 'nvidia,i2s-controller' missing or invalid\n");
		ret = -EINVAL;
		goto err;
	}

	tegra_wm8903_dai.platform_of_node = tegra_wm8903_dai.cpu_of_node;

	ret = tegra_asoc_utils_init(&machine->util_data, &pdev->dev);
	if (ret)
		goto err;

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n",
			ret);
		goto err_fini_utils;
	}

	return 0;

err_fini_utils:
	tegra_asoc_utils_fini(&machine->util_data);
err:
	return ret;
}

static int tegra_wm8903_driver_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct tegra_wm8903 *machine = snd_soc_card_get_drvdata(card);

	snd_soc_unregister_card(card);

	tegra_asoc_utils_fini(&machine->util_data);

	return 0;
}

static const struct of_device_id tegra_wm8903_of_match[] = {
	{ .compatible = "nvidia,tegra-audio-wm8903", },
	{},
};

static struct platform_driver tegra_wm8903_driver = {
	.driver = {
		.name = DRV_NAME,
		.pm = &snd_soc_pm_ops,
		.of_match_table = tegra_wm8903_of_match,
	},
	.probe = tegra_wm8903_driver_probe,
	.remove = tegra_wm8903_driver_remove,
=======
	.fully_routed = true,
};

/* older device-trees used wrong polarity for the headphones-detection GPIO */
static const struct tegra_asoc_data tegra_wm8903_data_legacy = {
	.mclk_rate = tegra_wm8903_mclk_rate,
	.card = &snd_soc_tegra_wm8903,
	.hp_jack_gpio_active_low = true,
	.add_common_dapm_widgets = true,
	.add_common_controls = true,
	.add_common_snd_ops = true,
	.add_mic_jack = true,
	.add_hp_jack = true,
};

static const struct tegra_asoc_data tegra_wm8903_data = {
	.mclk_rate = tegra_wm8903_mclk_rate,
	.card = &snd_soc_tegra_wm8903,
	.add_common_dapm_widgets = true,
	.add_common_controls = true,
	.add_common_snd_ops = true,
	.add_mic_jack = true,
	.add_hp_jack = true,
};

static const struct of_device_id tegra_wm8903_of_match[] = {
	{ .compatible = "ad,tegra-audio-plutux", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "ad,tegra-audio-wm8903-medcom-wide", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "ad,tegra-audio-wm8903-tec", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903-cardhu", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903-harmony", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903-picasso", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903-seaboard", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903-ventana", .data = &tegra_wm8903_data_legacy },
	{ .compatible = "nvidia,tegra-audio-wm8903", .data = &tegra_wm8903_data },
	{},
};
MODULE_DEVICE_TABLE(of, tegra_wm8903_of_match);

static struct platform_driver tegra_wm8903_driver = {
	.driver = {
		.name = "tegra-wm8903",
		.of_match_table = tegra_wm8903_of_match,
		.pm = &snd_soc_pm_ops,
	},
	.probe = tegra_asoc_machine_probe,
>>>>>>> upstream/android-13
};
module_platform_driver(tegra_wm8903_driver);

MODULE_AUTHOR("Stephen Warren <swarren@nvidia.com>");
MODULE_DESCRIPTION("Tegra+WM8903 machine ASoC driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS("platform:" DRV_NAME);
MODULE_DEVICE_TABLE(of, tegra_wm8903_of_match);
=======
>>>>>>> upstream/android-13
