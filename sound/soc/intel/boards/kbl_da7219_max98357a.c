<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
// Copyright(c) 2017-18 Intel Corporation.

/*
 * Intel Kabylake I2S Machine Driver with MAX98357A & DA7219 Codecs
 *
 * Modified from:
 *   Intel Kabylake I2S Machine driver supporting MAXIM98927 and
 *   RT5663 codecs
 */

#include <linux/input.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/jack.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include "../../codecs/da7219.h"
#include "../../codecs/hdac_hdmi.h"
<<<<<<< HEAD
#include "../skylake/skl.h"
=======
>>>>>>> upstream/android-13
#include "../../codecs/da7219-aad.h"

#define KBL_DIALOG_CODEC_DAI "da7219-hifi"
#define KBL_MAXIM_CODEC_DAI "HiFi"
#define MAXIM_DEV0_NAME "MX98357A:00"
#define DUAL_CHANNEL 2
#define QUAD_CHANNEL 4

static struct snd_soc_card *kabylake_audio_card;
static struct snd_soc_jack skylake_hdmi[3];

struct kbl_hdmi_pcm {
	struct list_head head;
	struct snd_soc_dai *codec_dai;
	int device;
};

struct kbl_codec_private {
	struct snd_soc_jack kabylake_headset;
	struct list_head hdmi_pcm_list;
};

enum {
	KBL_DPCM_AUDIO_PB = 0,
	KBL_DPCM_AUDIO_CP,
<<<<<<< HEAD
=======
	KBL_DPCM_AUDIO_REF_CP,
>>>>>>> upstream/android-13
	KBL_DPCM_AUDIO_DMIC_CP,
	KBL_DPCM_AUDIO_HDMI1_PB,
	KBL_DPCM_AUDIO_HDMI2_PB,
	KBL_DPCM_AUDIO_HDMI3_PB,
};

static int platform_clock_control(struct snd_soc_dapm_widget *w,
					struct snd_kcontrol *k, int  event)
{
	struct snd_soc_dapm_context *dapm = w->dapm;
	struct snd_soc_card *card = dapm->card;
	struct snd_soc_dai *codec_dai;
	int ret = 0;

	codec_dai = snd_soc_card_get_codec_dai(card, KBL_DIALOG_CODEC_DAI);
	if (!codec_dai) {
		dev_err(card->dev, "Codec dai not found; Unable to set/unset codec pll\n");
		return -EIO;
	}

	if (SND_SOC_DAPM_EVENT_OFF(event)) {
		ret = snd_soc_dai_set_pll(codec_dai, 0,
				     DA7219_SYSCLK_MCLK, 0, 0);
		if (ret)
			dev_err(card->dev, "failed to stop PLL: %d\n", ret);
	} else if (SND_SOC_DAPM_EVENT_ON(event)) {
		ret = snd_soc_dai_set_pll(codec_dai, 0,	DA7219_SYSCLK_PLL_SRM,
				     0, DA7219_PLL_FREQ_OUT_98304);
		if (ret)
			dev_err(card->dev, "failed to start PLL: %d\n", ret);
	}

	return ret;
}

static const struct snd_kcontrol_new kabylake_controls[] = {
	SOC_DAPM_PIN_SWITCH("Headphone Jack"),
	SOC_DAPM_PIN_SWITCH("Headset Mic"),
	SOC_DAPM_PIN_SWITCH("Spk"),
};

static const struct snd_soc_dapm_widget kabylake_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_MIC("Headset Mic", NULL),
	SND_SOC_DAPM_SPK("Spk", NULL),
	SND_SOC_DAPM_MIC("SoC DMIC", NULL),
<<<<<<< HEAD
	SND_SOC_DAPM_SPK("DP", NULL),
	SND_SOC_DAPM_SPK("HDMI", NULL),
=======
	SND_SOC_DAPM_SPK("HDMI1", NULL),
	SND_SOC_DAPM_SPK("HDMI2", NULL),
	SND_SOC_DAPM_SPK("HDMI3", NULL),
>>>>>>> upstream/android-13
	SND_SOC_DAPM_SUPPLY("Platform Clock", SND_SOC_NOPM, 0, 0,
			platform_clock_control, SND_SOC_DAPM_PRE_PMU |
			SND_SOC_DAPM_POST_PMD),
};

