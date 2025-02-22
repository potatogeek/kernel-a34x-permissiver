<<<<<<< HEAD
/*
 * Copyright (C) 2015 - 2016 Samsung Electronics Co., Ltd.
 *
 * Authors: Inha Song <ideal.song@samsung.com>
 *          Sylwester Nawrocki <s.nawrocki@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2015 - 2016 Samsung Electronics Co., Ltd.
//
// Authors: Inha Song <ideal.song@samsung.com>
//          Sylwester Nawrocki <s.nawrocki@samsung.com>
>>>>>>> upstream/android-13

#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "i2s.h"
#include "../codecs/wm5110.h"

/*
 * The source clock is XCLKOUT with its mux set to the external fixed rate
 * oscillator (XXTI).
 */
#define MCLK_RATE	24000000U

#define TM2_DAI_AIF1	0
#define TM2_DAI_AIF2	1

struct tm2_machine_priv {
	struct snd_soc_component *component;
	unsigned int sysclk_rate;
	struct gpio_desc *gpio_mic_bias;
};

static int tm2_start_sysclk(struct snd_soc_card *card)
{
	struct tm2_machine_priv *priv = snd_soc_card_get_drvdata(card);
	struct snd_soc_component *component = priv->component;
	int ret;

	ret = snd_soc_component_set_pll(component, WM5110_FLL1_REFCLK,
				    ARIZONA_FLL_SRC_MCLK1,
				    MCLK_RATE,
				    priv->sysclk_rate);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set FLL1 source: %d\n", ret);
		return ret;
	}

	ret = snd_soc_component_set_pll(component, WM5110_FLL1,
				    ARIZONA_FLL_SRC_MCLK1,
				    MCLK_RATE,
				    priv->sysclk_rate);
	if (ret < 0) {
		dev_err(component->dev, "Failed to start FLL1: %d\n", ret);
		return ret;
	}

	ret = snd_soc_component_set_sysclk(component, ARIZONA_CLK_SYSCLK,
				       ARIZONA_CLK_SRC_FLL1,
				       priv->sysclk_rate,
				       SND_SOC_CLOCK_IN);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set SYSCLK source: %d\n", ret);
		return ret;
	}

	return 0;
}

static int tm2_stop_sysclk(struct snd_soc_card *card)
{
	struct tm2_machine_priv *priv = snd_soc_card_get_drvdata(card);
	struct snd_soc_component *component = priv->component;
	int ret;

	ret = snd_soc_component_set_pll(component, WM5110_FLL1, 0, 0, 0);
	if (ret < 0) {
		dev_err(component->dev, "Failed to stop FLL1: %d\n", ret);
		return ret;
	}

	ret = snd_soc_component_set_sysclk(component, ARIZONA_CLK_SYSCLK,
				       ARIZONA_CLK_SRC_FLL1, 0, 0);
	if (ret < 0) {
		dev_err(component->dev, "Failed to stop SYSCLK: %d\n", ret);
		return ret;
	}

	return 0;
}

static int tm2_aif1_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = rtd->codec_dai->component;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13
	struct tm2_machine_priv *priv = snd_soc_card_get_drvdata(rtd->card);

	switch (params_rate(params)) {
	case 4000:
	case 8000:
	case 12000:
	case 16000:
	case 24000:
	case 32000:
	case 48000:
	case 96000:
	case 192000:
		/* Highest possible SYSCLK frequency: 147.456MHz */
		priv->sysclk_rate = 147456000U;
		break;
	case 11025:
	case 22050:
	case 44100:
	case 88200:
	case 176400:
		/* Highest possible SYSCLK frequency: 135.4752 MHz */
		priv->sysclk_rate = 135475200U;
		break;
	default:
		dev_err(component->dev, "Not supported sample rate: %d\n",
			params_rate(params));
		return -EINVAL;
	}

	return tm2_start_sysclk(rtd->card);
}

<<<<<<< HEAD
static struct snd_soc_ops tm2_aif1_ops = {
=======
static const struct snd_soc_ops tm2_aif1_ops = {
>>>>>>> upstream/android-13
	.hw_params = tm2_aif1_hw_params,
};

static int tm2_aif2_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = rtd->codec_dai->component;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13
	unsigned int asyncclk_rate;
	int ret;

