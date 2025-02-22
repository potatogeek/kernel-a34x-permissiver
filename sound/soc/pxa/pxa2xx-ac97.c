<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * linux/sound/pxa2xx-ac97.c -- AC97 support for the Intel PXA2xx chip.
 *
 * Author:	Nicolas Pitre
 * Created:	Dec 02, 2004
 * Copyright:	MontaVista Software Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dmaengine.h>
#include <linux/dma/pxa-dma.h>

<<<<<<< HEAD
=======
#include <sound/ac97/controller.h>
>>>>>>> upstream/android-13
#include <sound/core.h>
#include <sound/ac97_codec.h>
#include <sound/soc.h>
#include <sound/pxa2xx-lib.h>
#include <sound/dmaengine_pcm.h>

#include <mach/hardware.h>
#include <mach/regs-ac97.h>
#include <mach/audio.h>

<<<<<<< HEAD
static void pxa2xx_ac97_warm_reset(struct snd_ac97 *ac97)
=======
static void pxa2xx_ac97_warm_reset(struct ac97_controller *adrv)
>>>>>>> upstream/android-13
{
	pxa2xx_ac97_try_warm_reset();

	pxa2xx_ac97_finish_reset();
}

<<<<<<< HEAD
static void pxa2xx_ac97_cold_reset(struct snd_ac97 *ac97)
=======
static void pxa2xx_ac97_cold_reset(struct ac97_controller *adrv)
>>>>>>> upstream/android-13
{
	pxa2xx_ac97_try_cold_reset();

	pxa2xx_ac97_finish_reset();
}

<<<<<<< HEAD
static unsigned short pxa2xx_ac97_legacy_read(struct snd_ac97 *ac97,
					      unsigned short reg)
{
	int ret;

	ret = pxa2xx_ac97_read(ac97->num, reg);
	if (ret < 0)
		return 0;
	else
		return (unsigned short)(ret & 0xffff);
}

static void pxa2xx_ac97_legacy_write(struct snd_ac97 *ac97,
				     unsigned short reg, unsigned short val)
{
	int ret;

	ret = pxa2xx_ac97_write(ac97->num, reg, val);
}