static const struct snd_soc_dapm_route kabylake_map[] = {
	{ "Headphone Jack", NULL, "HPL" },
	{ "Headphone Jack", NULL, "HPR" },

	/* speaker */
	{ "Spk", NULL, "Speaker" },

	/* other jacks */
	{ "MIC", NULL, "Headset Mic" },
	{ "DMic", NULL, "SoC DMIC" },

<<<<<<< HEAD
	{ "HDMI", NULL, "hif5 Output" },
	{ "DP", NULL, "hif6 Output" },
=======
	{"HDMI1", NULL, "hif5-0 Output"},
	{"HDMI2", NULL, "hif6-0 Output"},
	{"HDMI3", NULL, "hif7-0 Output"},
>>>>>>> upstream/android-13

	/* CODEC BE connections */
	{ "HiFi Playback", NULL, "ssp0 Tx" },
	{ "ssp0 Tx", NULL, "codec0_out" },

	{ "Playback", NULL, "ssp1 Tx" },
	{ "ssp1 Tx", NULL, "codec1_out" },

	{ "codec0_in", NULL, "ssp1 Rx" },
	{ "ssp1 Rx", NULL, "Capture" },

	/* DMIC */
	{ "dmic01_hifi", NULL, "DMIC01 Rx" },
	{ "DMIC01 Rx", NULL, "DMIC AIF" },

	{ "hifi1", NULL, "iDisp1 Tx" },
	{ "iDisp1 Tx", NULL, "iDisp1_out" },
	{ "hifi2", NULL, "iDisp2 Tx" },
	{ "iDisp2 Tx", NULL, "iDisp2_out" },
	{ "hifi3", NULL, "iDisp3 Tx"},
	{ "iDisp3 Tx", NULL, "iDisp3_out"},

	{ "Headphone Jack", NULL, "Platform Clock" },
	{ "Headset Mic", NULL, "Platform Clock" },
};

static int kabylake_ssp_fixup(struct snd_soc_pcm_runtime *rtd,
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

	/* The ADSP will convert the FE rate to 48k, stereo */
	rate->min = rate->max = 48000;
<<<<<<< HEAD
	channels->min = channels->max = DUAL_CHANNEL;
=======
	chan->min = chan->max = DUAL_CHANNEL;
>>>>>>> upstream/android-13

	/* set SSP to 24 bit */
	snd_mask_none(fmt);
	snd_mask_set_format(fmt, SNDRV_PCM_FORMAT_S24_LE);

	return 0;
}

static int kabylake_da7219_codec_init(struct snd_soc_pcm_runtime *rtd)
{
	struct kbl_codec_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_component *component = rtd->codec_dai->component;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
=======
	struct snd_soc_component *component = asoc_rtd_to_codec(rtd, 0)->component;
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct snd_soc_jack *jack;
	int ret;

	/* Configure sysclk for codec */
	ret = snd_soc_dai_set_sysclk(codec_dai, DA7219_CLKSRC_MCLK, 24576000,
						SND_SOC_CLOCK_IN);
	if (ret) {
		dev_err(rtd->dev, "can't set codec sysclk configuration\n");
		return ret;
	}

	/*
	 * Headset buttons map to the google Reference headset.
	 * These can be configured by userspace.
	 */
	ret = snd_soc_card_jack_new(kabylake_audio_card, "Headset Jack",
			SND_JACK_HEADSET | SND_JACK_BTN_0 | SND_JACK_BTN_1 |
			SND_JACK_BTN_2 | SND_JACK_BTN_3 | SND_JACK_LINEOUT,
			&ctx->kabylake_headset, NULL, 0);
	if (ret) {
		dev_err(rtd->dev, "Headset Jack creation failed: %d\n", ret);
		return ret;
	}

	jack = &ctx->kabylake_headset;

	snd_jack_set_key(jack->jack, SND_JACK_BTN_0, KEY_PLAYPAUSE);
	snd_jack_set_key(jack->jack, SND_JACK_BTN_1, KEY_VOLUMEUP);
	snd_jack_set_key(jack->jack, SND_JACK_BTN_2, KEY_VOLUMEDOWN);
	snd_jack_set_key(jack->jack, SND_JACK_BTN_3, KEY_VOICECOMMAND);
	da7219_aad_jack_det(component, &ctx->kabylake_headset);

	ret = snd_soc_dapm_ignore_suspend(&rtd->card->dapm, "SoC DMIC");
	if (ret)
		dev_err(rtd->dev, "SoC DMIC - Ignore suspend failed %d\n", ret);

	return ret;
}