	switch (params_rate(params)) {
	case 8000:
	case 12000:
	case 16000:
		/* Highest possible ASYNCCLK frequency: 49.152MHz */
		asyncclk_rate = 49152000U;
		break;
	case 11025:
		/* Highest possible ASYNCCLK frequency: 45.1584 MHz */
		asyncclk_rate = 45158400U;
		break;
	default:
		dev_err(component->dev, "Not supported sample rate: %d\n",
			params_rate(params));
		return -EINVAL;
	}

	ret = snd_soc_component_set_pll(component, WM5110_FLL2_REFCLK,
				    ARIZONA_FLL_SRC_MCLK1,
				    MCLK_RATE,
				    asyncclk_rate);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set FLL2 source: %d\n", ret);
		return ret;
	}

	ret = snd_soc_component_set_pll(component, WM5110_FLL2,
				    ARIZONA_FLL_SRC_MCLK1,
				    MCLK_RATE,
				    asyncclk_rate);
	if (ret < 0) {
		dev_err(component->dev, "Failed to start FLL2: %d\n", ret);
		return ret;
	}

	ret = snd_soc_component_set_sysclk(component, ARIZONA_CLK_ASYNCCLK,
				       ARIZONA_CLK_SRC_FLL2,
				       asyncclk_rate,
				       SND_SOC_CLOCK_IN);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set ASYNCCLK source: %d\n", ret);
		return ret;
	}

	return 0;
}

static int tm2_aif2_hw_free(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component = rtd->codec_dai->component;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13
	int ret;

	/* disable FLL2 */
	ret = snd_soc_component_set_pll(component, WM5110_FLL2, ARIZONA_FLL_SRC_MCLK1,
				    0, 0);
	if (ret < 0)
		dev_err(component->dev, "Failed to stop FLL2: %d\n", ret);

	return ret;
}

