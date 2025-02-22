<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * atmel-pcm-dma.c  --  ALSA PCM DMA support for the Atmel SoC.
 *
 *  Copyright (C) 2012 Atmel
 *
 * Author: Bo Shen <voice.shen@atmel.com>
 *
 * Based on atmel-pcm by:
 * Sedji Gaouaou <sedji.gaouaou@atmel.com>
 * Copyright 2008 Atmel
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/atmel-ssc.h>
<<<<<<< HEAD
#include <linux/platform_data/dma-atmel.h>
=======
>>>>>>> upstream/android-13

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/dmaengine_pcm.h>

#include "atmel-pcm.h"

/*--------------------------------------------------------------------------*\
 * Hardware definition
\*--------------------------------------------------------------------------*/
static const struct snd_pcm_hardware atmel_pcm_dma_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED |
				  SNDRV_PCM_INFO_RESUME |
				  SNDRV_PCM_INFO_PAUSE,
	.period_bytes_min	= 256,		/* lighting DMA overhead */
	.period_bytes_max	= 2 * 0xffff,	/* if 2 bytes format */
	.periods_min		= 8,
	.periods_max		= 1024,		/* no limit */
	.buffer_bytes_max	= 512 * 1024,
};

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * atmel_pcm_dma_irq: SSC interrupt handler for DMAENGINE enabled SSC
 *
 * We use DMAENGINE to send/receive data to/from SSC so this ISR is only to
 * check if any overrun occured.
 */
static void atmel_pcm_dma_irq(u32 ssc_sr,
	struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct atmel_pcm_dma_params *prtd;

	prtd = snd_soc_dai_get_dma_data(rtd->cpu_dai, substream);
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct atmel_pcm_dma_params *prtd;

	prtd = snd_soc_dai_get_dma_data(asoc_rtd_to_cpu(rtd, 0), substream);
>>>>>>> upstream/android-13

	if (ssc_sr & prtd->mask->ssc_error) {
		if (snd_pcm_running(substream))
			pr_warn("atmel-pcm: buffer %s on %s (SSC_SR=%#x)\n",
				substream->stream == SNDRV_PCM_STREAM_PLAYBACK
				? "underrun" : "overrun", prtd->name,
				ssc_sr);

		/* stop RX and capture: will be enabled again at restart */
		ssc_writex(prtd->ssc->regs, SSC_CR, prtd->mask->ssc_disable);
		snd_pcm_stop_xrun(substream);

		/* now drain RHR and read status to remove xrun condition */
		ssc_readx(prtd->ssc->regs, SSC_RHR);
		ssc_readx(prtd->ssc->regs, SSC_SR);
	}
}

static int atmel_pcm_configure_dma(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct dma_slave_config *slave_config)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	struct atmel_pcm_dma_params *prtd;
	struct ssc_device *ssc;
	int ret;

<<<<<<< HEAD
	prtd = snd_soc_dai_get_dma_data(rtd->cpu_dai, substream);
=======
	prtd = snd_soc_dai_get_dma_data(asoc_rtd_to_cpu(rtd, 0), substream);
>>>>>>> upstream/android-13
	ssc = prtd->ssc;

	ret = snd_hwparams_to_dma_slave_config(substream, params, slave_config);
	if (ret) {
		pr_err("atmel-pcm: hwparams to dma slave configure failed\n");
		return ret;
	}

	slave_config->dst_addr = ssc->phybase + SSC_THR;
	slave_config->dst_maxburst = 1;

	slave_config->src_addr = ssc->phybase + SSC_RHR;
	slave_config->src_maxburst = 1;

	prtd->dma_intr_handler = atmel_pcm_dma_irq;

	return 0;
}

static const struct snd_dmaengine_pcm_config atmel_dmaengine_pcm_config = {
	.prepare_slave_config = atmel_pcm_configure_dma,
	.pcm_hardware = &atmel_pcm_dma_hardware,
	.prealloc_buffer_size = 64 * 1024,
};

int atmel_pcm_dma_platform_register(struct device *dev)
{
<<<<<<< HEAD
	return snd_dmaengine_pcm_register(dev, &atmel_dmaengine_pcm_config, 0);
}
EXPORT_SYMBOL(atmel_pcm_dma_platform_register);

void atmel_pcm_dma_platform_unregister(struct device *dev)
{
	snd_dmaengine_pcm_unregister(dev);
}
EXPORT_SYMBOL(atmel_pcm_dma_platform_unregister);

=======
	return devm_snd_dmaengine_pcm_register(dev,
					&atmel_dmaengine_pcm_config, 0);
}
EXPORT_SYMBOL(atmel_pcm_dma_platform_register);

>>>>>>> upstream/android-13
MODULE_AUTHOR("Bo Shen <voice.shen@atmel.com>");
MODULE_DESCRIPTION("Atmel DMA based PCM module");
MODULE_LICENSE("GPL");