static int kabylake_hdmi_init(struct snd_soc_pcm_runtime *rtd, int device)
{
	struct kbl_codec_private *ctx = snd_soc_card_get_drvdata(rtd->card);
<<<<<<< HEAD
	struct snd_soc_dai *dai = rtd->codec_dai;
=======
	struct snd_soc_dai *dai = asoc_rtd_to_codec(rtd, 0);
>>>>>>> upstream/android-13
	struct kbl_hdmi_pcm *pcm;

	pcm = devm_kzalloc(rtd->card->dev, sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	pcm->device = device;
	pcm->codec_dai = dai;

	list_add_tail(&pcm->head, &ctx->hdmi_pcm_list);

	return 0;
}

static int kabylake_hdmi1_init(struct snd_soc_pcm_runtime *rtd)
{
	return kabylake_hdmi_init(rtd, KBL_DPCM_AUDIO_HDMI1_PB);
}

static int kabylake_hdmi2_init(struct snd_soc_pcm_runtime *rtd)
{
	return kabylake_hdmi_init(rtd, KBL_DPCM_AUDIO_HDMI2_PB);
}

static int kabylake_hdmi3_init(struct snd_soc_pcm_runtime *rtd)
{
	return kabylake_hdmi_init(rtd, KBL_DPCM_AUDIO_HDMI3_PB);
}

static int kabylake_da7219_fe_init(struct snd_soc_pcm_runtime *rtd)
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
	DUAL_CHANNEL,
};

static const struct snd_pcm_hw_constraint_list constraints_channels = {
	.count = ARRAY_SIZE(channels),
	.list = channels,
	.mask = 0,
};

static unsigned int channels_quad[] = {
	QUAD_CHANNEL,
};

static struct snd_pcm_hw_constraint_list constraints_channels_quad = {
	.count = ARRAY_SIZE(channels_quad),
	.list = channels_quad,
	.mask = 0,
};

static int kbl_fe_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	/*
	 * On this platform for PCM device we support,
	 * 48Khz
	 * stereo
	 * 16 bit audio
	 */

	runtime->hw.channels_max = DUAL_CHANNEL;
	snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_CHANNELS,
					   &constraints_channels);

	runtime->hw.formats = SNDRV_PCM_FMTBIT_S16_LE;
	snd_pcm_hw_constraint_msbits(runtime, 0, 16, 16);

	snd_pcm_hw_constraint_list(runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE, &constraints_rates);

	return 0;
}

static const struct snd_soc_ops kabylake_da7219_fe_ops = {
	.startup = kbl_fe_startup,
};

static int kabylake_dmic_fixup(struct snd_soc_pcm_runtime *rtd,
		struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_interval *channels = hw_param_interval(params,
=======
	struct snd_interval *chan = hw_param_interval(params,
>>>>>>> upstream/android-13
				SNDRV_PCM_HW_PARAM_CHANNELS);

	/*
	 * set BE channel constraint as user FE channels
	 */

	if (params_channels(params) == 2)
<<<<<<< HEAD
		channels->min = channels->max = 2;
	else
		channels->min = channels->max = 4;
=======
		chan->min = chan->max = 2;
	else
		chan->min = chan->max = 4;
>>>>>>> upstream/android-13

	return 0;
}

static int kabylake_dmic_startup(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	runtime->hw.channels_min = runtime->hw.channels_max = QUAD_CHANNEL;
	snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_CHANNELS,
			&constraints_channels_quad);

	return snd_pcm_hw_constraint_list(substream->runtime, 0,
			SNDRV_PCM_HW_PARAM_RATE, &constraints_rates);
}

static struct snd_soc_ops kabylake_dmic_ops = {
	.startup = kabylake_dmic_startup,
};

<<<<<<< HEAD
static const unsigned int rates_16000[] = {
	16000,
};