<<<<<<< HEAD
static struct snd_soc_ops tm2_aif2_ops = {
=======
static const struct snd_soc_ops tm2_aif2_ops = {
>>>>>>> upstream/android-13
	.hw_params = tm2_aif2_hw_params,
	.hw_free = tm2_aif2_hw_free,
};

static int tm2_hdmi_hw_params(struct snd_pcm_substream *substream,
			      struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
>>>>>>> upstream/android-13
	unsigned int bfs;
	int bitwidth, ret;

	bitwidth = snd_pcm_format_width(params_format(params));
	if (bitwidth < 0) {
		dev_err(rtd->card->dev, "Invalid bit-width: %d\n", bitwidth);
		return bitwidth;
	}

	switch (bitwidth) {
	case 48:
		bfs = 64;
		break;
	case 16:
		bfs = 32;
		break;
	default:
		dev_err(rtd->card->dev, "Unsupported bit-width: %d\n", bitwidth);
		return -EINVAL;
	}

	switch (params_rate(params)) {
	case 48000:
	case 96000:
	case 192000:
		break;
	default:
		dev_err(rtd->card->dev, "Unsupported sample rate: %d\n",
			params_rate(params));
		return -EINVAL;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, SAMSUNG_I2S_OPCLK,
					0, SAMSUNG_I2S_OPCLK_PCLK);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_clkdiv(cpu_dai, SAMSUNG_I2S_DIV_BCLK, bfs);
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static struct snd_soc_ops tm2_hdmi_ops = {
=======
static const struct snd_soc_ops tm2_hdmi_ops = {
>>>>>>> upstream/android-13
	.hw_params = tm2_hdmi_hw_params,
};

static int tm2_mic_bias(struct snd_soc_dapm_widget *w,
				struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_card *card = w->dapm->card;
	struct tm2_machine_priv *priv = snd_soc_card_get_drvdata(card);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		gpiod_set_value_cansleep(priv->gpio_mic_bias,  1);
		break;
	case SND_SOC_DAPM_POST_PMD:
		gpiod_set_value_cansleep(priv->gpio_mic_bias,  0);
		break;
	}

	return 0;
}

static int tm2_set_bias_level(struct snd_soc_card *card,
				struct snd_soc_dapm_context *dapm,
				enum snd_soc_bias_level level)
{
	struct snd_soc_pcm_runtime *rtd;

<<<<<<< HEAD
	rtd = snd_soc_get_pcm_runtime(card, card->dai_link[0].name);

	if (dapm->dev != rtd->codec_dai->dev)
=======
	rtd = snd_soc_get_pcm_runtime(card, &card->dai_link[0]);

	if (dapm->dev != asoc_rtd_to_codec(rtd, 0)->dev)
>>>>>>> upstream/android-13
		return 0;

	switch (level) {
	case SND_SOC_BIAS_STANDBY:
		if (card->dapm.bias_level == SND_SOC_BIAS_OFF)
			tm2_start_sysclk(card);
		break;
	case SND_SOC_BIAS_OFF:
		tm2_stop_sysclk(card);
		break;
	default:
		break;
	}

	return 0;
}

static struct snd_soc_aux_dev tm2_speaker_amp_dev;

static int tm2_late_probe(struct snd_soc_card *card)
{
	struct tm2_machine_priv *priv = snd_soc_card_get_drvdata(card);
<<<<<<< HEAD
	struct snd_soc_dai_link_component dlc = { 0 };
=======
>>>>>>> upstream/android-13
	unsigned int ch_map[] = { 0, 1 };
	struct snd_soc_dai *amp_pdm_dai;
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_dai *aif1_dai;
	struct snd_soc_dai *aif2_dai;
	int ret;

<<<<<<< HEAD
	rtd = snd_soc_get_pcm_runtime(card, card->dai_link[TM2_DAI_AIF1].name);
	aif1_dai = rtd->codec_dai;
	priv->component = rtd->codec_dai->component;
=======
	rtd = snd_soc_get_pcm_runtime(card, &card->dai_link[TM2_DAI_AIF1]);
	aif1_dai = asoc_rtd_to_codec(rtd, 0);
	priv->component = asoc_rtd_to_codec(rtd, 0)->component;
>>>>>>> upstream/android-13

	ret = snd_soc_dai_set_sysclk(aif1_dai, ARIZONA_CLK_SYSCLK, 0, 0);
	if (ret < 0) {
		dev_err(aif1_dai->dev, "Failed to set SYSCLK: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	rtd = snd_soc_get_pcm_runtime(card, card->dai_link[TM2_DAI_AIF2].name);
	aif2_dai = rtd->codec_dai;
=======
	rtd = snd_soc_get_pcm_runtime(card, &card->dai_link[TM2_DAI_AIF2]);
	aif2_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13

	ret = snd_soc_dai_set_sysclk(aif2_dai, ARIZONA_CLK_ASYNCCLK, 0, 0);
	if (ret < 0) {
		dev_err(aif2_dai->dev, "Failed to set ASYNCCLK: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	dlc.of_node = tm2_speaker_amp_dev.codec_of_node;
	amp_pdm_dai = snd_soc_find_dai(&dlc);
=======
	amp_pdm_dai = snd_soc_find_dai(&tm2_speaker_amp_dev.dlc);
>>>>>>> upstream/android-13
	if (!amp_pdm_dai)
		return -ENODEV;

	/* Set the MAX98504 V/I sense PDM Tx DAI channel mapping */
	ret = snd_soc_dai_set_channel_map(amp_pdm_dai, ARRAY_SIZE(ch_map),
					  ch_map, 0, NULL);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_tdm_slot(amp_pdm_dai, 0x3, 0x0, 2, 16);
	if (ret < 0)
		return ret;

	return 0;
}

static const struct snd_kcontrol_new tm2_controls[] = {
	SOC_DAPM_PIN_SWITCH("HP"),
	SOC_DAPM_PIN_SWITCH("SPK"),
	SOC_DAPM_PIN_SWITCH("RCV"),
	SOC_DAPM_PIN_SWITCH("VPS"),
	SOC_DAPM_PIN_SWITCH("HDMI"),

	SOC_DAPM_PIN_SWITCH("Main Mic"),
	SOC_DAPM_PIN_SWITCH("Sub Mic"),
	SOC_DAPM_PIN_SWITCH("Third Mic"),

	SOC_DAPM_PIN_SWITCH("Headset Mic"),
};

static const struct snd_soc_dapm_widget tm2_dapm_widgets[] = {
	SND_SOC_DAPM_HP("HP", NULL),
	SND_SOC_DAPM_SPK("SPK", NULL),
	SND_SOC_DAPM_SPK("RCV", NULL),
	SND_SOC_DAPM_LINE("VPS", NULL),
	SND_SOC_DAPM_LINE("HDMI", NULL),

	SND_SOC_DAPM_MIC("Main Mic", tm2_mic_bias),
	SND_SOC_DAPM_MIC("Sub Mic", NULL),
	SND_SOC_DAPM_MIC("Third Mic", NULL),

	SND_SOC_DAPM_MIC("Headset Mic", NULL),
};

static const struct snd_soc_component_driver tm2_component = {
	.name	= "tm2-audio",
};

static struct snd_soc_dai_driver tm2_ext_dai[] = {
	{
		.name = "Voice call",
		.playback = {
			.channels_min = 1,
			.channels_max = 4,
			.rate_min = 8000,
			.rate_max = 48000,
			.rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 |
					SNDRV_PCM_RATE_48000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
		.capture = {
			.channels_min = 1,
			.channels_max = 4,
			.rate_min = 8000,
			.rate_max = 48000,
			.rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 |
					SNDRV_PCM_RATE_48000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
	},
	{
		.name = "Bluetooth",
		.playback = {
			.channels_min = 1,
			.channels_max = 4,
			.rate_min = 8000,
			.rate_max = 16000,
			.rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
		.capture = {
			.channels_min = 1,
			.channels_max = 2,
			.rate_min = 8000,
			.rate_max = 16000,
			.rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
	},
};

<<<<<<< HEAD
=======
SND_SOC_DAILINK_DEFS(aif1,
	DAILINK_COMP_ARRAY(COMP_CPU(SAMSUNG_I2S_DAI)),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "wm5110-aif1")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(voice,
	DAILINK_COMP_ARRAY(COMP_CPU(SAMSUNG_I2S_DAI)),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "wm5110-aif2")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(bt,
	DAILINK_COMP_ARRAY(COMP_CPU(SAMSUNG_I2S_DAI)),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "wm5110-aif3")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(hdmi,
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

>>>>>>> upstream/android-13
static struct snd_soc_dai_link tm2_dai_links[] = {
	{
		.name		= "WM5110 AIF1",
		.stream_name	= "HiFi Primary",
<<<<<<< HEAD
		.cpu_dai_name   = SAMSUNG_I2S_DAI,
		.codec_dai_name = "wm5110-aif1",
		.ops		= &tm2_aif1_ops,
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM,
	}, {
		.name		= "WM5110 Voice",
		.stream_name	= "Voice call",
		.cpu_dai_name   = SAMSUNG_I2S_DAI,
		.codec_dai_name = "wm5110-aif2",
=======
		.ops		= &tm2_aif1_ops,
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM,
		SND_SOC_DAILINK_REG(aif1),
	}, {
		.name		= "WM5110 Voice",
		.stream_name	= "Voice call",
>>>>>>> upstream/android-13
		.ops		= &tm2_aif2_ops,
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM,
		.ignore_suspend = 1,
<<<<<<< HEAD
	}, {
		.name		= "WM5110 BT",
		.stream_name	= "Bluetooth",
		.cpu_dai_name   = SAMSUNG_I2S_DAI,
		.codec_dai_name = "wm5110-aif3",
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM,
		.ignore_suspend = 1,
=======
		SND_SOC_DAILINK_REG(voice),
	}, {
		.name		= "WM5110 BT",
		.stream_name	= "Bluetooth",
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBM_CFM,
		.ignore_suspend = 1,
		SND_SOC_DAILINK_REG(bt),
>>>>>>> upstream/android-13
	}, {
		.name		= "HDMI",
		.stream_name	= "i2s1",
		.ops		= &tm2_hdmi_ops,
		.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				  SND_SOC_DAIFMT_CBS_CFS,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hdmi),
>>>>>>> upstream/android-13
	}
};

static struct snd_soc_card tm2_card = {
	.owner			= THIS_MODULE,

	.dai_link		= tm2_dai_links,
	.controls		= tm2_controls,
	.num_controls		= ARRAY_SIZE(tm2_controls),
	.dapm_widgets		= tm2_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(tm2_dapm_widgets),
	.aux_dev		= &tm2_speaker_amp_dev,
	.num_aux_devs		= 1,

	.late_probe		= tm2_late_probe,
	.set_bias_level		= tm2_set_bias_level,
};

static int tm2_probe(struct platform_device *pdev)
{
	struct device_node *cpu_dai_node[2] = {};
	struct device_node *codec_dai_node[2] = {};
	const char *cells_name = NULL;
	struct device *dev = &pdev->dev;
	struct snd_soc_card *card = &tm2_card;
	struct tm2_machine_priv *priv;
<<<<<<< HEAD
	struct of_phandle_args args;
=======
	struct snd_soc_dai_link *dai_link;
>>>>>>> upstream/android-13
	int num_codecs, ret, i;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	snd_soc_card_set_drvdata(card, priv);
	card->dev = dev;

	priv->gpio_mic_bias = devm_gpiod_get(dev, "mic-bias", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->gpio_mic_bias)) {
		dev_err(dev, "Failed to get mic bias gpio\n");
		return PTR_ERR(priv->gpio_mic_bias);
	}

	ret = snd_soc_of_parse_card_name(card, "model");
	if (ret < 0) {
		dev_err(dev, "Card name is not specified\n");
		return ret;
	}

	ret = snd_soc_of_parse_audio_routing(card, "samsung,audio-routing");
	if (ret < 0) {
		dev_err(dev, "Audio routing is not specified or invalid\n");
		return ret;
	}

<<<<<<< HEAD
	card->aux_dev[0].codec_of_node = of_parse_phandle(dev->of_node,
							"audio-amplifier", 0);
	if (!card->aux_dev[0].codec_of_node) {
=======
	card->aux_dev[0].dlc.of_node = of_parse_phandle(dev->of_node,
							"audio-amplifier", 0);
	if (!card->aux_dev[0].dlc.of_node) {
>>>>>>> upstream/android-13
		dev_err(dev, "audio-amplifier property invalid or missing\n");
		return -EINVAL;
	}

	num_codecs = of_count_phandle_with_args(dev->of_node, "audio-codec",
						 NULL);

	/* Skip the HDMI link if not specified in DT */
	if (num_codecs > 1) {
		card->num_links = ARRAY_SIZE(tm2_dai_links);
		cells_name = "#sound-dai-cells";
	} else {
		card->num_links = ARRAY_SIZE(tm2_dai_links) - 1;
	}

	for (i = 0; i < num_codecs; i++) {
		struct of_phandle_args args;

		ret = of_parse_phandle_with_args(dev->of_node, "i2s-controller",
						 cells_name, i, &args);
		if (ret) {
			dev_err(dev, "i2s-controller property parse error: %d\n", i);
			ret = -EINVAL;
			goto dai_node_put;
		}
		cpu_dai_node[i] = args.np;

		codec_dai_node[i] = of_parse_phandle(dev->of_node,
						     "audio-codec", i);
		if (!codec_dai_node[i]) {
			dev_err(dev, "audio-codec property parse error\n");
			ret = -EINVAL;
			goto dai_node_put;
		}
	}

	/* Initialize WM5110 - I2S and HDMI - I2S1 DAI links */
<<<<<<< HEAD
	for (i = 0; i < card->num_links; i++) {
		unsigned int dai_index = 0; /* WM5110 */

		card->dai_link[i].cpu_name = NULL;
		card->dai_link[i].platform_name = NULL;
=======
	for_each_card_prelinks(card, i, dai_link) {
		unsigned int dai_index = 0; /* WM5110 */

		dai_link->cpus->name = NULL;
		dai_link->platforms->name = NULL;
>>>>>>> upstream/android-13

		if (num_codecs > 1 && i == card->num_links - 1)
			dai_index = 1; /* HDMI */

<<<<<<< HEAD
		card->dai_link[i].codec_of_node = codec_dai_node[dai_index];
		card->dai_link[i].cpu_of_node = cpu_dai_node[dai_index];
		card->dai_link[i].platform_of_node = cpu_dai_node[dai_index];
	}

	if (num_codecs > 1) {
=======
		dai_link->codecs->of_node = codec_dai_node[dai_index];
		dai_link->cpus->of_node = cpu_dai_node[dai_index];
		dai_link->platforms->of_node = cpu_dai_node[dai_index];
	}

	if (num_codecs > 1) {
		struct of_phandle_args args;

>>>>>>> upstream/android-13
		/* HDMI DAI link (I2S1) */
		i = card->num_links - 1;

		ret = of_parse_phandle_with_fixed_args(dev->of_node,
						"audio-codec", 0, 1, &args);
		if (ret) {
			dev_err(dev, "audio-codec property parse error\n");
			goto dai_node_put;
		}

<<<<<<< HEAD
		ret = snd_soc_get_dai_name(&args, &card->dai_link[i].codec_dai_name);
=======
		ret = snd_soc_get_dai_name(&args, &card->dai_link[i].codecs->dai_name);
>>>>>>> upstream/android-13
		if (ret) {
			dev_err(dev, "Unable to get codec_dai_name\n");
			goto dai_node_put;
		}
	}

	ret = devm_snd_soc_register_component(dev, &tm2_component,
				tm2_ext_dai, ARRAY_SIZE(tm2_ext_dai));
	if (ret < 0) {
		dev_err(dev, "Failed to register component: %d\n", ret);
		goto dai_node_put;
	}

	ret = devm_snd_soc_register_card(dev, card);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(dev, "Failed to register card: %d\n", ret);
=======
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Failed to register card: %d\n", ret);
>>>>>>> upstream/android-13
		goto dai_node_put;
	}

dai_node_put:
	for (i = 0; i < num_codecs; i++) {
		of_node_put(codec_dai_node[i]);
		of_node_put(cpu_dai_node[i]);
	}

<<<<<<< HEAD
	of_node_put(card->aux_dev[0].codec_of_node);
=======
	of_node_put(card->aux_dev[0].dlc.of_node);
>>>>>>> upstream/android-13

	return ret;
}

static int tm2_pm_prepare(struct device *dev)
{
	struct snd_soc_card *card = dev_get_drvdata(dev);

	return tm2_stop_sysclk(card);
}

static void tm2_pm_complete(struct device *dev)
{
	struct snd_soc_card *card = dev_get_drvdata(dev);

	tm2_start_sysclk(card);
}

static const struct dev_pm_ops tm2_pm_ops = {
	.prepare	= tm2_pm_prepare,
	.suspend	= snd_soc_suspend,
	.resume		= snd_soc_resume,
	.complete	= tm2_pm_complete,
	.freeze		= snd_soc_suspend,
	.thaw		= snd_soc_resume,
	.poweroff	= snd_soc_poweroff,
	.restore	= snd_soc_resume,
};

static const struct of_device_id tm2_of_match[] = {
	{ .compatible = "samsung,tm2-audio" },
	{ },
};
MODULE_DEVICE_TABLE(of, tm2_of_match);

static struct platform_driver tm2_driver = {
	.driver = {
		.name		= "tm2-audio",
		.pm		= &tm2_pm_ops,
		.of_match_table	= tm2_of_match,
	},
	.probe	= tm2_probe,
};
module_platform_driver(tm2_driver);

MODULE_AUTHOR("Inha Song <ideal.song@samsung.com>");
MODULE_DESCRIPTION("ALSA SoC Exynos TM2 Audio Support");
MODULE_LICENSE("GPL v2");
