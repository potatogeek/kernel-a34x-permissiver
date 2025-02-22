// SPDX-License-Identifier: GPL-2.0
//
// mt6797-mt6351.c  --  MT6797 MT6351 ALSA SoC machine driver
//
// Copyright (c) 2018 MediaTek Inc.
// Author: KaiChieh Chuang <kaichieh.chuang@mediatek.com>

#include <linux/module.h>
#include <sound/soc.h>

#include "mt6797-afe-common.h"

<<<<<<< HEAD
=======
SND_SOC_DAILINK_DEFS(playback_1,
	DAILINK_COMP_ARRAY(COMP_CPU("DL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(playback_2,
	DAILINK_COMP_ARRAY(COMP_CPU("DL2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(playback_3,
	DAILINK_COMP_ARRAY(COMP_CPU("DL3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(capture_1,
	DAILINK_COMP_ARRAY(COMP_CPU("UL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(capture_2,
	DAILINK_COMP_ARRAY(COMP_CPU("UL2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(capture_3,
	DAILINK_COMP_ARRAY(COMP_CPU("UL3")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(capture_mono_1,
	DAILINK_COMP_ARRAY(COMP_CPU("UL_MONO_1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(hostless_lpbk,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless LPBK DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(hostless_speech,
	DAILINK_COMP_ARRAY(COMP_CPU("Hostless Speech DAI")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(primary_codec,
	DAILINK_COMP_ARRAY(COMP_CPU("ADDA")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "mt6351-snd-codec-aif1")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(pcm1,
	DAILINK_COMP_ARRAY(COMP_CPU("PCM 1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(pcm2,
	DAILINK_COMP_ARRAY(COMP_CPU("PCM 2")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

>>>>>>> upstream/android-13
static struct snd_soc_dai_link mt6797_mt6351_dai_links[] = {
	/* FE */
	{
		.name = "Playback_1",
		.stream_name = "Playback_1",
<<<<<<< HEAD
		.cpu_dai_name = "DL1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(playback_1),
>>>>>>> upstream/android-13
	},
	{
		.name = "Playback_2",
		.stream_name = "Playback_2",
<<<<<<< HEAD
		.cpu_dai_name = "DL2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(playback_2),
>>>>>>> upstream/android-13
	},
	{
		.name = "Playback_3",
		.stream_name = "Playback_3",
<<<<<<< HEAD
		.cpu_dai_name = "DL3",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(playback_3),
>>>>>>> upstream/android-13
	},
	{
		.name = "Capture_1",
		.stream_name = "Capture_1",
<<<<<<< HEAD
		.cpu_dai_name = "UL1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(capture_1),
>>>>>>> upstream/android-13
	},
	{
		.name = "Capture_2",
		.stream_name = "Capture_2",
<<<<<<< HEAD
		.cpu_dai_name = "UL2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(capture_2),
>>>>>>> upstream/android-13
	},
	{
		.name = "Capture_3",
		.stream_name = "Capture_3",
<<<<<<< HEAD
		.cpu_dai_name = "UL3",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(capture_3),
>>>>>>> upstream/android-13
	},
	{
		.name = "Capture_Mono_1",
		.stream_name = "Capture_Mono_1",
<<<<<<< HEAD
		.cpu_dai_name = "UL_MONO_1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_capture = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(capture_mono_1),
>>>>>>> upstream/android-13
	},
	{
		.name = "Hostless_LPBK",
		.stream_name = "Hostless_LPBK",
<<<<<<< HEAD
		.cpu_dai_name = "Hostless LPBK DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hostless_lpbk),
>>>>>>> upstream/android-13
	},
	{
		.name = "Hostless_Speech",
		.stream_name = "Hostless_Speech",
<<<<<<< HEAD
		.cpu_dai_name = "Hostless Speech DAI",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
>>>>>>> upstream/android-13
		.trigger = {SND_SOC_DPCM_TRIGGER_PRE,
			    SND_SOC_DPCM_TRIGGER_PRE},
		.dynamic = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(hostless_speech),
>>>>>>> upstream/android-13
	},
	/* BE */
	{
		.name = "Primary Codec",
<<<<<<< HEAD
		.cpu_dai_name = "ADDA",
		.codec_dai_name = "mt6351-snd-codec-aif1",
=======
>>>>>>> upstream/android-13
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
<<<<<<< HEAD
	},
	{
		.name = "PCM 1",
		.cpu_dai_name = "PCM 1",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
		SND_SOC_DAILINK_REG(primary_codec),
	},
	{
		.name = "PCM 1",
>>>>>>> upstream/android-13
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
<<<<<<< HEAD
	},
	{
		.name = "PCM 2",
		.cpu_dai_name = "PCM 2",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
=======
		SND_SOC_DAILINK_REG(pcm1),
	},
	{
		.name = "PCM 2",
>>>>>>> upstream/android-13
		.no_pcm = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ignore_suspend = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(pcm2),
>>>>>>> upstream/android-13
	},
};

static struct snd_soc_card mt6797_mt6351_card = {
	.name = "mt6797-mt6351",
	.owner = THIS_MODULE,
	.dai_link = mt6797_mt6351_dai_links,
	.num_links = ARRAY_SIZE(mt6797_mt6351_dai_links),
};

static int mt6797_mt6351_dev_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &mt6797_mt6351_card;
	struct device_node *platform_node, *codec_node;
<<<<<<< HEAD
=======
	struct snd_soc_dai_link *dai_link;
>>>>>>> upstream/android-13
	int ret, i;

	card->dev = &pdev->dev;

	platform_node = of_parse_phandle(pdev->dev.of_node,
					 "mediatek,platform", 0);
	if (!platform_node) {
		dev_err(&pdev->dev, "Property 'platform' missing or invalid\n");
		return -EINVAL;
	}
<<<<<<< HEAD
	for (i = 0; i < card->num_links; i++) {
		if (mt6797_mt6351_dai_links[i].platform_name)
			continue;
		mt6797_mt6351_dai_links[i].platform_of_node = platform_node;
=======
	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->platforms->name)
			continue;
		dai_link->platforms->of_node = platform_node;
>>>>>>> upstream/android-13
	}

	codec_node = of_parse_phandle(pdev->dev.of_node,
				      "mediatek,audio-codec", 0);
	if (!codec_node) {
		dev_err(&pdev->dev,
			"Property 'audio-codec' missing or invalid\n");
		return -EINVAL;
	}
<<<<<<< HEAD
	for (i = 0; i < card->num_links; i++) {
		if (mt6797_mt6351_dai_links[i].codec_name)
			continue;
		mt6797_mt6351_dai_links[i].codec_of_node = codec_node;
=======
	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->codecs->name)
			continue;
		dai_link->codecs->of_node = codec_node;
>>>>>>> upstream/android-13
	}

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret)
		dev_err(&pdev->dev, "%s snd_soc_register_card fail %d\n",
			__func__, ret);

	return ret;
}

#ifdef CONFIG_OF
static const struct of_device_id mt6797_mt6351_dt_match[] = {
	{.compatible = "mediatek,mt6797-mt6351-sound",},
	{}
};
#endif

static struct platform_driver mt6797_mt6351_driver = {
	.driver = {
		.name = "mt6797-mt6351",
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_OF
		.of_match_table = mt6797_mt6351_dt_match,
#endif
	},
	.probe = mt6797_mt6351_dev_probe,
};

module_platform_driver(mt6797_mt6351_driver);

/* Module information */
MODULE_DESCRIPTION("MT6797 MT6351 ALSA SoC machine driver");
MODULE_AUTHOR("KaiChieh Chuang <kaichieh.chuang@mediatek.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("mt6797 mt6351 soc card");