static const struct snd_pcm_hw_constraint_list constraints_16000 = {
	.count = ARRAY_SIZE(rates_16000),
	.list  = rates_16000,
=======
static unsigned int rates_16000[] = {
        16000,
};

static const struct snd_pcm_hw_constraint_list constraints_16000 = {
        .count = ARRAY_SIZE(rates_16000),
        .list  = rates_16000,
>>>>>>> upstream/android-13
};

static const unsigned int ch_mono[] = {
	1,
};

<<<<<<< HEAD
=======
static const struct snd_pcm_hw_constraint_list constraints_refcap = {
	.count = ARRAY_SIZE(ch_mono),
	.list  = ch_mono,
};

static int kabylake_refcap_startup(struct snd_pcm_substream *substream)
{
	substream->runtime->hw.channels_max = 1;
	snd_pcm_hw_constraint_list(substream->runtime, 0,
					SNDRV_PCM_HW_PARAM_CHANNELS,
					&constraints_refcap);

	return snd_pcm_hw_constraint_list(substream->runtime, 0,
					SNDRV_PCM_HW_PARAM_RATE,
					&constraints_16000);
}

static struct snd_soc_ops skylake_refcap_ops = {
	.startup = kabylake_refcap_startup,
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
	DAILINK_COMP_ARRAY(COMP_CODEC(MAXIM_DEV0_NAME,
				      KBL_MAXIM_CODEC_DAI)));

SND_SOC_DAILINK_DEF(ssp1_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("SSP1 Pin")));
SND_SOC_DAILINK_DEF(ssp1_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("i2c-DLGS7219:00",
				      KBL_DIALOG_CODEC_DAI)));

SND_SOC_DAILINK_DEF(dmic_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("DMIC01 Pin")));
SND_SOC_DAILINK_DEF(dmic_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("dmic-codec", "dmic-hifi")));

SND_SOC_DAILINK_DEF(idisp1_pin,
	DAILINK_COMP_ARRAY(COMP_CPU("iDisp1 Pin")));