static struct snd_ac97_bus_ops pxa2xx_ac97_ops = {
	.read	= pxa2xx_ac97_legacy_read,
	.write	= pxa2xx_ac97_legacy_write,
=======
static int pxa2xx_ac97_read_actrl(struct ac97_controller *adrv, int slot,
				  unsigned short reg)
{
	return pxa2xx_ac97_read(slot, reg);
}

static int pxa2xx_ac97_write_actrl(struct ac97_controller *adrv, int slot,
				   unsigned short reg, unsigned short val)
{
	return pxa2xx_ac97_write(slot, reg, val);
}

static struct ac97_controller_ops pxa2xx_ac97_ops = {
	.read	= pxa2xx_ac97_read_actrl,
	.write	= pxa2xx_ac97_write_actrl,
>>>>>>> upstream/android-13
	.warm_reset	= pxa2xx_ac97_warm_reset,
	.reset	= pxa2xx_ac97_cold_reset,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_stereo_in = {
	.addr		= __PREG(PCDR),
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "pcm_pcm_stereo_in",
	.maxburst	= 32,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_stereo_out = {
	.addr		= __PREG(PCDR),
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "pcm_pcm_stereo_out",
	.maxburst	= 32,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_aux_mono_out = {
	.addr		= __PREG(MODR),
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mono_out",
	.maxburst	= 16,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_aux_mono_in = {
	.addr		= __PREG(MODR),
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mono_in",
	.maxburst	= 16,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_mic_mono_in = {
	.addr		= __PREG(MCDR),
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mic_mono",
	.maxburst	= 16,
};

static int pxa2xx_ac97_hifi_startup(struct snd_pcm_substream *substream,
				    struct snd_soc_dai *cpu_dai)
{
	struct snd_dmaengine_dai_dma_data *dma_data;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &pxa2xx_ac97_pcm_stereo_out;
	else
		dma_data = &pxa2xx_ac97_pcm_stereo_in;

	snd_soc_dai_set_dma_data(cpu_dai, substream, dma_data);

	return 0;
}

static int pxa2xx_ac97_aux_startup(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *cpu_dai)
{
	struct snd_dmaengine_dai_dma_data *dma_data;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &pxa2xx_ac97_pcm_aux_mono_out;
	else
		dma_data = &pxa2xx_ac97_pcm_aux_mono_in;

	snd_soc_dai_set_dma_data(cpu_dai, substream, dma_data);

	return 0;
}

static int pxa2xx_ac97_mic_startup(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *cpu_dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return -ENODEV;
	snd_soc_dai_set_dma_data(cpu_dai, substream,
				 &pxa2xx_ac97_pcm_mic_mono_in);

	return 0;
}

#define PXA2XX_AC97_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 | \
		SNDRV_PCM_RATE_48000)

static const struct snd_soc_dai_ops pxa_ac97_hifi_dai_ops = {
	.startup	= pxa2xx_ac97_hifi_startup,
};

static const struct snd_soc_dai_ops pxa_ac97_aux_dai_ops = {
	.startup	= pxa2xx_ac97_aux_startup,
};

static const struct snd_soc_dai_ops pxa_ac97_mic_dai_ops = {
	.startup	= pxa2xx_ac97_mic_startup,
};

/*
 * There is only 1 physical AC97 interface for pxa2xx, but it
 * has extra fifo's that can be used for aux DACs and ADCs.
 */
static struct snd_soc_dai_driver pxa_ac97_dai_driver[] = {
{
	.name = "pxa2xx-ac97",
<<<<<<< HEAD
	.bus_control = true,
=======
>>>>>>> upstream/android-13
	.playback = {
		.stream_name = "AC97 Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "AC97 Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_hifi_dai_ops,
},
{
	.name = "pxa2xx-ac97-aux",
<<<<<<< HEAD
	.bus_control = true,
=======
>>>>>>> upstream/android-13
	.playback = {
		.stream_name = "AC97 Aux Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "AC97 Aux Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_aux_dai_ops,
},
{
	.name = "pxa2xx-ac97-mic",
<<<<<<< HEAD
	.bus_control = true,
=======
>>>>>>> upstream/android-13
	.capture = {
		.stream_name = "AC97 Mic Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_mic_dai_ops,
},
};

static const struct snd_soc_component_driver pxa_ac97_component = {
	.name		= "pxa-ac97",
<<<<<<< HEAD
	.ops		= &pxa2xx_pcm_ops,
	.pcm_new	= pxa2xx_soc_pcm_new,
	.pcm_free	= pxa2xx_pcm_free_dma_buffers,
=======
	.pcm_construct	= pxa2xx_soc_pcm_new,
	.open		= pxa2xx_soc_pcm_open,
	.close		= pxa2xx_soc_pcm_close,
	.hw_params	= pxa2xx_soc_pcm_hw_params,
	.prepare	= pxa2xx_soc_pcm_prepare,
	.trigger	= pxa2xx_soc_pcm_trigger,
	.pointer	= pxa2xx_soc_pcm_pointer,
>>>>>>> upstream/android-13
};

#ifdef CONFIG_OF
static const struct of_device_id pxa2xx_ac97_dt_ids[] = {
	{ .compatible = "marvell,pxa250-ac97", },
	{ .compatible = "marvell,pxa270-ac97", },
	{ .compatible = "marvell,pxa300-ac97", },
	{ }
};
MODULE_DEVICE_TABLE(of, pxa2xx_ac97_dt_ids);

#endif

static int pxa2xx_ac97_dev_probe(struct platform_device *pdev)
{
	int ret;
<<<<<<< HEAD
=======
	struct ac97_controller *ctrl;
	pxa2xx_audio_ops_t *pdata = pdev->dev.platform_data;
	void **codecs_pdata;
>>>>>>> upstream/android-13

	if (pdev->id != -1) {
		dev_err(&pdev->dev, "PXA2xx has only one AC97 port.\n");
		return -ENXIO;
	}

	ret = pxa2xx_ac97_hw_probe(pdev);
	if (ret) {
		dev_err(&pdev->dev, "PXA2xx AC97 hw probe error (%d)\n", ret);
		return ret;
	}

<<<<<<< HEAD
	ret = snd_soc_set_ac97_ops(&pxa2xx_ac97_ops);
	if (ret != 0)
		return ret;

=======
	codecs_pdata = pdata ? pdata->codec_pdata : NULL;
	ctrl = snd_ac97_controller_register(&pxa2xx_ac97_ops, &pdev->dev,
					    AC97_SLOTS_AVAILABLE_ALL,
					    codecs_pdata);
	if (IS_ERR(ctrl))
		return PTR_ERR(ctrl);

	platform_set_drvdata(pdev, ctrl);
>>>>>>> upstream/android-13
	/* Punt most of the init to the SoC probe; we may need the machine
	 * driver to do interesting things with the clocking to get us up
	 * and running.
	 */
<<<<<<< HEAD
	return snd_soc_register_component(&pdev->dev, &pxa_ac97_component,
=======
	return devm_snd_soc_register_component(&pdev->dev, &pxa_ac97_component,
>>>>>>> upstream/android-13
					  pxa_ac97_dai_driver, ARRAY_SIZE(pxa_ac97_dai_driver));
}

static int pxa2xx_ac97_dev_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	snd_soc_unregister_component(&pdev->dev);
	snd_soc_set_ac97_ops(NULL);
=======
	struct ac97_controller *ctrl = platform_get_drvdata(pdev);

	snd_ac97_controller_unregister(ctrl);
>>>>>>> upstream/android-13
	pxa2xx_ac97_hw_remove(pdev);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int pxa2xx_ac97_dev_suspend(struct device *dev)
{
	return pxa2xx_ac97_hw_suspend();
}

static int pxa2xx_ac97_dev_resume(struct device *dev)
{
	return pxa2xx_ac97_hw_resume();
}

static SIMPLE_DEV_PM_OPS(pxa2xx_ac97_pm_ops,
		pxa2xx_ac97_dev_suspend, pxa2xx_ac97_dev_resume);
#endif

static struct platform_driver pxa2xx_ac97_driver = {
	.probe		= pxa2xx_ac97_dev_probe,
	.remove		= pxa2xx_ac97_dev_remove,
	.driver		= {
		.name	= "pxa2xx-ac97",
#ifdef CONFIG_PM_SLEEP
		.pm	= &pxa2xx_ac97_pm_ops,
#endif
		.of_match_table = of_match_ptr(pxa2xx_ac97_dt_ids),
	},
};

module_platform_driver(pxa2xx_ac97_driver);

MODULE_AUTHOR("Nicolas Pitre");
MODULE_DESCRIPTION("AC97 driver for the Intel PXA2xx chip");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pxa2xx-ac97");