SND_SOC_DAILINK_DEF(idisp1_codec,
	DAILINK_COMP_ARRAY(COMP_CODEC("ehdaudio0D2",
				      "intel-hdmi-hifi1")));

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
/* kabylake digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link kabylake_dais[] = {
	/* Front End DAI links */
	[KBL_DPCM_AUDIO_PB] = {
		.name = "Kbl Audio Port",
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
		.init = kabylake_da7219_fe_init,
		.trigger = {
			SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.dpcm_playback = 1,
		.ops = &kabylake_da7219_fe_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(system, dummy, platform),
>>>>>>> upstream/android-13
	},
	[KBL_DPCM_AUDIO_CP] = {
		.name = "Kbl Audio Capture Port",
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
		.ops = &kabylake_da7219_fe_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(system, dummy, platform),
	},
	[KBL_DPCM_AUDIO_REF_CP] = {
		.name = "Kbl Audio Reference cap",
		.stream_name = "Wake on Voice",
		.init = NULL,
		.dpcm_capture = 1,
		.nonatomic = 1,
		.dynamic = 1,
		.ops = &skylake_refcap_ops,
		SND_SOC_DAILINK_REG(reference, dummy, platform),
>>>>>>> upstream/android-13
	},
	[KBL_DPCM_AUDIO_DMIC_CP] = {
		.name = "Kbl Audio DMIC cap",
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
		.ops = &kabylake_dmic_ops,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic, dummy, platform),
>>>>>>> upstream/android-13
	},
	[KBL_DPCM_AUDIO_HDMI1_PB] = {
		.name = "Kbl HDMI Port1",
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
	[KBL_DPCM_AUDIO_HDMI2_PB] = {
		.name = "Kbl HDMI Port2",
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
	[KBL_DPCM_AUDIO_HDMI3_PB] = {
		.name = "Kbl HDMI Port3",
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
		.codec_name = MAXIM_DEV0_NAME,
		.codec_dai_name = KBL_MAXIM_CODEC_DAI,
=======
		.no_pcm = 1,
>>>>>>> upstream/android-13
		.dai_fmt = SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = kabylake_ssp_fixup,
		.dpcm_playback = 1,
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
		.codec_name = "i2c-DLGS7219:00",
		.codec_dai_name = KBL_DIALOG_CODEC_DAI,
=======
		.no_pcm = 1,
>>>>>>> upstream/android-13
		.init = kabylake_da7219_codec_init,
		.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.ignore_pmdown_time = 1,
		.be_hw_params_fixup = kabylake_ssp_fixup,
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
		.be_hw_params_fixup = kabylake_dmic_fixup,
		.ignore_suspend = 1,
		.dpcm_capture = 1,
		.no_pcm = 1,
<<<<<<< HEAD
=======
		SND_SOC_DAILINK_REG(dmic_pin, dmic_codec, platform),
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
		.init = kabylake_hdmi1_init,
		.no_pcm = 1,
=======
		.dpcm_playback = 1,
		.init = kabylake_hdmi1_init,
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
		.init = kabylake_hdmi2_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = kabylake_hdmi2_init,
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
		.init = kabylake_hdmi3_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
=======
		.init = kabylake_hdmi3_init,
		.dpcm_playback = 1,
		.no_pcm = 1,
		SND_SOC_DAILINK_REG(idisp3_pin, idisp3_codec, platform),
>>>>>>> upstream/android-13
	},
};

#define NAME_SIZE	32
static int kabylake_card_late_probe(struct snd_soc_card *card)
{
	struct kbl_codec_private *ctx = snd_soc_card_get_drvdata(card);
	struct kbl_hdmi_pcm *pcm;
	struct snd_soc_component *component = NULL;
	int err, i = 0;
	char jack_name[NAME_SIZE];

	list_for_each_entry(pcm, &ctx->hdmi_pcm_list, head) {
		component = pcm->codec_dai->component;
		snprintf(jack_name, sizeof(jack_name),
			"HDMI/DP, pcm=%d Jack", pcm->device);
		err = snd_soc_card_jack_new(card, jack_name,
					SND_JACK_AVOUT, &skylake_hdmi[i],
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

/* kabylake audio machine driver for SPT + DA7219 */
static struct snd_soc_card kabylake_audio_card_da7219_m98357a = {
	.name = "kblda7219max",
	.owner = THIS_MODULE,
	.dai_link = kabylake_dais,
	.num_links = ARRAY_SIZE(kabylake_dais),
	.controls = kabylake_controls,
	.num_controls = ARRAY_SIZE(kabylake_controls),
	.dapm_widgets = kabylake_widgets,
	.num_dapm_widgets = ARRAY_SIZE(kabylake_widgets),
	.dapm_routes = kabylake_map,
	.num_dapm_routes = ARRAY_SIZE(kabylake_map),
	.fully_routed = true,
	.late_probe = kabylake_card_late_probe,
};

static int kabylake_audio_probe(struct platform_device *pdev)
{
	struct kbl_codec_private *ctx;

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	INIT_LIST_HEAD(&ctx->hdmi_pcm_list);

	kabylake_audio_card =
		(struct snd_soc_card *)pdev->id_entry->driver_data;

	kabylake_audio_card->dev = &pdev->dev;
	snd_soc_card_set_drvdata(kabylake_audio_card, ctx);
	return devm_snd_soc_register_card(&pdev->dev, kabylake_audio_card);
}

static const struct platform_device_id kbl_board_ids[] = {
	{
<<<<<<< HEAD
		.name = "kbl_da7219_max98357a",
=======
		.name = "kbl_da7219_mx98357a",
>>>>>>> upstream/android-13
		.driver_data =
			(kernel_ulong_t)&kabylake_audio_card_da7219_m98357a,
	},
	{ }
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(platform, kbl_board_ids);
>>>>>>> upstream/android-13

static struct platform_driver kabylake_audio = {
	.probe = kabylake_audio_probe,
	.driver = {
		.name = "kbl_da7219_max98357a",
		.pm = &snd_soc_pm_ops,
	},
	.id_table = kbl_board_ids,
};

module_platform_driver(kabylake_audio)

/* Module information */
MODULE_DESCRIPTION("Audio Machine driver-DA7219 & MAX98357A in I2S mode");
MODULE_AUTHOR("Naveen Manohar <naveen.m@intel.com>");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS("platform:kbl_da7219_max98357a");
=======
>>>>>>> upstream/android-13
